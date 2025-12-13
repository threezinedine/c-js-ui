#pragma once

#if __cplusplus
extern "C" {
#endif

/**
 * @file release_stack.h
 *
 * Definitions and utilities for managing a release stack.
 * The release stack is used to register cleanup functions that
 * will be called in reverse order upon shutdown, ensuring proper
 * resource management.
 */

#include "cju/core/core.h"
#include "cju/platform/platform.h"

/**
 * Function pointer type for release functions.
 * These functions take a single void pointer as user data.
 *
 * @param pUserData Pointer to user data to be passed to the release function.
 */
typedef void (*CuReleaseFunction)(void* pUserData);

/**
 * Structure representing an entry in the release stack.
 * Each entry contains a release function and associated user data.
 */
typedef struct CuReleaseStackEntry
{
	CuReleaseFunction pReleaseFunction; ///< Pointer to the release function, cannot be NULL
	void*			  pUserData;		///< Pointer to user data for the release function, can be NULL
} CuReleaseStackEntry;

typedef struct CuReleaseStack
{
	CuStack* pStack; ///< Underlying stack to hold release stack entries
} CuReleaseStack;

/**
 * Initialize a new release stack.
 * @return Pointer to the created CuReleaseStack instance.
 */
CuReleaseStack* cuReleaseStackCreate();

/**
 * Push a release function and its user data onto the release stack.
 * @param pReleaseStack Pointer to the CuReleaseStack instance.
 * @param pReleaseFunction Pointer to the release function to be registered.
 * @param pUserData Pointer to user data to be passed to the release function.
 */
void cuReleaseStackPush(CuReleaseStack* pReleaseStack, CuReleaseFunction pReleaseFunction, void* pUserData);

/**
 * Execute all release functions in the release stack in reverse order
 * and destroy the release stack.
 * @param pReleaseStack Pointer to the CuReleaseStack instance.
 */
void cuReleaseStackExecuteAndDestroy(CuReleaseStack* pReleaseStack);

#if __cplusplus
}
#endif