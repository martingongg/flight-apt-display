// =====================================================================
//  FlightAPT-Display - OpenSky REST client
// =====================================================================
//
//  The OpenSky `/states/all` endpoint returns an object that looks like
//
//   {
//     "time": 1700000000,
//     "states": [
//        ["icao24","cs   ","origin",ts,ts,lon,lat,baroAlt,
//         onGround,vel,heading,verticalRate,...],
//        ...
//     ]
//   }
//
//  We stream-parse that with ArduinoJson's "filter" feature so the
//  ESP32 only allocates memory for the fields we actually use.
// =====================================================================

#include "api.h"
#include "config.h"

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

namespace OpenSky {

// OpenSky's TLS cert rotates regularly, so we accept any cert.  The
// data is public anyway.
static WiFiClientSecure s_secureClient;

int fetchFlightsInBox(float laMin, float loMin,
                      float laMax, float loMax,
                      std::vector<Flight> &out) {
  out.clear();

  if (WiFi.status() != WL_CONNECTED) return -1;

  s_secureClient.setInsecure();

  String url = String(OPENSKY_URL) +
               "?lamin=" + String(laMin, 4) +
               "&lomin=" + String(loMin, 4) +
               "&lamax=" + String(laMax, 4) +
               "&lomax=" + String(loMax, 4);

  HTTPClient http;
  http.setTimeout(8000);
  http.setReuse(false);

  if (!http.begin(s_secureClient, url)) {
    Serial.println("[api] http.begin() failed");
    return -2;
  }

  if (strlen(OPENSKY_USER) > 0) {
    http.setAuthorization(OPENSKY_USER, OPENSKY_PASS);
  }

  int code = http.GET();
  if (code != HTTP_CODE_OK) {
    Serial.printf("[api] GET %s -> %d\n", url.c_str(), code);
    http.end();
    return -3;
  }

  // -----------------------------------------------------------------
  // Build a JSON filter so only the "states" array survives parsing.
  // -----------------------------------------------------------------
  StaticJsonDocument<128> filter;
  filter["states"] = true;

  DynamicJsonDocument doc(24 * 1024);          // ~24 KB on heap
  DeserializationError err = deserializeJson(
      doc, http.getStream(),
      DeserializationOption::Filter(filter));
  http.end();

  if (err) {
    Serial.printf("[api] JSON parse error: %s\n", err.c_str());
    return -4;
  }

  JsonArray states = doc["states"].as<JsonArray>();
  if (states.isNull()) {
    Serial.println("[api] no 'states' key in response");
    return 0;
  }

  out.reserve(states.size());
  for (JsonArray s : states) {
    Flight f;
    f.icao24        = s[0] | "";
    f.callsign      = String((const char*)(s[1] | "")); f.callsign.trim();
    f.originCountry = s[2] | "";
    f.lastContact   = s[4] | 0UL;
    f.longitude     = s[5] | NAN;
    f.latitude      = s[6] | NAN;
    f.baroAltitudeM = s[7] | NAN;
    f.onGround      = s[8] | false;
    f.velocityMs    = s[9] | NAN;
    f.headingDeg    = s[10] | NAN;

    // Skip records that don't have a usable position.
    if (isnan(f.latitude) || isnan(f.longitude)) continue;
    if (f.onGround) continue;

    out.push_back(f);
  }

  return (int)out.size();
}

}  // namespace OpenSky
