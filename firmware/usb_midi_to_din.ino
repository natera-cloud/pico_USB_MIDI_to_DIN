/*********************************************************************
 MIT license, check LICENSE for more information
 Copyright (c) 2023 rppicomidi
 
 This sketch is modified to act as a USB MIDI Host to DIN MIDI converter.
 It receives MIDI from a USB device and forwards it to Serial2 (GP8)
 at the standard MIDI baud rate of 31250.
*********************************************************************/

#ifndef USE_TINYUSB_HOST
#error "Please Select USB Stack: Adafruit TinyUSB Host in Tools menu"
#else
// All Serial Monitor Output is on Serial1 (UART0).
// MIDI DIN Output is on Serial2 (UART1, default TX: GP8).
#endif

#include "Adafruit_TinyUSB.h"

#define LANGUAGE_ID 0x0409  // English

// Add USB MIDI Host support to Adafruit_TinyUSB
#include "usb_midi_host.h"

// USB Host object
Adafruit_USBH_Host USBHost;

// holding device descriptor
tusb_desc_device_t desc_device;

// holding the device address of the MIDI device
uint8_t midi_dev_addr = 0;


// the setup function runs once when you press reset or power the board
void setup() {
  // For Debugging: Connect to this via USB or another UART-USB converter on GP0/GP1
  Serial1.begin(115200);

  // For MIDI DIN Output: Connect your MIDI OUT circuit to GP8 (default TX for Serial2)
  Serial2.begin(31250);

  while (!Serial1) {
    delay(100);  // wait for serial port to initialize
  }

  USBHost.begin(0);  // 0 means use native RP2040 host

  Serial1.println("=========================================");
  Serial1.println("    Pico USB MIDI Host to DIN Converter  ");
  Serial1.println("=========================================");
}

void loop() {
  // TinyUSB Host task
  USBHost.task();
}


//--------------------------------------------------------------------+
// TinyUSB Host callbacks
//--------------------------------------------------------------------+

// Invoked when a new device is mounted (configured)
void tuh_mount_cb(uint8_t daddr) {
  Serial1.printf("Device attached, address = %d\r\n", daddr);
  // Get and print device descriptor
  tuh_descriptor_get_device(daddr, &desc_device, 18, print_device_descriptor, 0);
}

/// Invoked when device is unmounted (bus reset/unplugged)
void tuh_umount_cb(uint8_t daddr) {
  Serial1.printf("Device removed, address = %d\r\n", daddr);
}

// Invoked when a MIDI device is mounted
void tuh_midi_mount_cb(uint8_t dev_addr, uint8_t in_ep, uint8_t out_ep, uint8_t num_cables_rx, uint16_t num_cables_tx) {
  Serial1.printf("MIDI device mounted: address = %u, IN endpoint %u has %u cables, OUT endpoint %u has %u cables\r\n",
                 dev_addr, in_ep & 0xf, num_cables_rx, out_ep & 0xf, num_cables_tx);

  if (midi_dev_addr == 0) {
    // Connect to the first available MIDI device
    midi_dev_addr = dev_addr;
  } else {
    Serial1.println("A different USB MIDI Device is already connected. Only one device is supported.");
  }
}

// Invoked when a MIDI device is unmounted
void tuh_midi_umount_cb(uint8_t dev_addr, uint8_t instance) {
  if (dev_addr == midi_dev_addr) {
    midi_dev_addr = 0;
    Serial1.printf("MIDI device unmounted: address = %d, instance = %d\r\n", dev_addr, instance);
  }
}

// *** CORE LOGIC ***
// Invoked when MIDI data is received from the USB device
void tuh_midi_rx_cb(uint8_t dev_addr, uint32_t num_packets) {
  if (midi_dev_addr == dev_addr && num_packets != 0) {
    uint8_t cable_num;
    uint8_t buffer[48];  // Buffer to hold incoming MIDI packets

    // Read MIDI packets from the USB stream
    while (uint32_t bytes_read = tuh_midi_stream_read(dev_addr, &cable_num, buffer, sizeof(buffer))) {
      // --- 1. FORWARD TO MIDI DIN OUT ---
      // Write the received bytes directly to the MIDI output UART (Serial2)
      Serial2.write(buffer, bytes_read);

      // --- 2. (Optional) PRINT TO DEBUG CONSOLE ---
      // Print the MIDI message to Serial1 for debugging purposes
      Serial1.printf("USB MIDI RX | Cable #%u | Bytes: %lu | Data: ", cable_num, bytes_read);
      for (uint32_t i = 0; i < bytes_read; i++) {
        Serial1.printf("%02X ", buffer[i]);
      }
      Serial1.println();
    }
  }
}

