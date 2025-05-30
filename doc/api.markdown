# Arduino Logger Library API Documentation

This document provides a detailed reference for the Arduino Logger Library, designed for logging messages in Arduino sketches with support for multiple log levels, serial output, and custom callbacks. The library is lightweight, flexible, and optimized for embedded systems, with support for both C-strings and Flash strings (`PROGMEM`).

## Table of Contents
- [Overview](#overview)
- [Loglevel Enum](#loglevel-enum)
- [loggerCallback Type](#loggercallback-type)
- [Logger Class](#logger-class)
  - [Constructor and Destructor](#constructor-and-destructor)
  - [Initialization Methods](#initialization-methods)
  - [Configuration Methods](#configuration-methods)
  - [Logging Methods](#logging-methods)
- [Constants and Macros](#constants-and-macros)
- [Usage Examples](#usage-examples)
  - [Basic Logging](#basic-logging)
  - [Callback Logging](#callback-logging)
  - [Flash String Logging](#flash-string-logging)
- [Error Handling](#error-handling)

## Overview
The Arduino Logger Library provides a `Logger` class for logging messages at different severity levels (`error`, `info`, `debug`). It supports output to a `Stream` (e.g., `Serial`) and/or a custom callback for advanced handling (e.g., MQTT, file logging). The library is designed for Arduino platforms, with features like:
- Configurable log levels and enable/disable toggle.
- Support for variable argument formatting (similar to `printf`).
- Flash string support to optimize RAM usage.
- Extensible log level enum for custom levels.

The global `logger` instance is provided for convenience, allowing immediate use without manual instantiation.

## Loglevel Enum
The `Loglevel` enum defines the available logging levels, each associated with a numeric value for filtering messages.

```cpp
enum class Loglevel : uint8_t {
  err = 1,   // Error messages (critical issues)
  info = 2,  // Informational messages (general status)
  debug = 3  // Debug messages (detailed diagnostics)
};
```

- **Type**: `uint8_t` (supports up to 255 levels).
- **Usage**: Set the log level to filter messages (e.g., `logger.setLogLevel(Loglevel::info)`).
- **Extensibility**: Add custom levels by extending the enum in `Logger.h` (e.g., `custom = 4`).

## loggerCallback Type
The `loggerCallback` type defines a callback function for processing log messages.

```cpp
using loggerCallback = Callback<void, uint8_t, const char *, const char *, const char *>;
```

- **Parameters**:
  - `uint8_t level`: Numeric log level (e.g., `1` for `err`).
  - `const char *levelN`: Log level name (e.g., `"ERROR"`).
  - `const char *str`: Message content (formatted message).
  - `const char *strN`: Full log message (including level prefix, e.g., `"ERROR: message"`).
- **Return**: `void`.
- **Usage**: Allows custom handling, such as sending logs to an external system.

**Example**:
```cpp
loggerCallback cb = loggerCallback([](uint8_t level, const char *levelN, const char *str, const char *strN) {
  Serial.print("Level: ");
  Serial.println(levelN);
  // Send strN to MQTT or another system
});
logger.begin(Serial, cb);
```

## Logger Class
The `Logger` class is the core of the library, providing methods for initialization, configuration, and logging.

### Constructor and Destructor
#### `Logger::Logger()`
- **Description**: Initializes a `Logger` instance with default settings.
- **Defaults**:
  - Log level: `info` (2).
  - Enabled: `true` (1).
  - Stream: `nullptr`.
  - Callback: None.
- **Usage**: Typically not called directly; use the global `logger` instance.

#### `Logger::~Logger()`
- **Description**: Cleans up resources by clearing the stream pointer.
- **Usage**: Automatically called when the `Logger` instance is destroyed.

### Initialization Methods
#### `void Logger::begin(Stream &strm)`
- **Description**: Initializes the logger to output to a specified `Stream` (e.g., `Serial`).
- **Parameters**:
  - `strm`: Reference to a `Stream` object (e.g., `Serial`).
- **Usage**: Use for basic serial logging.
- **Example**:
  ```cpp
  Serial.begin(115200);
  logger.begin(Serial);
  ```

#### `void Logger::begin(loggerCallback ev)`
- **Description**: Initializes the logger with a custom callback for log events.
- **Parameters**:
  - `ev`: Callback function of type `loggerCallback`.
- **Usage**: Use for custom log handling without a stream.
- **Example**:
  ```cpp
  logger.begin(loggerCallback([](uint8_t level, const char *levelN, const char *str, const char *strN) {
    // Custom handling
  }));
  ```

#### `void Logger::begin(Stream &strm, loggerCallback ev)`
- **Description**: Initializes the logger with both a stream and a callback.
- **Parameters**:
  - `strm`: Reference to a `Stream` object.
  - `ev`: Callback function of type `loggerCallback`.
- **Usage**: Combines serial output and custom handling.
- **Example**:
  ```cpp
  logger.begin(Serial, loggerCallback([](uint8_t level, const char *levelN, const char *str, const char *strN) {
    Serial.println(strN);
  }));
  ```

### Configuration Methods
#### `void Logger::setLogLevel(Loglevel level)`
- **Description**: Sets the maximum log level for output.
- **Parameters**:
  - `level`: Log level from `Loglevel` enum (e.g., `Loglevel::debug`).
- **Behavior**: Only messages with a level less than or equal to the set level are logged.
- **Example**:
  ```cpp
  logger.setLogLevel(Loglevel::debug); // Allow err, info, and debug messages
  ```

#### `Loglevel Logger::getLogLevel()`
- **Description**: Retrieves the current log level.
- **Returns**: Current `Loglevel` value.
- **Example**:
  ```cpp
  Loglevel level = logger.getLogLevel();
  if (level == Loglevel::debug) {
    Serial.println("Debug mode enabled");
  }
  ```

#### `void Logger::setEnabled(uint8_t en)`
- **Description**: Enables or disables logging.
- **Parameters**:
  - `en`: `1` to enable, `0` to disable.
- **Example**:
  ```cpp
  logger.setEnabled(0); // Disable logging
  logger.error("This won't log");
  logger.setEnabled(1); // Re-enable logging
  ```

#### `uint8_t Logger::getEnabled()`
- **Description**: Checks if logging is enabled.
- **Returns**: `1` if enabled, `0` if disabled.
- **Example**:
  ```cpp
  if (logger.getEnabled()) {
    logger.info("Logging is active");
  }
  ```

### Logging Methods
The library provides methods for logging at different levels, with support for both C-strings and Flash strings (`__FlashStringHelper`).

#### `void Logger::error(const char *s, ...)`
- **Description**: Logs an error message with variable arguments.
- **Parameters**:
  - `s`: Format string (C-string).
  - `...`: Variable arguments for formatting.
- **Example**:
  ```cpp
  logger.error("Error code: %i, %s", 404, "Not found");
  ```

#### `void Logger::error(const __FlashStringHelper *message, ...)`
- **Description**: Logs an error message stored in PROGMEM.
- **Parameters**:
  - `message`: Flash string (e.g., `F("message")`).
  - `...`: Variable arguments for formatting.
- **Example**:
  ```cpp
  logger.error(F("Error: %s"), "Critical");
  ```

#### `void Logger::info(const char *s, ...)`
- **Description**: Logs an info message with variable arguments.
- **Parameters**:
  - `s`: Format string (C-string).
  - `...`: Variable arguments for formatting.
- **Example**:
  ```cpp
  logger.info("Temperature: %f°C", 25.5);
  ```

#### `void Logger::info(const __FlashStringHelper *message, ...)`
- **Description**: Logs an info message stored in PROGMEM.
- **Parameters**:
  - `message`: Flash string.
  - `...`: Variable arguments for formatting.
- **Example**:
  ```cpp
  logger.info(F("System running"));
  ```

#### `void Logger::debug(const char *s, ...)`
- **Description**: Logs a debug message with variable arguments.
- **Parameters**:
  - `s`: Format string (C-string).
  - `...`: Variable arguments for formatting.
- **Example**:
  ```cpp
  logger.debug("Sensor value: %i", 1023);
  ```

#### `void Logger::debug(const __FlashStringHelper *message, ...)`
- **Description**: Logs a debug message stored in PROGMEM.
- **Parameters**:
  - `message`: Flash string.
  - `...`: Variable arguments for formatting.
- **Example**:
  ```cpp
  logger.debug(F("Debug mode: %s"), "ON");
  ```

## Constants and Macros
- **LOG_BUFFER_SIZE**:
  - Default: `255` (bytes).
  - Purpose: Defines the size of the log message buffer.
  - Customization: Define before including `Logger.h`:
    ```cpp
    #define LOG_BUFFER_SIZE 512
    #include <Logger.h>
    ```
- **LOGGER_MSG_LONG**:
  - Value: `F("ERROR: message too long")`.
  - Purpose: Error message output when a log exceeds `LOG_BUFFER_SIZE`.
- **logError**, **logInfo**, **logDebug**:
  - Values: `"ERROR"`, `"INFO"`, `"DEBUG"`.
  - Purpose: Static strings for log level names.

## Usage Examples
### Basic Logging
Log messages to `Serial` with different levels.

```cpp
#include <Arduino.h>
#include <Logger.h>

void setup() {
  Serial.begin(115200);
  logger.begin(Serial);
  logger.setLogLevel(Loglevel::debug);
  logger.info(F("System initialized"));
}

void loop() {
  logger.error("Error: %i", 500);
  logger.info("Status: %s", "OK");
  logger.debug("Counter: %i", millis() / 1000);
  delay(1000);
}
```

**Output** (if level is `debug`):
```
INFO: System initialized
ERROR: Error: 500
INFO: Status: OK
DEBUG: Counter: 1
```

### Callback Logging
Use a callback to process logs, e.g., for external systems.

```cpp
#include <Arduino.h>
#include <Logger.h>

void setup() {
  Serial.begin(115200);
  logger.begin(Serial, loggerCallback([](uint8_t level, const char *levelN, const char *str, const char *strN) {
    Serial.print("Callback received: ");
    Serial.println(strN);
    // Example: Send strN to MQTT
  }));
  logger.setLogLevel(Loglevel::info);
  logger.info(F("Logger with callback"));
}

void loop() {
  logger.info("Time: %lu ms", millis());
  delay(1000);
}
```

**Output**:
```
INFO: Logger with callback
Callback received: INFO: Logger with callback
INFO: Time: 1000 ms
Callback received: INFO: Time: 1000 ms
```

### Flash String Logging
Use Flash strings to save RAM.

```cpp
#include <Arduino.h>
#include <Logger.h>

void setup() {
  Serial.begin(115200);
  logger.begin(Serial);
  logger.setLogLevel(Loglevel::err);
  logger.error(F("Critical error: %s"), "System halt");
}

void loop() {
  // No debug output due to log level
  logger.debug(F("This won't log"));
  delay(1000);
}
```

**Output**:
```
ERROR: Critical error: System halt
```

## Error Handling
- **Buffer Overflow**:
  - If a message exceeds `LOG_BUFFER_SIZE`, the library outputs `ERROR: message too long` to the stream (if available).
  - Solution: Increase `LOG_BUFFER_SIZE` or shorten messages.
- **Invalid Log Level**:
  - If an invalid `Loglevel` is used, the library outputs `ERROR: Invalid log level` to the stream.
  - Solution: Use defined `Loglevel` values.
- **No Stream or Callback**:
  - If neither `_strm` nor `_logEvent` is set, or `_logEvent.valid()` is false, logging is skipped silently.
  - Solution: Ensure proper initialization with `begin()`.

## Notes
- The library uses `vsnprintf` for safe string formatting, preventing buffer overflows.
- Flash string methods (`__FlashStringHelper`) copy the string to a temporary buffer, respecting `LOG_BUFFER_SIZE`.
- The global `logger` instance is defined in `Logger.cpp` and declared in `Logger.h` with `extern`.
- For memory-constrained devices, minimize `LOG_BUFFER_SIZE` and use Flash strings.
