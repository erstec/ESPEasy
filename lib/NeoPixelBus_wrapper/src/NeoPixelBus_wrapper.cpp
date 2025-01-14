#ifndef _NEOPIXELBUS_WRAPPER_CPP
#define _NEOPIXELBUS_WRAPPER_CPP
#include "./NeoPixelBus_wrapper.h"

NeoPixelBus_wrapper::NeoPixelBus_wrapper(uint16_t     _maxPixels,
                                         int16_t      _gpioPin,
                                         neoPixelType _stripType)
  : numLEDs(_maxPixels) {
  if (NEO_GRBW == (_stripType & NEO_GRBW)) {
    #ifdef ESP8266
    neopixels_grbw = new (std::nothrow) NEOPIXEL_LIB<NeoGrbwFeature, METHOD>(_maxPixels);
    #endif // ifdef ESP8266
    #ifdef ESP32
    neopixels_grbw = new (std::nothrow) NEOPIXEL_LIB<NeoGrbwFeature, METHOD>(_maxPixels, _gpioPin);
    #endif // ifdef ESP32
  } else if (NEO_GRB == (_stripType & NEO_GRB)) {
    #ifdef ESP8266
    neopixels_grb = new (std::nothrow) NEOPIXEL_LIB<NeoGrbFeature, METHOD>(_maxPixels);
    #endif // ifdef ESP8266
    #ifdef ESP32
    neopixels_grb = new (std::nothrow) NEOPIXEL_LIB<NeoGrbFeature, METHOD>(_maxPixels, _gpioPin);
    #endif // ifdef ESP32
  }
}

NeoPixelBus_wrapper::~NeoPixelBus_wrapper() {
  delete neopixels_grb;
  neopixels_grb = nullptr;
  delete neopixels_grbw;
  neopixels_grbw = nullptr;
}

void NeoPixelBus_wrapper::begin() {
  if (nullptr != neopixels_grb) {
    neopixels_grb->Begin();
  }

  if (nullptr != neopixels_grbw) {
    neopixels_grbw->Begin();
  }
}

void NeoPixelBus_wrapper::show(void) {
  if (nullptr != neopixels_grb) {
    neopixels_grb->Show();
  }

  if (nullptr != neopixels_grbw) {
    neopixels_grbw->Show();
  }
}

void NeoPixelBus_wrapper::setBrightness(uint8_t b) {
  if (nullptr != neopixels_grb) {
    neopixels_grb->SetBrightness(b);
  }

  if (nullptr != neopixels_grbw) {
    neopixels_grbw->SetBrightness(b);
  }
}

void NeoPixelBus_wrapper::setPixelColor(uint16_t pxl,
                                        uint8_t  r,
                                        uint8_t  g,
                                        uint8_t  b) {
  if (nullptr != neopixels_grb) {
    neopixels_grb->SetPixelColor(pxl, RgbColor(r, g, b));
  }

  if (nullptr != neopixels_grbw) {
    neopixels_grbw->SetPixelColor(pxl, RgbwColor(r, g, b));
  }
}

void NeoPixelBus_wrapper::setPixelColor(uint16_t pxl,
                                        uint8_t  r,
                                        uint8_t  g,
                                        uint8_t  b,
                                        uint8_t  w) {
  if (nullptr != neopixels_grb) {
    neopixels_grb->SetPixelColor(pxl, RgbColor(r, g, b));
  }

  if (nullptr != neopixels_grbw) {
    neopixels_grbw->SetPixelColor(pxl, RgbwColor(r, g, b, w));
  }
}

void NeoPixelBus_wrapper::setPixelColor(uint16_t pxl,
                                        uint32_t c) {
  if (nullptr != neopixels_grb) {
    neopixels_grb->SetPixelColor(pxl, RgbColor((c >> 16) & 0xFF, (c >> 8) & 0xFF, c & 0xFF)); // Unfold the Color(r,g,b,w) static
  }

  if (nullptr != neopixels_grbw) {
    neopixels_grbw->SetPixelColor(pxl, RgbwColor((c >> 16) & 0xFF, (c >> 8) & 0xFF, c & 0xFF, (c >> 24) & 0xFF));
  }
}

uint32_t NeoPixelBus_wrapper::getPixelColor(uint16_t n) {
  if (nullptr != neopixels_grb) {
    const RgbColor color = neopixels_grb->GetPixelColor(n);
    return Color(color.R, color.G, color.B);
  }

  if (nullptr != neopixels_grbw) {
    const RgbwColor color = neopixels_grbw->GetPixelColor(n);
    return Color(color.R, color.G, color.B, color.W);
  }

  return 0u; // Fall-through value...
}

#endif // ifndef _NEOPIXELBUS_WRAPPER_CPP
