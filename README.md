<div align="center">

# 🎛️ ESP32 WiFi Servo Controller

### Control a servo from any phone or laptop browser. No router. No internet. No app.

The ESP32 creates its **own password-protected WiFi network** and hosts a sleek web dashboard that controls an **SG90 servo** in real time.

![Platform](https://img.shields.io/badge/Platform-ESP32-blue?style=for-the-badge)
![Framework](https://img.shields.io/badge/Framework-Arduino-00979D?style=for-the-badge&logo=arduino&logoColor=white)
![Servo](https://img.shields.io/badge/Servo-SG90-orange?style=for-the-badge)
![Internet](https://img.shields.io/badge/Internet-Not%20Required-brightgreen?style=for-the-badge)
![License](https://img.shields.io/badge/License-MIT-yellow?style=for-the-badge)

[▶ YouTube](https://www.youtube.com/@techtadka360official) · [📸 Instagram](https://www.instagram.com/techtadka360official) · [📘 Facebook](https://www.facebook.com/share/1EkKAJNLdB/)

</div>

---

## 📖 Table of Contents

- [Overview](#-overview)
- [Watch the Demo](#-watch-the-demo)
- [Features](#-features)
- [How It Works](#-how-it-works)
- [Components Required](#-components-required)
- [Circuit and Wiring](#-circuit-and-wiring)
- [Software Setup](#-software-setup)
- [Usage](#-usage)
- [Configuration](#-configuration)
- [HTTP API](#-http-api)
- [Troubleshooting](#-troubleshooting)
- [Project Structure](#-project-structure)
- [Future Improvements](#-future-improvements)
- [Contributing](#-contributing)
- [Acknowledgements](#-acknowledgements)
- [License](#-license)
- [Connect With Me](#-connect-with-me)

---

## 🔎 Overview

Most ESP32 web-control projects need a home WiFi router, and many servo-control demos need a custom mobile app. This project needs neither.

The ESP32 runs in **Access Point (hotspot) mode**. You connect your phone or laptop to the WiFi network it creates, open a browser, and a full control dashboard loads straight from the ESP32. Everything is stored inside a single `.ino` file, so there is nothing else to install on your phone and no external server.

It works anywhere: in a workshop, outdoors, or at a college exhibition, with no internet connection at all.

---

## 🎬 Watch the Demo

Full build video on all three platforms:

| Platform | Video |
|----------|-------|
| ▶ **YouTube** | [Watch on YouTube](https://youtube.com/shorts/3-bds_YYXvM?si=2OO8KIHMmzt4YGhL) |
| 📸 **Instagram** | [Watch on Instagram](https://www.instagram.com/reel/DfHiahlpOvz/?utm_source=ig_web_copy_link&dlrf=MzRlODBiNWFlZA==&srtk=MzRlODBiNWFlZA==) |
| 📘 **Facebook** | [Watch on Facebook](https://www.facebook.com/share/r/193bfd1ufY/) |

---

## ✨ Features

- 📡 **Works fully offline.** The ESP32 creates its own WiFi hotspot.
- 🔒 **Password protected.** WPA2 hotspot, so only people with the password can connect.
- 🌐 **No app needed.** Works in any modern browser on Android, iOS, Windows, macOS and Linux.
- 🎯 **Interactive gauge.** Drag the servo horn or tap the arc to rotate.
- 🎚️ **Smooth slider.** Precise control from 0° to 180°.
- ⚡ **Quick-angle buttons.** Jump instantly to 0°, 45°, 90°, 135° or 180°.
- ➕➖ **Fine adjustment.** -1° and +1° buttons, plus a Centre button.
- 🔄 **Live feedback.** The dashboard shows the angle reported back by the ESP32.
- 👥 **Multi-device sync.** Several devices can be open at once and stay in sync.
- 📱 **Responsive design.** Built for phones first, and also looks good on laptops.
- 🧩 **Single-file project.** The web page is embedded in the sketch.

---

## ⚙️ How It Works

```
 Phone / Laptop                          ESP32                         SG90 Servo
┌──────────────┐   1. Join WiFi    ┌──────────────┐
│   Browser    │ ────────────────► │  Access Point │
│              │                   │ "ESP32-Servo" │
│  Dashboard   │ 2. GET /          │               │
│  (HTML/JS)   │ ◄──────────────── │  Web Server   │
│              │                   │               │   PWM signal
│  Drag/Tap    │ 3. GET /set?angle │               │ ───────────────► Horn rotates
│              │ ────────────────► │   GPIO 18     │
│              │ 4. {"angle": N}   │               │
│              │ ◄──────────────── │               │
└──────────────┘                   └──────────────┘
```

1. The ESP32 starts a WiFi hotspot named `ESP32-Servo`.
2. Your device joins it and opens `http://192.168.4.1`.
3. The ESP32 serves the dashboard page.
4. When you move the control, the browser sends the angle to the ESP32.
5. The ESP32 moves the servo and replies with the angle it applied.
6. The dashboard polls the ESP32 every few seconds, so every connected device stays in sync.

---

## 🧰 Components Required

| # | Component | Quantity | Notes |
|---|-----------|----------|-------|
| 1 | ESP32 development board | 1 | Any common ESP32 DevKit board |
| 2 | SG90 micro servo motor | 1 | 5V hobby servo |
| 3 | Breadboard | 1 | Or solder directly |
| 4 | Jumper wires | 3+ | Male-to-male |
| 5 | USB data cable | 1 | For power and programming |

---

## 🔌 Circuit and Wiring

| SG90 Servo Wire | Colour | ESP32 Pin |
|-----------------|--------|-----------|
| Signal | Orange / Yellow | **GPIO 18** |
| VCC | Red | **VIN / 5V** |
| GND | Brown | **GND** |

```
        ESP32                         SG90 Servo
   ┌─────────────┐
   │       GPIO18├────────────────────► Signal (Orange)
   │        VIN  ├────────────────────► VCC    (Red)
   │        GND  ├────────────────────► GND    (Brown)
   └─────────────┘
```

> **⚠️ Power note:** One SG90 can usually run from the ESP32's USB-powered 5V pin. If the servo jitters or the ESP32 resets, power the servo from a separate 5V supply and **connect its GND to the ESP32 GND**.

<!-- Add your circuit diagram here, for example:
![Circuit Diagram](circuit-diagram.png)
-->

---

## 💻 Software Setup

### 1. Install the Arduino IDE
Download it from [arduino.cc](https://www.arduino.cc/en/software).

### 2. Add ESP32 board support
1. Open **File → Preferences**.
2. In *Additional Boards Manager URLs*, add:
   ```
   https://espressif.github.io/arduino-esp32/package_esp32_index.json
   ```
3. Open **Tools → Board → Boards Manager**, search for **esp32**, and install the package by **Espressif Systems**.

### 3. Install the servo library
Open **Sketch → Include Library → Manage Libraries**, search for **ESP32Servo**, and install it.

### 4. Upload the code
1. Clone or download this repository.
2. Open `esp32_servo_web/esp32_servo_web.ino`. The folder name must match the file name.
3. Select your board (for example **ESP32 Dev Module**) and the correct COM port.
4. Click **Upload**.

---

## 🚀 Usage

1. Power the ESP32. The hotspot starts automatically.
2. On your phone or laptop, open WiFi settings and connect to:

   | Setting | Value |
   |---------|-------|
   | **WiFi name (SSID)** | `ESP32-Servo` |
   | **Password** | `12345678` |

3. Open a browser and go to **`http://192.168.4.1`**.
4. Control the servo by dragging the horn, using the slider, or tapping the angle buttons.

> Your phone may say **"No internet connection"**. That is normal, since the ESP32 has no internet. Stay connected.

---

## 🛠️ Configuration

Change these values at the top of the sketch:

```cpp
const char* AP_SSID = "ESP32-Servo";   // WiFi name people will see
const char* AP_PASS = "12345678";      // WiFi password (minimum 8 characters)

const int SERVO_PIN = 18;              // GPIO pin connected to the servo signal wire
```

> **🔐 Security tip:** Change the default password before sharing or demonstrating your project in public.

**Servo pulse range:** the sketch uses `500–2400 µs`. If your servo does not reach the full 0° to 180° range, adjust these numbers in `servo.attach(SERVO_PIN, 500, 2400);`.

---

## 🔗 HTTP API

The dashboard talks to the ESP32 with two simple endpoints. You can also call them from other tools.

| Endpoint | Method | Description | Example response |
|----------|--------|-------------|------------------|
| `/` | GET | Serves the dashboard page | HTML page |
| `/set?angle=N` | GET | Moves the servo to `N` degrees (0 to 180) | `{"angle":90}` |
| `/status` | GET | Returns the current angle | `{"angle":90}` |

Example:

```bash
curl "http://192.168.4.1/set?angle=135"
```

Values outside 0 to 180 are limited to that range automatically.

---

## 🩺 Troubleshooting

| Problem | Possible cause | Fix |
|---------|----------------|-----|
| Cannot see the `ESP32-Servo` WiFi | Board not powered or code not uploaded | Check the USB cable and re-upload |
| Cannot connect to the WiFi | Wrong password | Use the password in your sketch (minimum 8 characters) |
| Page does not open | Wrong address | Use `http://192.168.4.1` (not `https`) |
| Phone says "No internet" and switches network | Phone prefers mobile data | Stay connected, or turn mobile data off temporarily |
| Servo jitters or ESP32 resets | Not enough power | Use a separate 5V supply for the servo and share GND |
| Servo does not move | Wrong pin or loose wire | Check the signal wire is on GPIO 18 |
| Old page or odd characters appear | Browser cache | Close the tab and reopen the address |
| Upload fails | Wrong board or port | Select the correct board and COM port; hold the **BOOT** button while uploading if needed |

---

## 📁 Project Structure

```
esp32-servo-web-controller/
├── esp32_servo_web/
│   └── esp32_servo_web.ino    # Complete sketch with embedded web dashboard
├── circuit-diagram.png        # Wiring diagram (add yours)
├── README.md                  # You are here
└── LICENSE                    # MIT License
```

---

## 🔮 Future Improvements

- [ ] Bluetooth control option
- [ ] Control multiple servos from one dashboard
- [ ] Save favourite angles
- [ ] Sweep and loop modes
- [ ] Join an existing WiFi router as an alternative to hotspot mode
- [ ] Dark and light theme toggle

Have an idea? Open an issue.

---

## 🤝 Contributing

Contributions are welcome.

1. Fork this repository.
2. Create a branch: `git checkout -b feature/your-feature`
3. Commit your changes: `git commit -m "Add your feature"`
4. Push the branch: `git push origin feature/your-feature`
5. Open a Pull Request.

---

## 🙏 Acknowledgements

- Dashboard concept inspired by a servo-controller demo from [@electronicaavanzadapcm](https://www.instagram.com/electronicaavanzadapcm). This version is built differently: it runs as a web page hosted by the ESP32, so no mobile app is needed.
- [ESP32Servo](https://github.com/madhephaestus/ESP32Servo) library.
- The Arduino and ESP32 open-source community.

---

## 📄 License

This project is licensed under the **MIT License**. You are free to use, modify and share it. See the `LICENSE` file for details.

---

## 🌐 Connect With Me

<div align="center">

### **TechTadka360**
*Electronics · ESP32 · Arduino · DIY Projects*

[![YouTube](https://img.shields.io/badge/YouTube-@techtadka360official-FF0000?style=for-the-badge&logo=youtube&logoColor=white)](https://www.youtube.com/@techtadka360official)
[![Instagram](https://img.shields.io/badge/Instagram-@techtadka360official-E4405F?style=for-the-badge&logo=instagram&logoColor=white)](https://www.instagram.com/techtadka360official)
[![Facebook](https://img.shields.io/badge/Facebook-TechTadka360-1877F2?style=for-the-badge&logo=facebook&logoColor=white)](https://www.facebook.com/share/1EkKAJNLdB/)
[![GitHub](https://img.shields.io/badge/GitHub-Mr--soumik-181717?style=for-the-badge&logo=github&logoColor=white)](https://github.com/Mr-soumik)

**If this project helped you, please ⭐ star the repository, subscribe on YouTube and follow on Instagram and Facebook.**

*Build one small thing every day. Small projects today become big inventions tomorrow.* 🚀

</div>
