# DoraChan [ドラちゃん]
DORACHAN - Cat access control using RFID tags /  - 猫のアクセス制御

## What this is
Dorachan is implemented on a MSP430G2553 low power MCU. It processes data from a WIEGAND protocol RFID tag reader which has been modified for extended read range.
It reads the tag on the collar of my cats at a distance, so when they're close to the window, the window will open for them. It uses an IR beam to detect when the cat has entered and to close the window behind the cat.
Window is controlled with a powerful servo motor (Futaba compatible type) and mechanical arm fabricated out of metal.

ドラちゃんはMSP430G2553低電力MCUに実装されています。 拡張された読み取り範囲用に変更されたWIEGANDプロトコルRFIDタグリーダーからのデータを処理します。 猫の首輪のタグを遠くから読み取るので、猫が窓に近づくと窓が開きます。 IRビームを使用して、猫がいつ入ったかを検出し、猫の後ろの窓を閉じます。
強力なサーボモーター（双葉対応タイプ）と金属製のメカニカルアームで窓を制御します。
