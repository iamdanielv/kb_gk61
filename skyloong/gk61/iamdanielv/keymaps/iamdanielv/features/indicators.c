#include "defines.h"
#include "indicators.h"
#include "indicator_queue.h"
#include "fn_mode.h"
#include "color.h"
#include "quantum.h"
#include "rgb_matrix.h"

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

rgb_t get_complementary_color(rgb_t rgb_led, bool darken) {
    uint8_t new_r = 0xFF - rgb_led.r;
    uint8_t new_g = 0xFF - rgb_led.g;
    uint8_t new_b = 0xFF - rgb_led.b;

    if (darken) {
        // darken the new color by shifting all values down
        if( new_r > 0x80) { new_r = new_r - 0x80;}
        if( new_g > 0x80) { new_g = new_g - 0x80;}
        if( new_b > 0x80) { new_b = new_b - 0x80;}
    }

    return (rgb_t){.r = new_r, .g = new_g, .b = new_b};
}

hsv_t get_base_hsv_color_shifted(bool clockwise) {
    // get the current base hsv value
    hsv_t base_color = rgb_matrix_get_hsv();
    base_color.v = 255;
    if(clockwise){
        if(base_color.h > 21){
            base_color.h = base_color.h - 21;
        } else {
            base_color.h = 255 - base_color.h;
        }
    } else {
        if(base_color.h < 234){
            base_color.h = base_color.h + 21;
        } else {
            base_color.h = base_color.h - 234;
        }
    }
    return base_color;
}

