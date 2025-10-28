# Pico USB MIDI Host to DIN Converter

!(https://www.raspberrypi.com/products/raspberry-pi-pico/featured.jpg)

A simple and effective USB MIDI Host to 5-pin DIN MIDI converter using a Raspberry Pi Pico (RP2040). This project allows you to connect modern USB-only MIDI controllers to legacy synthesizers or sound modules that only have traditional 5-pin DIN MIDI inputs.

---

## 🎵 Background

Modern MIDI controllers often connect and are powered via a single USB cable. However, classic sound modules, like the Roland Sonic Cell, only accept MIDI signals through a 5-pin DIN connector and cannot provide power to the controller. This necessitates separate power adapters and extra cables, which can be cumbersome.

This project was born out of a desire to bridge that gap, creating a simple converter that allows a USB MIDI keyboard to control a DIN MIDI sound module, streamlining the setup.

---

## ✨ Features

* **USB MIDI Host:** Receives MIDI data from a class-compliant USB MIDI device.
* **5-Pin DIN MIDI Out:** Forwards the received MIDI messages to a standard 5-pin MIDI OUT port.
* **Simple & Low-Cost:** Built with a Raspberry Pi Pico and a handful of common components.

---

## 🛠️ Hardware

### Required Components
* **MCU:** Raspberry Pi Pico or a compatible RP2040 board (e.g., RP2040 Zero)
* **Connectors:**
    * 5-pin DIN female socket
    * USB Type-C breakout board (for power)
* **Resistors:**
    * 2 x 220Ω (for MIDI OUT circuit)
    * 1 x 5.1kΩ (for the Type-C breakout board to enable C-to-C power delivery)

### Wiring

1.  **MIDI OUT Circuit:**
    * Connect **GP8 (UART1 TX)** of the Pico to the **MIDI DIN Pin 5** through a **220Ω resistor**.
    * Connect **3.3V** from the Pico to the **MIDI DIN Pin 4** through a **220Ω resistor**.
    * Connect **GND** from the Pico to the **MIDI DIN Pin 2**.
2.  **Power:**
    * Solder the 5.1kΩ resistor to the CC pin on the Type-C breakout board if it's not already present.
    * Connect the VBUS and GND from the breakout board to the VSYS and GND pins on the Pico to power it.
3.  **USB MIDI Host:**
    * Connect your USB MIDI device to the Pico's main USB port. You will need a USB OTG adapter (Micro USB or Type-C depending on your RP2040 board).

---

## ⚙️ Software & Setup

This project is built using the Arduino framework for the Raspberry Pi Pico.

### 1. Arduino IDE Setup
* Install the Arduino IDE.
* Add the Raspberry Pi Pico board to the IDE. In the "Additional Board Manager URLs" field in Preferences, add the following URL:
    ```
    [https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json](https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json)
    ```
* Go to **Tools > Board > Boards Manager**, search for "pico", and install "Raspberry Pi Pico/RP2040".

### 2. Board Configuration
* In the **Tools** menu, select the following options:
    * **Board:** "Raspberry Pi Pico" (or your specific RP2040 board)
    * **USB Stack:** "Adafruit TinyUSB Host"

### 3. Libraries
* This sketch requires the **Adafruit TinyUSB Library**. Install it from **Tools > Manage Libraries**.

### 4. Uploading
* Connect the Pico to your computer while holding the BOOTSEL button.
* Select the correct COM port and click the "Upload" button.

---

## 📄 License

This project is licensed under the **Apache License 2.0**. Please see the `LICENSE` file for details.

Note: This software contains code derived from the `pico-usb-midi-host-to-din-midi` sketch by rppicomidi, which is licensed under the MIT License. The original copyright notice is retained in the source file header.
