#pragma once

#include "cju/core/log/logger.h"

/**
 * @brief Converts a CuLogLevel to its corresponding string representation.
 *
 * @param level The log level to convert.
 *
 * @return A string representation of the log level.
 */
const char* cuLogLevelToString(CuLogLevel level);