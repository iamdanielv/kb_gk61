// Copyright 2024 DV (@iamdanielv)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

enum layer_names {
    _WIN_LYR,
    _WIN_ALT_LYR,
    _CTL_LYR,
    _NUM_LYR,
    _NAV_LYR,
    _FN_LYR,
};

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

// Define aliases to simplify keymap
#define FN_W_CAPS LT(_WIN_ALT_LYR,KC_CAPS)
#define FN_W_RALT LT(_WIN_ALT_LYR,KC_RALT)
#define FN_LEFT   LT(_FN_LYR, KC_LEFT)
#define APP_DWN   TD(_DN_MU)
#define RCTL_RGT  RCTL_T(KC_RIGHT)
#define RSFT_UP   RSFT_T(KC_UP)
#define TG_NAV    TG(_NAV_LYR)
#define TG_NUM    TG(_NUM_LYR)
#define TG_CTL    TD(TD_CTL_TG)
#define TD_KB_RST TD(TD_RESET)
#define TD_KB_CLR TD(TD_CLEAR)

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

// clang-format off
tap_dance_action_t tap_dance_actions[] = {
    [_DN_MU]    = ACTION_TAP_DANCE_TAP_HOLD(KC_DOWN, KC_APP),  // on Tap: "DOWN" on Hold:"App"
    [TD_RESET]  = ACTION_TAP_DANCE_FN(safe_reset),
    [TD_CLEAR]  = ACTION_TAP_DANCE_FN(safe_clear),
    [TD_CTL_TG] = ACTION_TAP_DANCE_LAYER_TOGGLE(KC_RCTL, _CTL_LYR)
};
// clang-format on


/*uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case RSFT_T(KC_UP):
            return TAPPING_TERM - 80; //right shift hold time less than default tapping term 80ms
        default:
            return TAPPING_TERM;
    }
}*/

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
    [_WIN_ALT_LYR] = LAYOUT_all(        // 1
       KC_GRV,    KC_F1,     KC_F2,     KC_F3,     KC_F4,     KC_F5,     KC_F6,     KC_F7,     KC_F8,     KC_F9,     KC_F10,   KC_F11,   KC_F12,   KC_DEL,
       KC_GRV,    KC_HOME,   KC_UP,     KC_END,    C(KC_R),   KC_PGUP,   _______,   _______,   _______,   _______,   KC_PSCR,  KC_SCRL,  KC_PAUS,  _______,
       _______,   KC_LEFT,   KC_DOWN,   KC_RIGHT,  C(KC_F),   KC_PGDN,   _______,   _______,   _______,   _______,   KC_HOME,  KC_END,             _______,
       _______,   MY_UNDO,   MY_CUT,    MY_COPY,   MY_PASTE,  KC_SPC,    _______,   _______,   KC_PGDN,   KC_PGUP,   _______,            _______,
       _______,   _______,   _______,              _______,   _______,   _______,              _______,   _______,   _______,  _______,            _______
    ),
    [_CTL_LYR] = LAYOUT_all(            // 2
       _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,  _______,  _______,  _______,
       _______,   TD_KB_RST, _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   RGB_M_P,  RGB_RMOD, RGB_MOD,  RGB_TOG,
       _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   RGB_SPD,  RGB_SPI,            _______,
       _______,   TD_KB_CLR, _______,   _______,   _______,   _______,   NK_TOGG,   RGB_HUI,   RGB_VAD,   RGB_VAI,   KC_UP,              _______,
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
       _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,            _______,
       _______,   _______,   _______,              _______,   _______,   _______,              _______,   TG_NAV,    _______,   TG_NUM,            TD_TG_CTL
    )
};
// clang-format on

