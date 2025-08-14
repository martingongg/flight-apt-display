# FlightAPT-Display ✈️

Project by: Sean Young (@1shyoung), Martin Gong (@martingongg)

A real-time, ESP32-based digital display that shows active passenger flights over its current location. This project leverages open-source technologies to provide a professional-grade flight tracking solution that is both cost-effective and highly customizable for makers and hobbyists.

![Build Status](https://img.shields.io/badge/build-passing-brightgreen)
![Platform](https://img.shields.io/badge/platform-ESP32-blue)
![License](https://img.shields.io/badge/license-MIT-lightgrey)

---

## 📋 Features

* **Real-Time Flight Tracking:** Connects to public aviation data APIs via WiFi to display live aircraft positions.
* **High-Performance Platform:** Built on the ESP32-WROOM-32, offering a dual-core processor, ample RAM, and built-in WiFi for smooth, responsive operation.
* **Cost-Effective & Accessible:** Uses readily available components and open-source software, making it an affordable alternative to proprietary systems.
* **Portable & Flexible:** Low power consumption allows for both stationary home setups and portable, battery-powered field use.
* **Customizable:** A flexible platform suitable for various applications, from a dedicated flight tracker to a multi-purpose display for other projects.

---

## ⚙️ Hardware Required

* **Microcontroller:** ESP32-WROOM-32
* **Display:** Any compatible SPI or I2C display (e.g., TFT, OLED)
* **Power Supply:** 5V USB power supply or a 3.3V LiPo battery for portable use.
* **Enclosure:** (Optional) A 3D-printed or project case for protection.

### Why the ESP32?

The **ESP32-WROOM-32** is the optimal choice for this application. Its integrated WiFi, generous memory, and dual-core architecture enable continuous collection, decoding, and visualization of aircraft data without the performance bottlenecks of traditional microcontrollers.

| Feature         | ESP32                | ESP8266            | Arduino Uno      |
| :-------------- | :------------------- | :----------------- | :--------------- |
| **Processor** | Dual-core 240 MHz    | Single-core 80 MHz | Single-core 16 MHz |
| **RAM** | 520 KB SRAM          | 160 KB             | 2 KB             |
| **WiFi** | Built-in 802.11b/g/n | Built-in 802.11b/g/n | Requires shield  |
| **GPIO Pins** | 32                   | 17                 | 14               |
| **Price Range** | $6–8                 | $3–5               | $20–25           |

---

## 🚀 Software Installation & Setup

### 1. Configure Arduino IDE

1.  **Install Arduino IDE:** Download and install the latest version from the [official Arduino website](https://www.arduino.cc/en/software).
2.  **Add ESP32 Board Manager:**
    * Open Arduino IDE and go to `File` > `Preferences` (or `Arduino IDE` > `Settings...` on macOS).
    * In the "Additional boards manager URLs" field, paste the following URL:
        ```
        [https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json](https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json)
        ```
    * Click "OK".
3.  **Install ESP32 Boards:**
    * Go to `Tools` > `Board` > `Boards Manager...`.
    * Search for "ESP32" and install the package by **Espressif Systems**.
    * After installation, select your specific ESP32 board from the `Tools` > `Board` menu (e.g., "ESP32 Dev Module").

### 2. Install Libraries

This project requires several libraries. Install them using the Arduino Library Manager (`Sketch` > `Include Library` > `Manage Libraries...`):

* `WiFi.h`: For connecting to the internet.
* `HTTPClient.h`: For making requests to the flight data API.
* `ArduinoJson.h`: For parsing the JSON data returned by the API.
* A library for your specific display (e.g., `TFT_eSPI` for TFT displays).

### 3. Configure Project Settings

1.  **Clone the Repository:**
    ```sh
    git clone [https://github.com/your-username/flight-apt-display.git](https://github.com/your-username/flight-apt-display.git)
    ```
2.  **Add WiFi Credentials:** Open the main `.ino` file and update the following lines with your network details:
    ```cpp
    const char* ssid = "YOUR_WIFI_SSID";
    const char* password = "YOUR_WIFI_PASSWORD";
    ```
3.  **Set API Endpoint & Location:** Configure the API URL and your geographical coordinates (latitude/longitude) to fetch flights for your specific area.

---

## 🛠️ Troubleshooting

Attention to hardware design significantly impacts system reliability. A stable power supply, proper shielding, and good antenna placement are key to creating a robust, professional-grade monitoring solution.

| Issue                  | Symptoms                       | Resolution                                             |
| :--------------------- | :----------------------------- | :----------------------------------------------------- |
| **WiFi Disconnections**| Periodic connectivity loss     | Disable modem sleep; implement reconnection logic.      |
| **Random Resets** | Unexpected reboots             | Increase power supply capacity; add bulk capacitance.  |
| **Display Corruption** | Garbled screen content         | Reduce I2C/SPI speed; improve ground connections.      |
| **Memory Exhaustion** | Crashes after extended operation | Implement memory monitoring; reduce buffer sizes.      |

---

## 🙏 Acknowledgements

* This project was developed by **Sean Young ("1shyoung")**.
* Inspiration and core concepts were adapted from the [flightportal](https://github.com/smartbutnot/flightportal) project. Thank you to the original creators for their excellent work.

---

## 📜 License

This project is licensed under the MIT License. See the `LICENSE` file for details.
