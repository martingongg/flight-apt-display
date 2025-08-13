# flight-apt-display
A ESP32 - based digital display showing active passenger flights over it's current set location.

The primary objectives of this project include developing a reliable, cost-effective solution that provides professional-grade functionality while remaining accessible to makers and hobbyists. Current market solutions often lack customization options or require expensive proprietary hardware. This implementation leverages open-source technologies and readily available components to create a flexible platform suitable for various applications ranging from kitchen timers to productivity management tools.

## Hardware Architecture and Component Selection

### 2.1 Microcontroller Selection
The **ESP32-WROOM-32** emerges as the optimal microcontroller choice for this application. Its specifications significantly exceed traditional Arduino boards while maintaining compatibility with the Arduino IDE and ecosystem.

| Feature      | ESP32              | ESP8266           | Arduino Uno          |
|--------------|--------------------|-------------------|----------------------|
| Processor    | Dual-core 240 MHz  | Single-core 80 MHz| Single-core 16 MHz   |
| RAM          | 520 KB SRAM        | 160 KB            | 2 KB                 |
| WiFi         | Built-in 802.11b/g/n | Built-in 802.11b/g/n | Requires shield     |
| GPIO Pins    | 32                 | 17                | 14                   |
| Price Range  | $6–8               | $3–5              | $20–25               |

## Common Issues and Solutions

| Issue              | Symptoms                     | Resolution                                                   |
|--------------------|------------------------------|--------------------------------------------------------------|
| WiFi Disconnections| Periodic connectivity loss   | Disable modem sleep, implement reconnection logic            |
| Random Resets      | Unexpected reboots            | Increase power supply capacity, add bulk capacitance         |
| Display Corruption | Garbled screen content        | Reduce I2C speed, improve ground connections                 |
| Audio Distortion   | Crackling or noise            | Separate analog/digital grounds, add ferrite beads           |
| Memory Exhaustion  | Crashes after extended operation | Implement memory monitoring, reduce buffer sizes          |
