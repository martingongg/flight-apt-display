// =====================================================================
//  FlightAPT-Display - User configuration
// =====================================================================
//  Edit the values in this file before flashing the sketch.
//  Nothing here changes program behaviour at compile time other than
//  the constants below; everything else lives in the .ino files.
// =====================================================================

#pragma once

// ---------------------------------------------------------------------
// 1) Wi-Fi credentials
// ---------------------------------------------------------------------
#define WIFI_SSID       "YOUR_WIFI_SSID"
#define WIFI_PASSWORD   "YOUR_WIFI_PASSWORD"

// ---------------------------------------------------------------------
// 2) Geographical search box (decimal degrees)
//    Defaults are roughly Seattle-Tacoma area.  The OpenSky Network
//    API expects a bounding box: (lamin, lomin) -> (lamax, lomax).
//    Make the box ~0.5 deg on a side for "what's overhead right now".
// ---------------------------------------------------------------------
#define HOME_LAT        47.6062f     // Your latitude  (deg, +N)
#define HOME_LON       -122.3321f    // Your longitude (deg, +E / -W)
#define SEARCH_RADIUS    0.40f       // +/- degrees around HOME_*

// ---------------------------------------------------------------------
// 3) OpenSky Network API
//    Anonymous access works (rate-limited to 10 s).  If you have a
//    free account, fill in the credentials below for a higher rate.
// ---------------------------------------------------------------------
#define OPENSKY_USER    ""           // optional - leave blank for anon
#define OPENSKY_PASS    ""           // optional - leave blank for anon
#define OPENSKY_URL     "https://opensky-network.org/api/states/all"

// How often to poll the API (ms).  10 s is the anonymous minimum.
#define POLL_INTERVAL_MS  15000UL

// How long a single flight stays on screen before we rotate to the
// next one when more than one aircraft is in the box (ms).
#define FLIGHT_DWELL_MS   8000UL

// ---------------------------------------------------------------------
// 4) HUB75 64x64 RGB LED matrix wiring (ESP32-WROOM-32)
//    Default pin map matches the popular "ESP32 HUB75 Matrix Shield".
//    Change these if your wiring is different.
// ---------------------------------------------------------------------
#define PANEL_RES_X     64           // pixels wide
#define PANEL_RES_Y     64           // pixels tall
#define PANEL_CHAIN      1           // number of panels chained

#define R1_PIN          25
#define G1_PIN          26
#define B1_PIN          27
#define R2_PIN          14
#define G2_PIN          12
#define B2_PIN          13
#define A_PIN           23
#define B_PIN           19
#define C_PIN            5
#define D_PIN           17
#define E_PIN           18           // required for 1/32 scan (64-row)
#define LAT_PIN          4
#define OE_PIN           15
#define CLK_PIN          16

// Display brightness, 0..255
#define DISPLAY_BRIGHTNESS 90
