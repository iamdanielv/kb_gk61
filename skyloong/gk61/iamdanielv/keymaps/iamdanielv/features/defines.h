#pragma once

enum layer_names {
    _WIN_LYR,    // 0
    _WIN_FN_LYR, // 1
    _CTL_LYR,    // 2
    _NUM_LYR,    // 3
    _ARROW_LYR,  // 4
};

// ******************************
// * Aliases to simplify keymap *
// ******************************
// Win_FN layer on hold, caps lock on tap
#define FN_W_CAPS LT(_WIN_FN_LYR, KC_CAPS)
// Arrows layer on hold, ralt on tap
#define ARWS_RALT LT(_ARROW_LYR, KC_RALT)

#define FN_LEFT   LT(_CTL_LYR, KC_LEFT)
#define APP_DWN   TD(_DN_MU)
#define RCTL_RGT  RCTL_T(KC_RIGHT)
#define RSFT_UP   RSFT_T(KC_UP)

#define TG_NUM    TG(_NUM_LYR)
#define TG_ARWS   TG(_ARROW_LYR)
#define TD_KB_RST TD(TD_RESET)
#define TD_KB_CLR TD(TD_CLEAR)

// shortcuts
#define MY_UNDO   C(KC_Z)
#define MY_CUT    C(KC_X)
#define MY_COPY   C(KC_INS)
#define MY_PASTE  S(KC_INS)
#define MY_TASK   LCTL(LSFT(KC_ESC))
#define MY_CONS   LCTL(LSFT(KC_GRV))
#define MY_BACK   A(KC_LEFT)
#define MY_FWD    A(KC_RIGHT)

// placeholder, this is handled in process_record_user
#define MY_GRV    LT(0, KC_GRV)
