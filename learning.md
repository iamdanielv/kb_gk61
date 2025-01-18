# Things I have learned and interesting links

## ⌨️ Layers

I mostly use Linux and Windows for development and since I had a limited number of layers,
I removed the Mac layers in favor of adding other layers to better suit my needs.

### Default Layer

The default layer has to allow us ways of getting to all the other layers and have the most used keys.
I have made the following choices:

#### Arrow keys:

The keyboard already came with a clever trick for re-using the
Fn, Shift, Menu, RightCtl keys as arrows on tap and the modifiers on hold.
I kept that feature but extended it. I was having problems with the shift key
not registering properly when typing fast. I added a new arrow layer to mitigate this.
I kept the left, down, right on the default layer on those keys since I tend to use them the most. 

#### On Default layer:

**Cap Lock** I don't use Caps Lock key too much, so I re-purposed it to be a temp layer switch
- Tap = Acts like Cap Lock 
- Hold = Momentarily switches to the Win Alt Layer (Sometimes called Win_Fn)

**Right Alt**
- Tap = Acts like Right Alt
- Hold = Momentarily enables the arrow layer, turning RSFT, FN, Menu, RCTL to arrow keys

**Fn Key**
- Tap = Left Arrow
- Hold = Momentarily switches to the Control Layer

**Right Ctl**
- Tap = Right Arrow
- Hold = acts like Right Ctl 

---

### Win Alt Layer (Sometimes  called Win_Fn)

This is a momentary layer but can be locked by entering this layer and pressing the `left win` key

**Getting here**: You can ge to this layer from the default layer by holding the `caps lock` key

**Locking this layer**: `left win` will lock this layer, pressing it again will unlock

**Getting out**: release the `caps lock` key or press the `left win` key if layer locked

**Keys available here**:

