#pragma once

#include QMK_KEYBOARD_H

#define ACTION_TAP_DANCE_TAP_HOLD(on_tap, on_hold) \
    { .fn = {NULL, td_tap_hold_finished, td_tap_hold_reset}, .user_data = (void *)&((td_tap_hold_t){on_tap, on_hold, 0}), }


enum tap_hold_keys {
    _DN_MU,   // Tap--KC_DOWN  and  Hold--KC_APP
    TD_RESET, // require 3 taps to reset board
    TD_CLEAR, // require 3 taps to clear eeprom
    TD_CTL_TG // require double tap to enable ctl layer
};

typedef struct {
    uint16_t on_tap;  // key to send on tap
    uint16_t on_hold; // key to send on hold
    uint16_t chosen;  // key to "release" when finished
                      // the value for it is set in td_tap_hold_finished
} td_tap_hold_t;


void td_tap_hold_finished(tap_dance_state_t *state, void *user_data);

void td_tap_hold_reset(tap_dance_state_t *state, void *user_data);

void safe_reset(tap_dance_state_t *state, void *user_data);

void safe_clear(tap_dance_state_t *state, void *user_data);
