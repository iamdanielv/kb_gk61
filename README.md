# Skyloong GK61 - 🚧 Custom QMK firmware

I bought this keyboard from amazon:
[skyloong New GK61](https://www.amazon.ca/dp/B0CQD2YL32?ref=ppx_yo2ov_dt_b_product_details&th=1)

but when I tried to write a custom firmware I wasn't able to get the provided source to work on my keyboard.

Turns out mine is not exactly a [pro 48](https://github.com/NaturalZh/qmk_firmware07072023/tree/gk61_pro_48_v202/keyboards/skyloong/gk61/pro_48) or a [pro_ds](http://github.com/NaturalZh/qmk_firmware07072023/tree/gk61_pro_ds/keyboards/skyloong/gk61/pro_ds) but something in the middle.

## ⚠️ Disclaimer
> [!CAUTION]
> This is a custom firmware that could break your keyboard.
>
> 🛑 **Use at your own risk!** 🛑
> 
> I have tried my best to make sure everything works, but I may miss something.
>
> It works for my keyboard, but there may be revisions in the future that are not compatible. Use at your own risk!

## Smushing

There are subtle differences between the rgb driver and the pin outs between the keyboards.

I looked through their repo, smushed both branches together and came up with the code here.

Since I didn't want to mess up the existing pro ds or pro 48 directory structures, I created a new folder
called `iamdanielv` that can be added to the repo without breaking anything.

Copy the folder structure from `skyloong\gk61\iamdanielv` into the qmk repo `keyboard` directory.

## New Keymap

I left the `default` and `via` keymaps, just updated a little bit to match my keyboard.

I created a **new keymap** `iamdanielv` that enables via, function keys, navigation, a num pad and layer for RGB / Keyboard management.

## Compiling

You can now use QMK builder with the following command:

```shell
qmk compile -j 0 -kb skyloong/gk61/iamdanielv -km iamdanielv
```

> the `-j 0` uses parallel build to speed it up a bit

qmk should generate a new bin file: `skyloong_gk61_iamdanielv_iamdanielv.bin` in your qmk_firmware folder.

For me it was: `C:\Users\<USER>\qmk_firmware\skyloong_gk61_iamdanielv_iamdanielv.bin`

## Using via
In order to use via, you may have to upload the json specification, it can be found under the `extras` folder.

## Conclusion and Next Steps

**That's it!** You now should be able to flash that firmware using qmk toolbox.

Let me know if you have any questions. I can be reached at [@IAmDanielV](https://twitter.com/IAmDanielV).
