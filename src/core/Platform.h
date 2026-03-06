#pragma once

#if defined(ARDUINO)

#include "Arduino.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using LpLogMirrorCallback = void (*)(const char* data, size_t len);
inline LpLogMirrorCallback gLpLogMirrorCallback = nullptr;

inline void lpSetLogMirrorCallback(LpLogMirrorCallback callback) {
  gLpLogMirrorCallback = callback;
}

inline void lpLogWriteBytes(const char* data, size_t len) {
  if (data == nullptr || len == 0) {
    return;
  }
  Serial.write(reinterpret_cast<const uint8_t*>(data), len);
#if defined(DEV_ENABLED)
  if (gLpLogMirrorCallback != nullptr) {
    gLpLogMirrorCallback(data, len);
  }
#else
  (void)gLpLogMirrorCallback;
#endif
}

inline void lpLogPrint(const String& value) {
  lpLogWriteBytes(value.c_str(), value.length());
}

inline void lpLogPrint(const char* value) {
  if (value == nullptr) {
    return;
  }
  lpLogWriteBytes(value, strlen(value));
}

inline void lpLogPrint(char value) {
  lpLogWriteBytes(&value, 1);
}

inline void lpLogPrint(signed char value) {
  const String rendered(static_cast<int>(value));
  lpLogWriteBytes(rendered.c_str(), rendered.length());
}

inline void lpLogPrint(unsigned char value) {
  const String rendered(static_cast<unsigned int>(value));
  lpLogWriteBytes(rendered.c_str(), rendered.length());
}

inline void lpLogPrint(const __FlashStringHelper* value) {
  if (value == nullptr) {
    return;
  }
  const String rendered(value);
  lpLogWriteBytes(rendered.c_str(), rendered.length());
}

template <typename T>
inline void lpLogPrint(const T& value) {
  const String rendered(value);
  lpLogWriteBytes(rendered.c_str(), rendered.length());
}

template <typename T>
inline void lpLogPrintln(const T& value) {
  lpLogPrint(value);
  lpLogWriteBytes("\n", 1);
}

inline void lpLogVPrintf(const char* format, va_list args) {
  if (format == nullptr) {
    return;
  }

  va_list argsCopy;
  va_copy(argsCopy, args);
  const int required = vsnprintf(nullptr, 0, format, argsCopy);
  va_end(argsCopy);
  if (required <= 0) {
    return;
  }

  char stackBuffer[256];
  const size_t requiredLen = static_cast<size_t>(required);
  if (requiredLen < sizeof(stackBuffer)) {
    vsnprintf(stackBuffer, sizeof(stackBuffer), format, args);
    lpLogWriteBytes(stackBuffer, requiredLen);
    return;
  }

  char* heapBuffer = static_cast<char*>(malloc(requiredLen + 1));
  if (heapBuffer == nullptr) {
    static const char kLogOom[] = "[log] printf OOM\n";
    lpLogWriteBytes(kLogOom, sizeof(kLogOom) - 1);
    return;
  }
  vsnprintf(heapBuffer, requiredLen + 1, format, args);
  lpLogWriteBytes(heapBuffer, requiredLen);
  free(heapBuffer);
}

inline void lpLogPrintf(const char* format, ...) {
  va_list args;
  va_start(args, format);
  lpLogVPrintf(format, args);
  va_end(args);
}

#define LP_LOG(...) lpLogPrint(__VA_ARGS__)
#define LP_LOGF(...) lpLogPrintf(__VA_ARGS__)
#define LP_LOGLN(...) lpLogPrintln(__VA_ARGS__)

#define LP_RANDOM(...) random(__VA_ARGS__)
#define LP_STRING String

#ifndef MIN
#define MIN(x,y) (((x) < (y)) ? (x) : (y))
#endif

#ifndef MAX
#define MAX(x,y) (((x) > (y)) ? (x) : (y))
#endif

#else

#include "ofMain.h"

#define LP_LOG(...) ofLog(OF_LOG_WARNING, __VA_ARGS__)
#define LP_LOGF(...) ofLog(OF_LOG_WARNING, __VA_ARGS__)
#define LP_LOGLN ofLogWarning
#define LP_RANDOM ofRandom
#define LP_STRING std::string

#endif
