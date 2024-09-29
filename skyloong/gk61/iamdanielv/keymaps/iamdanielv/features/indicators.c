#include "defines.h"
#include "indicators.h"
#include "indicator_queue.h"
#include "fn_mode.h"

// clang-format off
/* LED Matrix
* ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐
* │Esc│ 1 │ 2 │ 3 │ 4 │ 5 │ 6 │ 7 │ 8 │ 9 │ 0 │ - │ = │Bsp│
* │ 0 │ 1 │ 2 │ 3 │ 4 │ 5 │ 6 │ 7 │ 8 │ 9 │10 │11 │12 │13 │
* ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
* │Tab│ Q │ W │ E | R │ T │ Y │ U │ I │ O │ P │ [ │ ] │ \ │
* │14 │15 │16 │17 │18 │19 │20 │21 │22 │23 │24 │25 │26 │27 │
* ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
* │Cap│ A │ S │ D │ F │ G │ H │ J │ K │ L │ ; │ ' │   |Ent│
* │28 │29 │30 │31 │32 │33 │34 │35 │36 │37 │38 │39 │   │40 │
* ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
* │Sft│ Z │ X │ C │ V │ B │ N │ M │ , │ . │ / │   |Sft|   │
* │41 │42 |43 │44 │45 │46 │47 │48 │49 │50 │51 │   │52 │   │
* ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
* │Ctl│GUI│Alt│   │Spc│Spc│Mut│   |Spc|Alt│ Fn│App│   │Ctl|
* │53 │54 │55 │   │56 │57 │58 │   │59 │60 │ 61│62 │   │63 │
* └───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┘
*/
// clang-format on

void blink_numbers(bool isEnabling) {
    for (int i = 1; i <= 12; i++) { // 1(1) to EQL(12)
        if (isEnabling) {
            // enabling, flash white
            indicator_enqueue(i, 200, 3, RGB_WHITE);
        } else {
            // disabling, flash red
            indicator_enqueue(i, 150, 4, RGB_RED);
        }
    }
}

void blink_arrows(void) {
    indicator_enqueue(61, 200, 3, RGB_WHITE); // left
    indicator_enqueue(62, 200, 3, RGB_WHITE); // down
    indicator_enqueue(52, 200, 3, RGB_WHITE); // up
    indicator_enqueue(63, 200, 3, RGB_WHITE); // right
}

void blink_NKRO(bool isEnabling) {
    if (isEnabling) {
        const uint8_t led_indexes[12] = {
            45, 46, 47, 48, 49, // V B N M ,
            33, 34, 35, 36,     // G H J K
            20, 21, 22          // Y U I
        };

        for (int i = 0; i < 12; i++) {
            indicator_enqueue(led_indexes[i], 200, 3, RGB_WHITE);
        }
    } else {
        const uint8_t led_indexes[4] = {
            46, 48, // B M
            34, 35  // H J
        };

        for (int i = 0; i < 4; i++) {
            indicator_enqueue(led_indexes[i], 150, 3, RGB_RED);
        }
    }
}

