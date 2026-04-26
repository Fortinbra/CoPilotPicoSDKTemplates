/**
 * @file main.c
 * @brief Application entry point for a Pico SDK project.
 *
 * Replace this file with your own application logic.
 * Keep main() thin: initialise hardware, start tasks or the scheduler, then
 * enter the main loop (or hand off to an RTOS).
 */

#include <stdint.h>
#include "pico/stdlib.h"

/* ---------------------------------------------------------------------------
 * Private helpers
 * --------------------------------------------------------------------------*/

/**
 * @brief Initialise all hardware peripherals required by the application.
 * @return PICO_OK on success, or a PICO_ERROR_* code on failure.
 */
static int hardware_init(void) {
    stdio_init_all();

    /* TODO: initialise your peripherals here, e.g.:
     *   my_driver_init(&driver_config);
     *   my_component_init(&component_config, &component_handle);
     */

    return PICO_OK;
}

/* ---------------------------------------------------------------------------
 * Entry point
 * --------------------------------------------------------------------------*/

int main(void) {
    int rc = hardware_init();
    if (rc != PICO_OK) {
        /* Unrecoverable initialisation failure — halt. */
        while (true) {
            tight_loop_contents();
        }
    }

    /* Main application loop. */
    while (true) {
        /* TODO: implement your application logic here. */
        tight_loop_contents();
    }

    /* Unreachable. */
    return 0;
}