// clang-format off
#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [_WIN_LYR]       = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [_WIN_ALT_LYR]       = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS) },
    [_CTL_LYR] = { ENCODER_CCW_CW(RGB_HUD, RGB_HUI) },
    [_NUM_LYR] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS) },
    [_NAV_LYR] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS) },
    [_FN_LYR] = { ENCODER_CCW_CW(RGB_HUD, RGB_HUI) },
};
#endif
// clang-format on

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {

    if (IS_LAYER_ON(_WIN_ALT_LYR)) {
        const uint8_t led_indexes[33] = {
            CAPS_LOCK_INDEX,     // use the caps lock as indicator
            RIGHT_ALT_KEY_INDEX, // use right alt as indicator

            0,1,2,3,4,5,6,7,8,9,10,11,12,13, // the whole first row is used for Fn keys

            //  2nd row similar to NAV Layer
            15, // q = 15 Home
            16, // w = 16 Up arrow
            17, // e = 17 End
            18, // r = 18 Ctrl-R
            19, // t = 19 PgUp
            // 3rd row similar to NAV Layer
            29, // a = 29 Left arrow
            30, // s = 30 Down arrow
            31, // d = 31 Right arrow
            32, // f = 32 Ctrl-F
            33, // g = 33 PgDn

            // right side cluster
            24, // p = 24 PS
            25, // [ = 25 SL
            26, // ] = 26 PA
            38, // ; = 38 HM
            39, // ' = 39 END
            49, // , = 49 PgDn
            50  // . = 50 PgUp
        };

        for (int i = 0; i < 33; i++) {
            RGB_MATRIX_INDICATOR_SET_COLOR(led_indexes[i], 255, 255, 0);
        }
    }

    if (IS_LAYER_ON(_CTL_LYR)) {
        // #define CAPS_LOCK_INDEX 28
        // #define WIN_MOD_INDEX 16
        // #define MAC_MOD_INDEX 17
        const uint8_t led_indexes[13] = {
            RIGHT_CTL_KEY_INDEX, // use the right ctl key as indicator

            //  KB related
            15, // q = 15 QK_BOOT
            42, // z = EE_CLR

            // RGB Control
            24, // p = 24 Solid Color
            25, // [ = 25 RGB_RMOD
            26, // ] = 26 RGB_MOD
            27, // \ = 27 RGB Toggle

            38, // ; = 38 RGB Speed Down
            39, // ' = 39 RGB Speed Up

            47, // n = 47 NKR toggle
            48, // m = 48 RGB Hue Increase
            49, // , = 49 RGB Decrease Bright
            50, // . = 50 RGB Increase Bright
        };

        for (int i = 0; i < 13; i++) {
            RGB_MATRIX_INDICATOR_SET_COLOR(led_indexes[i], 0, 255, 255);
        }
        // RGB_MATRIX_INDICATOR_SET_COLOR(LEFT_CTL_KEY_INDEX, 0, 255, 255);
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

        // highlight the Win and Mac Toggle buttons
        //RGB_MATRIX_INDICATOR_SET_COLOR(WIN_MOD_INDEX, 128, 128, 128);
        //RGB_MATRIX_INDICATOR_SET_COLOR(MAC_MOD_INDEX, 128, 128, 128);

        // highlight the aux buttons on right of keyboard
        const uint8_t led_indexes[13] = {
            1,2,3,4,5,6,7,8,9,10,11,12,13, // the whole first row is used for media keys
        };

        for (int i = 0; i < 13; i++) {
            RGB_MATRIX_INDICATOR_SET_COLOR(led_indexes[i], 128, 128, 128);
        }
    }

    /*switch (get_highest_layer(layer_state)) {
       case 2:{
         RGB_MATRIX_INDICATOR_SET_COLOR(WIN_MOD_INDEX, 255, 255, 255);
         if (!rgb_matrix_get_flags()) {
             RGB_MATRIX_INDICATOR_SET_COLOR(MAC_MOD_INDEX, 0, 0, 0);
          }
       } break;
       case 3:{
         RGB_MATRIX_INDICATOR_SET_COLOR(MAC_MOD_INDEX, 0, 255, 255);
         if (!rgb_matrix_get_flags()) {
             RGB_MATRIX_INDICATOR_SET_COLOR(WIN_MOD_INDEX, 0, 0, 0);
          }
       } break;

       case 0:{
        if (L_WIN) {
             RGB_MATRIX_INDICATOR_SET_COLOR(WIN_MOD_INDEX, 255, 255, 255);
             if (!rgb_matrix_get_flags()) {
                RGB_MATRIX_INDICATOR_SET_COLOR(MAC_MOD_INDEX, 0, 0, 0);
             }
             }else{
                 if (!rgb_matrix_get_flags()) {
                    RGB_MATRIX_INDICATOR_SET_COLOR(WIN_MOD_INDEX, 0, 0, 0);
                  }
               }
          } break;

      case 1:{
        if (L_WIN_ALT_LYR) {
             RGB_MATRIX_INDICATOR_SET_COLOR(MAC_MOD_INDEX, 0, 255, 255);
             if (!rgb_matrix_get_flags()) {
                RGB_MATRIX_INDICATOR_SET_COLOR(WIN_MOD_INDEX, 0, 0, 0);
             }
             }else{
                 if (!rgb_matrix_get_flags()) {
                    RGB_MATRIX_INDICATOR_SET_COLOR(MAC_MOD_INDEX, 0, 0, 0);
                  }
               }
          } break;

       default:{
          if (!rgb_matrix_get_flags()) {
             RGB_MATRIX_INDICATOR_SET_COLOR(WIN_MOD_INDEX, 0, 0, 0);
             RGB_MATRIX_INDICATOR_SET_COLOR(MAC_MOD_INDEX, 0, 0, 0);
          }
       }
     }*/
    return true;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    tap_dance_action_t *action;

    switch (keycode) {
#ifdef RGB_MATRIX_ENABLE
        case RGB_TOG:
            if (record->event.pressed) {
                switch (rgb_matrix_get_flags()) {
                    case LED_FLAG_ALL: {
                        rgb_matrix_set_flags(LED_FLAG_NONE);
                        rgb_matrix_set_color_all(0, 0, 0);
                    } break;
                    default: {
                        rgb_matrix_set_flags(LED_FLAG_ALL);
                    } break;
                }
            }
            return false;
        case RGB_VAI:
            rgb_matrix_set_flags(LED_FLAG_ALL);
            return true;
#endif
        // case TO(0):
        //     if (record->event.pressed) {
        //         L_WIN = 1;
        //         // set_single_persistent_default_layer(0); // Save default layer 0 to eeprom
        //     } else {
        //         L_WIN = 0;
        //     }
        //     return true; // continue all further processing of this key

        // case MO(2):
        //     if (record->event.pressed) {
        //         FN_WIN = 1;
        //     } else {
        //         FN_WIN = 0;
        //     }
        //     return true; // continue all further processing of this key

        // case TO(1):
        //     if (record->event.pressed) {
        //         L_WIN_ALT_LYR = 1;
        //         // set_single_persistent_default_layer(1);  //Save default layer 1 to eeprom
        //     } else {
        //         L_WIN_ALT_LYR = 0;
        //     }
        //     return true; // continue all further processing of this key

        // case MO(3):
        //     if (record->event.pressed) {
        //         FN_WIN_ALT_LYR = 1;
        //     } else {
        //         FN_WIN_ALT_LYR = 0;
        //     }
        //     return true; // continue all further processing of this key

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
}
