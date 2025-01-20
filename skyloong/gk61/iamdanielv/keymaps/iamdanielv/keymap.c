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
#include "features/rgb_keys.h"

// *****************************
// * Custom processing of keys *
// *****************************
enum custom_keycodes { KC_SWP_FN = SAFE_RANGE };

// clang-format off
tap_dance_action_t tap_dance_actions[] = {
    [_DN_MU]    = ACTION_TAP_DANCE_TAP_HOLD(KC_DOWN, KC_APP),  // on Tap: "DOWN" on Hold:"App"
    [TD_RESET]  = ACTION_TAP_DANCE_FN(safe_reset),
    [TD_CLEAR]  = ACTION_TAP_DANCE_FN(safe_clear)
};
// clang-format on

bool fn_mode_enabled = false;

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
       KC_LSFT,   KC_Z,      KC_X,      KC_C,      KC_V,      KC_B,      KC_N,      KC_M,      KC_COMM,   KC_DOT,    KC_SLSH,            KC_RSFT,
       KC_LCTL,   KC_LGUI,   KC_LALT,              KC_SPC,    KC_SPC,    KC_MUTE,              KC_SPC,    ARWS_RALT, FN_LEFT,  APP_DWN,            RCTL_RGT
    ),
    [_WIN_FN_LYR] = LAYOUT_all(         // 1
       KC_GRV,    _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,  _______,  _______,  KC_DEL,
       MY_GRV,    MY_CONS,   MY_TASK,   C(KC_F),   C(KC_R),   C(KC_H),   KC_PGUP,   KC_HOME,   KC_UP,     KC_END,    KC_PSCR,  KC_SCRL,  KC_PAUS,  KC_INS,
       _______,   KC_LALT,   KC_LGUI,   KC_LSFT,   KC_LCTL,   C(KC_G),   KC_PGDN,   KC_LEFT,   KC_DOWN,   KC_RIGHT,  KC_HOME,  KC_END,             _______,
       _______,   MY_UNDO,   MY_CUT,    MY_COPY,   MY_PASTE,  KC_SPC,    KC_BSPC,   KC_DEL,    MY_BACK,   MY_FWD,    _______,            _______,
       KC_SWP_FN, QK_LLCK,   _______,              _______,   _______,   _______,              _______,   _______,   _______,  _______,            _______
    ),
    [_CTL_LYR] = LAYOUT_all(            // 2
       _______,   XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,
       XXXXXXX,   TD_KB_RST, XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,   RM_HUED,   RM_HUEU,   RGB_M_P,  RM_PREV,  RM_NEXT,  RM_TOGG,
       XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,   RM_SATD,   RM_SATU,   RM_SPDD,  RM_SPDU,            _______,
       XXXXXXX,   TD_KB_CLR, XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,   NK_TOGG,   XXXXXXX,   RM_VALD,   RM_VALU,   _______,            _______,
       KC_SWP_FN, XXXXXXX,   XXXXXXX,              XXXXXXX,   XXXXXXX,   XXXXXXX,              XXXXXXX,   TG_ARWS,   _______,  TG_NUM,             QK_LLCK
    ),
    [_NUM_LYR] = LAYOUT_all(            // 3
       _______,   _______,   _______,   _______,   _______,   _______,   KC_NUM,    KC_P7,     KC_P8,     KC_P9,     KC_PAST,   _______, _______,  _______,
       XXXXXXX,   KC_BTN1,   KC_MS_U,   KC_BTN2,   XXXXXXX,   XXXXXXX,   XXXXXXX,   KC_P4,     KC_P5,     KC_P6,     KC_PPLS,   _______, _______,  _______,
       XXXXXXX,   KC_MS_L,   KC_MS_D,   KC_MS_R,   XXXXXXX,   XXXXXXX,   XXXXXXX,   KC_P1,     KC_P2,     KC_P3,     KC_PENT,   _______,           _______,
       XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,   KC_SPC,    XXXXXXX,   KC_P0,     KC_PDOT,   KC_PDOT,   KC_PSLS,            _______,
       XXXXXXX,   XXXXXXX,   XXXXXXX,              _______,   KC_SPC,    _______,              _______,   _______,   _______,   TG_NUM,            _______
    ),
    [_ARROW_LYR] =  LAYOUT_all(         // 4
       _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   KC_VOLD, KC_VOLU,  _______,
       _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______, _______,  QK_LLCK,
       _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,           _______,
       _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,            KC_UP,
       _______,   _______,   _______,              _______,   _______,   _______,              _______,   _______,   KC_LEFT,   KC_DOWN,           KC_RIGHT
    )
};
// clang-format on

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    tap_dance_action_t *action;

    if (keycode == KC_SWP_FN) {
        if (record->event.pressed) {
            fn_mode_enabled = !fn_mode_enabled;
            blink_numbers(fn_mode_enabled);
            indicator_enqueue(SPACE_KI, 200, 3, RGB_WHITE); // blink space too
            indicator_enqueue(LEFT_ALT_KI, 200, 3, RGB_BLACK); // blink left alt
            indicator_enqueue(RIGHT_ALT_KI, 200, 3, RGB_BLACK); // blink right alt
        }
        return false;
    }

    if (!process_fn_mode(keycode, record)) { return false; }
    if (!process_rgb_keys(keycode, record)) { return false; }

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
        case TD(_DN_MU):
            // this is needed to process the key before the timeout
            action = &tap_dance_actions[QK_TAP_DANCE_GET_INDEX(keycode)];
            if (!record->event.pressed &&
                action->state.count &&
                !action->state.finished) {
                td_tap_hold_t *tap_hold = (td_tap_hold_t *)action->user_data;
                tap_code16(tap_hold->on_tap);
            }
            break;
        case QK_LLCK:
            // when we lock a layer, flash the space bar area
            indicator_enqueue(SPACE_KI, 200, 3, RGB_WHITE); // blink space

            if (IS_LAYER_ON(_WIN_FN_LYR)) {
                indicator_enqueue(LEFT_WIN_KI, 200, 3, RGB_RED); // blink left win

                //blink the new arrow keys
                indicator_enqueue(I_KI, 150, 2, RGB_RED); // up - I
                indicator_enqueue(J_KI, 150, 2, RGB_RED); // left - J
                indicator_enqueue(K_KI, 150, 2, RGB_RED); // down - K
                indicator_enqueue(L_KI, 150, 2, RGB_RED); // right - L
            }else if (IS_LAYER_ON(_ARROW_LYR)) {
                indicator_enqueue(FN_KI, 200, 2, RGB_RED); // left - Right Fn
                indicator_enqueue(RIGHT_MENU_KI, 200, 2, RGB_RED); // down - Right Menu
                indicator_enqueue(RIGHT_CTL_KI, 200, 2, RGB_RED); // right - Right Ctl
                indicator_enqueue(RIGHT_SFT_KI, 200, 2, RGB_RED); // up - right shift
            }
            return true;
        case MY_GRV:
            if (record->tap.count > 0) { // Key is being tapped
                if (record->event.pressed) {
                    // Handle tap press event...
                    if (record->tap.count == 1) {
                        register_code16(KC_GRV);
                    } else if (record->tap.count == 2) {
                        // this is the 2nd tap,
                        // but we already did something on the 1st tap
                        // delete the previous key
                        tap_code(KC_BSPC);
                        register_code16(KC_TILD);
                    }
                } else {
                    // Handle tap release event...
                    if (record->tap.count == 1) {
                        unregister_code16(KC_GRV);
                    } else if (record->tap.count == 2) {
                        unregister_code16(KC_TILD);
                    }
                }
            } else { // Key is being held
                if (record->event.pressed) {
                    // Handle hold press event...
                    // send backticks to start a code block
                    SEND_STRING("``````");
                    // move cursor to the middle of the code block
                    tap_code(KC_LEFT);
                    tap_code(KC_LEFT);
                    tap_code(KC_LEFT);
                }
            }
            return false; // we handled all cases, stop further processing
        default:
            return true;
    }

    return true;
}

