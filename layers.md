# ⌨️ Layers

I mostly use Linux and Windows for development and since I had a limited number of layers,
I removed the Mac layers in favor of adding other layers to better suit my needs.

## Arrow keys

The keyboard already came with a clever trick for re-using the
Fn, Shift, Menu, RightCtl keys as arrows on tap and the modifiers on hold.
I kept that feature but extended it.

I was having problems with the shift key not registering properly when typing fast.
I kept the left, down, right on the default layer on those keys since I tend to use them the most.
I modified the right shift key to act as regular shift on tap or hold, but on double tap it acts as up arrow,
on double tap hold it repeats the up arrow key.

I also added a new arrow layer that can be toggled with double tap of right alt key it can also be temporarily
enabled with hold of right alt key. This lets me use my right hand for all of the arrow keys and my left hand for the other keys.

## Default Layer

The default layer has to allow us ways of getting to all the other layers and have the most used keys.
I have made the following choices:

### Keys available in Default layer

#### Esc Key

- **Tap**: ESC
- **Hold**: ``````
- **Double Tap**: `

#### Caps Lock Key

- **Tap**: Caps Lock
- **Hold**: Temporarily switches to `EXT_LYR`
- **Double Tap**: toggles the `HRM_BASE_LYR`
- **Double Hold**: Temporarily switches to `NUM_LYR`

#### Enter Key Key

- **Tap**: Enter
- **Hold**: Shift

##### Right Alt Key

- **Tap**: KC_RALT
- **Hold**: Temporarily switches to `ARROW_LYR`
- **Double Tap**: Layer lock 'ARROW_LYR'
- **Double Hold**: KC_RALT

##### Fn Key

- **Tap**: Left Arrow
- **Hold**: Temporarily switches to 'KBCTL_LYR'

##### Right App (Menu) Key

- **Tap**: Down Arrow
- **Hold**: App/Menu

##### Right Ctl

- **Tap**: Right Arrow
- **Hold**: Right Ctl

---

## Home Row Mods Layer

Inspired by home row mods, but also adds more keys with dual roles.

### Keys available in Home Row Mods

#### A

- **Tap**: A
- **Hold**: LGUI

#### S

- **Tap**: S
- **Hold**: LALT

#### D

- **Tap**: D
- **Hold**: LSFT

#### F

- **Tap**: F
- **Hold**: LCTL

#### J

- **Tap**: J
- **Hold**: RCTL

#### K

- **Tap**: K
- **Hold**: RSFT

#### L

- **Tap**: L
- **Hold**: RALT

#### T

- **Tap**: T
- **Hold**: Ctl and H (Find and replace in VS Code)

#### R

- **Tap**: R
- **Hold**: Ctl and R (Refresh in browser)

#### G

- **Tap**: G
- **Hold**: Ctl and G (Go to Line)

#### LSFT

- **Double Tap**: Layer Lock
- **Hold**: Left Shift

#### SCLN

- **Tap**: ;
- **Hold**: Home Key

#### QUOT

- **Tap**: '
- **Hold**: End Key

#### COMM

- **Tap**: ,
- **Hold**: Alt and Left Arrow (Back in browser, previous edit in VS Code)

#### DOT

- **Tap**: .
- **Hold**: Alt and Right Arrow (Forward in browser, next edit in VS Code)

#### ENTER

- **Tap**: Enter
- **Hold**: Shift

---

## Extend Layer

Inspired by the [Extend](https://colemakmods.github.io/ergonomic-mods/extend.html) layer but with some tweaks.
This is very similar to the Home row layer, but acts directly on key press no need to hold the key or have a delay.

**Getting here**: You can temporarily get here by holding the `Caps Lock Key` or toggle this layer on the `KBCTL_LYR` layer with the `/` key  

**Getting out**: you can either toggle the layer like above, or double tap the `Left Shift` key

### Keys available on Extend Layer

First Row:

- **ESC**: `
- **Numbers and - =**: Turn into Function keys F1 - F12
- **Backspace**: delete

Second Row:

- **Q**: MY_CONS - pushes Left Ctl, Left Shift, and Esc (show console on VS Code)
- **W**: MY_TASK - pushes Left Ctl, Left Shift, and GRV (show Task Manager on Windows)
- **E**: Ctl + F - find in most programs
- **R**: Ctl + R - refresh in most programs
- **Y**: KC_PGUP
- **U**: KC_HOME
- **I**: KC_UP
- **O**: KC_END
- **P**: Print Screen
- **[**: Scroll lock
- **]**: Pause
- **\\**: KC_INS

Third Row:

