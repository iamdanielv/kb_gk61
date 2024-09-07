// Copyright 2024 DV (@iamdanielv)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

#include "features/indicator_queue.h"
#include "features/fn_mode.h"

enum layer_names {
    _WIN_LYR,     // 0
    _WIN_FN_LYR,  // 1
    _CTL_LYR,     // 2
    _NUM_LYR,     // 3
    _NAV_LYR,     // 4
    _FN_LYR,      // 5
};

void blink_arrows(void);
void blink_NKRO(bool);
void blink_numbers(bool);
void highlight_fn_keys(uint8_t led_min, uint8_t led_max);

// ***********************
// * Keyboard Management *
// ***********************

// from https://thomasbaart.nl/2018/12/13/qmk-basics-tap-dance/
void safe_reset(tap_dance_state_t *state, void *user_data) {
    if (state->count >= 3) {
        // Reset the keyboard if you tap the key more than three times
        reset_keyboard();
        reset_tap_dance(state);
    }
}

void safe_clear(tap_dance_state_t *state, void *user_data) {
    if (state->count >= 3) {
        // clear eprom if you tap the key more than three times
        eeconfig_init();
        soft_reset_keyboard();
        reset_tap_dance(state);
    }
}

// ************
// * Tap Hold *
// ************
enum tap_hold_keys {
    _DN_MU,   // Tap--KC_DOWN  and  Hold--KC_APP
    TD_RESET, // require 3 taps to reset board
    TD_CLEAR, // require 3 taps to clear eeprom
    TD_CTL_TG // require double tap to enable ctl layer
};

typedef struct {
    uint16_t on_tap;  // key to send on tap
    uint16_t on_hold; // key to send on hold
    uint16_t chosen;  // key to "release" when finished
                      // the value for it is set in td_tap_hold_finished
} td_tap_hold_t;


void td_tap_hold_finished(tap_dance_state_t *state, void *user_data) {
    td_tap_hold_t *tap_hold = (td_tap_hold_t *)user_data;

    if (state->pressed) {
        if (state->count == 1
#ifndef PERMISSIVE_HOLD
            && !state->interrupted
#endif
        ) {
            // "press" the key specified on_hold
            register_code16(tap_hold->on_hold);
            // keep track of which key was used
            tap_hold->chosen = tap_hold->on_hold;
        } else {
            // "press" the key specified on_tap
            register_code16(tap_hold->on_tap);
            // keep track of which key was used
            tap_hold->chosen = tap_hold->on_tap;
        }
    }
}

void td_tap_hold_reset(tap_dance_state_t *state, void *user_data) {
    td_tap_hold_t *tap_hold = (td_tap_hold_t *)user_data;

    if (tap_hold->chosen) {
        // there is something to cleanup
        // release the key that was "pressed"
        unregister_code16(tap_hold->chosen);

        // reset the key to 0 to signify nothing is selected
        tap_hold->chosen = 0;
    }
}

#define ACTION_TAP_DANCE_TAP_HOLD(on_tap, on_hold) \
    { .fn = {NULL, td_tap_hold_finished, td_tap_hold_reset}, .user_data = (void *)&((td_tap_hold_t){on_tap, on_hold, 0}), }


// *****************************
// * Custom processing of keys *
// *****************************
enum custom_keycodes {
    KC_SWP_FN = SAFE_RANGE
};

// clang-format off
tap_dance_action_t tap_dance_actions[] = {
    [_DN_MU]    = ACTION_TAP_DANCE_TAP_HOLD(KC_DOWN, KC_APP),  // on Tap: "DOWN" on Hold:"App"
    [TD_RESET]  = ACTION_TAP_DANCE_FN(safe_reset),
    [TD_CLEAR]  = ACTION_TAP_DANCE_FN(safe_clear),
    [TD_CTL_TG] = ACTION_TAP_DANCE_LAYER_TOGGLE(KC_RCTL, _CTL_LYR)
};
// clang-format on

