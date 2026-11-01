# ProjectStudy-26ics5 Vibelt
2026年度 東京科学大学付属科学技術高等学校 課題研究 情報システム分野 5班<br>
デバイス側のArduino言語プログラムです。

## 1. beltgraph
https://github.com/ProjectStudy-26ics5/graph-X.git<br>
v1.0.0-betaを用います。

`vibelt/src/beltgraph`に配置しています。
Arduino IDEの仕様で、ディレクトリをネストした構造にするために、`src/`というディレクトリ下に置いています。

## 2. bgをSDカードに書き込む
ファイルシステムを利用しないで、生書き込みをします。
元のデータは失われます。
元の運用に戻したい場合は、SDカードをフォーマットしなおしてください。

> [!CAUTION]
> `"SDカードのパス"`を間違えると、システムファイルを破壊する可能性があります。

### Linux
(1) SDカードのパスを調べる
```bash
lsblk
```
(2) 書き込む
```bash
sudo umount "SDカードのパス"
sudo dd if="入力bg" of="SDカードのパス" bs=512 seek="開始セクタ番号" status=progress
sudo sync
```
(3) 書き込まれたのを確認する
```bash
sudo head -c 1024 "SDカードのパス" | xxd
```

### MacOS
(1) SDカードのパスを調べる
```bash
diskutil list
```
(2) 書き込む
```bash
diskutil unmountDisk "SDカードのパス"
sudo dd if="入力bg" of="SDカードのパス" bs=512 seek="開始セクタ番号" status=progress
```
(3) 書き込まれたのを確認する
```bash
sudo head -c 1024 "SDカードのパス" | xxd
```
(4) 取り出す
```bash
diskutil eject "SDカードのパス"
```

## 3. ボードマネージャ
esp32 by Espressif Systems<br>
ESP32S3 Dev Module

## 4. 依存ライブラリ
* SdFat by Bill Greiman
* Adafruit ILI9341 by Adafruit
* XPT2046_Touchscreen by Paul Stoffregen
* TinyGPSPlus by Mikal Hart
