#include <Arduino.h>

#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif

// Settings
static const uint8_t buf_len = 255;

// Globals
static char *msg_ptr = NULL;
static volatile uint8_t msg_flag = 0;

// Tasks
// read message from serial buffer
void readSerial(void *parameters) {
  char c;
  char buf[buf_len];
  uint8_t idx = 0;

  // clear buffer
  memset(buf, 0, buf_len);

  while(1) {
    // read chars from serial
    if (Serial.available() > 0) {
      c = Serial.read();

      if (idx < buf_len - 1) {
        buf[idx] = c;
        idx++;
      }

      if (c=='\n') {
        // end message
        // replace new line with null character
        buf[idx-1] = '\0';

        // try allocating memory and copying the msg
        // if message buffer is in use, ignore the message
        if (msg_flag == 0) {
          msg_ptr = (char*) pvPortMalloc(idx * sizeof(char));

          // if malloc returns 0 (out of memory) throw error and return
          configASSERT(msg_ptr);

          // copy msg
          memcpy(msg_ptr, buf, idx);

          // notify other task by setting message flag
          msg_flag = 1;
        }

        // reset receive buffer
        memset(buf, 0, buf_len);
        idx = 0;

      }
    }
  }
}



void setup() {

}

void loop() {
}
