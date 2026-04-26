#pragma once

/**
 * @file module.h
 * @brief Public interface for the <module_name> module.
 *
 * <Replace this with a one-paragraph description of the module's responsibility.>
 *
 * ## Usage
 * @code{.c}
 *   ModuleConfig cfg = {
 *       .field = value,
 *   };
 *   ModuleHandle *handle = NULL;
 *   if (module_init(&cfg, &handle) != PICO_OK) {
 *       // handle error
 *   }
 *   module_do_something(handle);
 *   module_deinit(handle);
 * @endcode
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "pico/stdlib.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ---------------------------------------------------------------------------
 * Types
 * --------------------------------------------------------------------------*/

/** Configuration passed to module_init(). */
typedef struct {
    /* TODO: add configuration fields */
    uint placeholder; /**< Replace with real fields. */
} ModuleConfig;

/**
 * @brief Opaque handle for a module instance.
 *
 * The internal layout is private to module.c.  Callers hold a pointer and
 * pass it to every module function.
 */
typedef struct ModuleHandle ModuleHandle;

/* ---------------------------------------------------------------------------
 * Lifecycle
 * --------------------------------------------------------------------------*/

/**
 * @brief Initialise the module.
 *
 * @param[in]  config  Pointer to a fully populated ModuleConfig struct.
 *                     Must not be NULL.
 * @param[out] handle  On success, receives a pointer to the allocated handle.
 *                     Must not be NULL.
 * @return PICO_OK on success.
 * @return PICO_ERROR_INVALID_ARG if @p config or @p handle is NULL, or if
 *         a configuration field is out of range.
 * @return PICO_ERROR_NO_DATA if memory allocation fails.
 */
int module_init(const ModuleConfig *config, ModuleHandle **handle);

/**
 * @brief Release all resources held by the module instance.
 *
 * After this call, @p handle is invalid and must not be used again.
 *
 * @param[in] handle  Handle obtained from module_init().  If NULL, the
 *                    function returns immediately without side-effects.
 */
void module_deinit(ModuleHandle *handle);

/* ---------------------------------------------------------------------------
 * Operations
 * --------------------------------------------------------------------------*/

/**
 * @brief Perform the primary operation of the module.
 *
 * @param[in] handle  Valid handle obtained from module_init().
 * @return PICO_OK on success.
 * @return PICO_ERROR_INVALID_ARG if @p handle is NULL.
 */
int module_do_something(ModuleHandle *handle);

#ifdef __cplusplus
}
#endif
