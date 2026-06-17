#define reverse 1

#define FASTLED_INTERNAL
#include <FastLED.h>

// #define MOSFET_GATE_PIN 7

#define NUM_LEDS 25

#define THIRTY_MINUTES 1800000

CRGB leds[NUM_LEDS];

struct Facts {
    int8_t x;
    int8_t y;
    CRGB base_color;
    bool on;
    uint8_t current_brightness;
};

static const CRGB WARM = CRGB(255, 192, 128);
static const CRGB COOL = CRGB(128, 192, 255);
static const CRGB RED = CRGB(255,  32,  32);
static const CRGB LIGHTRED = CRGB(255,  64,  64);
static const CRGB AMBER = CRGB(255,  64,  32);

struct Facts facts[NUM_LEDS];

uint8_t chase_loop_1[] = {0, 1, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24};

uint8_t spacelab[] = {9, 8, 7, 6, 5, 4, 3, 2};
uint8_t spacelab_i = 0;

uint8_t skylab[] = {19, 20, 21, 16, 15, 14};
uint8_t skylab_i = 0;

uint8_t rocket[] = {11, 12};
uint8_t rocket_i = 0;

uint8_t exhaust[] = {10, 13};
uint8_t exhaust_i = 0;

uint8_t advance[] = {17, 18};
uint8_t advance_i = 0;

// uint8_t special[] = {24, 0, 1, 22, 23};
uint8_t special[] = {1, 0, 24, 23, 22};
uint8_t special_i = 0;


void setup() {
    pinMode(13, OUTPUT);
    
    // pinMode(MOSFET_GATE_PIN, OUTPUT);
    // digitalWrite(MOSFET_GATE_PIN, LOW); // Keep power on

    // pinMode(RELAY_PIN,INPUT_PULLUP);
    FastLED.addLeds<NEOPIXEL, A4>(leds, NUM_LEDS);

    //             x     y  color
    facts[ 9] = {-39,  -64, WARM, 0, 0};  // S
    facts[ 8] = {-28,  -64, WARM, 0, 0};  // P
    facts[ 7] = {-17,  -64, WARM, 0, 0};  // A
    facts[ 6] = { -6,  -64, WARM, 0, 0};  // C
    facts[ 5] = {  6,  -64, WARM, 0, 0};  // E
    facts[ 4] = { 17,  -64, WARM, 0, 0};  // L
    facts[ 3] = { 28,  -64, WARM, 0, 0};  // A
    facts[ 2] = { 39,  -64, WARM, 0, 0};  // B

    facts[11] = {  0,  -38, COOL, 0, 0};  // 1000
    facts[12] = {  0,  -49, COOL, 0, 0};  // 100
    facts[13] = { 16,  -40, RED, 0, 0};  // right exhaust
    facts[10] = {-16,  -40, RED, 0, 0};  // left exhaust

    facts[16] = { 35,  111, WARM, 0, 0};  // L
    facts[19] = {-35,  111, WARM, 0, 0};  // S
    facts[15] = { 50,   40, WARM, 0, 0};  // A
    facts[20] = {-50,   40, WARM, 0, 0};  // K
    facts[14] = { 47,  -24, WARM, 0, 0};  // B
    facts[21] = {-47,  -24, WARM, 0, 0};  // Y

    facts[17] = { 10,  124, COOL, 0, 0};  // top right lane
    facts[18] = {-10,  124, COOL, 0, 0};  // top left lane

    facts[ 0] = { 63, -104, LIGHTRED, 0, 0};  // right special
    facts[23] = {-63, -104, LIGHTRED, 0, 0};  // left special

    facts[ 1] = { 58,  -60, AMBER, 0, 0};  // right inlane
    facts[22] = {-58,  -60, CRGB(64, 0, 0), 0, 0};  // left inlane

    facts[24] = {  0, -116, AMBER, 0, 0};   // double bonus

}

void loop() {
    chase();
}

void chase() {
  spacelab_loop();
  rocket_loop();
  exhaust_loop();
  advance_loop();
  skylab_loop();
  special_bounce2();
  decay(150);
}

void spacelab_loop() {
  // lights 8 in series before unlighting
  set(spacelab[spacelab_i % 8], spacelab_i < 8);

  spacelab_i = (spacelab_i + 1) % 16;
}
void skylab_loop() {
  // lights 6 in series before unlighting
  set(skylab[skylab_i % 6], skylab_i < 6);

  skylab_i = (skylab_i + 1) % 12;
}
void rocket_loop() {
  // lights 2 in series before unlighting
  set(rocket[rocket_i % 2], rocket_i < 2);

  rocket_i = (rocket_i + 1) % 4;
}
void exhaust_loop() {
  // alternates between two, on 2s
  set(exhaust[exhaust_i / 2], 1);
  set(exhaust[1-(exhaust_i/2)], 0);

  exhaust_i = (exhaust_i + 1) % 4;
}
void advance_loop() {
  // alternates between two, on 2s
  set(advance[advance_i / 2], 1);
  set(advance[1- (advance_i/2)], 0);

  advance_i = (advance_i + 1) % 4;
}
void special_loop() {
  // cycles through 5, 2 lit at any given time
  set(special[special_i], 1);
  set(special[(special_i + 3) % 5], 0);

  special_i = (special_i + 1) % 5;
}
void special_bounce() {
  // bounce back and forth through 5, one lit at a time
  if (special_i >= 5) {
    set(special[9-special_i], 0);
  } else {
    set(special[special_i], 0);
  }

  special_i = (special_i + 1) % 10;

  if (special_i >= 5) {
    set(special[9-special_i], 1);
  } else {
    set(special[special_i], 1);
  }

  special_i = (special_i + 1) % 10;

}
void special_bounce2() {
  // bounce back and forth through 5, one lit at a time
  if (special_i < 10) {
    // right to left
    if ((special_i % 10) < 5) {
      // turning on
      set(special[special_i % 5], 1);
    } else {
      // turning off
      set(special[special_i % 5], 0);
    }
  } else {
    // left to right
    if ((special_i % 10) < 5) {
      // turning on
      set(special[4 - (special_i % 5)], 1);
    } else {
      // turning off
      set(special[4 - (special_i % 5)], 0);
    }
  }

  special_i = (special_i + 1) % 20;

}

void set(uint8_t index, uint8_t on) {
    facts[index].on = on;
}
void decay(unsigned long ms) {
  unsigned long start = millis();
  while (millis() - start < ms) {
    for (uint8_t i=0; i<NUM_LEDS; i++) {
      if (facts[i].on) {
        facts[i].current_brightness = qadd8(facts[i].current_brightness, 4);
      } else if (facts[i].current_brightness > 8) {
        facts[i].current_brightness -= 8;
      } else {
        facts[i].current_brightness = 0;
      }
      leds[i] = facts[i].base_color;
      leds[i] %= facts[i].current_brightness;
    }
    FastLED.show();
  }
}