#pragma once

#include "common.h"

/**
 * Compares two null-terminated strings for equality.
 *
 * @param pStrA Pointer to the first string.
 * @param pStrB Pointer to the second string.
 *
 * @return CU_TRUE if the strings are equal, CU_FALSE otherwise.
 */
b8 cuStrEqual(const char* pStrA, const char* pStrB);