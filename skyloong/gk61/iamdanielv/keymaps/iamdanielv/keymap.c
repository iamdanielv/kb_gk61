// Copyright 2024 DV (@iamdanielv)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "features/defines.h"
#include "quantum.h"

#include "features/dv_layer_lock.h"
#include "features/indicator_queue.h"
#include "features/fn_mode.h"
#include "features/tap_hold.h"
#include "features/indicators.h"
#include "features/rgb_keys.h"

bool fn_mode_enabled = false;
bool recalculate_rgb = true;

// *****************************
// * Custom processing of keys *
// *****************************
enum custom_keycodes { KC_SWP_FN = SAFE_RANGE };

// clang-format off
tap_dance_action_t tap_dance_actions[] = {

    [TD_RESET]     = ACTION_TAP_DANCE_FN(safe_reset),
    [TD_CLEAR]     = ACTION_TAP_DANCE_FN(safe_clear),

    // Tap: CAPS_LOCK; Hold: MO(EXT_LYR); Double Tap: TO(HRM_LYR); Double Hold: MO(NUM_LYR)
    [TD_MO_CAPS]   = ACTION_TAP_DANCE_FN_ADVANCED(NULL, mo_caps_finished, mo_caps_reset),
    // on Tap: Esc; on Double Tap: `; on Hold: ``````
    [TD_GRV]       = ACTION_TAP_DANCE_FN_ADVANCED(NULL, grv_finished, grv_reset),
    [TD_RALT]      = ACTION_TAP_DANCE_FN_ADVANCED(NULL, ralt_finished, ralt_reset),
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
       MY_GRV,    KC_1,      KC_2,      KC_3,      KC_4,      KC_5,      KC_6,      KC_7,      KC_8,      KC_9,      KC_0,     KC_MINS,  KC_EQL,   KC_BSPC,
       KC_TAB,    KC_Q,      KC_W,      KC_E,      KC_R,      KC_T,      KC_Y,      KC_U,      KC_I,      KC_O,      KC_P,     KC_LBRC,  KC_RBRC,  KC_BSLS,
       MO_CAPS,   KC_A,      KC_S,      KC_D,      KC_F,      KC_G,      KC_H,      KC_J,      KC_K,      KC_L,      KC_SCLN,  KC_QUOT,            MY_ENT,
       KC_LSFT,   KC_Z,      KC_X,      KC_C,      KC_V,      KC_B,      KC_N,      KC_M,      KC_COMM,   KC_DOT,    KC_SLSH,            UP_RSFT,
       KC_LCTL,   KC_LGUI,   KC_LALT,              KC_SPC,    KC_SPC,    KC_MUTE,              KC_SPC,    ARWS_RALT, KBCTL_LFT,DN_APP,             RCTL_RGT
    ),
    [HRM_BASE_LYR] =  LAYOUT_all(
       _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______, _______,  _______,
       _______,   _______,   _______,   _______,   CTLR_R,    CTLH_T,    _______,   _______,   _______,   _______,   _______,   _______, _______,  _______,
       _______,   GUI_A,     ALT_S,     SFT_D,     CTL_F,     CTLG_G,    _______,   CTL_J,     SFT_K,     ALT_L,     HM_SCLN,   END_QUOT,          _______,
       LSFT_LLCK, _______,   _______,   CTLS_C,    _______,   _______,   _______,   _______,   ALFT_COMM, ARGT_DOT,  _______,            _______,
       _______,   _______,   _______,              _______,   _______,   _______,              _______,   _______,   _______,   _______,           _______
    ),
    [EXT_LYR] = LAYOUT_all(
       KC_GRV,    KC_F1,     KC_F2,     KC_F3,     KC_F4,     KC_F5,     KC_F6,     KC_F7,     KC_F8,     KC_F9,     KC_F10,   KC_F11,   KC_F12,   KC_DEL,
       _______,   MY_CONS,   MY_TASK,   C(KC_F),   C(KC_R),   C(KC_H),   KC_PGUP,   KC_HOME,   KC_UP,     KC_END,    KC_PSCR,  KC_SCRL,  KC_PAUS,  KC_INS,
       _______,   KC_LGUI,   KC_LALT,   KC_LSFT,   KC_LCTL,   C(KC_G),   KC_PGDN,   KC_LEFT,   KC_DOWN,   KC_RIGHT,  KC_HOME,  KC_END,             _______,
       LSFT_LLCK, MY_UNDO,   MY_CUT,    MY_COPY,   MY_PASTE,  KC_SPC,    KC_BSPC,   KC_DEL,    MY_BACK,   MY_FWD,    _______,            KC_VOLU,
       _______,   _______,   _______,              _______,   KBCTL_SPC, _______,              _______,   _______,   MY_BACK,  KC_VOLD,            MY_FWD
    ),
    [KBCTL_LYR] = LAYOUT_all(
       _______,   XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,
       _______,   TD_KB_RST, XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,   RM_HUED,   RM_HUEU,   RGB_M_P,  RM_PREV,  RM_NEXT,  RM_TOGG,
       _______,   XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,   RM_SATD,   RM_SATU,   RM_SPDD,  RM_SPDU,            _______,
       LSFT_LLCK, TD_KB_CLR, XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,   NK_TOGG,   XXXXXXX,   RM_VALD,   RM_VALU,   TG_EXT,             TG_HRM,
       KC_SWP_FN, _______,   _______,              _______,   _______,   _______,              _______,   TG_ARWS,   _______,  TG_NUM,             QK_LLCK
    ),
    [NUM_LYR] = LAYOUT_all(
       _______,   _______,   _______,   _______,   _______,   _______,   KC_NUM,    KC_P7,     KC_P8,     KC_P9,     KC_PAST,   _______, _______,  _______,
       _______,   KC_BTN1,   KC_MS_U,   KC_BTN2,   MSW_UP,    XXXXXXX,   XXXXXXX,   KC_P4,     KC_P5,     KC_P6,     KC_PPLS,   _______, _______,  _______,
       _______,   KC_MS_L,   KC_MS_D,   KC_MS_R,   MSW_DN,    XXXXXXX,   XXXXXXX,   KC_P1,     KC_P2,     KC_P3,     KC_PENT,   _______,           _______,
       LSFT_LLCK, XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,   KC_SPC,    XXXXXXX,   KC_P0,     KC_PDOT,   KC_PDOT,   KC_PSLS,            _______,
       _______,   _______,   _______,              _______,   KBCTL_SPC, _______,              _______,   _______,   _______,   QK_LLCK,            _______
    ),
    [ARROW_LYR] =  LAYOUT_all(
       _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   KC_VOLD, KC_VOLU,  KBCTL_BSPC,
       _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______, _______,  _______,
       _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,           _______,
       _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,            KC_UP,
       _______,   _______,   _______,              _______,   _______,   _______,              _______,   _______,   KC_LEFT,   KC_DOWN,           KC_RIGHT
    )
};
// clang-format on

