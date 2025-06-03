#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <Arduino.h>
#include <Callback.h>

/**
 * @file Logger.h
 * @brief A lightweight logging library for Arduino with support for streams and callbacks.
 *
 * The Logger class provides a flexible, configurable logging mechanism for Arduino projects.
 * It supports multiple log levels, output to streams (e.g., Serial), and custom callback handlers
 * for advanced logging (e.g., MQTT). The library integrates with the Callback library for type-safe
 * callback handling and is optimized for resource-constrained environments.
 */

/**
 * @def LOG_BUFFER_SIZE
 * @brief Default size of the log message buffer.
 *
 * Defines the maximum size of the buffer used for formatting log messages.
 * Can be overridden by defining LOG_BUFFER_SIZE externally before including this header.
 */
#ifndef LOG_BUFFER_SIZE
#define LOG_BUFFER_SIZE 255
#endif

/**
 * @def LOGGER_MSG_LONG
 * @brief Error message displayed when a log message exceeds the buffer size.
 */
#define LOGGER_MSG_LONG F("ERROR: message too long")

/**
 * @enum Loglevel
 * @brief Enumeration of available log levels.
 *
 * Defines the severity levels for log messages. The values are stored as uint8_t to support
 * up to 255 levels, though only three are predefined.
 */
enum class Loglevel : uint8_t {
  err = 1,   ///< Error messages, critical issues.
  info = 2,  ///< Informational messages, general status updates.
  debug = 3  ///< Debug messages, detailed diagnostic information.
};

/**
 * @var logError
 * @brief Static string representing the "ERROR" log level.
 */
static const char logError[] = "ERROR";

/**
 * @var logInfo
 * @brief Static string representing the "INFO" log level.
 */
static const char logInfo[] = "INFO";

/**
 * @var logDebug
 * @brief Static string representing the "DEBUG" log level.
 */
static const char logDebug[] = "DEBUG";

/**
 * @typedef loggerCallback
 * @brief Callback type for handling log events.
 *
 * A type alias for a Callback object that processes log events with the following parameters:
 * - uint8_t: Log level (raw value).
 * - const char*: Log level name (e.g., "ERROR").
 * - const char*: Format string of the log message.
 * - const char*: Formatted log message.
 */
using loggerCallback = Callback<void, uint8_t, const char *, const char *, const char *>;

/**
 * @class Logger
 * @brief A class for logging messages with configurable levels, streams, and callbacks.
 *
 * The Logger class supports logging to a stream (e.g., Serial) and/or a callback function.
 * It allows setting log levels, enabling/disabling logging, and formatting messages with
 * variadic arguments. The class is designed for Arduino and integrates with the Callback library.
 */
class Logger {
 private:
  uint8_t _logLevel = 2;     ///< Current log level (default: info).
  uint8_t _enabled = 1;      ///< Logging enabled flag (default: enabled).
  Stream *_strm = nullptr;   ///< Pointer to the output stream (e.g., Serial).
  loggerCallback _logEvent;  ///< Callback for handling log events.

 public:
  /**
   * @brief Default constructor.
   *
   * Initializes the Logger with default settings (info level, enabled, no stream or callback).
   */
  Logger();

  /**
   * @brief Destructor.
   *
   * Cleans up resources, if any.
   */
  ~Logger();

  /**
   * @brief Initializes the logger with an output stream.
   * @param strm Reference to the output stream (e.g., Serial).
   */
  void begin(Stream &strm);

  /**
   * @brief Initializes the logger with a callback handler.
   * @param ev Callback function to handle log events.
   */
  void begin(loggerCallback ev);

  /**
   * @brief Initializes the logger with both a stream and a callback.
   * @param strm Reference to the output stream (e.g., Serial).
   * @param ev Callback function to handle log events.
   */
  void begin(Stream &strm, loggerCallback ev);

  /**
   * @brief Sets the current logging level.
   * @param level The log level to set (e.g., Loglevel::err).
   */
  void setLogLevel(Loglevel level);

  /**
   * @brief Gets the current logging level.
   * @return The current log level.
   */
  Loglevel getLogLevel();

  /**
   * @brief Enables or disables logging.
   * @param en 1 to enable logging, 0 to disable.
   */
  void setEnabled(uint8_t en);

  /**
   * @brief Gets whether logging is enabled.
   * @return 1 if logging is enabled, 0 if disabled.
   */
  uint8_t getEnabled();

  /**
   * @brief Logs a message with the specified level and variadic arguments.
   * @param level The log level (e.g., Loglevel::err).
   * @param s Format string for the log message.
   * @param args Variadic arguments for formatting the message.
   */
  void log(Loglevel level, const char *s, va_list args);

  /**
   * @brief Logs an error message with a flash string.
   * @param s Format string stored in PROGMEM.
   * @param ... Variadic arguments for formatting.
   */
  void error(const __FlashStringHelper *s, ...);

  /**
   * @brief Logs an error message with a regular string.
   * @param s Format string.
   * @param ... Variadic arguments for formatting.
   */
  void error(const char *s, ...);

  /**
   * @brief Logs an info message with a flash string.
   * @param s Format string stored in PROGMEM.
   * @param ... Variadic arguments for formatting.
   */
  void info(const __FlashStringHelper *s, ...);

  /**
   * @brief Logs an info message with a regular string.
   * @param s Format string.
   * @param ... Variadic arguments for formatting.
   */
  void info(const char *s, ...);

  /**
   * @brief Logs a debug message with a flash string.
   * @param s Format string stored in PROGMEM.
   * @param ... Variadic arguments for formatting.
   */
  void debug(const __FlashStringHelper *s, ...);

  /**
   * @brief Logs a debug message with a regular string.
   * @param s Format string.
   * @param ... Variadic arguments for formatting.
   */
  void debug(const char *s, ...);
};

/**
 * @var logger
 * @brief Global Logger instance for convenient access.
 *
 * An external Logger object that can be used throughout the application without
 * needing to create a new instance.
 */
extern Logger logger;

#endif