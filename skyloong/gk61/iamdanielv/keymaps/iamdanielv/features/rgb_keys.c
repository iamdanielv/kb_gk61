#include "indicators.h"
#include "rgb_keys.h"
#include "indicator_queue.h"

bool process_rgb_keys(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case RGB_TOG:
            if (record->event.pressed) {
                switch (rgb_matrix_get_flags()) {
                    case LED_FLAG_ALL: {
                        rgb_matrix_set_flags_noeeprom(LED_FLAG_INDICATOR);
                        // rgb_matrix_set_color_all(0, 0, 0);
                        indicator_enqueue(57, 200, 3, INDICATOR_RGB_DARK_RED); // blink space too
                        indicator_enqueue(55, 200, 3, INDICATOR_RGB_DARK_RED); // blink left alt
                        indicator_enqueue(60, 200, 3, INDICATOR_RGB_DARK_RED); // blink right alt
                    } break;
                    default: {
                        HSV current_hsv = rgb_matrix_get_hsv();
                        RGB rgb         = hsv_to_rgb(current_hsv);
                        rgb_matrix_set_color_all(rgb.r, rgb.g, rgb.b);
                        rgb_matrix_set_flags_noeeprom(LED_FLAG_ALL);
                        indicator_enqueue(57, 200, 3, RGB_WHITE); // blink space too
                        indicator_enqueue(55, 200, 3, RGB_WHITE); // blink left alt
                        indicator_enqueue(60, 200, 3, RGB_WHITE); // blink right alt
                    } break;
                }
            }
            return false;
        case RGB_MOD:
            if (record->event.pressed) {
                rgb_matrix_step_noeeprom();
            }
            return false;
        case RGB_RMOD:
            if (record->event.pressed) {
                rgb_matrix_step_reverse_noeeprom();
            }
            return false;
        case RGB_M_P:
            if (record->event.pressed) {
                rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
                indicator_enqueue(57, 200, 3, RGB_WHITE); // blink space too
                indicator_enqueue(55, 200, 3, RGB_WHITE); // blink left alt
                indicator_enqueue(60, 200, 3, RGB_WHITE); // blink right alt
            }
            return false;
        case RGB_SPI:
            if (record->event.pressed) {
                if (rgb_matrix_get_speed() >= (255 - RGB_MATRIX_SPD_STEP)) {
                    // this update would put us at max
                    blink_arrows();
                    indicator_enqueue(39, 200, 4, RGB_RED);   // ' - SPI
                    indicator_enqueue(38, 200, 2, RGB_BLACK); // ; - SPD
                    indicator_enqueue(57, 200, 3, RGB_WHITE); // blink space too
                }
                rgb_matrix_increase_speed_noeeprom();
            }
            return false;
        case RGB_SPD:
            if (record->event.pressed) {
                if (rgb_matrix_get_speed() <= RGB_MATRIX_SPD_STEP) {
                    blink_arrows();
                    indicator_enqueue(39, 200, 2, RGB_BLACK); // ' - SPI
                    indicator_enqueue(38, 200, 4, RGB_RED);   // ; - SPD
                    indicator_enqueue(57, 200, 3, RGB_WHITE); // blink space too
                    rgb_matrix_set_speed_noeeprom(RGB_MATRIX_SPD_STEP);
                }
                rgb_matrix_decrease_speed_noeeprom();
            }
            return false;
        case RGB_HUI:
            if (record->event.pressed) {
                if (rgb_matrix_get_hue() >= (255 - RGB_MATRIX_HUE_STEP)) {
                    // this update would put us at max
                    indicator_enqueue(23, 200, 4, INDICATOR_RGB_DARK_RED); // O - HUI
                    indicator_enqueue(22, 200, 2, RGB_BLACK);              // I - HUD
                    indicator_enqueue(57, 200, 3, RGB_WHITE);              // blink space too
                } else {
                    indicator_enqueue(23, 150, 1, RGB_WHITE); // O - HUI
                }
                rgb_matrix_increase_hue_noeeprom();
            }
            return false;
        case RGB_HUD:
            if (record->event.pressed) {
                if (rgb_matrix_get_hue() <= RGB_MATRIX_HUE_STEP) {
                    // this update would put us at min
                    indicator_enqueue(23, 200, 2, RGB_BLACK);              // O - HUI
                    indicator_enqueue(22, 200, 4, INDICATOR_RGB_DARK_RED); // I - HUD
                    indicator_enqueue(57, 200, 3, RGB_WHITE);              // blink space too
                } else {
                    indicator_enqueue(22, 150, 1, RGB_WHITE); // I - HUD
                }
                rgb_matrix_decrease_hue_noeeprom();
            }
            return false;
        case RGB_SAI:
            if (record->event.pressed) {
                if (rgb_matrix_get_sat() >= (255 - RGB_MATRIX_SAT_STEP)) {
                    // this update would put us at max
                    indicator_enqueue(37, 200, 4, INDICATOR_RGB_DARK_RED); // L - SAI
                    indicator_enqueue(36, 200, 2, RGB_BLACK);              // K - SAD
                    indicator_enqueue(57, 200, 3, RGB_WHITE);              // blink space too
                } else {
                    indicator_enqueue(37, 150, 1, RGB_WHITE); // L - SAI
                }
                rgb_matrix_increase_sat_noeeprom();
            }
            return false;
        case RGB_SAD:
            if (record->event.pressed) {
                if (rgb_matrix_get_sat() <= RGB_MATRIX_SAT_STEP) {
                    // this update would put us at min
                    indicator_enqueue(37, 200, 2, RGB_BLACK);              // L - SAI
                    indicator_enqueue(36, 200, 4, INDICATOR_RGB_DARK_RED); // K - SAD
                    indicator_enqueue(57, 200, 3, RGB_WHITE);              // blink space too
                } else {
                    indicator_enqueue(36, 150, 1, RGB_WHITE); // K - SAD
                }
                rgb_matrix_decrease_sat_noeeprom();
            }
            return false;
        case RGB_VAI:
            if (record->event.pressed) {
                if (rgb_matrix_get_val() >= (RGB_MATRIX_MAXIMUM_BRIGHTNESS - RGB_MATRIX_VAL_STEP)) {
                    indicator_enqueue(50, 200, 4, RGB_RED);   // . - VAI
                    indicator_enqueue(49, 200, 2, RGB_BLACK); // , - VAD
                    indicator_enqueue(57, 200, 3, RGB_WHITE); // blink space too
                    blink_arrows();
                } else {
                    indicator_enqueue(50, 150, 1, RGB_WHITE); // . - VAI
                }
                rgb_matrix_increase_val_noeeprom();
            }
            return false;
        case RGB_VAD:
            if (record->event.pressed) {
                if (rgb_matrix_get_val() <= RGB_MATRIX_VAL_STEP) {
                    indicator_enqueue(50, 200, 2, RGB_BLACK); // . - VAI
                    indicator_enqueue(49, 200, 4, RGB_RED);   // , - VAD
                    indicator_enqueue(57, 200, 3, RGB_WHITE); // blink space too
                    blink_arrows();
                } else {
                    indicator_enqueue(49, 150, 1, RGB_WHITE); // , - VAD
                }
                rgb_matrix_decrease_val_noeeprom();
            }
            return false;
    }
    return true;
}