void highlight_fn_keys(uint8_t led_min, uint8_t led_max) {
    // get the current hsv value
    HSV current_hsv = rgb_matrix_get_hsv();
    // maximize brightness
    current_hsv.v = 255;

    rgb_led_t rgb = hsv_to_rgb(current_hsv);
    rgb_led_t new_rgb = get_complementary_color(rgb, false);
    for (int i = 1; i <= 12; i++) { // 1 to EQL(12)
        RGB_MATRIX_INDICATOR_SET_COLOR(i, new_rgb.r, new_rgb.g, new_rgb.b);
    }
}

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    uint8_t current_layer = get_highest_layer(layer_state);
    if (current_layer == _WIN_LYR) {
        if (rgb_matrix_get_flags() == LED_FLAG_INDICATOR) {
            for (int i = led_min; i < led_max; i++) {
                rgb_matrix_set_color(i, 0, 0, 0);
            }
            // rgb_matrix_set_color_all(0, 0, 0);
        }
    }

    if (IS_LAYER_ON(_WIN_FN_LYR) ||
        // IS_LAYER_ON(_CTL_LYR) ||  //ignore the CTL layer since we want to see RGB effects on that layer
        IS_LAYER_ON(_NUM_LYR) ||
        IS_LAYER_ON(_NAV_LYR) ||
        IS_LAYER_ON(_FN_LYR)) {
        // we are in a custom layer, clear all background colors
        // this will make our custom colors stand out more
        for (int i = led_min; i <= led_max; i++) {
            RGB_MATRIX_INDICATOR_SET_COLOR(i, 0, 0, 0);
        }
    }

    if (fn_mode_enabled) {
        highlight_fn_keys(led_min, led_max);
    }

    if (IS_LAYER_ON(_WIN_FN_LYR)) {
        // this layer has many functions, so just change the whole color
        for (int i = led_min; i <= led_max; i++) {
            RGB_MATRIX_INDICATOR_SET_COLOR(i, 0xFF, 0x80, 0x00);
        }

        // no matter what, this layer also uses fn keys
        highlight_fn_keys(led_min, led_max);

        // highlight right shift as moving to ctl layer
        RGB_MATRIX_INDICATOR_SET_COLOR(52, 0x7A, 0x00, 0xFF);

        // highlight the fn button
        RGB_MATRIX_INDICATOR_SET_COLOR(RIGHT_FN_KEY_INDEX, 128, 128, 128);

        // highlight the toggle buttons
        RGB_MATRIX_INDICATOR_SET_COLOR(RIGHT_CTL_KEY_INDEX, 0x7A, 0x00, 0xFF);
        RGB_MATRIX_INDICATOR_SET_COLOR(RIGHT_MENU_KEY_INDEX, 0, 255, 0);
        RGB_MATRIX_INDICATOR_SET_COLOR(RIGHT_ALT_KEY_INDEX, 0, 0, 255);
    }

    if (IS_LAYER_ON(_CTL_LYR)) {
        // #define CAPS_LOCK_INDEX 28
        // #define WIN_MOD_INDEX 16
        // #define MAC_MOD_INDEX 17
        const uint8_t led_indexes[4] = {
            RIGHT_CTL_KEY_INDEX, // use the right ctl key as indicator

            // RGB Control
            24, // p = 24 for persistent color
            // 25, // [ = 25 RGB_RMOD
            // 26, // ] = 26 RGB_MOD
            // 27, // \ = 27 RGB Toggle

            // 38, // ; = 38 RGB Speed Down
            // 39, // ' = 39 RGB Speed Up

            47, // n = 47 NKR toggle
            // 48, // m = 48 RGB Hue Increase
            // 49, // , = 49 RGB Decrease Bright
            // 50, // . = 50 RGB Increase Bright
            LEFT_CTL_KEY_INDEX // used for Fn Swap
        };
        for (int i = 0; i < 4; i++) {
            RGB_MATRIX_INDICATOR_SET_COLOR(led_indexes[i], 0x7A, 0x00, 0xFF);
        }

        const uint8_t led_off_indexes[4] = {
            // turn off some of the LEDS to make it easier to see our indicators
            29,         // A
            14, 28, 41, // TAB, CAPS, LSFT
        };
        for (int i = 0; i < 4; i++) {
            RGB_MATRIX_INDICATOR_SET_COLOR(led_off_indexes[i], 0x00, 0x00, 0x00);
        }

        // highlight Q as reset
        RGB_MATRIX_INDICATOR_SET_COLOR(15, 0xFF, 0x00, 0x00);

        // highlight Z as clear eeprom
        RGB_MATRIX_INDICATOR_SET_COLOR(42, 0x7A, 0x00, 0xFF);
    }

    if (IS_LAYER_ON(_NUM_LYR)) {
        const uint8_t led_indexes[20] = {
            RIGHT_MENU_KEY_INDEX, // use the Right Menu key as indicator

            //  Light up the numpad to make it easier to see
            6,                      // 6 = 6 is used as numlock and starts the numpad
            7,  8,  9,  10, 11, 12, // 7, 8, 9, 0 = 7, 8, 9, Asterisk, minus, equals
            21, 22, 23, 24,         // U, I, O, P = 4, 5, 6, Plus
            35, 36, 37, 38,         // J, K, L, ; = 1, 2, 3, Enter
            48, 49, 50, 51          // M, ,, ., / = 0, dot, dot, slash
        };

        for (int i = 0; i < 20; i++) {
            RGB_MATRIX_INDICATOR_SET_COLOR(led_indexes[i], 0, 255, 0);
        }
    }

    if (IS_LAYER_ON(_NAV_LYR)) {
        const uint8_t led_indexes[11] = {
            RIGHT_ALT_KEY_INDEX, // use the right alt key as indicator

            //  first row
            15, // q = 15 Home
            16, // w = 16 Up arrow
            17, // e = 17 End
            18, // r = 18 Ctrl-R
            19, // t = 19 PgUp
            // second row
            29, // a = 29 Left arrow
            30, // s = 30 Down arrow
            31, // d = 31 Right arrow
            32, // f = 32 Ctrl-F
            33, // g = 33 PgDn
        };

        for (int i = 0; i < 11; i++) {
            RGB_MATRIX_INDICATOR_SET_COLOR(led_indexes[i], 0, 0, 255);
        }
    }

    if (IS_LAYER_ON(_FN_LYR)) {
        // highlight the fn button
        RGB_MATRIX_INDICATOR_SET_COLOR(RIGHT_FN_KEY_INDEX, 128, 128, 128);

        // highlight right shift as moving to ctl layer
        RGB_MATRIX_INDICATOR_SET_COLOR(52, 0x7A, 0x00, 0xFF);

        // highlight the toggle buttons
        RGB_MATRIX_INDICATOR_SET_COLOR(RIGHT_CTL_KEY_INDEX, 0x7A, 0x00, 0xFF);
        RGB_MATRIX_INDICATOR_SET_COLOR(RIGHT_MENU_KEY_INDEX, 0, 255, 0);
        RGB_MATRIX_INDICATOR_SET_COLOR(RIGHT_ALT_KEY_INDEX, 0, 0, 255);

        // highlight the aux buttons on right of keyboard
        const uint8_t led_indexes[7] = {
            7, 8, 9, 10, 11, 12, 13, // first row for media keys
        };

        for (int i = 0; i < 7; i++) {
            RGB_MATRIX_INDICATOR_SET_COLOR(led_indexes[i], 128, 128, 128);
        }
    }

    process_indicator_queue(led_min, led_max);

    return true;
}
