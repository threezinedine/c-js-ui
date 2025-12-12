#pragma once

#include "common.h"

typedef u64 CuTimestamp; ///< Represents the UNIX timestamp.

typedef struct CuDateTime
{
	u32 year;		  ///< Year component of the date
	u32 month;		  ///< Month component of the date.
	u32 day;		  ///< Day component of the date.
	u32 hours;		  ///< Hours component of the time.
	u32 minutes;	  ///< Minutes component of the time.
	u32 seconds;	  ///< Seconds component of the time.
	u32 milliseconds; ///< Milliseconds component of the time.
} CuDateTime;

/**
 * Retrieves the current high-resolution timestamp.
 *
 * @return The current timestamp in microseconds.
 */
CuTimestamp cuGetCurrentTimestamp();

/**
 * Converts a timestamp to a human-readable date and time structure.
 *
 * @param timestamp The timestamp to convert.
 *
 * @return A CuDateTime structure representing the date and time.
 */
CuDateTime cuTimestampToDateTime(CuTimestamp timestamp);