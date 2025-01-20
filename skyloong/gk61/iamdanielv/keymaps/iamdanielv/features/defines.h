// Copyright 2025 DV (@iamdanielv)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

enum layer_names {
    _WIN_LYR,    // 0
    _WIN_FN_LYR, // 1
    _CTL_LYR,    // 2
    _NUM_LYR,    // 3
    _ARROW_LYR,  // 4
};

// ***************
// * Key Indexes *
// ***************
#define TAB_KI 14
#define CAPS_KI 28
#define Q_KI 15
#define Z_KI 42
#define LEFT_SFT_KI 41
#define LEFT_CTL_KI 53
#define LEFT_WIN_KI 54
#define LEFT_ALT_KI 55
#define SPACE_KI 57
#define RIGHT_ALT_KI 60
#define FN_KI 61
#define RIGHT_MENU_KI 62
#define RIGHT_CTL_KI 63
#define RIGHT_SFT_KI 52
#define ENTER_KI 40

#define F_KI 32

#define I_KI 22
#define J_KI 35
#define K_KI 36
#define L_KI 37

#define W_KI 16
#define A_KI 29
#define S_KI 30
#define D_KI 31

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

//toggles
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
