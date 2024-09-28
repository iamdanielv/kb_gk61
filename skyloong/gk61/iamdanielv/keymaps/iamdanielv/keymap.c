// Copyright 2024 DV (@iamdanielv)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
// imports from QMK
#include "process_tap_dance.h"
#include "color.h"
#include "quantum.h"

#include "features/defines.h"
#include "features/indicator_queue.h"
#include "features/fn_mode.h"
#include "features/tap_hold.h"
#include "features/indicators.h"


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
            indicator_enqueue(57, 200, 3, RGB_WHITE );  // blink space too
            indicator_enqueue(55, 200, 3, RGB_BLACK );  // blink left alt
            indicator_enqueue(60, 200, 3, RGB_BLACK );  // blink right alt
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
                        indicator_enqueue(57, 200, 3, INDICATOR_RGB_DARK_RED );  // blink space too
                        indicator_enqueue(55, 200, 3, INDICATOR_RGB_DARK_RED );  // blink left alt
                        indicator_enqueue(60, 200, 3, INDICATOR_RGB_DARK_RED );  // blink right alt
                    } break;
                    default: {
                        HSV current_hsv = rgb_matrix_get_hsv();
                        RGB rgb = hsv_to_rgb(current_hsv);
                        rgb_matrix_set_color_all(rgb.r, rgb.g, rgb.b);
                        rgb_matrix_set_flags_noeeprom(LED_FLAG_ALL);
                        indicator_enqueue(57, 200, 3, RGB_WHITE );  // blink space too
                        indicator_enqueue(55, 200,  3, RGB_WHITE ); // blink left alt
                        indicator_enqueue(60, 200, 3, RGB_WHITE );  // blink right alt
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
                indicator_enqueue(57, 200, 3, RGB_WHITE );  // blink space too
                indicator_enqueue(55, 200, 3, RGB_WHITE );  // blink left alt
                indicator_enqueue(60, 200, 3, RGB_WHITE );  // blink right alt
            }
            return false;
        case RGB_SPI:
            if (record->event.pressed) {
                if ( rgb_matrix_get_speed() >= (255 - RGB_MATRIX_SPD_STEP)) {
                    // this update would put us at max
                    blink_arrows();
                    indicator_enqueue(39, 200, 4, RGB_RED );    // ' - SPI
                    indicator_enqueue(38, 200, 2, RGB_BLACK );  // ; - SPD
                    indicator_enqueue(57, 200, 3, RGB_WHITE );  // blink space too
                }
                rgb_matrix_increase_speed_noeeprom();
            }
            return false;
        case RGB_SPD:
            if (record->event.pressed) {
                if (rgb_matrix_get_speed() <= RGB_MATRIX_SPD_STEP) {
                    blink_arrows();
                    indicator_enqueue(39, 200, 2, RGB_BLACK ); // ' - SPI
                    indicator_enqueue(38, 200, 4, RGB_RED );   // ; - SPD
                    indicator_enqueue(57, 200, 3, RGB_WHITE ); // blink space too
                    rgb_matrix_set_speed_noeeprom(RGB_MATRIX_SPD_STEP);
                }
                rgb_matrix_decrease_speed_noeeprom();
            }
            return false;
        case RGB_HUI:
            if (record->event.pressed) {
                if ( rgb_matrix_get_hue() >= (255 - RGB_MATRIX_HUE_STEP)) {
                    // this update would put us at max
                    indicator_enqueue(23, 200, 4, INDICATOR_RGB_DARK_RED ); // O - HUI
                    indicator_enqueue(22, 200, 2, RGB_BLACK );              // I - HUD
                    indicator_enqueue(57, 200, 3, RGB_WHITE );              // blink space too
                }
                rgb_matrix_increase_hue_noeeprom();
            }
            return false;
        case RGB_HUD:
            if (record->event.pressed) {
                if ( rgb_matrix_get_hue() <= RGB_MATRIX_HUE_STEP ) {
                    // this update would put us at min
                    indicator_enqueue(23, 200, 2, RGB_BLACK );              // O - HUI
                    indicator_enqueue(22, 200, 4, INDICATOR_RGB_DARK_RED ); // I - HUD
                    indicator_enqueue(57, 200, 3, RGB_WHITE );              // blink space too
                }
                rgb_matrix_decrease_hue_noeeprom();
            }
            return false;
        case RGB_SAI:
            if (record->event.pressed) {
                if ( rgb_matrix_get_sat() >= (255 - RGB_MATRIX_SAT_STEP)) {
                    // this update would put us at max
                    indicator_enqueue(37, 200, 4, INDICATOR_RGB_DARK_RED ); // L - SAI
                    indicator_enqueue(36, 200, 2, RGB_BLACK );              // K - SAD
                    indicator_enqueue(57, 200, 3, RGB_WHITE );              // blink space too
                }
                rgb_matrix_increase_sat_noeeprom();
            }
            return false;
        case RGB_SAD:
            if (record->event.pressed) {
                if ( rgb_matrix_get_sat() <= RGB_MATRIX_SAT_STEP ) {
                    // this update would put us at min
                    indicator_enqueue(37, 200, 2, RGB_BLACK );              // L - SAI
                    indicator_enqueue(36, 200, 4, INDICATOR_RGB_DARK_RED ); // K - SAD
                    indicator_enqueue(57, 200, 3, RGB_WHITE );              // blink space too
                }
                rgb_matrix_decrease_sat_noeeprom();
            }
            return false;
        case RGB_VAI:
            if (record->event.pressed) {
                if (rgb_matrix_get_val() >= (RGB_MATRIX_MAXIMUM_BRIGHTNESS - RGB_MATRIX_VAL_STEP)) {
                    indicator_enqueue(50, 200, 4, RGB_RED );    // . - VAI
                    indicator_enqueue(49, 200, 2, RGB_BLACK );  // , - VAD
                    indicator_enqueue(57, 200, 3, RGB_WHITE );  // blink space too
                    blink_arrows();
                }
                rgb_matrix_increase_val_noeeprom();
            }
            return false;
        case RGB_VAD:
            if (record->event.pressed) {
                if (rgb_matrix_get_val() <= RGB_MATRIX_VAL_STEP) {
                    indicator_enqueue(50, 200, 2, RGB_BLACK );  // . - VAI
                    indicator_enqueue(49, 200, 4, RGB_RED );    // , - VAD
                    indicator_enqueue(57, 200, 3, RGB_WHITE );  // blink space too
                    blink_arrows();
                }
                rgb_matrix_decrease_val_noeeprom();
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
       KC_GRV,    MY_CONS,   MY_TASK,   C(KC_F),   C(KC_R),   _______,   KC_PGUP,   KC_HOME,   KC_UP,     KC_END,    KC_PSCR,  KC_SCRL,  KC_PAUS,  KC_INS,
       _______,   KC_LALT,   KC_LGUI,   KC_LSFT,   KC_LCTL,   _______,   KC_PGDN,   KC_LEFT,   KC_DOWN,   KC_RIGHT,  KC_HOME,  KC_END,             _______,
       _______,   MY_UNDO,   MY_CUT,    MY_COPY,   MY_PASTE,  KC_SPC,    _______,   _______,   MY_BACK,   MY_FWD,    _______,            MO_CTL,
       _______,   _______,   _______,              _______,   _______,   _______,              _______,   TG_NAV,    _______,  TG_NUM,             TD_TG_CTL
    ),
    [_CTL_LYR] = LAYOUT_all(            // 2
       _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,  _______,  _______,  _______,
       _______,   TD_KB_RST, _______,   _______,   _______,   _______,   _______,   _______,   RGB_HUD,   RGB_HUI,   RGB_M_P,  RGB_RMOD, RGB_MOD,  RGB_TOG,
       _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   RGB_SAD,   RGB_SAI,   RGB_SPD,  RGB_SPI,            _______,
       _______,   TD_KB_CLR, _______,   _______,   _______,   _______,   NK_TOGG,   _______,   RGB_VAD,   RGB_VAI,   _______,            _______,
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


