/*
 * Temperature & Humidity Sensor
 * 
 * Using DHT11 sensor to measure temperature and humidity.
 * 
 */

#include <dht11.h>

#define DHT_SENSOR_PIN (2)

dht11_t dht11;

void setup() {
  dht11_init(&dht11, DHT_SENSOR_PIN, DHT_TYPE_11);
  Serial.begin(9600);
}

static bool measure(float *temperature, float *humidity)
{
  static unsigned long ts = millis();
  
  if (millis() - ts > 3000UL) {
    if (dht11_measure(&dht11, temperature, humidity)) {
      ts = millis();
      return (true);
    }
  }
  return (false);
}

void loop() {
  float temperature;
  float humidity;

  /* Measure temperature and humidity. 
   * If the function returns true, 
   * then a measurement is available. */
   if (measure(&temperature, &humidity)) {
    Serial.print("T = ");
    Serial.print(temperature, 1);
    Serial.print(" deg. C, H = ");
    Serial.print(humidity, 1);
    Serial.println("%");
   }
}
