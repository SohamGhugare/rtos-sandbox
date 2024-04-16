// using only core 1
#include <stdlib.h>
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

// Pins
static const int led_pin = LED_BUILTIN;

// Settings
static const uint8_t buf_len = 20;

// Globals
static int led_delay = 500;

// toggle led task
void toggleLED(void *parameter) {
  while (1) {
    digitalWrite(led_pin, HIGH);
    vTaskDelay(led_delay / portTICK_PERIOD_MS);
    digitalWrite(led_pin, LOW);
    vTaskDelay(led_delay / portTICK_PERIOD_MS);
  }
}

// read serial data
void readSerial(void *parameters) {
  char c;
  char buf[buf_len];
  uint8_t idx = 0;

  // clearing buffer
  memset(buf, 0, buf_len);

  while (1) {
    // reading from serial
    if (Serial.available() > 0) {
      c = Serial.read();

      // update delay buf
      if (c != '\n') {
        if(idx < buf_len - 1){
          buf[idx] = c;
          idx++;
        }
      } else { // update delay time
        led_delay = atoi(buf);
        Serial.print("Updated LED delay to: ");
        Serial.println(led_delay);
        memset(buf, 0, buf_len);
        idx = 0;
      }
    }
  }
}

void setup() {
  // configure pin
  pinMode(led_pin, OUTPUT);

  // create task
  xTaskCreatePinnedToCore( // xTaskCreate on vanilla FreeRTOS
      toggleLED,           // Function to be called
      "Toggle LED",        // Task name
      1024,                // Stack size
      NULL,                // Task parameters
      1,                   // Task Priority
      NULL,                // Task Handle
      app_cpu);            // CPU Core 1
  )
}

void loop() {
  // put your main code here, to run repeatedly:
}
