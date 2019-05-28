#ifndef _DHT11_H_
#define _DHT11_H_

#ifdef __cplusplus
extern "C"{
#endif

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define	DHT_TYPE_11 (0)
#define	DHT_TYPE_21 (1)
#define	DHT_TYPE_22 (2)

typedef struct {
	uint8_t pin;
	uint8_t type;
	uint8_t bit;
	uint8_t port;
	uint32_t max_cycles;
	uint8_t data[6];
	uint8_t dht_state;
	unsigned long dht_timestamp;
} dht11_t;

int dht11_init(dht11_t *ctx, uint8_t pin, uint8_t type);
bool dht11_measure(dht11_t *ctx, float *temperature, float *humidity);

#ifdef __cplusplus
}
#endif

#endif /* _DHT11_H_ */
