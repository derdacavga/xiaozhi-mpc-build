# ESP32-S3 Xiaozhi AI Assistant with MCP Hardware Control

An offline/online voice-activated AI assistant built on the **ESP32-S3**, powered by the **Xiaozhi AI** framework and integrated with **MCP (Multi-Control Processor / Model Context Protocol)** tools to control external hardware devices using natural voice commands.

## 📺 Video Tutorial

[![Watch the tutorial](https://img.youtube.com/vi/bf-1MPShis4/0.jpg)](https://www.youtube.com/watch?v=bf-1MPShis4)
> **Click the image above to watch the full tutorial on YouTube.**

---
## 📺 Previous Version Video Tutorial

[![Watch the tutorial](https://img.youtube.com/vi/hBlcBcnU5mk/0.jpg)](https://www.youtube.com/watch?v=hBlcBcnU5mk)
> **Click the image above to watch the full tutorial on YouTube.**

---

## Features

- **Voice Assistant Integration:** Complete setup with Xiaozhi AI server console (`xiaozhi.me`).
- **ESP-IDF v6+ Compatible:** Updated project structure and build configurations for recent ESP-IDF toolchains.
- **Custom Wake Word:** Configured with wake word support (e.g., "Sophia") and multi-wake options.
- **Custom MCP Hardware Control:** Control real-world peripherals via voice prompts:
  - **Servo Motor:** Precise angle/degree positioning.
  - **Standard LED / Lamp:** On/Off toggling and brightness control.
  - **Addressable RGB LEDs:** Dynamic color switching and patterns.
  - **Relay Module:** High-voltage switching representation (On/Off).
- **Web-Based Firmware Builder Alternative:** Quick-flash options directly from the web console for no-compile setups.

---

## Hardware Requirements (BOM)

| Component | Description |
| :--- | :--- |
| **Microcontroller** | ESP32-S3 Dev Board (N16R8 — 16MB Flash, 8MB Octal PSRAM) |
| **Display** | SSD1306 0.96" I2C OLED (128x64) |
| **Microphone** | INMP441 I2S MEMS Omnidirectional Microphone |
| **Audio Amplifier** | MAX98357A I2S Mono DAC / Amplifier |
| **Actuators & Outputs** | Micro Servo (SG90), Relay Module, LEDs, Ws2812b Addressable RGB LED |

---

## Configuration & Setup

 Included in the tutorial

---

## Extending MCP Tools

Custom hardware interactions are defined inside `custom_devices.cc`. To register a new peripheral:

1. Declare the tool in `custom_devices.cc` specifying parameters and callback routines.
2. Expose the function signature in the corresponding header file.
3. Map the target GPIO pins inside the board's `config` file.

---

## 🤝 Support

If you found this project helpful, please consider:
* **Subscribing** to the YouTube Channel.
* Giving the video a **Like**.
* Starring this GitHub Repository!

* **YouTube:** [https://www.youtube.com/@DsnIndustries/videos]
* **Patreon:** [https://www.patreon.com/c/dsnIndustries]

Happy Making!

## Games
* **Maze Escape:** https://play.google.com/store/apps/details?id=com.DsnMechanics.MazeEscape
* **Air Hockey:** https://play.google.com/store/apps/details?id=com.DsnMechanics.AirHockey
* **Click Challenge:** https://play.google.com/store/apps/details?id=com.DsNMechanics.ClickChallenge
* **Flying Triangels:** https://play.google.com/store/apps/details?id=com.DsnMechanics.Triangle
* **SkyScrapper:** https://play.google.com/store/apps/details?id=com.DsnMechanics.SkyScraper