hsv_t get_base_hsv_color_shifted_quarter(bool clockwise) {
        // get the current base hsv value
        hsv_t current_hsv = rgb_matrix_get_hsv();

        // maximize brightness
        current_hsv.v = 255;
        // offset hue by a quarter
        if(clockwise){
            if(current_hsv.h > 64){
                current_hsv.h = current_hsv.h - 64;
            } else {
                current_hsv.h = 255 - current_hsv.h;
            }
        } else {
            if(current_hsv.h < 191){
                current_hsv.h = current_hsv.h + 64;
            } else {
                current_hsv.h = current_hsv.h - 191;
            }
        }
        return current_hsv;
}

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
    indicator_enqueue(RIGHT_FN_KEY_INDEX, 200, 3, RGB_WHITE); // left
    indicator_enqueue(RIGHT_MENU_KEY_INDEX, 200, 3, RGB_WHITE); // down
    indicator_enqueue(RIGHT_SFT_KEY_INDEX, 200, 3, RGB_WHITE); // up
    indicator_enqueue(RIGHT_CTL_KEY_INDEX, 200, 3, RGB_WHITE); // right
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
    hsv_t current_hsv = rgb_matrix_get_hsv();

    // maximize brightness
    current_hsv.v = 255;

    rgb_t rgb = hsv_to_rgb(current_hsv);
    rgb_t new_rgb = get_complementary_color(rgb, false);
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
        }
    }

    // clear out all the colors
    if (IS_LAYER_ON(_WIN_FN_LYR) ||
        // IS_LAYER_ON(_CTL_LYR) ||  //ignore the CTL layer since we want to see RGB effects on that layer
        IS_LAYER_ON(_NUM_LYR)
        //|| IS_LAYER_ON(_ARROW_LYR)) //ignore the ARROW layer
    ){
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

        // use the base color to highlight features of this layer
        hsv_t current_hsv = rgb_matrix_get_hsv();
        // maximize brightness
        current_hsv.v = 255;
        rgb_t new_rgb = hsv_to_rgb(current_hsv);

        //highlight the arrow keys
        RGB_MATRIX_INDICATOR_SET_COLOR(22, new_rgb.r, new_rgb.g, new_rgb.b); // up - I
        RGB_MATRIX_INDICATOR_SET_COLOR(35, new_rgb.r, new_rgb.g, new_rgb.b); // left - J
        RGB_MATRIX_INDICATOR_SET_COLOR(36, new_rgb.r, new_rgb.g, new_rgb.b); // down - K
        RGB_MATRIX_INDICATOR_SET_COLOR(37, new_rgb.r, new_rgb.g, new_rgb.b); // right - L

        // higlight the f key to show home row
        RGB_MATRIX_INDICATOR_SET_COLOR(32, new_rgb.r, new_rgb.g, new_rgb.b); // f key

        // layer lock key
        RGB_MATRIX_INDICATOR_SET_COLOR(LEFT_WIN_KEY_INDEX, 0xFF,0x00, 0x00); // left GUI/win
    }

    if (IS_LAYER_ON(_CTL_LYR)) {
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
            RGB_MATRIX_INDICATOR_SET_COLOR(led_indexes[i], 0xFF, 0xFF, 0xFF);
        }

        const uint8_t led_off_indexes[4] = {
            // turn off some of the LEDS to make it easier to see our indicators
            29,         // A
            14, 28, 41, // TAB, CAPS, LSFT
        };
        for (int i = 0; i < 4; i++) {
            RGB_MATRIX_INDICATOR_SET_COLOR(led_off_indexes[i], 0x00, 0x00, 0x00);
        }

        // highlight number layer toggle
        hsv_t base_color_offset_ccw = get_base_hsv_color_shifted_quarter(false);
        rgb_t num_lyr_color = hsv_to_rgb(base_color_offset_ccw);
        RGB_MATRIX_INDICATOR_SET_COLOR(RIGHT_MENU_KEY_INDEX, num_lyr_color.r, num_lyr_color.g, num_lyr_color.b);

        // highlight arrow layer toggle
        hsv_t base_color_offset_cw = get_base_hsv_color_shifted_quarter(true);
        rgb_t arrow_lyr_color = hsv_to_rgb(base_color_offset_cw);
        RGB_MATRIX_INDICATOR_SET_COLOR(RIGHT_ALT_KEY_INDEX, arrow_lyr_color.r, arrow_lyr_color.g, arrow_lyr_color.b);

        // highlight Q as reset
        RGB_MATRIX_INDICATOR_SET_COLOR(15, 0xFF, 0x00, 0x00);

        // highlight Z as clear eeprom
        RGB_MATRIX_INDICATOR_SET_COLOR(42, 0x7A, 0x00, 0xFF);
    }

    if (IS_LAYER_ON(_NUM_LYR)) {
        const uint8_t led_indexes[24] = {
            RIGHT_MENU_KEY_INDEX, // use the Right Menu key as indicator

            //  Light up the numpad to make it easier to see
            6,                      // 6 = 6 is used as numlock and starts the numpad
            7,  8,  9,  10, 11, 12, // 7, 8, 9, 0 = 7, 8, 9, Asterisk, minus, equals
            21, 22, 23, 24,         // U, I, O, P = 4, 5, 6, Plus
            35, 36, 37, 38,         // J, K, L, ; = 1, 2, 3, Enter
            48, 49, 50, 51,         // M, ,, ., / = 0, dot, dot, slash
            16, 29, 30, 31          // w, a, s, d used to move the mouse
        };

        // get the current base hsv value shifted by a quarter counter clockwise
        hsv_t base_color_offset = get_base_hsv_color_shifted_quarter(false);
        rgb_t num_lyr_color = hsv_to_rgb(base_color_offset);

        for (int i = 0; i < 24; i++) {
            RGB_MATRIX_INDICATOR_SET_COLOR(led_indexes[i], num_lyr_color.r, num_lyr_color.g, num_lyr_color.b);
        }
    }

    if (IS_LAYER_ON(_ARROW_LYR)) {
        // get the current base hsv value shifted by a quarter clockwise
        hsv_t base_color_offset = get_base_hsv_color_shifted_quarter(true);
        rgb_t arrow_lyr_color = hsv_to_rgb(base_color_offset);

        // highlight the arrow keys
        RGB_MATRIX_INDICATOR_SET_COLOR(RIGHT_SFT_KEY_INDEX, arrow_lyr_color.r, arrow_lyr_color.g, arrow_lyr_color.b); // up (right shift)
        RGB_MATRIX_INDICATOR_SET_COLOR(RIGHT_FN_KEY_INDEX, arrow_lyr_color.r, arrow_lyr_color.g, arrow_lyr_color.b); //left
        RGB_MATRIX_INDICATOR_SET_COLOR(RIGHT_MENU_KEY_INDEX, arrow_lyr_color.r, arrow_lyr_color.g, arrow_lyr_color.b); // down
        RGB_MATRIX_INDICATOR_SET_COLOR(RIGHT_CTL_KEY_INDEX, arrow_lyr_color.r, arrow_lyr_color.g, arrow_lyr_color.b); // right

        // layer lock key
        RGB_MATRIX_INDICATOR_SET_COLOR(27, 0xFF, 0x00, 0x00); // back slash

        // volume up and down
        RGB_MATRIX_INDICATOR_SET_COLOR(11, arrow_lyr_color.r, arrow_lyr_color.g, arrow_lyr_color.b); // volume down - -
        RGB_MATRIX_INDICATOR_SET_COLOR(12, arrow_lyr_color.r, arrow_lyr_color.g, arrow_lyr_color.b); // volume up - +
    }

    process_indicator_queue(led_min, led_max);

    return true;
}
