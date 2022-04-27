#include <FastLED.h>

#define LED_PIN     5
#define NUM_LEDS    10
#define BRIGHTNESS  30
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 100

CRGBPalette16 currentPalette;
TBlendType    currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

int timer;


void setup() {
  delay( 500 ); // power-up safety delay
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );
  pinMode (2, INPUT);
  currentPalette = RainbowColors_p;
  currentBlending = LINEARBLEND;
  Serial.begin(9600);
  timer = millis();
}


void loop()
{
  timer = timer + 10;
  Serial.println(timer);
  uint8_t currentMinute = timer / 1000 % 60 % 10;
  static uint8_t startIndex = 0;
  startIndex = startIndex + 1; /* motion speed */

  FillLEDsFromPaletteColors( startIndex, currentMinute);
  FastLED.show();
  if (digitalRead(2) == 1) {
    ResetLEDs();
    delay(1000);
    timer = 0;
  }
  else if (currentMinute == 9) {
    ResetLEDs();
    delay(1000);
    timer = 0;
  }
  FastLED.delay(1000 / UPDATES_PER_SECOND);

}

void ResetLEDs() {
  for ( int i = 0; i < NUM_LEDS; ++i) {
    leds[i] = CRGB( 0, 0, 0);
  }
}

void FillLEDsFromPaletteColors( uint8_t colorIndex, uint8_t currentMinute)
{
  for ( int i = 0; i <= currentMinute; ++i) {
    if (i ==  currentMinute) {
      leds[i] = CRGB( 200, 200, 200);
    }
    else {
      leds[i] = ColorFromPalette( currentPalette, colorIndex, BRIGHTNESS, currentBlending);
    }

    colorIndex += 10;
  }
}



const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM =
{
  CRGB::Red,
  CRGB::Gray,
  CRGB::Blue,
  CRGB::Black,

  CRGB::Red,
  CRGB::Gray,
  CRGB::Blue,
  CRGB::Black,

  CRGB::Red,
  CRGB::Red,
  CRGB::Gray,
  CRGB::Gray,
  CRGB::Blue,
  CRGB::Blue,
  CRGB::Black,
  CRGB::Black
};
