#ifdef __cplusplus
extern "C"{
#endif

#ifndef _HCSR04_H_
#define _HCSR04_H_

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define PULSE_TIMEOUT 150000L // 100ms
#define DEFAULT_DELAY 10
#define DEFAULT_PINGS 5

typedef struct {
	int echo_pin;
	int trigger_pin;
	long distance;
} hcsr04_t;

int hcsr04_init(hcsr04_t *ctx, int echo_pin, int trigger_pin);
long hcsr04_distance(hcsr04_t *ctx);
long hcsr04_distance_avg(hcsr04_t *ctx, int wait, int count);
void hcsr04_ping(hcsr04_t *ctx);
long hcsr04_getdistance(hcsr04_t *ctx);

#endif /* _HCSR04_H_ */

#ifdef __cplusplus
}
#endif