**1st Row**: GRV, function keys and Del are available here:
- ESC = \`
- 1 = F1
- 2 = F2
- ...
- 9 = F9
- 0 = F10
- \- = F11
- = = F12
- Backspace = Del
  
**2nd Row **: Inspired by the [Extend](https://colemakmods.github.io/ergonomic-mods/extend.html) layer but with some tweaks
- Tab = \` on tap, ~ on double tap, and `````` (code block) on hold
- Q = MY_CONS - pushes Left Ctl, Left Shift, and Esc (show console on VS Code) 
- W = MY_TASK - pushes Left Ctl, Left Shift, and GRV (show Task Manager on Windows)
- E = Ctl + F - find in most programs
- R = Ctl + R - refresh in most programs
- T = nothing for now
- Y = KC_PGUP
- U = KC_HOME
- I = KC_UP
- O = KC_END
- P = Print Screen
- [ = Scroll lock
- ] = Pause
- \ = KC_INS

**3rd Row **: Inspired by the [Extend](https://colemakmods.github.io/ergonomic-mods/extend.html) layer but with some tweaks
- A = LALT
- S = KC_LGUI
- D = KC_LSFT
- F = KV_LCTL
- G = nothing for now
- H = KC_PGDN
- J = KC_LEFT
- K = KC_DOWN
- L = KC_RIGHT
- ; = KC_HOME
- ' = KC_END

**4th Row **: Inspired by the [Extend](https://colemakmods.github.io/ergonomic-mods/extend.html) layer but with some tweaks
- Z = C(KC_Z) - Ctl+z for undo
- X = C(KC_X) - Ctl+x for cut
- C = C(KC_INS) - Ctl+ins for copy
- V = S(KC_INS) - shift+ins for paste
- B = KC_SPC
- N = KC_BSPC
- M = KC_DEL
- , = Browser Back
- . = Browser Forward
- / = nothing for now

**5th Row **: Mostly transparent, but has Layer Lock
- KC_LGUI (Left Win) - Layer Lock

---

### Number layer and mouse movement

This is a layer accesible from the CTL Layer

**Getting here**: You can ge to this layer by pushing `FN`, then tap `Menu` key

**Getting out**: Tap the `Menu` key

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

### ARROW Layer

This is a layer accesible from the CTL Layer and by holding down `right alt` on the base layer.

**Getting here**:
- Momentarily by holding the `right alt` key on the base layer.
- Toggle from the CTL layer and pushing `right alt`
- Can be layer locked while in this layer with the `\` key

**Getting out**:
- if here momentarily, release the `right alt` key 
- if layer locked, press the `\` key

**Keys available here**:
- - = Volume Down 
- = = Volume Up
- Right Shift - KC_UP
- Right FN - KC_LEFT
- Menu - KC_DOWN
- Right Ctl - KC_RIGHT

---

### CTL Layer

This is a momentary layer but can be locked by entering this layer and pressing the `right ctl` key

**Getting here**: You can ge to this layer from the default layer by holding the `Fn` key

**Toggle this layer**: `right ctl` will lock this layer, pressing it again will unlock

**Getting out**: release the `Fn` key or press the `right ctl` key if layer locked

**Keys available here**:

While on the Control Layer, you have the following available:

- `Q` - triple tap to `enter bootloader mode`
- `Z` - triple tap to `clear eeprom`

- `LCTL` - Swap Number row for Fn Keys (1 is F1 ... + is F12)
- `N` - Toggle NKRO
- `I` - Change Background Color HUE Down
- `O` - Change Background Color HUE Up
- `K` - Change Background Color SAT Down
- `L` - Change Background Color SAT Up
- `,` - Change Background Color VAL Down - RGB_VAD
- `.` - Change Background Color VAL Up - RGB_VAI
- `;` - RGB_SPD
- `'` - RGB_SPI
- `P` - RGB Matrix solid color
- `[` - RGB_RMOD
- `]` - RGB_RMD
- `\` - RGB_TOG -- this will turn off the colors, but still enable indicators
---

## 🥾 Safe keyboard reset and eeprom clear

I wanted to have the Keyboard reset for flashing from a key press,
but didn't want to accidentally trigger it.

[Thomas Baart](https://thomasbaart.nl/) has a great idea: [Safe reset](https://thomasbaart.nl/2018/12/13/qmk-basics-tap-dance/#safe-reset)

I extended that to also work for eeprom clear.

Currently this is in the `Control Layer`, you can get to it by `holding FN`

To **reset the keyboard**, tap the q key at least 3 times after enabling the control layer.

To **clear the eeprom**, tap the z key at least 3 times after enabling the control layer.

---

## 🤔 Things to consider

- [SpaceFn](https://kbd.news/The-SpaceFN-concept-2315.html)
- [Changing Tap and Hold - re-using LT(0,KC_)](https://docs.qmk.fm/mod_tap#changing-both-tap-and-hold)
- [Ergonomic Mods - Extend](https://colemakmods.github.io/ergonomic-mods/extend.html)

---

## 🔗 Links

- [Thomas Baart](https://thomasbaart.nl/) or [microblog](https://micro.thomasbaart.nl/) 
- [KBD News](https://kbd.news/)
- [Pascal Getreuer](https://getreuer.info/index.html) - [keyboards](https://getreuer.info/posts/keyboards/index.html) - [Getreuer GitHub](https://github.com/getreuer/qmk-keymap/)

---

## ASCII Representation of Keyboard

```
,-----------------------------------------------------------------------------------------.
|  `  |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  |  0  |  -  |  =  | Backspace |
|-----------------------------------------------------------------------------------------+
|   Tab  |  Q  |  W  |  E  |  R  |  T  |  Y  |  U  |  I  |  O  |  P  |  [  |  ]  |   \    |
|-----------------------------------------------------------------------------------------+
| CapsLock |  A  |  S  |  D  |  F  |  G  |  H  |  J  |  K  |  L  |  ;  |  '  |   Enter    |
|-----------------------------------------------------------------------------------------+
|   Shift     |  Z  |  X  |  C  |  V  |  B  |  N  |  M  |  ,  |  .  |  /  |    Shift      |
|-----------------------------------------------------------------------------------------+
|  Ctl  |  Win  |  Alt  |              Space               |  Alt  |  Fn  | Menu  |  Ctl  |
`-----------------------------------------------------------------------------------------'
```