// **********************************************************************
// * we want the ability to break out the key handlers but we also want *
// * to inline, this will allow us to do that                           *
// * inline void myInlinedFunction() __attribute__((always_inline));    *
// **********************************************************************

// function definitions for key handlers
inline bool handle_backspace(keyrecord_t *record) __attribute__((always_inline));
inline bool handle_nkro_toggle(keyrecord_t *record) __attribute__((always_inline));
inline bool handle_dn_app(keyrecord_t *record) __attribute__((always_inline));

// **********************************************************************
// * process_record_user is the main function that handles key presses  *
// * and is the entry point for all key processing                      *
// **********************************************************************
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (keycode == KC_SWP_FN) {
        if (record->event.pressed) {
            fn_mode_enabled = !fn_mode_enabled;
            blink_numbers(fn_mode_enabled);
            blink_space(true);
        }
        return false;
    }

    if (keycode == QK_LLCK) {
        // when we lock or unlock a layer, flash the space bar area
        blink_space(true);

        if (IS_LAYER_ON(EXT_LYR)) {
            // blink the new arrow keys
            indicator_enqueue(I_KI, 150, 2, INDICATOR_RGB_DARK_RED); // up - I
            indicator_enqueue(J_KI, 150, 2, INDICATOR_RGB_DARK_RED); // left - J
            indicator_enqueue(K_KI, 150, 2, INDICATOR_RGB_DARK_RED); // down - K
            indicator_enqueue(L_KI, 150, 2, INDICATOR_RGB_DARK_RED); // right - L
        }
        // we only handled the flashing of the indicators, so keep processing the key code
    }

    if (!process_fn_mode(keycode, record)) { return false; }
    if (!process_rgb_keys(keycode, record)) { return false; }
    if (!dv_process_layer_lock(keycode, record, QK_LLCK)) { return false; }
    if (!handle_lt_0( keycode,  record)) { return false; }

    switch (keycode) {
        case KC_BSPC:
            return handle_backspace(record);
        case QK_MAGIC_TOGGLE_NKRO:
            return handle_nkro_toggle(record);
        case DN_APP:
            return handle_dn_app(record);
        default:
            // everything else should be handled normally
            return true;
    }

    return true;
}

bool handle_backspace(keyrecord_t *record) {
    // based on: https://getreuer.info/posts/keyboards/macros3/index.html
    // shift + backspace is delete
    // both shift held is shift + delete
    static uint16_t registered_key = KC_NO;
    if (record->event.pressed) { // On key press.
        const uint8_t mods = get_mods();
#ifndef NO_ACTION_ONESHOT
        uint8_t shift_mods = (mods | get_oneshot_mods()) & MOD_MASK_SHIFT;
#else
        uint8_t shift_mods = mods & MOD_MASK_SHIFT;
#endif // NO_ACTION_ONESHOT
        if (shift_mods) { // At least one shift key is held.
            registered_key = KC_DEL;
            // If one shift is held, clear it from the mods. But if both
            // shifts are held, leave as is to send Shift + Del.
            if (shift_mods != MOD_MASK_SHIFT) {
#ifndef NO_ACTION_ONESHOT
                del_oneshot_mods(MOD_MASK_SHIFT);
#endif // NO_ACTION_ONESHOT
                unregister_mods(MOD_MASK_SHIFT);
            }
        } else {
            registered_key = KC_BSPC;
        }
        register_code(registered_key);
        set_mods(mods);
    } else {                     // On key release.
        wait_ms(TAP_CODE_DELAY); // wait a little bit, so programs don't filter the press
        unregister_code(registered_key);
    }
    return false;
}

bool handle_nkro_toggle(keyrecord_t *record) {
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
}

bool handle_dn_app(keyrecord_t *record) {
    // act as down arrow on tap, KC_APP on hold
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
            wait_ms(TAP_CODE_DELAY); // wait a little bit, so programs don't filter the press
            unregister_code16(KC_DOWN);
        } else {
            // we switched to a tap code on press to react immediately,
            // no need to unregister since the tap will handle it above
            // unregister_code16(KC_APP);
        }
    }
    return false;
}
