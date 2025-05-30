#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <Arduino.h>
#include <Callback.h>

#ifndef LOG_BUFFER_SIZE
#define LOG_BUFFER_SIZE 255  // Default log buffer size if not defined externally
#endif

#define LOGGER_MSG_LONG F("ERROR: message too long")  // Message shown when log exceeds buffer size

// Log levels (can be extended, max 255 levels due to uint8_t)
enum class Loglevel : uint8_t {
  err = 1,    // Error messages
  info = 2,   // Informational messages
  debug = 3   // Debug messages
};

// Static strings representing the log levels
static const char logError[] = "ERROR";
static const char logInfo[] = "INFO";
static const char logDebug[] = "DEBUG";

// Callback type for log event handler
using loggerCallback = Callback<void, uint8_t, const char *, const char *, const char *>;

// Logger class declaration
class Logger {
 private:
  uint8_t _logLevel = 2;    // Default log level: info
  uint8_t _enabled = 1;     // Logging enabled by default
  Stream *_strm = nullptr;  // Output stream (e.g., Serial)
  loggerCallback _logEvent; // Callback for log events (e.g., to forward via MQTT)

 public:
  Logger();
  ~Logger();

  // Initialize with output stream
  void begin(Stream &strm);

  // Initialize with callback handler
  void begin(loggerCallback ev);

  // Initialize with both stream and callback
  void begin(Stream &strm, loggerCallback ev);

  // Set the current logging level
  void setLogLevel(Loglevel level);

  // Get the current logging level
  Loglevel getLogLevel();

  // Enable or disable logging
  void setEnabled(uint8_t en);

  // Get whether logging is enabled
  uint8_t getEnabled();

  // Log function with variadic arguments
  void log(Loglevel level, const char *s, va_list args);

  // Log functions for different levels with format strings
  void error(const __FlashStringHelper *s, ...);
  void error(const char *s, ...);

  void info(const __FlashStringHelper *s, ...);
  void info(const char *s, ...);

  void debug(const __FlashStringHelper *s, ...);
  void debug(const char *s, ...);
};

// External logger instance declaration
extern Logger logger;

#endif

