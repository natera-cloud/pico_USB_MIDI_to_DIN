# Pico USB MIDI Host to DIN Converter



A simple and effective USB MIDI Host to 5-pin DIN MIDI converter using a Raspberry Pi Pico (RP2040). This project allows you to connect modern USB-only MIDI controllers to legacy synthesizers or sound modules that only have traditional 5-pin DIN MIDI inputs.

Raspberry Pi Pico (RP2040) を使用した、シンプルで効果的なUSB MIDIホストから5ピンDIN MIDIへの変換器です。このプロジェクトにより、USB接続しかできない現代のMIDIコントローラーを、従来の5ピンDIN MIDI入力しか持たないシンセサイザーや音源モジュールに接続することができます。

---

## 🎵 Background / 背景

Modern MIDI controllers often connect and are powered via a single USB cable. However, classic sound modules, like the Roland Sonic Cell, only accept MIDI signals through a 5-pin DIN connector and cannot provide power to the controller. This necessitates separate power adapters and extra cables, which can be cumbersome. This project was born out of a desire to bridge that gap.

現代のMIDIキーボードはUSBケーブル1本で接続と電源供給が完結することがほとんどです。しかし、RolandのSonic Cellのようなクラシックな音源モジュールは、MIDI信号を5ピンDINコネクタでしか受け付けず、キーボードへ電源を供給することもできません。これでは別途ACアダプターやケーブルが必要になり、非常に煩わしいです。このプロジェクトは、そのギャップを埋めるために生まれました。

---

## ✨ Features / 特徴

* **USB MIDI Host:** Receives MIDI data from a class-compliant USB MIDI device. (USB MIDIデバイスからのMIDIデータを受信)
* **5-Pin DIN MIDI Out:** Forwards the received MIDI messages to a standard 5-pin MIDI OUT port. (受信したMIDIメッセージを標準の5ピンMIDI OUTポートへ転送)
* **Simple & Low-Cost:** Built with a Raspberry Pi Pico and a handful of common components. (Picoと少数の部品で構成されたシンプルで低コストな設計)

---

## 🛠️ Hardware / ハードウェア

### Required Components / 必要な部品
* **MCU:** Raspberry Pi Pico or a compatible RP2040 board (e.g., RP2040 Zero)
* **Connectors / コネクタ:**
    * 5-pin DIN female socket (5ピンDINメスソケット)
    * USB Type-C breakout board (電源供給用USB-Cブレークアウト基板)
* **Resistors / 抵抗:**
    * 2 x 220Ω (for MIDI OUT circuit / MIDI OUT回路用)
    * 1 x 5.1kΩ (for the Type-C breakout board to enable C-to-C power delivery / C to Cケーブルでの給電有効化用)

### Wiring / 配線

1.  **MIDI OUT Circuit / MIDI OUT回路:**
    * Connect **GP8 (UART1 TX)** of the Pico to the **MIDI DIN Pin 5** through a **220Ω resistor**. (PicoのGP8を220Ω抵抗を介してDIN 5番ピンに接続)
    * Connect **3.3V** from the Pico to the **MIDI DIN Pin 4** through a **220Ω resistor**. (Picoの3.3Vを220Ω抵抗を介してDIN 4番ピンに接続)
    * Connect **GND** from the Pico to the **MIDI DIN Pin 2**. (PicoのGNDをDIN 2番ピンに接続)
2.  **Power / 電源:**
    * Solder the 5.1kΩ resistor to the CC pin on the Type-C breakout board if it's not already present. (C to Cケーブルでの給電に必要であれば、USB-C基板のCCピンに5.1kΩ抵抗をはんだ付け)
    * Connect the VBUS and GND from the breakout board to the VSYS and GND pins on the Pico to power it. (USB-C基板のVBUSとGNDをPicoのVSYSとGNDに接続)
3.  **USB MIDI Host:**
    * Connect your USB MIDI device to the Pico's main USB port. You will need a USB OTG adapter. (USB MIDIデバイスをPico本体のUSBポートに接続。OTGアダプタが必要です)

---

## ⚙️ Installation & Setup / 導入方法

There are two ways to get this software onto your Raspberry Pi Pico. The easy method using a `.uf2` file is recommended for most users.
ソフトウェアをPicoにインストールする方法は2つあります。ほとんどのユーザーには、簡単な`.uf2`ファイルを使う方法をお勧めします。

