/*
 * DHT11, DHT21 and DHT22 library.
 * Based on Adafruit Industries' DHT driver library.
 */

#include "dht11.h"

/* States */
#define DHT_IDLE		(0)
#define DHT_BEGIN_MEASUREMENT	(1)
#define DHT_BEGIN_MEASUREMENT_2	(2)
#define DHT_DO_READING		(3)
#define DHT_COOLDOWN		(4)

/* Number of milliseconds before a new sensor read maybe initiated. */
#define COOLDOWN_TIME		(2000)

static int dht11_read(dht11_t *ctx);
static int dht11_read_data(dht11_t *ctx);
static uint32_t dht11_expect_pulse(dht11_t *ctx, char level);
static float dht11_read_temperature(dht11_t *ctx);
static float dht11_read_humidity(dht11_t *ctx);

int dht11_init(dht11_t *ctx, uint8_t pin, uint8_t type)
{
	if (!ctx)
		return (-1);
	ctx->pin = pin;
	ctx->type = type;
	ctx->bit = digitalPinToBitMask(pin);
	ctx->port = digitalPinToPort(pin);
	ctx->max_cycles = microsecondsToClockCycles(1000);
	ctx->dht_state = DHT_IDLE;
	pinMode(pin, INPUT);
	digitalWrite(pin, HIGH);
	return (0);
}

/* Instruct DHT to begin sampling. Keep polling until it returns true.
 * The temperature is in degrees Celcius and the humidity is in %. */
bool dht11_measure(dht11_t *ctx, float *temperature, float *humidity)
{
	if (!ctx)
		return (false);
	if (dht11_read(ctx)) {
		*temperature = dht11_read_temperature(ctx);
		*humidity = dht11_read_humidity(ctx);
		return (true);
	}
	return (false);
}

/* State machine of non-blocking read. */
static int dht11_read(dht11_t *ctx)
{
	int status = 0;

	switch (ctx->dht_state) {
	case DHT_IDLE:
		/* We may begin measurement any time. */
		ctx->dht_state = DHT_BEGIN_MEASUREMENT;
		break;
	case DHT_BEGIN_MEASUREMENT:
		/* Initiate a sensor read. The read begins by going to
		   high impedance state for 250ms. */
		digitalWrite(ctx->pin, HIGH);
		/* Reset 40 bits of received data to zero. */
		memset(ctx->data, 0, 5*sizeof(ctx->data[0]));
		ctx->dht_timestamp = millis();
		ctx->dht_state = DHT_BEGIN_MEASUREMENT_2;
		break;
	case DHT_BEGIN_MEASUREMENT_2:
		/* After the high impedence state,
		   pull the pin low for 20ms. */
		if ((millis() - ctx->dht_timestamp) > 250) {
			pinMode(ctx->pin, OUTPUT);
			digitalWrite(ctx->pin, LOW);
			ctx->dht_timestamp = millis();
			ctx->dht_state = DHT_DO_READING;
		}
		break;
	case DHT_DO_READING:
		/* Wait for 20ms. */
		if ((millis() - ctx->dht_timestamp) > 20) {
			ctx->dht_timestamp = millis();
			ctx->dht_state = DHT_COOLDOWN;
			status = dht11_read_data(ctx);
			// if (!status)
			//	Serial.println("Reading failed.");
		}
		break;
	case DHT_COOLDOWN:
		/* If it has been less than 2 seconds
		   since the last time we read the sensor,
		   then let the sensor cool down. */
		if ((millis() - ctx->dht_timestamp) > COOLDOWN_TIME)
			ctx->dht_state = DHT_IDLE;
		break;
	default:
		break;
	}
	return (status);
}

