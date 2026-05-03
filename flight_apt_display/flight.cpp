// =====================================================================
//  FlightAPT-Display - Flight helpers (great-circle math)
// =====================================================================
#include "flight.h"
#include <math.h>

static inline float deg2rad(float d) { return d * (float)M_PI / 180.0f; }
static inline float rad2deg(float r) { return r * 180.0f / (float)M_PI; }

float Flight::distanceKm(float lat0, float lon0) const {
  // Haversine
  const float R = 6371.0088f;          // mean Earth radius, km
  float dLat = deg2rad(latitude  - lat0);
  float dLon = deg2rad(longitude - lon0);
  float a = sinf(dLat / 2) * sinf(dLat / 2) +
            cosf(deg2rad(lat0)) * cosf(deg2rad(latitude)) *
            sinf(dLon / 2) * sinf(dLon / 2);
  float c = 2.0f * atan2f(sqrtf(a), sqrtf(1 - a));
  return R * c;
}

float Flight::bearingDeg(float lat0, float lon0) const {
  float phi1   = deg2rad(lat0);
  float phi2   = deg2rad(latitude);
  float dLam   = deg2rad(longitude - lon0);
  float y      = sinf(dLam) * cosf(phi2);
  float x      = cosf(phi1) * sinf(phi2) -
                 sinf(phi1) * cosf(phi2) * cosf(dLam);
  float brg    = rad2deg(atan2f(y, x));
  if (brg < 0) brg += 360.0f;
  return brg;
}