### 1. Easy Installation via UF2 (Recommended) / UF2ファイルでの簡単導入 (推奨) ✅

This is the simplest way to get started. / これが最も簡単な方法です。

1.  Go to the [**Releases**](https://github.com/your-username/your-repo-name/releases) page of this repository. (このリポジトリの **Releases** ページに移動します)
2.  Download the latest `.uf2` file (e.g., `pico-usb-midi-to-din-v1.0.uf2`). (最新の`.uf2`ファイルをダウンロードします)
3.  Hold down the **BOOTSEL** button on your Raspberry Pi Pico and connect it to your computer with a USB cable. (Picoの **BOOTSEL** ボタンを押しながら、PCにUSBケーブルで接続します)
4.  It will appear on your computer as a new drive called `RPI-RP2`. (PC上に`RPI-RP2`という名前の新しいドライブが表示されます)
5.  Drag and drop the downloaded `.uf2` file onto that `RPI-RP2` drive. (ダウンロードした`.uf2`ファイルを、そのドライブにドラッグ＆ドロップします)
6.  The Pico will automatically reboot, and the program will start running. That's it! 🚀 (Picoが自動的に再起動し、プログラムが実行されます。これで完了です！)

### 2. Building from Source (For Developers) / ソースからのビルド (開発者向け) 🧑‍💻

If you want to modify the code, you'll need to compile it yourself using the Arduino IDE.
コードを自分で改造したい場合は、Arduino IDEを使ってコンパイルする必要があります。

#### Arduino IDE Setup / 環境構築
1.  Install the Arduino IDE. (Arduino IDEをインストール)
2.  Add the Raspberry Pi Pico board support. In **File > Preferences**, add the following URL to "Additional Board Manager URLs": (Picoのボード定義を追加。**ファイル > 環境設定** の「追加のボードマネージャURL」に以下を追記)
    ```
    [https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json](https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json)
    ```
3.  Go to **Tools > Board > Boards Manager**, search for "pico", and install **"Raspberry Pi Pico/RP2040"**. (**ツール > ボード > ボードマネージャ** で "pico" と検索し、**"Raspberry Pi Pico/RP2040"** をインストール)

#### Board Configuration / ボード設定
* In the **Tools** menu, select the following options: (**ツール** メニューで以下を選択)
    * **Board:** "Raspberry Pi Pico"
    * **USB Stack:** "Adafruit TinyUSB Host"

#### Libraries / ライブラリ
* **Adafruit TinyUSB Library**: Install this from the Arduino IDE's Library Manager (**Tools > Manage Libraries**). (Arduino IDEのライブラリマネージャからインストール)
* **rppicomidi/usb_midi_host**: This library needs to be installed manually. Download the ZIP file from the [**GitHub repository**](https://github.com/rppicomidi/usb_midi_host) and install it in the Arduino IDE via **Sketch > Include Library > Add .ZIP Library...**. ([**リポジトリからZIPファイルをダウンロード**](https://github.com/rppicomidi/usb_midi_host)し、Arduino IDEの **スケッチ > ライブラリをインクルード > .ZIP形式のライブラリをインストール...** から手動でインストール)

#### Uploading / 書き込み
1.  Open the `.ino` sketch file in the Arduino IDE. (`.ino`ファイルをArduino IDEで開く)
2.  Connect the Pico to your computer. (PicoをPCに接続)
3.  Select the correct COM port and click the "Upload" button. (正しいCOMポートを選択し、「マイコンに書き込む」ボタンをクリック)

---

## 📄 License / ライセンス

This project is licensed under the **Apache License 2.0**. Please see the `LICENSE` file for details.
このプロジェクトは **Apache License 2.0** の下で公開されています。詳細は`LICENSE`ファイルをご覧ください。

*Note: This software contains code derived from the `pico-usb-midi-host-to-din-midi` sketch by rppicomidi, which is licensed under the MIT License. The original copyright notice is retained in the source file header.*
*(注: このソフトウェアは、MITライセンスで公開されているrppicomidi氏のスケッチを改変したコードを含んでいます。オリジナルの著作権表示はソースファイルヘッダに保持されています。)*
