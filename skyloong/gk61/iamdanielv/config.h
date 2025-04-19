// Copyright 2024 DV (@iamdanielv)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#define IS31FL3743A_I2C_ADDRESS_1 IS31FL3743A_I2C_ADDRESS_GND_GND
#define IS31FL3743A_SDB_PIN A4

#define TAPPING_TERM_PER_KEY

// LED Indexes
#define WIN_MOD_INDEX 16
#define MAC_MOD_INDEX 17
#define CAPS_LOCK_INDEX 28

// Set defaults for LED matrix
#define RGB_MATRIX_DEFAULT_MODE RGB_MATRIX_SOLID_COLOR
#define RGB_MATRIX_DEFAULT_HUE 150
#define RGB_MATRIX_DEFAULT_SAT 255
#define RGB_MATRIX_DEFAULT_VAL 255

#define g_suspend_state rgb_matrix_get_suspend_state()