- **A**: LGUI
- **S**: LALT
- **D**: LSFT
- **F**: LCTL
- **G**: Ctl and G (Go to Line)
- **H**: KC_PGDN
- **J**: Left Arrow
- **K**: Down Arrow
- **L**: Right Arrow
- **;**: Home
- **'**: End

Fourth Row:

- **LSFT**: double tap to toggle the layer
- **Z**: undo
- **X**: cut
- **C**: Copy
- **V**: Paste
- **B**: Space
- **N**: Backspace
- **M**: Delete
- **,**: Alt and Left Arrow (Back in browser, previous edit in VS Code)
- **.**: Alt and Right Arrow (Forward in browser, next edit in VS Code)
- **Right Shift**: Volume Up
- **Right App (Menu)**: Volume Down

Fifth Row:

- **SPACE**: On hold will momentarily enable the KBD_CTL layer
- **FN**: Alt and Left Arrow (Back in browser, previous edit in VS Code)
- **Right App (Menu)**: Volume Down
- **Right Ctl**: Alt and Right Arrow (Forward in browser, next edit in VS Code)

---

## Number layer and mouse movement

**Getting here**: You can momentarily enable this layer by double tap and hold the `Caps Lock` key or toggle this layer with `Right Menu/App` on the `KBCTL_LYR`.

**Getting out**: If using momentary, release the caps lock. If using toggle, you can toggle this layer with `Right Menu/App` or double tap the `Left Shift` key.

I find myself using numbers a lot and sometimes it's nice to have a num pad on your tiny keyboard.
I implemented a layer that has a **numpad on the right side**, re-using the 7 8 9 and going down.

- 6 7 8 9 0 = NumLock  7 8 9 *
- X U I O P = Disabled 4 5 6 +
- X J K L ; = Disabled 1 2 3 <Num_Pad_Enter>
- X M , . / = Disabled 0 . . /

I had a little bit of extra space on that layer, so I added **mouse movement on the left side** under W A S D

- Q W E = LeftMouse MouseUp   RightMouse
- A S D = MouseLeft MouseDown MouseRight

---

## ARROW Layer

Replaces the bottom right hand modifiers with arrow keys. Also adds volume down on `-` and volume up on `+`.

**Getting here**:

- Momentarily by holding the `right alt` key on the base layer.
- Toggle layer by double tapping `right alt`.
- Toggle from the `KBCTL_LYR` layer and pushing `right alt`

**Getting out**:

- if here momentarily, release the `right alt` key
- if layer locked, press the `\` key

### Keys available in ARROW Layer

- **-** = Volume Down  
- **=** = Volume Up  
- **Right Shift** - KC_UP  
- **Right FN** - KC_LEFT  
- **Menu** - KC_DOWN  
- **Right Ctl** - KC_RIGHT  

---

## KBDCTL_LYR Layer

This is a momentary layer but can be locked by entering this layer and pressing the `right ctl` key

**Getting here**: You can ge to this layer from the default layer by holding the `Fn` key

**Toggle this layer**: `right ctl` will lock this layer, pressing it again will unlock

**Getting out**: release the `Fn` key or press the `right ctl` key if layer locked

### Keys available in KBDCTL_LYR Layer

While on the Control Layer, you have the following available:

- **Q** - triple tap to `enter bootloader mode`
- **Z** - triple tap to `clear eeprom`

- **LCTL** - Swap Number row for Fn Keys (1 is F1 ... + is F12)
- **N** - Toggle NKRO
- **I** - Change Background Color HUE Down
- **O** - Change Background Color HUE Up
- **K** - Change Background Color SAT Down
- **L** - Change Background Color SAT Up
- **,** - Change Background Color VAL Down - RGB_VAD
- **.** - Change Background Color VAL Up - RGB_VAI
- **;** - RGB_SPD
- **'** - RGB_SPI
- **P** - RGB Matrix solid color
- **[** - RGB_RMOD
- **]** - RGB_RMD
- **\\** - RGB_TOG -- this will turn off the colors, but still enable indicators

---

# 🥾 Safe keyboard reset and eeprom clear

I wanted to have the Keyboard reset for flashing from a key press,
but didn't want to accidentally trigger it.

[Thomas Baart](https://thomasbaart.nl/) has a great idea: [Safe reset](https://thomasbaart.nl/2018/12/13/qmk-basics-tap-dance/#safe-reset)

I extended that to also work for eeprom clear.

Currently this is in the `KBCTL_LYR`, you can get to it by `holding FN`

To **reset the keyboard**, tap the q key at least 3 times after enabling the control layer.

To **clear the eeprom**, tap the z key at least 3 times after enabling the control layer.
