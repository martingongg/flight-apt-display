// =====================================================================
//  FlightAPT-Display - Flight data model
// =====================================================================
#pragma once

#include <Arduino.h>

struct Flight {
  String   icao24;        // unique ICAO 24-bit address (hex)
  String   callsign;      // e.g. "UAL123"
  String   originCountry;
  float    longitude;     // deg, +E
  float    latitude;      // deg, +N
  float    baroAltitudeM; // meters
  float    velocityMs;    // m/s
  float    headingDeg;    // deg from true north (0..360)
  bool     onGround;
  uint32_t lastContact;   // unix seconds

  // Convenience helpers --------------------------------------------------
  float altitudeFt() const  { return baroAltitudeM * 3.28084f; }
  float velocityKt() const  { return velocityMs    * 1.94384f; }
  float distanceKm(float lat0, float lon0) const;
  float bearingDeg(float lat0, float lon0) const;
};
