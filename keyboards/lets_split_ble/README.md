# Let's Split BLE

BLE Pro Microを搭載したLet's Split用の設定ファイルです。

## ピンアサイン
ProMicro互換です。GPIOとピン名称のエイリアス設定は下記ファイルを参照してください。
```
./board/custom_board.h
```

## RGB
動作未確認です。

# いくつかの問題

## 右手のデバイスキー反転問題
本家ではRev2で解消されたのですが、マスターと反対側のデバイスのキーマップが反転する問題がありました。
無線化が前提だっため、本設定ファイルでは左手をマスターにして、右手側のキーマップファイルを反転させてあります。

[Your right side has the TRRS on the LEFT and the keymap is reversed](https://github.com/nicinabox/lets-split-guide/blob/master/troubleshooting.md#your-right-side-has-the-trrs-on-the-left-and-the-keymap-is-reversed)