bool fn_mode_enabled = false;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

    tap_dance_action_t *action;

    if (keycode == KC_SWP_FN) {
        if (record->event.pressed) {
            fn_mode_enabled = !fn_mode_enabled;
            blink_numbers(fn_mode_enabled);
        }
        return false;
    }

    if (!process_fn_mode(keycode, record)) { return false; }

    switch (keycode) {
        case QK_MAGIC_TOGGLE_NKRO:
            if (record->event.pressed) {
                clear_keyboard(); // clear first buffer to prevent stuck keys
                wait_ms(50);
                keymap_config.nkro = !keymap_config.nkro;
                blink_NKRO(keymap_config.nkro);
                wait_ms(50);
                clear_keyboard(); // clear first buffer to prevent stuck keys
                wait_ms(50);
            }
            return false;
        case RGB_TOG:
            if (record->event.pressed) {
                switch (rgb_matrix_get_flags()) {
                    case LED_FLAG_ALL: {
                        rgb_matrix_set_flags_noeeprom(LED_FLAG_INDICATOR);
                        //rgb_matrix_set_color_all(0, 0, 0);
                    } break;
                    default: {
                        HSV current_hsv = rgb_matrix_get_hsv();
                        RGB rgb = hsv_to_rgb(current_hsv);
                        rgb_matrix_set_color_all(rgb.r, rgb.g, rgb.b);
                        rgb_matrix_set_flags_noeeprom(LED_FLAG_ALL);
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
            }
            return false;
        case RGB_VAI:
            if (record->event.pressed) {
                rgb_matrix_set_flags_noeeprom(LED_FLAG_ALL);
                if (rgb_matrix_get_val() >= (RGB_MATRIX_MAXIMUM_BRIGHTNESS - RGB_MATRIX_VAL_STEP)) {
                    blink_arrows();
                }
                rgb_matrix_increase_val_noeeprom();
            }
            return false;
        case RGB_VAD:
            if (record->event.pressed) {
                if (rgb_matrix_get_val() <= RGB_MATRIX_VAL_STEP) {
                    blink_arrows();
                }
                rgb_matrix_decrease_val_noeeprom();
            }
            return false;
        case RGB_SPI:
            if (record->event.pressed) {
                if ( rgb_matrix_get_speed() >= (255 - RGB_MATRIX_SPD_STEP)) {
                    // this update would put us at max
                    blink_arrows();
                }

                rgb_matrix_increase_speed_noeeprom();
            }
            return false;
        case RGB_SPD:
            if (record->event.pressed) {
                if (rgb_matrix_get_speed() <= RGB_MATRIX_SPD_STEP) {
                    blink_arrows();
                    rgb_matrix_set_speed_noeeprom(RGB_MATRIX_SPD_STEP);
                }
                rgb_matrix_decrease_speed_noeeprom();
            }
            return false;
        case RGB_HUI:
            if (record->event.pressed) {
                rgb_matrix_increase_hue_noeeprom();
            }
            return false;

        case TD(_DN_MU):
            action = &tap_dance_actions[QK_TAP_DANCE_GET_INDEX(keycode)];
            if (!record->event.pressed &&
                action->state.count &&
                !action->state.finished) {
                td_tap_hold_t *tap_hold = (td_tap_hold_t *)action->user_data;
                tap_code16(tap_hold->on_tap);
            }

        default:
            return true;
    }

    return true;
}


// ******************************
// * Aliases to simplify keymap *
// ******************************
#define FN_W_CAPS LT(_WIN_FN_LYR, KC_CAPS)
#define FN_W_RALT LT(_WIN_FN_LYR, KC_RALT)

#define MY_UNDO   C(KC_Z)
#define MY_CUT    C(KC_X)
#define MY_COPY   C(KC_INS)
#define MY_PASTE  S(KC_INS)

#define FN_LEFT   LT(_FN_LYR, KC_LEFT)
#define APP_DWN   TD(_DN_MU)
#define RCTL_RGT  RCTL_T(KC_RIGHT)
#define RSFT_UP   RSFT_T(KC_UP)

#define TG_NAV    TG(_NAV_LYR)
#define TG_NUM    TG(_NUM_LYR)
#define TG_CTL    TG(_CTL_LYR)
#define MO_CTL    MO(_CTL_LYR)
#define TD_TG_CTL TD(TD_CTL_TG)
#define TD_KB_RST TD(TD_RESET)
#define TD_KB_CLR TD(TD_CLEAR)


uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    // to use this, make sure to put:
    //#define TAPPING_TERM_PER_KEY in config.h
    switch (keycode) {
        case RSFT_UP:
            //return TAPPING_TERM - 80; //right shift hold time less than default tapping term 80ms
            return 115;
        default:
            return TAPPING_TERM;
    }
}

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
     * ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐
     * │Esc│ 1 │ 2 │ 3 │ 4 │ 5 │ 6 │ 7 │ 8 │ 9 │ 0 │ - │ = │Bsp│
     * ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
     * │Tab│ Q │ W │ E | R │ T │ Y │ U │ I │ O │ P │ [ │ ] │ \ │
     * ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
     * │Cap│ A │ S │ D │ F │ G │ H │ J │ K │ L │ ; │ ' │   |Ent│
     * ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
     * │Sft│ Z │ X │ C │ V │ B │ N │ M │ , │ . │ / │   |Sft|   │
     * ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
     * │Ctl│GUI│Alt│   │Spc│Spc│Mut│   |Spc|Alt│ Fn│App│   │Ctl|
     * └───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┘
     */
    [_WIN_LYR] = LAYOUT_all(            // 0
       KC_ESC,    KC_1,      KC_2,      KC_3,      KC_4,      KC_5,      KC_6,      KC_7,      KC_8,      KC_9,      KC_0,     KC_MINS,  KC_EQL,   KC_BSPC,
       KC_TAB,    KC_Q,      KC_W,      KC_E,      KC_R,      KC_T,      KC_Y,      KC_U,      KC_I,      KC_O,      KC_P,     KC_LBRC,  KC_RBRC,  KC_BSLS,
       FN_W_CAPS, KC_A,      KC_S,      KC_D,      KC_F,      KC_G,      KC_H,      KC_J,      KC_K,      KC_L,      KC_SCLN,  KC_QUOT,            KC_ENT,
       KC_LSFT,   KC_Z,      KC_X,      KC_C,      KC_V,      KC_B,      KC_N,      KC_M,      KC_COMM,   KC_DOT,    KC_SLSH,            RSFT_UP,
       KC_LCTL,   KC_LGUI,   KC_LALT,              KC_SPC,    KC_SPC,    KC_MUTE,              KC_SPC,    FN_W_RALT, FN_LEFT,  APP_DWN,            RCTL_RGT
    ),
    [_WIN_FN_LYR] = LAYOUT_all(        // 1
       KC_GRV,    KC_F1,     KC_F2,     KC_F3,     KC_F4,     KC_F5,     KC_F6,     KC_F7,     KC_F8,     KC_F9,     KC_F10,   KC_F11,   KC_F12,   KC_DEL,
       KC_GRV,    KC_HOME,   KC_UP,     KC_END,    C(KC_R),   KC_PGUP,   _______,   _______,   _______,   _______,   KC_PSCR,  KC_SCRL,  KC_PAUS,  _______,
       _______,   KC_LEFT,   KC_DOWN,   KC_RIGHT,  C(KC_F),   KC_PGDN,   _______,   _______,   _______,   _______,   KC_HOME,  KC_END,             _______,
       _______,   MY_UNDO,   MY_CUT,    MY_COPY,   MY_PASTE,  KC_SPC,    _______,   _______,   KC_PGDN,   KC_PGUP,   _______,            KC_UP,
       _______,   _______,   _______,              _______,   _______,   _______,              _______,   _______,   KC_LEFT,  KC_DOWN,            KC_RGHT
    ),
    [_CTL_LYR] = LAYOUT_all(            // 2
       _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,  _______,  _______,  _______,
       _______,   TD_KB_RST, _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   RGB_M_P,  RGB_RMOD, RGB_MOD,  RGB_TOG,
       _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   RGB_SPD,  RGB_SPI,            _______,
       _______,   TD_KB_CLR, _______,   _______,   _______,   _______,   NK_TOGG,   RGB_HUI,   RGB_VAD,   RGB_VAI,   _______,            _______,
       KC_SWP_FN, _______,   _______,              _______,   _______,   _______,              _______,   _______,   _______,  _______,            TG_CTL
    ),
    [_NUM_LYR] = LAYOUT_all(            // 3
       _______,   _______,   _______,   _______,   _______,   _______,   KC_NUM,    KC_P7,     KC_P8,     KC_P9,     KC_PAST,   _______, _______,  _______,
       _______,   KC_BTN1,   KC_MS_U,   KC_BTN2,   _______,   _______,   XXXXXXX,   KC_P4,     KC_P5,     KC_P6,     KC_PPLS,   _______, _______,  _______,
       _______,   KC_MS_L,   KC_MS_D,   KC_MS_R,   _______,   _______,   XXXXXXX,   KC_P1,     KC_P2,     KC_P3,     KC_PENT,   _______,           _______,
       _______,   _______,   _______,   _______,   _______,   _______,   XXXXXXX,   KC_P0,     KC_PDOT,   KC_PDOT,   KC_PSLS,            _______,
       _______,   _______,   _______,              _______,   _______,   _______,              _______,   _______,   _______,   TG_NUM,            _______
    ),
    [_NAV_LYR] = LAYOUT_all(            // 4
       _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______, _______,  _______,
       _______,   KC_HOME,   KC_UP,     KC_END,    C(KC_R),   KC_PGUP,   _______,   _______,   _______,   _______,   _______,   _______, _______,  _______,
       _______,   KC_LEFT,   KC_DOWN,   KC_RIGHT,  C(KC_F),   KC_PGDN,   _______,   _______,   _______,   _______,   _______,   _______,           _______,
       _______,   MY_UNDO,   MY_CUT,    MY_COPY,   MY_PASTE,  KC_SPC,    _______,   _______,   _______,   _______,   _______,            _______,
       _______,   _______,   _______,              _______,   _______,   _______,              _______,   TG_NAV,    _______,   _______,           _______
    ),
    [_FN_LYR] =  LAYOUT_all(            // 5
       _______,   _______,   _______,   _______,   _______,   _______,   _______,   KC_MPRV,   KC_MPLY,   KC_MNXT,   KC_MUTE,   KC_VOLD, KC_VOLU,  KC_DEL,
       _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______, _______,  _______,
       _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,           _______,
       _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,            MO_CTL,
       _______,   _______,   _______,              _______,   _______,   _______,              _______,   TG_NAV,    _______,   TG_NUM,            TD_TG_CTL
    )
};
// clang-format on


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
bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {

    uint8_t current_layer = get_highest_layer(layer_state);
    if(current_layer == _WIN_LYR)
    {
        if (rgb_matrix_get_flags() == LED_FLAG_INDICATOR) {
            for (int i = led_min; i < led_max; i++) {
                rgb_matrix_set_color(i, 0, 0, 0);
            }
            //rgb_matrix_set_color_all(0, 0, 0);
        }
    }

    if (IS_LAYER_ON(_WIN_FN_LYR) ||
        //IS_LAYER_ON(_CTL_LYR) ||  //ignore the CTL layer since we want to see RGB effects on that layer
        IS_LAYER_ON(_NUM_LYR) ||
        IS_LAYER_ON(_NAV_LYR) ||
        IS_LAYER_ON(_FN_LYR)) {
        // we are in a custom layer, clear all background colors
        // this will make our custom colors stand out more
        for (int i = led_min; i < led_max; i++) {
            rgb_matrix_set_color(i, 0, 0, 0);
        }
    }

    if(fn_mode_enabled){
        highlight_fn_keys(led_min, led_max);
    }

    if (IS_LAYER_ON(_WIN_FN_LYR)) {
        if(!fn_mode_enabled){
            // we are not in fn_mode, but this layer also uses fn keys
            highlight_fn_keys(led_min, led_max);
        }

        const uint8_t led_indexes[9] = {
            CAPS_LOCK_INDEX,     // use the caps lock as indicator
            RIGHT_ALT_KEY_INDEX, // use right alt as indicator

            // right side cluster
            24, // p = 24 PS
            25, // [ = 25 SL
            26, // ] = 26 PA
            38, // ; = 38 HM
            39, // ' = 39 END
            49, // , = 49 PgDn
            50  // . = 50 PgUp
        };

        for (int i = 0; i < 9; i++) {
            RGB_MATRIX_INDICATOR_SET_COLOR(led_indexes[i], 255, 255, 0);
        }
    }

    if (IS_LAYER_ON(_CTL_LYR)) {
        // #define CAPS_LOCK_INDEX 28
        // #define WIN_MOD_INDEX 16
        // #define MAC_MOD_INDEX 17
        const uint8_t led_indexes[4] = {
            RIGHT_CTL_KEY_INDEX, // use the right ctl key as indicator

            // RGB Control
            24, // p = 24 for persistent color
            //25, // [ = 25 RGB_RMOD
            //26, // ] = 26 RGB_MOD
            //27, // \ = 27 RGB Toggle

            //38, // ; = 38 RGB Speed Down
            //39, // ' = 39 RGB Speed Up

            47, // n = 47 NKR toggle
            //48, // m = 48 RGB Hue Increase
            //49, // , = 49 RGB Decrease Bright
            //50, // . = 50 RGB Increase Bright
            LEFT_CTL_KEY_INDEX // used for Fn Swap
        };

        for (int i = 0; i < 4; i++) {
            RGB_MATRIX_INDICATOR_SET_COLOR(led_indexes[i], 0, 255, 255);
        }

        const uint8_t led_off_indexes[4] = {
            // turn off some of the LEDS to make it easier to see our indicators
            29, // A
            14, 28, 41, // TAB, CAPS, LSFT
        };
        for (int i = 0; i < 4; i++) {
            RGB_MATRIX_INDICATOR_SET_COLOR(led_off_indexes[i], 0x00, 0x00, 0x00);
        }

        // light up the M key in white
        RGB_MATRIX_INDICATOR_SET_COLOR(48, 0x80, 0x80, 0x80);

        // highlight Q as reset
        RGB_MATRIX_INDICATOR_SET_COLOR(15, 0xFF, 0x00, 0x00);

        // highlight Z as clear eeprom
        RGB_MATRIX_INDICATOR_SET_COLOR(42, 0x7A, 0x00, 0xFF);
    }

    if (IS_LAYER_ON(_NUM_LYR)) {
        const uint8_t led_indexes[9] = {
            RIGHT_MENU_KEY_INDEX, // use the Right Menu key as indicator

            //  Light up around the numpad to make it easier to see
            //  left side
            6,  // 6 = 6 is used as numlock and starts the numpad
            20, // y = 20 is first key to left, acting as border
            34, // h = 34
            47, // n = 47
            // right side
            10, // 0 = 10
            24, // p = 24
            38, // ; = 38
            51  // / = 51
        };

        for (int i = 0; i < 9; i++) {
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
        //highlight the fn button
        RGB_MATRIX_INDICATOR_SET_COLOR(RIGHT_FN_KEY_INDEX, 128, 128, 128);

        // highlight the toggle buttons
        RGB_MATRIX_INDICATOR_SET_COLOR(RIGHT_CTL_KEY_INDEX, 0, 255, 255);
        RGB_MATRIX_INDICATOR_SET_COLOR(RIGHT_MENU_KEY_INDEX, 0, 255, 0);
        RGB_MATRIX_INDICATOR_SET_COLOR(RIGHT_ALT_KEY_INDEX, 0, 0, 255);

        // highlight the aux buttons on right of keyboard
        const uint8_t led_indexes[7] = {
            7, 8,9,10,11,12,13, // first row for media keys
        };

        for (int i = 0; i < 7; i++) {
            RGB_MATRIX_INDICATOR_SET_COLOR(led_indexes[i], 128, 128, 128);
        }
    }

    process_indicator_queue(led_min, led_max);

    return true;
}

void blink_arrows(void){
    indicator_enqueue(52, 200, 3, RGB_WHITE );  // up
    indicator_enqueue(61, 200, 3, RGB_WHITE );  // left
    indicator_enqueue(62, 200, 3, RGB_WHITE );  // down
    indicator_enqueue(63, 200, 3, RGB_WHITE );  // right
}

void blink_NKRO(bool isEnabling){
    if(isEnabling){
        const uint8_t led_indexes[12] = {
            45, 46, 47, 48, 49, // V B N M ,
            33, 34, 35, 36, // G H J K
            20, 21, 22 // Y U I
        };

        for (int i = 0; i < 12; i++) {
            indicator_enqueue(led_indexes[i], 200, 3, RGB_WHITE );
        }
    }
    else {
        const uint8_t led_indexes[4] = {
            46, 48, // B M
            34, 35  // H J
        };

        for (int i = 0; i < 4; i++) {
            indicator_enqueue(led_indexes[i], 150, 3, RGB_RED );
        }
    }
}

void blink_numbers(bool isEnabling){
    for( int i = 1; i <= 12; i++){ // 1(1) to EQL(12)
        if(isEnabling){
            // enabling, flash white
            indicator_enqueue(i, 200, 3, RGB_WHITE);
        } else {
            // disabling, flash red
            indicator_enqueue(i, 150, 4, RGB_RED);
        }
    }
}

void highlight_fn_keys(uint8_t led_min, uint8_t led_max)
{
    // get the current hsv value
    HSV current_hsv = rgb_matrix_get_hsv();
    // maximize brightness
    current_hsv.v = 255;

    rgb_led_t rgb = hsv_to_rgb(current_hsv);
    rgb_led_t new_rgb = get_complementary_color(rgb, false);
    for( int i = 55; i >= 44; i--){ // 55 - 44 are the number keys and - =
        RGB_MATRIX_INDICATOR_SET_COLOR(i, new_rgb.r, new_rgb.g, new_rgb.b);
    }
}
