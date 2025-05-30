#include <Arduino.h>
#include <Logger.h>

// Setup runs once at boot
void setup() {
  Serial.begin(115200);  // Initialize serial communication

  // Start the logger with Serial and a custom callback
  logger.begin(Serial, loggerCallback([](uint8_t level, const char *levelN, const char *str, const char *strN) {
    // Handle log messages in the callback (e.g., print again or send elsewhere)
    Serial.println(level);    // Numeric log level
    Serial.println(levelN);  // Level name string
    Serial.println(str);     // Format string
    Serial.println(strN);    // Final formatted message
  }));

  // Set the log level to debug (most verbose)
  logger.setLogLevel(Loglevel::debug);

  // Log startup info
  logger.info(F("Logger started!"));
}

// Loop runs continuously
void loop() {
  // Log an error message with formatting
  logger.error(F("Error occured: %i, %s"), 10, "Error description");

  // Log an informational message with formatting
  logger.info(F("Hello this is an ifo message: %i, %s"), 1, "HELLO");

  delay(1000);  // Wait 1 second before repeating
}

