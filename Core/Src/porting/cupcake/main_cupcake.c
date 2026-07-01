/*
 * Cupcake overlay stub — linked into firmware .overlay_cupcake.
 * Replace cupcake.bin on SD with a full build from the port repo when ready.
 */
#include <odroid_system.h>

#include "main.h"
#include "common.h"
#include "appid.h"

void app_main_cupcake(uint8_t load_state, uint8_t start_paused, int8_t save_slot)
{
    (void)load_state;
    (void)start_paused;
    (void)save_slot;

    odroid_system_init(APPID_HOMEBREW, 22050);

    /* Smoke-test stub: brief run then return to the launcher. */
    for (volatile uint32_t i = 0; i < 8000000u; i++) {
        wdog_refresh();
    }
}
