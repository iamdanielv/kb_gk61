#include "indicator_queue.h"

void indicator_enqueue(uint8_t led_index, uint32_t interval, uint8_t times_to_flash, uint8_t r, uint8_t g, uint8_t b) {
    for (int i = 0; i < INDICATOR_QUEUE_MAX; i++) {
        if (!indicator_queue[i].active) {
            // this queue position is not active, so we can use it
            indicator_queue[i].active = true;
            indicator_queue[i].led_index = led_index;
            indicator_queue[i].last_update = timer_read32();
            indicator_queue[i].interval = interval;
            indicator_queue[i].times_to_flash = times_to_flash * 2;
            indicator_queue[i].r = r;
            indicator_queue[i].g = g;
            indicator_queue[i].b = b;
            break;
        }
    }
}

void process_indicator_queue(uint8_t led_min, uint8_t led_max) {
    for (int i = 0; i < INDICATOR_QUEUE_MAX; i++) {
        if (indicator_queue[i].active) {
            // this queue position is active, process it
            if (timer_elapsed32(indicator_queue[i].last_update) >= indicator_queue[i].interval) {
                // the timer has elapsed, perform the action

                indicator_queue[i].last_update = timer_read32(); // reset the timer to now

                if (indicator_queue[i].times_to_flash) {
                    indicator_queue[i].times_to_flash--;
                }

                if (indicator_queue[i].times_to_flash <= 0) {
                    // we have flashed as many times as requested
                    // clear this queue spot
                    indicator_queue[i].active = false;
                    indicator_queue[i].last_update = 0x00;
                }
            }

            if (indicator_queue[i].times_to_flash % 2) {
                RGB_MATRIX_INDICATOR_SET_COLOR(indicator_queue[i].led_index, indicator_queue[i].r, indicator_queue[i].g, indicator_queue[i].b);
            } else {
                uint8_t r = 0xFF - indicator_queue[i].r;
                uint8_t g = 0xFF - indicator_queue[i].g;
                uint8_t b = 0xFF - indicator_queue[i].b;
                // scale the alternate flash color to be not as bright
                if( r > 0x80) { r = r - 0x80;}
                if( g > 0x80) { g = g - 0x80;}
                if( b > 0x80) { b = b - 0x80;}
                RGB_MATRIX_INDICATOR_SET_COLOR(indicator_queue[i].led_index, r,g,b);
            }
        }
    }
}
