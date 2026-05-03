# FlightAPT-Display ✈️

Project by: Sean Young (@1shyoung), Martin Gong (@martingongg)

A real-time, ESP32-based digital display that shows active passenger
flights overhead on a 64×64 HUB75 RGB LED matrix.  This project leverages
open-source technologies to provide a professional-grade flight tracking
solution that is both cost-effective and highly customizable for makers
and hobbyists.

![Build Status](https://img.shields.io/badge/build-passing-brightgreen)
![Platform](https://img.shields.io/badge/platform-ESP32-blue)
![License](https://img.shields.io/badge/license-MIT-lightgrey)

---

## 📋 Features

* **Real-Time Flight Tracking:** Polls the public [OpenSky Network](https://opensky-network.org/) API every 15 s for every aircraft inside a configurable bounding box around your home.
* **High-Performance Platform:** Built on the ESP32-WROOM-32 — dual-core 240 MHz processor, 520 KB SRAM, and built-in 802.11 b/g/n WiFi.
* **Cost-Effective & Accessible:** Stock 64×64 / 2.5 mm HUB75 panels are widely available; everything else is open-source.
* **Portable & Flexible:** Runs from any 5 V supply that can deliver ≥ 3 A peak for the panel; a small LiPo + boost converter makes it field-portable.
* **Customizable:** Modular sketch (config / api / display / flight) makes it easy to retarget the display to a different data source.

---

## ⚙️ Hardware

| Part                   | Notes                                          |
| :--------------------- | :--------------------------------------------- |
| ESP32-WROOM-32 dev kit | Any 38-pin board works.                        |
| 64×64 HUB75 RGB matrix | 2.5 mm pitch (the supplied STL casing fits this size). |
| 5 V / 4 A PSU          | Panel + ESP32 share the rail.                  |
| 3D-printed enclosure   | STL files in repo root.                        |
| 16-pin HUB75 ribbon    | Comes with most panels.                        |

### Default pin map (edit in `config.h`)

| Signal | GPIO | | Signal | GPIO |
| :----- | :--- | :- | :----- | :--- |
| R1     | 25   | | R2     | 14   |
| G1     | 26   | | G2     | 12   |
| B1     | 27   | | B2     | 13   |
| A      | 23   | | B      | 19   |
| C      | 5    | | D      | 17   |
| E      | 18   | | LAT    | 4    |
| OE     | 15   | | CLK    | 16   |

> **E** is required for the 1/32 scan rate of 64-row panels.

---

## 🚀 Software Installation

### 1. Arduino IDE

1. Install [Arduino IDE 2.x](https://www.arduino.cc/en/software).
2. `File ▸ Preferences ▸ Additional boards manager URLs`
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
3. `Tools ▸ Board ▸ Boards Manager` → install **esp32 by Espressif Systems**.
4. Select **ESP32 Dev Module** from `Tools ▸ Board`.

### 2. Libraries

Install from the Library Manager (`Sketch ▸ Include Library ▸ Manage Libraries…`):

* **ESP32-HUB75-MatrixPanel-DMA** by *mrcodetastic / mrfaptastic*
* **ArduinoJson** by *Benoit Blanchon* (v6.x)
* **Adafruit GFX Library** (auto-installed with the matrix lib)

`WiFi.h`, `WiFiClientSecure.h`, and `HTTPClient.h` ship with the ESP32 core.

### 3. Configure

Open [`flight_apt_display/config.h`](./flight_apt_display/config.h) and set:

```cpp
#define WIFI_SSID      "your-ssid"
#define WIFI_PASSWORD  "your-password"

#define HOME_LAT        47.6062f     // your latitude  (+N)
#define HOME_LON       -122.3321f    // your longitude (-W)
#define SEARCH_RADIUS    0.40f       // ± degrees around home
```

The sketch uses anonymous OpenSky access by default.  If you have a
free OpenSky account, set `OPENSKY_USER` / `OPENSKY_PASS` for higher
rate limits.

### 4. Build & Flash

1. Open `flight_apt_display/flight_apt_display.ino`.
2. Select your serial port and click **Upload**.
3. Open the Serial Monitor at **115 200 baud** to watch the boot log.

---

## 🖼️ On-screen layout

```
┌────────────────────── 64 px ──────────────────────┐
│ AAL237                                            │  ← callsign (yellow, marquees if long)
│ United States                                     │  ← origin country (grey)
├───────────────────────────────────────────────────┤
│   ╭───╮                                           │
│   │ ↗ │   34500ft        ← altitude  (white)      │
│   ╰───╯    420kt          ← ground speed (green)  │
│            285HDG         ← heading      (orange) │
├───────────────────────────────────────────────────┤
│ D 12.3km                                       ↗  │  ← distance from home
│ B 045°                                            │  ← bearing from home
└───────────────────────────────────────────────────┘
```

The compass on the left always shows true-north up; the bearing arrow
on the bottom-right points from your home toward the aircraft.

---

## 📁 Source layout

```
flight_apt_display/
├── flight_apt_display.ino   - main sketch (setup/loop, scheduling)
├── config.h                 - user-editable WiFi / location / pins
├── api.{h,cpp}              - OpenSky Network REST client
├── flight.{h,cpp}           - Flight data model + great-circle math
├── display.{h,cpp}          - HUB75 rendering (splash / status / card)
└── libraries.txt            - dependency list
```

---

## 🛠️ Troubleshooting

| Issue                  | Symptoms                       | Resolution                                             |
| :--------------------- | :----------------------------- | :----------------------------------------------------- |
| **WiFi Disconnections**| Periodic connectivity loss     | `WiFi.setSleep(false)` is enabled; verify router 2.4 GHz radio is reachable. |
| **Random Resets**      | Unexpected reboots             | Increase PSU current (panel pulls 2–3 A peaks); add a 1000 µF bulk cap across the panel rail. |
| **Display Corruption** | Garbled / shimmery image       | Try `cfg.clkphase = true` in `display.cpp`; shorten the HUB75 ribbon. |
| **Memory Exhaustion**  | Crashes after a few hours      | Lower the `DynamicJsonDocument` size in `api.cpp` or shrink `SEARCH_RADIUS`. |
| **HTTP -1 / -3**       | OpenSky returns 429            | Anonymous endpoint is rate-limited to 10 s; back off `POLL_INTERVAL_MS` or use credentials. |

---

## 🙏 Acknowledgements

* Developed by **Sean Young (@1shyoung)** and **Martin Gong (@martingongg)**.
* Inspiration adapted from the excellent [flightportal](https://github.com/smartbutnot/flightportal) project.
* Live data courtesy of the [OpenSky Network](https://opensky-network.org/).
* Display driver: [ESP32-HUB75-MatrixPanel-DMA](https://github.com/mrcodetastic/ESP32-HUB75-MatrixPanel-DMA).

---

## 📜 License

MIT — see [`LICENSE`](./LICENSE) (or the badge at the top).
