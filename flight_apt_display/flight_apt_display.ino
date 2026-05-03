// =====================================================================
//  FlightAPT-Display - main sketch
// =====================================================================
//
//  An ESP32-WROOM-32 polls the OpenSky Network REST API every few
//  seconds for aircraft inside a bounding box around the user's home
//  coordinates and shows them, one at a time, on a 64x64 HUB75 RGB LED
//  matrix.
//
//  Authors:  Sean Young (1shyoung), Martin Gong (martingongg)
//  License:  MIT
//
//  Build:
//    Board    : "ESP32 Dev Module"
//    Flash    : 4 MB / 80 MHz / QIO
//    Partition: "Default 4MB with spiffs (1.2MB APP/1.5MB SPIFFS)"
//    Upload   : 921600 baud
//
//  Required libraries (Library Manager):
//    * ESP32-HUB75-MatrixPanel-DMA   (mrcodetastic / mrfaptastic)
//    * ArduinoJson                   (Benoit Blanchon)   v6.x
//    * Adafruit GFX Library          (pulled in by HUB75 lib)
//
// =====================================================================

#include <Arduino.h>
#include <WiFi.h>

#include "config.h"
#include "flight.h"
#include "api.h"
#include "display.h"

// =====================================================================
// Globals
// =====================================================================
static std::vector<Flight> g_flights;
static unsigned long       g_lastPollMs   = 0;
static unsigned long       g_lastRotateMs = 0;
static size_t              g_currentIdx   = 0;

// =====================================================================
// Wi-Fi management
// =====================================================================
static void connectWiFi() {
  Serial.printf("[wifi] connecting to '%s'...\n", WIFI_SSID);
  Display::showStatus("WiFi", WIFI_SSID);

  WiFi.persistent(false);
  WiFi.mode(WIFI_STA);
  WiFi.setSleep(false);                          // anti-disconnect
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED &&
         millis() - start < 30000UL) {
    delay(250);
    Serial.print('.');
  }
  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    Serial.printf("[wifi] connected, IP %s, RSSI %ddBm\n",
                  WiFi.localIP().toString().c_str(),
                  WiFi.RSSI());
    Display::showStatus("WiFi OK",
                        WiFi.localIP().toString().c_str());
    delay(800);
  } else {
    Serial.println("[wifi] failed -> restarting");
    Display::showStatus("WiFi FAIL", "rebooting");
    delay(2000);
    ESP.restart();
  }
}

static void ensureWiFi() {
  if (WiFi.status() == WL_CONNECTED) return;
  Serial.println("[wifi] dropped, reconnecting");
  WiFi.disconnect(true);
  connectWiFi();
}

// =====================================================================
// API polling
// =====================================================================
static void pollFlights() {
  ensureWiFi();

  float laMin = HOME_LAT - SEARCH_RADIUS;
  float laMax = HOME_LAT + SEARCH_RADIUS;
  float loMin = HOME_LON - SEARCH_RADIUS;
  float loMax = HOME_LON + SEARCH_RADIUS;

  std::vector<Flight> next;
  int n = OpenSky::fetchFlightsInBox(laMin, loMin, laMax, loMax, next);
  if (n < 0) {
    Serial.printf("[api] fetch failed (%d)\n", n);
    return;                                      // keep showing old data
  }

  Serial.printf("[api] %d flight(s) in box\n", n);

  // Sort by distance from home so the closest plane is shown first.
  std::sort(next.begin(), next.end(),
            [](const Flight &a, const Flight &b) {
              return a.distanceKm(HOME_LAT, HOME_LON) <
                     b.distanceKm(HOME_LAT, HOME_LON);
            });

  g_flights    = std::move(next);
  g_currentIdx = 0;
  g_lastRotateMs = millis();
}

// =====================================================================
// Render the currently-selected flight (or the no-flights screen)
// =====================================================================
static void render() {
  if (g_flights.empty()) {
    Display::showNoFlights();
    return;
  }

  if (g_currentIdx >= g_flights.size()) g_currentIdx = 0;
  Display::showFlight(g_flights[g_currentIdx], HOME_LAT, HOME_LON);
}

// =====================================================================
// Arduino entry points
// =====================================================================
void setup() {
  Serial.begin(115200);
  delay(200);
  Serial.println();
  Serial.println("=== FlightAPT-Display starting ===");

  Display::begin();
  Display::showSplash();
  delay(1500);

  connectWiFi();

  // First poll right away
  pollFlights();
  render();
  g_lastPollMs = millis();
}

void loop() {
  unsigned long now = millis();

  // 1) Poll the API on a fixed cadence
  if (now - g_lastPollMs >= POLL_INTERVAL_MS) {
    g_lastPollMs = now;
    pollFlights();
    render();
  }

  // 2) Rotate to the next flight every FLIGHT_DWELL_MS
  if (!g_flights.empty() &&
      now - g_lastRotateMs >= FLIGHT_DWELL_MS) {
    g_lastRotateMs = now;
    g_currentIdx   = (g_currentIdx + 1) % g_flights.size();
    render();
  }

  // 3) Re-draw every 250 ms so marquee text & arrows stay live
  static unsigned long lastFrame = 0;
  if (now - lastFrame >= 250UL) {
    lastFrame = now;
    if (!g_flights.empty()) render();
  }

  delay(10);
}
