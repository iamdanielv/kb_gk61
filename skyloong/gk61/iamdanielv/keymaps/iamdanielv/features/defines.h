// Copyright 2025 DV (@iamdanielv)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

enum layer_names {
    BASE_LYR,       // 0 - regular qwerty
    HRM_BASE_LYR,   // 1 - home row mods qwerty
    EXT_LYR,        // 2 - similar to extend
    KBCTL_LYR,      // 3 - keyboard control layer
    NUM_LYR,        // 4 - numpad
    ARROW_LYR,      // 5 - arrow overlay on right hand modifiers
};

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

// *****************
// * Home Row Mods *
// *****************
// based on: https://precondition.github.io/home-row-mods#getting-started-with-home-row-mods-on-qmk

// Left-hand home row mods
#define GUI_A LGUI_T(KC_A)
#define ALT_S LALT_T(KC_S)
#define SFT_D LSFT_T(KC_D)
#define CTL_F LCTL_T(KC_F)

// Right-hand home row mods
#define CTL_J RCTL_T(KC_J)
#define SFT_K RSFT_T(KC_K)
#define ALT_L RALT_T(KC_L)
#define GUI_SCLN RGUI_T(KC_SCLN)

// custom keycodes used in the HRM Layer
#define CTLS_C C_S_T(KC_C) // tap: C ; hold: ctl and shift

#define CTLH_T  LT(0, KC_T) // tap: t; hold: ctl and h
#define CTLR_R  LT(0, KC_R) // tap: r; hold: ctl and r
#define LSFT_LLCK LT(0, KC_LSFT) // double tap: Layer lock ; hold: left shift
#define HM_SCLN   LT(0, KC_SCLN) // tap: ; ; hold: Home key
#define END_QUOT  LT(0, KC_QUOT) // tap: ' ; hold: End key
// Custom right Shift key:
// require at least 2 taps in order to start pushing up arrow
// this will prevent accidental arrow push on shift
// this also handles a double tap and hold which causes the up key to auto repeat
#define UP_RSFT LT(0, KC_RSFT)
#define MY_ENT LT(0, KC_ENT) // tap: Enter ; hold: Shift


// by default, use a regular key layout without home row mods
// the KB_CTL_LYR can toggle the home row mod enabled layer
#define TG_HRM TG(HRM_BASE_LYR)

// ***************
// * Key Indexes *
// ***************

#define ESC_KI 0

// Modifiers
#define TAB_KI 14
#define CAPS_KI 28
#define LEFT_SFT_KI 41
#define LEFT_CTL_KI 53
#define LEFT_WIN_KI 54
#define LEFT_ALT_KI 55
#define SPACE_KI 57
#define FN_KI 61
#define RIGHT_ALT_KI 60
#define RIGHT_MENU_KI 62
#define RIGHT_CTL_KI 63
#define RIGHT_SFT_KI 52
#define ENTER_KI 40

// Letter Keys
#define A_KI 29
#define B_KI 46
#define C_KI 44
#define D_KI 31
#define E_KI 17
#define F_KI 32
#define G_KI 33
#define H_KI 34
#define I_KI 22
#define J_KI 35
#define K_KI 36
#define L_KI 37
#define M_KI 48
#define N_KI 47
#define O_KI 23
#define P_KI 24
#define Q_KI 15
#define R_KI 18
#define S_KI 30
#define T_KI 19
#define U_KI 21
#define V_KI 45
#define W_KI 16
#define X_KI 43
#define Y_KI 20
#define Z_KI 42

// number keys
#define N1_KI 1
#define N2_KI 2
#define N3_KI 3
#define N4_KI 4
#define N5_KI 5
#define N6_KI 6
#define N7_KI 7
#define N8_KI 8
#define N9_KI 9
#define N0_KI 10

// symbols
#define COMM_KI 49 // ,
#define DOT_KI 50  // .
#define BSLS_KI 27 // \ backslash
#define SLSH_KI 51 // / forwardslash
#define SCLN_KI 38 // ;
#define QUOT_KI 39 // '
#define MINS_KI 11 // -
#define EQL_KI 12  // =

// ******************************
// * Aliases to simplify keymap *
// ******************************
#define MO_CAPS   TD(TD_MO_CAPS)
// Tap: KC_DOWN; Hold: KC_APP
#define DN_APP LT(0, KC_APP)
#define MY_GRV TD(TD_GRV)

// Tap: ralt; Hold: enable Arrows layer
#define ARWS_RALT LT(ARROW_LYR, KC_RALT)

#define KBCTL_SPC LT(KBCTL_LYR, KC_SPC)
#define KBCTL_BSPC LT(KBCTL_LYR, KC_BSPC)
#define KBCTL_LEFT LT(KBCTL_LYR, KC_LEFT)
#define RCTL_RGT RCTL_T(KC_RIGHT)
#define RSFT_UP RSFT_T(KC_UP)

// toggles
#define TG_NUM TG(NUM_LYR)
#define TG_W_FN TG(EXT_LYR)
#define TG_ARWS TG(ARROW_LYR)
#define TD_KB_RST TD(TD_RESET)
#define TD_KB_CLR TD(TD_CLEAR)

// shortcuts
#define MY_UNDO C(KC_Z)
#define MY_CUT C(KC_X)
#define MY_COPY C(KC_INS)
#define MY_PASTE S(KC_INS)
#define MY_TASK LCTL(LSFT(KC_ESC))
#define MY_CONS LCTL(LSFT(KC_GRV))
#define MY_BACK A(KC_LEFT)
#define MY_FWD A(KC_RIGHT)
#define MSW_UP KC_MS_WH_UP
#define MSW_DN KC_MS_WH_DOWN
