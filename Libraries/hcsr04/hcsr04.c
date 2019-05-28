#include "hcsr04.h"

static long microseconds2centimeter(long duration);

int hcsr04_init(hcsr04_t *ctx, int echo_pin, int trigger_pin)
{
	if (!ctx)
		return (-1);
	ctx->echo_pin = echo_pin;
	ctx->trigger_pin = trigger_pin;
	pinMode(echo_pin, INPUT);
	pinMode(trigger_pin, OUTPUT);
	ctx->distance = 999;
	return (0);
}

long hcsr04_distance(hcsr04_t *ctx)
{
	long d = 0;
	long duration = 0;

	if (!ctx)
		return (0);
	digitalWrite(ctx->trigger_pin, LOW);
	delayMicroseconds(2);
	digitalWrite(ctx->trigger_pin, HIGH);
	delayMicroseconds(2);
	digitalWrite(ctx->trigger_pin, LOW);
	delayMicroseconds(2);
	duration = pulseIn(ctx->echo_pin, HIGH, PULSE_TIMEOUT);
	d = microseconds2centimeter(duration);
	delay(25);
	return (d);
}

long hcsr04_distance_avg(hcsr04_t *ctx, int wait, int count)
{
	long min = 999, max = 0, avg = 0, d = 0;
	int x;

	if (!ctx)
		return (0);
	if (wait < 25)
		wait = 25;
	if (count < 1)
		count = 1;
	for (x = 0; x < count + 2; x++) {
		d = hcsr04_distance(ctx);
		if (d < min)
			min = d;
		if (d > max)
			max = d;
		avg += d;
	}
	avg -= (max + min);
	avg /= count;
	return (avg);
}

void hcsr04_ping(hcsr04_t *ctx)
{
	if (!ctx)
		return;
	ctx->distance = hcsr04_distance(ctx);
}

long hcsr04_getdistance(hcsr04_t *ctx)
{
	if (!ctx)
		return (0);
	return (ctx->distance);
}

static long microseconds2centimeter(long duration)
{
	long d = (duration * 100) / 5882;
	return (d);
}
