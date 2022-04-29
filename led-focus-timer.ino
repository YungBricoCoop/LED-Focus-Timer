#include <FastLED.h>

#define LED_PIN     5
#define NUM_LEDS    10
#define BRIGHTNESS  80
#define LED_TYPE    WS2812B //https://fr.aliexpress.com/item/2036819167.html?spm=a2g0o.order_list.0.0.370c5e5bdFYsjG&gatewayAdapt=glo2fra
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 10
#define COLOR_FADE_SPEED 1
#define TIME_BETWEEN_STEP 1 //Time in minutes
#define BLINK_NUMBER 20
#define BLINK_DELAY 200 //Time in ms

uint16_t timer = 0;
uint8_t buttonState = 0;
uint8_t currentMinutes = 0;
uint8_t colorFadeSpeed = 0;

void setup() {
  delay( 500 );
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );
  pinMode (2, INPUT);
}


void loop()
{

  if (digitalRead(2) == 1) {
    buttonState += 1;
    delay(1000);
  }
  else if (buttonState == 1) {
    timer = timer + 1;
    currentMinutes = timer / UPDATES_PER_SECOND / 60 / TIME_BETWEEN_STEP;
    colorFadeSpeed = colorFadeSpeed + COLOR_FADE_SPEED;
    FillLEDsFromPaletteColors( colorFadeSpeed, currentMinutes);
    FastLED.show();
    
    if (currentMinutes == 9) {
      BlinkLEDs();
      ResetToDefaultValues();
      delay(1000);
    }
  }
  else if (buttonState == 2) {
      ResetToDefaultValues();
      delay(1000);
  }

  FastLED.show();
  FastLED.delay(1000 / UPDATES_PER_SECOND);

}

void ResetToDefaultValues(){
      ResetLEDs();
      timer = 0;
      currentMinutes = 0;
      buttonState = 0;
}

void BlinkLEDs() {
  uint8_t blinkColorIndex = 0;

  for ( int i = 0; i < BLINK_NUMBER; ++i) {

    //Show LEDs Fade
    for ( int j = 0; j < NUM_LEDS; ++j) {
      blinkColorIndex += 2;
      leds[j] = ColorFromPalette( RainbowColors_p, blinkColorIndex, BRIGHTNESS, LINEARBLEND);
    }
    FastLED.show();
    delay(BLINK_DELAY);

    //Hide LEDs
    for ( int j = 0; j < NUM_LEDS; ++j) {
      leds[j] = CRGB( 0, 0, 0);
    }
    FastLED.show();
    delay(BLINK_DELAY);
  }
}

void ResetLEDs() {
  for ( int i = 0; i < NUM_LEDS; ++i) {
    leds[i] = CRGB( 0, 0, 0);
  }
  FastLED.show();
}

void FillLEDsFromPaletteColors( uint8_t colorIndex, uint8_t currentMinute)
{
  for ( int i = 0; i <= currentMinute; ++i) {
    //Light current time led in white
    if (i ==  currentMinute) {
      leds[i] = CRGB( 240, 240, 240);
    }
    else {
      leds[i] = ColorFromPalette( RainbowColors_p, colorIndex, BRIGHTNESS, LINEARBLEND);
    }

    colorIndex += 10;
  }
}
