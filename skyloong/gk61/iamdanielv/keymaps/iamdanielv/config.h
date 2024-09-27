// Copyright 2024 DV (@iamdanielv)
// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#define DYNAMIC_KEYMAP_LAYER_COUNT 6 /* This is 4 by default. */

#define TAPPING_TERM_PER_KEY

#define RGB_MATRIX_SLEEP

#define DUMMY_MOD_NEUTRALIZER_KEYCODE KC_F18
// Neutralize left alt, left GUI and right GUI
#define MODS_TO_NEUTRALIZE { MOD_BIT(KC_LEFT_ALT), MOD_BIT(KC_LEFT_GUI), MOD_BIT(KC_RIGHT_GUI), }
