// using only core 1
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

// Pins
static const int led_pin = LED_BUILTIN;

// Task
void toggleLED(void *parameter) {
  while(1) {
    digitalWrite(led_pin, HIGH);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    digitalWrite(led_pin, LOW);
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

void setup() {
  // configure pin
  pinMode(led_pin, OUTPUT);

  // create task
  xTaskCreatePinnedToCore(  // xTaskCreate on vanilla FreeRTOS
    toggleLED,              // Function to be called
    "Toggle LED",           // Task name
    1024,                   // Stack size
    NULL,                   // Task parameters
    1,                      // Task Priority
    NULL,                   // Task Handle
    app_cpu);               // CPU Core 1
  )
}

void loop() {
  // put your main code here, to run repeatedly:

}
