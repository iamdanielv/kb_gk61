// Copyright 2024 DV (@iamdanielv)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
// imports from QMK
#include "process_tap_dance.h"
#include "color.h"
#include "quantum.h"

#include "features/defines.h"
#include "features/fn_mode.h"
#include "features/indicator_queue.h"
#include "features/indicators.h"
#include "features/tap_hold.h"
#include "features/rgb_keys.h"

// *****************************
// * Custom processing of keys *
// *****************************
enum custom_keycodes { KC_SWP_FN = SAFE_RANGE };

// clang-format off
tap_dance_action_t tap_dance_actions[] = {

    [TD_RESET]  = ACTION_TAP_DANCE_FN(safe_reset),
    [TD_CLEAR]  = ACTION_TAP_DANCE_FN(safe_clear),

    // on Tap: caps lock; on Hold: MO(EXT_LYR); on Double Tap Hold: MO(NUM_LYR)
    [TD_CAPS_MO]   = ACTION_TAP_DANCE_FN_ADVANCED(NULL, caps_mo_finished, caps_mo_reset),
    // on Tap: `; on Double Tap: ~; on Hold: ``````
    [TD_GRV]       = ACTION_TAP_DANCE_FN_ADVANCED(NULL, grv_finished, grv_reset)
};
// clang-format on

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case DN_APP:
            return 150;
        default:
            return TAPPING_TERM;
    }
}

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
    [BASE_LYR] = LAYOUT_all(
       KC_ESC,    KC_1,      KC_2,      KC_3,      KC_4,      KC_5,      KC_6,      KC_7,      KC_8,      KC_9,      KC_0,     KC_MINS,  KC_EQL,   KC_BSPC,
       KC_TAB,    KC_Q,      KC_W,      KC_E,      KC_R,      KC_T,      KC_Y,      KC_U,      KC_I,      KC_O,      KC_P,     KC_LBRC,  KC_RBRC,  KC_BSLS,
       CAPS_MO,   KC_A,      KC_S,      KC_D,      KC_F,      KC_G,      KC_H,      KC_J,      KC_K,      KC_L,      KC_SCLN,  KC_QUOT,            MY_ENT,
       KC_LSFT,   KC_Z,      KC_X,      KC_C,      KC_V,      KC_B,      KC_N,      KC_M,      KC_COMM,   KC_DOT,    KC_SLSH,            UP_RSFT,
       KC_LCTL,   KC_LGUI,   KC_LALT,              KC_SPC,    KC_SPC,    KC_MUTE,              KC_SPC,    ARWS_RALT, KBCTL_LEFT,DN_APP,            RCTL_RGT
    ),
    [EXT_LYR] = LAYOUT_all(
       _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,  _______,  _______,  _______,
       MY_GRV,    MY_CONS,   MY_TASK,   C(KC_F),   C(KC_R),   C(KC_H),   KC_PGUP,   KC_HOME,   KC_UP,     KC_END,    KC_PSCR,  KC_SCRL,  KC_PAUS,  KC_INS,
       _______,   KC_LALT,   KC_LGUI,   KC_LSFT,   KC_LCTL,   C(KC_G),   KC_PGDN,   KC_LEFT,   KC_DOWN,   KC_RIGHT,  KC_HOME,  KC_END,             _______,
       _______,   MY_UNDO,   MY_CUT,    MY_COPY,   MY_PASTE,  KC_SPC,    KC_BSPC,   KC_DEL,    MY_BACK,   MY_FWD,    _______,            _______,
       KC_SWP_FN, QK_LLCK,   _______,              _______,   KBCTL_SPC, _______,              _______,   _______,   _______,  _______,            _______
    ),
    [KBCTL_LYR] = LAYOUT_all(
       _______,   XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,
       XXXXXXX,   TD_KB_RST, XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,   RM_HUED,   RM_HUEU,   RGB_M_P,  RM_PREV,  RM_NEXT,  RM_TOGG,
       XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,   RM_SATD,   RM_SATU,   RM_SPDD,  RM_SPDU,            _______,
       XXXXXXX,   TD_KB_CLR, XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,   NK_TOGG,   XXXXXXX,   RM_VALD,   RM_VALU,   _______,            TG_W_FN,
       KC_SWP_FN, XXXXXXX,   XXXXXXX,              XXXXXXX,   XXXXXXX,   XXXXXXX,              XXXXXXX,   TG_ARWS,   _______,  TG_NUM,             QK_LLCK
    ),
    [NUM_LYR] = LAYOUT_all(
       _______,   _______,   _______,   _______,   _______,   _______,   KC_NUM,    KC_P7,     KC_P8,     KC_P9,     KC_PAST,   _______, _______,  _______,
       XXXXXXX,   KC_BTN1,   KC_MS_U,   KC_BTN2,   MSW_UP,    XXXXXXX,   XXXXXXX,   KC_P4,     KC_P5,     KC_P6,     KC_PPLS,   _______, _______,  _______,
       XXXXXXX,   KC_MS_L,   KC_MS_D,   KC_MS_R,   MSW_DN,    XXXXXXX,   XXXXXXX,   KC_P1,     KC_P2,     KC_P3,     KC_PENT,   _______,           _______,
       XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,   KC_SPC,    XXXXXXX,   KC_P0,     KC_PDOT,   KC_PDOT,   KC_PSLS,            _______,
       XXXXXXX,   XXXXXXX,   XXXXXXX,              _______,   KBCTL_SPC, _______,              _______,   _______,   _______,   TG_NUM,            _______
    ),
    [ARROW_LYR] =  LAYOUT_all(
       _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   KC_VOLD, KC_VOLU,  KBCTL_BSPC,
       _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______, _______,  _______,
       _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,           _______,
       _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,            KC_UP,
       _______,   _______,   _______,              _______,   _______,   _______,              _______,   TG_ARWS,   KC_LEFT,   KC_DOWN,           KC_RIGHT
    )
};
// clang-format on

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (keycode == KC_SWP_FN) {
        if (record->event.pressed) {
            fn_mode_enabled = !fn_mode_enabled;
            blink_numbers(fn_mode_enabled);
            indicator_enqueue(SPACE_KI, 200, 3, RGB_WHITE);     // blink space too
            indicator_enqueue(LEFT_ALT_KI, 200, 3, RGB_BLACK);  // blink left alt
            indicator_enqueue(RIGHT_ALT_KI, 200, 3, RGB_BLACK); // blink right alt
        }
        return false;
    }

    if (!process_fn_mode(keycode, record)) { return false; }
    if (!process_rgb_keys(keycode, record)) { return false; }

    switch (keycode) {
        case KC_BSPC: {
            // based on: https://getreuer.info/posts/keyboards/macros3/index.html
            // shift + backspace is delete
            // both shift held is shift + delete
            static uint16_t registered_key = KC_NO;
            if (record->event.pressed) {  // On key press.
                const uint8_t mods = get_mods();
                #ifndef NO_ACTION_ONESHOT
                uint8_t shift_mods = (mods | get_oneshot_mods()) & MOD_MASK_SHIFT;
                #else
                        uint8_t shift_mods = mods & MOD_MASK_SHIFT;
                #endif  // NO_ACTION_ONESHOT
                if (shift_mods) {  // At least one shift key is held.
                    registered_key = KC_DEL;
                    // If one shift is held, clear it from the mods. But if both
                    // shifts are held, leave as is to send Shift + Del.
                    if (shift_mods != MOD_MASK_SHIFT) {
                    #ifndef NO_ACTION_ONESHOT
                        del_oneshot_mods(MOD_MASK_SHIFT);
                    #endif  // NO_ACTION_ONESHOT
                        unregister_mods(MOD_MASK_SHIFT);
                    }
                } else {
                    registered_key = KC_BSPC;
                }

                register_code(registered_key);
                set_mods(mods);
            } else {  // On key release.
                wait_ms(50); // wait a little bit, so programs don't filter the press
                unregister_code(registered_key);
            }
        }
        return false;

        case MY_ENT:
            // By doing it this way, we can react immediately on key press
            if (record->event.pressed) {
                // we are registering a key
                if (record->tap.count) {
                    register_code16(KC_ENT);
                } else {
                    register_code16(KC_RSFT);
                }
            } else {
                // we are releasing a key
                if (record->tap.count) {
                    wait_ms(50); // wait a little bit, so programs don't filter the press
                    unregister_code16(KC_ENT);
                } else {
                    unregister_code16(KC_RSFT);
                }
            }
            return false;
        case UP_RSFT:
            // By doing it this way, we can react immediately on key press
            if (record->event.pressed) {
                // we are registering a key
                if (record->tap.count > 1) {
                    // require at least 2 taps in order to start pushing up arrow
                    // this will prevent accidental arrow push on shift
                    // this also handles a double tap and hold which causes the up key to auto repeat
                    register_code16(KC_UP);
                } else {
                    register_code16(KC_RSFT);
                }
            } else {
                // we are releasing a key
                if (record->tap.count > 1) {
                    unregister_code16(KC_UP);
                } else {
                    unregister_code16(KC_RSFT);
                }
            }
            return false;
        case DN_APP:
            // By doing it this way, we can react immediately on key press
            if (record->event.pressed) {
                // we are registering a key
                if (record->tap.count) {
                    // this also handles a double tap and hold which causes the down key to auto repeat
                    register_code16(KC_DOWN);
                } else {
                    tap_code16(KC_APP);
                }
            } else {
                // we are releasing a key
                if (record->tap.count) {
                    wait_ms(50); // wait a little bit, so programs don't filter the press
                    unregister_code16(KC_DOWN);
                } else {
                    // we switched to a tap code on press to react immediately,
                    // no need to unregister since the tap will handle it above
                    // unregister_code16(KC_APP);
                }
            }
            return false;
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
        case QK_LLCK:
            // when we lock a layer, flash the space bar area
            indicator_enqueue(SPACE_KI, 200, 3, RGB_WHITE); // blink space

            if (IS_LAYER_ON(EXT_LYR)) {
                indicator_enqueue(LEFT_WIN_KI, 200, 3, RGB_RED); // blink left win

                // blink the new arrow keys
                indicator_enqueue(I_KI, 150, 2, RGB_RED); // up - I
                indicator_enqueue(J_KI, 150, 2, RGB_RED); // left - J
                indicator_enqueue(K_KI, 150, 2, RGB_RED); // down - K
                indicator_enqueue(L_KI, 150, 2, RGB_RED); // right - L
            } else if (IS_LAYER_ON(ARROW_LYR)) {
                indicator_enqueue(FN_KI, 200, 2, RGB_RED);         // left - Right Fn
                indicator_enqueue(RIGHT_MENU_KI, 200, 2, RGB_RED); // down - Right Menu
                indicator_enqueue(RIGHT_CTL_KI, 200, 2, RGB_RED);  // right - Right Ctl
                indicator_enqueue(RIGHT_SFT_KI, 200, 2, RGB_RED);  // up - right shift
            }
            return true;
        default:
            return true;
    }

    return true;
}
