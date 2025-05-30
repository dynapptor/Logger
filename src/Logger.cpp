#include "Logger.h"

// Constructor
Logger::Logger() {}

// Destructor
Logger::~Logger() {}

// Initialize logger with output stream (e.g., Serial)
void Logger::begin(Stream &strm) {
  _strm = &strm;
}

// Initialize logger with callback function
void Logger::begin(loggerCallback ev) {
  _logEvent = ev;
}

// Initialize logger with both stream and callback
void Logger::begin(Stream &strm, loggerCallback ev) {
  _strm = &strm;
  _logEvent = ev;
}

// Set current logging level
void Logger::setLogLevel(Loglevel level) {
  _logLevel = static_cast<uint8_t>(level);
}

// Get current logging level
Loglevel Logger::getLogLevel() {
  return static_cast<Loglevel>(_logLevel);
}

// Enable or disable logging
void Logger::setEnabled(uint8_t en) {
  _enabled = en;
}

// Check if logging is enabled
uint8_t Logger::getEnabled() {
  return _enabled;
}

// Core log function (used by error/info/debug)
void Logger::log(Loglevel level, const char *s, va_list args) {
  if (!_enabled || static_cast<uint8_t>(level) > _logLevel || (!_strm && !_logEvent.valid())) return;

  char _buf[LOG_BUFFER_SIZE];  // Log message buffer
  uint16_t pti = 0;            // Print text index
  const char *levelN;          // String name of the log level

  // Prepend level prefix to buffer
  switch (level) {
    case Loglevel::err:
      strcpy(_buf, logError);
      _buf[5] = ':';
      _buf[6] = ' ';
      levelN = logError;
      pti = 7;
      break;
    case Loglevel::info:
      strcpy(_buf, logInfo);
      _buf[4] = ':';
      _buf[5] = ' ';
      levelN = logInfo;
      pti = 6;
      break;
    case Loglevel::debug:
      strcpy(_buf, logDebug);
      _buf[5] = ':';
      _buf[6] = ' ';
      levelN = logDebug;
      pti = 7;
      break;
  }

  // Format the message
  vsnprintf(&_buf[pti], LOG_BUFFER_SIZE - pti, s, args);

  // Send to stream
  if (_strm) _strm->println(_buf);

  // Trigger callback with raw and formatted data
  if (_logEvent.valid()) _logEvent(static_cast<uint8_t>(level), levelN, s, &_buf[pti]);
}

// Variadic overloads for F() string helpers and regular strings
void Logger::error(const __FlashStringHelper *s, ...) {
  va_list args;
  va_start(args, s);
  log(Loglevel::err, reinterpret_cast<const char *>(s), args);
  va_end(args);
}

void Logger::error(const char *s, ...) {
  va_list args;
  va_start(args, s);
  log(Loglevel::err, s, args);
  va_end(args);
}

void Logger::info(const __FlashStringHelper *s, ...) {
  va_list args;
  va_start(args, s);
  log(Loglevel::info, reinterpret_cast<const char *>(s), args);
  va_end(args);
}

void Logger::info(const char *s, ...) {
  va_list args;
  va_start(args, s);
  log(Loglevel::info, s, args);
  va_end(args);
}

void Logger::debug(const __FlashStringHelper *s, ...) {
  va_list args;
  va_start(args, s);
  log(Loglevel::debug, reinterpret_cast<const char *>(s), args);
  va_end(args);
}

void Logger::debug(const char *s, ...) {
  va_list args;
  va_start(args, s);
  log(Loglevel::debug, s, args);
  va_end(args);
}

