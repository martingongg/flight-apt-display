// =====================================================================
//  FlightAPT-Display - HUB75 64x64 RGB matrix output
// =====================================================================
//
//  Uses the excellent ESP32-HUB75-MatrixPanel-DMA library by mrcodetastic
//  (formerly mrfaptastic).
//
//  https://github.com/mrcodetastic/ESP32-HUB75-MatrixPanel-DMA
//
//  Layout (64x64):
//
//   row  0..7   : header bar (callsign + country, scrolling if too long)
//   row  9..30  : compass / heading arrow on left, big info on right
//   row 33..62  : altitude, speed, distance, bearing
//
// =====================================================================

#include "display.h"
#include "config.h"

#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include <Fonts/TomThumb.h>

namespace Display {

// ---------------------------------------------------------------------
// Singleton matrix panel
// ---------------------------------------------------------------------
static MatrixPanel_I2S_DMA *s_panel = nullptr;

// Convenience colour constants (RGB565)
static uint16_t C_BLACK;
static uint16_t C_WHITE;
static uint16_t C_RED;
static uint16_t C_GREEN;
static uint16_t C_BLUE;
static uint16_t C_YELLOW;
static uint16_t C_CYAN;
static uint16_t C_ORANGE;
static uint16_t C_GREY;

// ---------------------------------------------------------------------
// Internal helpers
// ---------------------------------------------------------------------
static void initColours() {
  C_BLACK  = s_panel->color565(  0,   0,   0);
  C_WHITE  = s_panel->color565(255, 255, 255);
  C_RED    = s_panel->color565(255,  40,  40);
  C_GREEN  = s_panel->color565( 40, 220,  60);
  C_BLUE   = s_panel->color565( 60, 120, 255);
  C_YELLOW = s_panel->color565(255, 220,  20);
  C_CYAN   = s_panel->color565( 40, 220, 220);
  C_ORANGE = s_panel->color565(255, 140,  20);
  C_GREY   = s_panel->color565(120, 120, 120);
}

// Draw a single-pixel arrow showing `headingDeg` (0 = north, CW).
// Centred at (cx, cy) with given outer radius.
static void drawHeadingArrow(int cx, int cy, int r,
                             float headingDeg, uint16_t col) {
  float t = headingDeg * (float)M_PI / 180.0f;
  // North-up: y axis points up on the panel (negative dy)
  float dx = sinf(t);
  float dy = -cosf(t);

  int x1 = cx + (int)roundf(dx * r);
  int y1 = cy + (int)roundf(dy * r);
  int x2 = cx - (int)roundf(dx * r * 0.6f);
  int y2 = cy - (int)roundf(dy * r * 0.6f);

  s_panel->drawLine(x2, y2, x1, y1, col);

  // Arrow head: two short barbs 30 deg either side of the tip.
  float ah = r * 0.55f;
  for (int s = -1; s <= 1; s += 2) {
    float a = t + (float)s * (150.0f * (float)M_PI / 180.0f);
    int hx = x1 + (int)roundf(sinf(a) * ah);
    int hy = y1 - (int)roundf(cosf(a) * ah);
    s_panel->drawLine(x1, y1, hx, hy, col);
  }
}

// Scroll-or-print: if `text` fits in `w` pixels (TomThumb glyphs ~ 4 px
// wide) we left-align it, otherwise we draw a rolling scroll using the
// current millis() to advance.
static void drawScrollableText(int x, int y, int w,
                               const String &text, uint16_t col) {
  s_panel->setFont(&TomThumb);
  s_panel->setTextColor(col);
  s_panel->setTextWrap(false);

  int charW = 4;                  // TomThumb is 3 px + 1 spacing
  int textW = text.length() * charW;

  if (textW <= w) {
    s_panel->setCursor(x, y);
    s_panel->print(text);
    return;
  }

  // Marquee
  int total = textW + w;
  int phase = (int)((millis() / 60UL) % total);
  int sx = x + w - phase;

  // Clip with a transient "viewport" by drawing two passes.
  s_panel->setCursor(sx, y);
  s_panel->print(text);
}

// ---------------------------------------------------------------------
// Public API
// ---------------------------------------------------------------------
void begin() {
  HUB75_I2S_CFG::i2s_pins pins = {
      R1_PIN, G1_PIN, B1_PIN,
      R2_PIN, G2_PIN, B2_PIN,
      A_PIN,  B_PIN,  C_PIN, D_PIN, E_PIN,
      LAT_PIN, OE_PIN, CLK_PIN
  };

  HUB75_I2S_CFG cfg(PANEL_RES_X, PANEL_RES_Y, PANEL_CHAIN, pins);
  cfg.clkphase = false;          // fixes ghosting on most panels
  cfg.double_buff = true;

  s_panel = new MatrixPanel_I2S_DMA(cfg);
  s_panel->begin();
  s_panel->setBrightness8(DISPLAY_BRIGHTNESS);
  s_panel->clearScreen();

  initColours();
}

void showSplash() {
  s_panel->fillScreen(C_BLACK);
  s_panel->setFont(&TomThumb);

  s_panel->setTextColor(C_CYAN);
  s_panel->setCursor(6, 18);
  s_panel->print("FlightAPT");

  s_panel->setTextColor(C_WHITE);
  s_panel->setCursor(10, 30);
  s_panel->print("Display");

  // Simple little plane silhouette
  s_panel->drawLine(20, 44, 44, 44, C_YELLOW);
  s_panel->drawLine(28, 40, 36, 40, C_YELLOW);
  s_panel->drawLine(28, 48, 36, 48, C_YELLOW);
  s_panel->drawPixel(32, 38, C_YELLOW);
  s_panel->drawPixel(32, 50, C_YELLOW);

  s_panel->setTextColor(C_GREY);
  s_panel->setCursor(10, 60);
  s_panel->print("booting...");

  s_panel->flipDMABuffer();
}

void showStatus(const char *line1, const char *line2) {
  s_panel->fillScreen(C_BLACK);
  s_panel->setFont(&TomThumb);
  s_panel->setTextColor(C_WHITE);

  s_panel->setCursor(2, 24);
  s_panel->print(line1);

  if (line2) {
    s_panel->setTextColor(C_GREY);
    s_panel->setCursor(2, 36);
    s_panel->print(line2);
  }

  s_panel->flipDMABuffer();
}

void showNoFlights() {
  s_panel->fillScreen(C_BLACK);
  s_panel->setFont(&TomThumb);

  s_panel->setTextColor(C_CYAN);
  s_panel->setCursor(2, 10);
  s_panel->print("FlightAPT");

  s_panel->drawFastHLine(0, 13, 64, C_GREY);

  s_panel->setTextColor(C_WHITE);
  s_panel->setCursor(4, 30);
  s_panel->print("No aircraft");
  s_panel->setCursor(10, 40);
  s_panel->print("overhead");

  s_panel->setTextColor(C_GREY);
  s_panel->setCursor(8, 58);
  s_panel->print("watching...");

  s_panel->flipDMABuffer();
}

void showFlight(const Flight &f, float homeLat, float homeLon) {
  s_panel->fillScreen(C_BLACK);
  s_panel->setFont(&TomThumb);

  // ---- Header: callsign --------------------------------------------
  String cs = f.callsign.length() ? f.callsign : f.icao24;
  cs.toUpperCase();

  s_panel->setTextColor(C_YELLOW);
  drawScrollableText(1, 7, 62, cs, C_YELLOW);

  // origin country in dim grey just below the callsign
  if (f.originCountry.length() > 0) {
    drawScrollableText(1, 14, 62, f.originCountry, C_GREY);
  }

  s_panel->drawFastHLine(0, 17, 64, C_BLUE);

  // ---- Heading compass on the left ---------------------------------
  int cx = 12, cy = 30, r = 9;
  s_panel->drawCircle(cx, cy, r, C_GREY);

  // Cardinal markers
  s_panel->drawPixel(cx,         cy - r, C_RED);    // N
  s_panel->drawPixel(cx + r,     cy,     C_GREY);   // E
  s_panel->drawPixel(cx,         cy + r, C_GREY);   // S
  s_panel->drawPixel(cx - r,     cy,     C_GREY);   // W

  if (!isnan(f.headingDeg)) {
    drawHeadingArrow(cx, cy, r - 1, f.headingDeg, C_CYAN);
  }

  // ---- Right column: altitude + speed ------------------------------
  char buf[24];

  s_panel->setTextColor(C_WHITE);
  s_panel->setCursor(26, 24);
  if (!isnan(f.baroAltitudeM)) {
    snprintf(buf, sizeof(buf), "%5dft", (int)f.altitudeFt());
  } else {
    snprintf(buf, sizeof(buf), "  ---ft");
  }
  s_panel->print(buf);

  s_panel->setTextColor(C_GREEN);
  s_panel->setCursor(26, 32);
  if (!isnan(f.velocityMs)) {
    snprintf(buf, sizeof(buf), "%4dkt", (int)f.velocityKt());
  } else {
    snprintf(buf, sizeof(buf), " ---kt");
  }
  s_panel->print(buf);

  s_panel->setTextColor(C_ORANGE);
  s_panel->setCursor(26, 40);
  if (!isnan(f.headingDeg)) {
    snprintf(buf, sizeof(buf), "%3dHDG", (int)f.headingDeg);
  } else {
    snprintf(buf, sizeof(buf), "---HDG");
  }
  s_panel->print(buf);

  s_panel->drawFastHLine(0, 44, 64, C_BLUE);

  // ---- Bottom row: distance + bearing from home --------------------
  float dKm  = f.distanceKm(homeLat, homeLon);
  float brg  = f.bearingDeg(homeLat, homeLon);

  s_panel->setTextColor(C_CYAN);
  s_panel->setCursor(1, 53);
  snprintf(buf, sizeof(buf), "D %4.1fkm", dKm);
  s_panel->print(buf);

  s_panel->setTextColor(C_YELLOW);
  s_panel->setCursor(1, 61);
  snprintf(buf, sizeof(buf), "B %3d%c", (int)brg, (char)0xF8 /* deg */);
  s_panel->print(buf);

  // Tiny bearing arrow on the right of the bottom block
  drawHeadingArrow(56, 55, 6, brg, C_YELLOW);

  s_panel->flipDMABuffer();
}

}  // namespace Display