/* Read sensor data. This is identical to Adafruit's blocking driver. */
static int dht11_read_data(dht11_t *ctx)
{
	uint32_t cycles[80];

	{
		/* Turn off the interrupts temporarily
		   because this section is timing critical and
		   we don't want any interruptions. */
		noInterrupts();

		/* End the start signal by setting
		 * data line high for 40 microseconds. */
		digitalWrite(ctx->pin, HIGH);
		delayMicroseconds(40);

		/* Now start reading the data line
		 * to get the value from the DHT sensor. */
		pinMode(ctx->pin, INPUT);
		/* Delay a bit to let the sensor pull data line low. */
		delayMicroseconds(10);

		/* First expect a low signal for ~80 microseconds
		 * followed by a high signal for ~80 microseconds again. */
		if (dht11_expect_pulse(ctx, LOW) == 0)
			return (0);
		if (dht11_expect_pulse(ctx, HIGH) == 0)
			return (0);

		/* Now read the 40 bits sent by the sensor.
		 * Each bit is sent as a 50 microseconds low pulse
		 * followed by a variable length high pulse.
		 * If the high pulse is ~28 microseconds then it is a 0 and
		 * if it is ~70 microseconds then it is a 1.
		 * We measure the cycle count of the initial 50 microseconds
		 * low pulse and use that to compare to the cycle count of the
		 * high pulse to determine if the bit is a
		 * 0 (high state cycle count < low state cycle count), or a
		 * 1 (high state cycle count > low state cycle count).
		 * Note that for speed all the pulses are read into a array and
		 * then examined in a later step. */
		for (int i=0; i<80; i+=2) {
			cycles[i] = dht11_expect_pulse(ctx, LOW);
			cycles[i+1] = dht11_expect_pulse(ctx, HIGH);
		}

		/* Timing critical code is now complete.
		 * Enable the interrupts back. */
		interrupts();
	}

	/* Inspect pulses and determine whether they are 0 or 1. */
	for (int i=0; i<40; i++) {
		uint32_t low_cycles = cycles[(2 * i)];
		uint32_t high_cycles = cycles[(2 * i) + 1];
		if ((low_cycles == 0) || (high_cycles == 0))
			return (0);
		ctx->data[i / 8] <<= 1;
		/* Now compare the low and high cycle times
		   to see if the bit is a 0 or 1. */
		if (high_cycles > low_cycles) {
			ctx->data[i / 8] |= 1;
		}
	}

	/* Check whether we read 40 bits and that the checksum matches. */
	if (ctx->data[4] == ((ctx->data[0] + ctx->data[1] +
			      ctx->data[2] + ctx->data[3]) & 0xFF))
		return (1);
	return (0);
}

/* Expect the input to be at the specified level and
 * return the number of loop cycles spent there. */
static uint32_t dht11_expect_pulse(dht11_t *ctx, char level)
{
	uint32_t count = 0;

#ifdef __AVR
	/* On AVR platforms use direct GPIO port access as
	 * it's much faster and better for catching pulses
	 * that are 10's of microseconds in length. */
	uint8_t port_state = (level) ? ctx->bit : 0;
	while ((*portInputRegister(ctx->port) & ctx->bit) == port_state) {
		if (count++ >= ctx->max_cycles) {
			// exceeded timeout, fail
			return (0);
		}
	}
#else
	/* Otherwise fall back to using digitalRead
	 * (this seems to be necessary on ESP8266 right now,
	 * perhaps bugs in direct port access functions?). */
	while (digitalRead(ctx->pin) == level) {
		if (count++ >= ctx->max_cycles) {
			// exceeded timeout, fail
			return (0);
		}
	}
#endif
	return (count);
}

static float dht11_read_temperature(dht11_t *ctx)
{
	int16_t value;
	float fvalue = 0.0;

	switch (ctx->type) {
	case DHT_TYPE_11:
		value = ctx->data[2];
		fvalue = (float)value;
		break;
	case DHT_TYPE_21:
	case DHT_TYPE_22:
		value = ((ctx->data[2] & 0x7F) << 8);
		value |= ctx->data[3];
		if ((ctx->data[2] & 0x80) != 0) {
			value = -value;
		}
		fvalue = (((float)value) / 10.0);
		break;
	default:
		fvalue = NAN;
		break;
	};
	return (fvalue);
}

static float dht11_read_humidity(dht11_t *ctx)
{
	int16_t value;
	float fvalue;

	switch (ctx->type) {
	case DHT_TYPE_11:
		value = ctx->data[0];
		fvalue = (float)value;
		break;
	case DHT_TYPE_21:
	case DHT_TYPE_22:
		value = (ctx->data[0] << 8);
		value |= ctx->data[1];
		fvalue = ((float)value / 10.0);
		break;
	default:
		fvalue = NAN;
		break;
	}
	return (fvalue);
}
