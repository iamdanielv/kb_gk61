// Copyright 2023 linlin012 (@linlin012)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#define IS31FL3743A_I2C_ADDRESS_1 IS31FL3743A_I2C_ADDRESS_GND_GND
#define IS31FL3743A_SDB_PIN A4

#define TAPPING_TERM_PER_KEY

// LED Indexes
#define WIN_MOD_INDEX 16
#define MAC_MOD_INDEX 17
#define CAPS_LOCK_INDEX 28
#define LEFT_CTL_KEY_INDEX 53
#define LEFT_WIN_KEY_INDEX 54
#define LEFT_ALT_KEY_INDEX 55
#define RIGHT_ALT_KEY_INDEX 60
#define RIGHT_FN_KEY_INDEX 61
#define RIGHT_MENU_KEY_INDEX 62
#define RIGHT_CTL_KEY_INDEX 63

// Set defaults for LED matrix
#define RGB_MATRIX_DEFAULT_MODE RGB_MATRIX_SOLID_COLOR
#define RGB_MATRIX_DEFAULT_HUE 166
#define RGB_MATRIX_DEFAULT_SAT 255
#define RGB_MATRIX_DEFAULT_VAL 192

#define g_suspend_state rgb_matrix_get_suspend_state()
