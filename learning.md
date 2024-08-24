# Things I have learned and interesting links

## Layers

I mostly use Linux and Windows for development and since I had a limited number of layers,
I removed the Mac layers in favor of adding other layers to better suit my needs.

### Default Layer

The default layer has to allow us ways of getting to all the other layers and have the most used keys.
I have made the following choices:

#### Arrow keys:

The keyboard already came with a clever trick for re-using the
Fn, Shift, Menu, RightCtl keys as arrows on tap and the modifiers on hold.
I kept that feature but extended it.

#### Getting to layers:

**Cap Lock** I don't use Caps Lock key too much, so I re-purposed it to be a temp layer switch
- Tap = Acts like Cap Lock 
- Hold = Momentarily switches to the Win Alt Layer (Sometimes called Win_Fn)

**Right Shift**
- Tap = Up Arrow
- Hold = Acts like shift key

**Right Alt**
- Tap = Acts like Right Alt
- Hold = Momentarily switches to the Win Alt Layer (Sometimes  called Win_Fn)

**Fn Key**
- Tap = Left Arrow
- Hold = Momentarily switches to the Fn Layer

**Right Ctl**
- Tap = Right Arrow
- Hold = acts like Right Ctl 

---

### Win Alt Layer (Sometimes  called Win_Fn)

This is a momentary layer

**Getting here**: You can ge to this layer from the default layer by either holding the `caps lock` key or holding the `right alt` key

**Getting out**: release the `caps lock` or `right alt` key

**Keys available here**:
- ESC = \`
- Tab = \` // this makes it easier to tap the \` key while holding cap lock

**Function Keys**: The function keys are available here
- 1 = F1
- 2 = F2
- ..
- 9 = F9
- 0 = F10
- \- = F11
- = = F12

- Backspace = Del
- Q = KC_HOME
- W = KC_UP
- E = KC_END
- R = C(KC_R) - control-r for refresh
- T = KC_PGUP
- A = KC_LEFT
- S = KC_DOWN
- D = KC_RIGHT
- F = C(KC_F) - control-f for find
- G = KC_PGDN
- Z = C(KC_Z) - control-z for undo
- X = C(KC_X) - control-x for cut
- C = C(KC_INS) - control ins for copy
- V = S(KC_INS) - shift ins for paste

- P = Print Screen
- [ = Scroll lock
- ] = Pause

- ; = Home
- ' = End

- < = Page Down
- \> = Page Up

---

### Number layer and mouse movement

This is a toggled layer accesible from the Fn Layer

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

### Navigation layer

This is a toggled layer accesible from the Fn Layer

**Getting here**: You can ge to this layer by pushing `FN`, then tap `Right Alt` key

**Getting out**: Tap the `Right Alt` key

- Q = KC_HOME
- W = KC_UP
- E = KC_END
- R = C(KC_R) - control-r for refresh
- T = KC_PGUP
- A = KC_LEFT
- S = KC_DOWN
- D = KC_RIGHT
- F = C(KC_F) - control-f for find
- G = KC_PGDN
- Z = C(KC_Z) - control-z for undo
- X = C(KC_X) - control-x for cut
- C = C(KC_INS) - control ins for copy
- V = S(KC_INS) - shift ins for paste

---

### Fn Layer

This is a momentary layer accessible from all layers

**Getting here**: You can ge to this layer by holding the `Fn` key

**Getting out**: release the `Fn` key
- 7 = Previous track
- 8 = Play/Pause
- 9 = Next track 
- 0 = Mute
- - = Volume Down 
- = = Volume Up
- Backspace = Del

The function layer also lets you toggle on the Navigation, Number and Control layers.
 - Right_Alt = Toggle navigation layer
 - Right Menu = Toggle Number layer
 - Right Ctl = Toggle Control layer -- must double tap the key to enable

---

## Safe keyboard reset and eeprom clear

I wanted to have the Keyboard reset for flashing from a key press,
but didn't want to accidentally trigger it.

[Thomas Baart](https://thomasbaart.nl/) has a great idea: [Safe reset](https://thomasbaart.nl/2018/12/13/qmk-basics-tap-dance/#safe-reset)

I extended that to also work for eeprom clear.

Currently this is in the `Control Layer`, you can get to it by `pushing FN, then double tap on Right CTL.`

To **reset the keyboard**, tap the q key at least 3 times after enabling the control layer.

To **clear the eeprom**, tap the z key at least 3 times after enabling the control layer.

---

## Things to consider

- [SpaceFn](https://kbd.news/The-SpaceFN-concept-2315.html)

---

## Links

- [Thomas Baart](https://thomasbaart.nl/) or [microblog](https://micro.thomasbaart.nl/) 
- [KBD News](https://kbd.news/)