//--------------------------------------------------------------------+
// Helper functions for printing device info (unchanged from original)
//--------------------------------------------------------------------+

void print_device_descriptor(tuh_xfer_t *xfer) {
  if (XFER_RESULT_SUCCESS != xfer->result) {
    Serial1.printf("Failed to get device descriptor\r\n");
    return;
  }

  uint8_t const daddr = xfer->daddr;
  uint16_t temp_buf[128];

  Serial1.println("--------------------");
  Serial1.printf("Device Descriptor:\r\n");
  Serial1.printf("  iManufacturer       %u     ", desc_device.iManufacturer);
  if (XFER_RESULT_SUCCESS == tuh_descriptor_get_manufacturer_string_sync(daddr, LANGUAGE_ID, temp_buf, sizeof(temp_buf))) {
    print_utf16(temp_buf, TU_ARRAY_SIZE(temp_buf));
  }
  Serial1.println();

  Serial1.printf("  iProduct            %u     ", desc_device.iProduct);
  if (XFER_RESULT_SUCCESS == tuh_descriptor_get_product_string_sync(daddr, LANGUAGE_ID, temp_buf, sizeof(temp_buf))) {
    print_utf16(temp_buf, TU_ARRAY_SIZE(temp_buf));
  }
  Serial1.println();

  Serial1.printf("  iSerialNumber       %u     ", desc_device.iSerialNumber);
  if (XFER_RESULT_SUCCESS == tuh_descriptor_get_serial_string_sync(daddr, LANGUAGE_ID, temp_buf, sizeof(temp_buf))) {
    print_utf16(temp_buf, TU_ARRAY_SIZE(temp_buf));
  }
  Serial1.println();
  Serial1.println("--------------------");
}

static void _convert_utf16le_to_utf8(const uint16_t *utf16, size_t utf16_len, uint8_t *utf8, size_t utf8_len) {
  (void)utf8_len;
  for (size_t i = 0; i < utf16_len; i++) {
    uint16_t chr = utf16[i];
    if (chr < 0x80) {
      *utf8++ = chr & 0xff;
    } else if (chr < 0x800) {
      *utf8++ = (uint8_t)(0xC0 | (chr >> 6 & 0x1F));
      *utf8++ = (uint8_t)(0x80 | (chr >> 0 & 0x3F));
    } else {
      *utf8++ = (uint8_t)(0xE0 | (chr >> 12 & 0x0F));
      *utf8++ = (uint8_t)(0x80 | (chr >> 6 & 0x3F));
      *utf8++ = (uint8_t)(0x80 | (chr >> 0 & 0x3F));
    }
  }
}

static int _count_utf8_bytes(const uint16_t *buf, size_t len) {
  size_t total_bytes = 0;
  for (size_t i = 0; i < len; i++) {
    uint16_t chr = buf[i];
    if (chr < 0x80) {
      total_bytes += 1;
    } else if (chr < 0x800) {
      total_bytes += 2;
    } else {
      total_bytes += 3;
    }
  }
  return total_bytes;
}

static void print_utf16(uint16_t *temp_buf, size_t buf_len) {
  size_t utf16_len = ((temp_buf[0] & 0xff) - 2) / sizeof(uint16_t);
  size_t utf8_len = _count_utf8_bytes(temp_buf + 1, utf16_len);
  _convert_utf16le_to_utf8(temp_buf + 1, utf16_len, (uint8_t *)temp_buf, sizeof(uint16_t) * buf_len);
  ((uint8_t *)temp_buf)[utf8_len] = '\0';
  Serial1.print((char *)temp_buf);
}
