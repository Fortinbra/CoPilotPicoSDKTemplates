/**
 * @file module.c
 * @brief Implementation of the <module_name> module.
 *
 * This file contains private implementation details.  Only the interface
 * declared in module.h is part of the public API.
 */

#include "module.h"

#include <stdlib.h>
#include <string.h>

/* ---------------------------------------------------------------------------
 * Private types
 * --------------------------------------------------------------------------*/

/**
 * @brief Internal state for a module instance.
 *
 * The definition is kept private here; callers see only a forward-declared
 * pointer (ModuleHandle *).
 */
struct ModuleHandle {
    ModuleConfig config; /**< Copy of the configuration provided at init. */
    bool         active; /**< True if the module is in the operational state. */
    /* TODO: add hardware handles, buffers, or other state here. */
};

/* ---------------------------------------------------------------------------
 * Private helpers
 * --------------------------------------------------------------------------*/

/**
 * @brief Validate a ModuleConfig struct.
 * @return true if the configuration is valid.
 */
static bool config_is_valid(const ModuleConfig *config) {
    if (config == NULL) {
        return false;
    }
    /* TODO: validate individual fields */
    return true;
}

/* ---------------------------------------------------------------------------
 * Lifecycle
 * --------------------------------------------------------------------------*/

int module_init(const ModuleConfig *config, ModuleHandle **handle) {
    if (!config_is_valid(config) || handle == NULL) {
        return PICO_ERROR_INVALID_ARG;
    }

    ModuleHandle *h = calloc(1, sizeof(ModuleHandle));
    if (h == NULL) {
        return PICO_ERROR_NO_DATA;
    }

    h->config = *config;
    h->active = false;

    /* TODO: initialise hardware resources */

    h->active = true;
    *handle   = h;
    return PICO_OK;
}

void module_deinit(ModuleHandle *handle) {
    if (handle == NULL) {
        return;
    }

    /* TODO: release hardware resources */

    handle->active = false;
    free(handle);
}

/* ---------------------------------------------------------------------------
 * Operations
 * --------------------------------------------------------------------------*/

int module_do_something(ModuleHandle *handle) {
    if (handle == NULL || !handle->active) {
        return PICO_ERROR_INVALID_ARG;
    }

    /* TODO: implement the operation */

    return PICO_OK;
}
