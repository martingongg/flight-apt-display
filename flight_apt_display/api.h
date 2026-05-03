// =====================================================================
//  FlightAPT-Display - OpenSky REST client
// =====================================================================
#pragma once

#include <Arduino.h>
#include <vector>
#include "flight.h"

namespace OpenSky {

// Pull every aircraft inside the bounding box.  Returns the number of
// flights stored in `out` (which is cleared first).  On error returns
// a negative value.
int fetchFlightsInBox(float laMin, float loMin,
                      float laMax, float loMax,
                      std::vector<Flight> &out);

}  // namespace OpenSky
