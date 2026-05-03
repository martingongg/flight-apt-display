// =====================================================================
//  FlightAPT-Display - HUB75 64x64 RGB matrix output
// =====================================================================
#pragma once

#include "flight.h"

namespace Display {

void begin();                                   // init the panel
void showSplash();                              // boot screen
void showStatus(const char *line1,
                const char *line2 = nullptr);   // generic message
void showNoFlights();                           // "no aircraft" screen
void showFlight(const Flight &f,
                float homeLat, float homeLon);  // pretty flight card

}  // namespace Display
