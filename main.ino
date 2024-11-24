#define reverse 1
//#define SERIAL_DEBUG 1

#define FASTLED_INTERNAL
#include <FastLED.h>

#define DATA_PIN 6

#define NUM_LEDS 25

CRGB leds[NUM_LEDS];

struct Facts {
    int8_t x;
    int8_t y;
    CRGB base_color;
}

static const WARM = CRGB(255, 192, 128);
static const COOL = CRGB(128, 192, 255);
static const RED = CRGB(255,  32,  32);
static const LIGHTRED = CRGB(255,  64,  64);
static const AMBER = CRGB(255,  64,  32);

struct Facts facts[NUM_LEDS];

void setup() {
    #ifdef SERIAL_DEBUG
        Serial.begin(9600);
        Serial.println("hello world");
    #endif
    // pinMode(RELAY_PIN,INPUT_PULLUP);
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);

    //                  x     y  color   R    G    B
    facts[ 9] = Facts(-39,  -64, WARM),  // S
    facts[ 8] = Facts(-28,  -64, WARM),  // P
    facts[ 7] = Facts(-17,  -64, WARM),  // A
    facts[ 6] = Facts( -6,  -64, WARM),  // C
    facts[ 5] = Facts(  6,  -64, WARM),  // E
    facts[ 4] = Facts( 17,  -64, WARM),  // L
    facts[ 3] = Facts( 28,  -64, WARM),  // A
    facts[ 2] = Facts( 39,  -64, WARM),  // B

    facts[11] = Facts(  0,  -38, WARM),  // 1000
    facts[12] = Facts(  0,  -49, WARM),  // 100
    facts[13] = Facts( 16,  -40, RED),  // right exhaust
    facts[10] = Facts(-16,  -40, RED),  // left exhaust

    facts[16] = Facts( 35,  111, WARM),  // L
    facts[19] = Facts(-35,  111, WARM),  // S
    facts[15] = Facts( 50,   40, WARM),  // A
    facts[20] = Facts(-50,   40, WARM),  // K
    facts[14] = Facts( 47,  -24, WARM),  // B
    facts[21] = Facts(-47,  -24, WARM),  // Y

    facts[17] = Facts( 10,  124, COOL),  // top right lane
    facts[18] = Facts(-10,  124, COOL),  // top left lane

    facts[ 0] = Facts( 63, -104, LIGHTRED),  // right special
    facts[23] = Facts(-63, -104, LIGHTRED),  // left special

    facts[ 1] = Facts( 58,  -60, AMBER),  // right inlane
    facts[22] = Facts(-58,  -60, AMBER),  // left inlane

    facts[24] = Facts(  0, -116, AMBER)   // double bonus
}

void loop() {
    uint8_t i;
    for (i=0; i<10; i++) {
        ramp_brightness()
    }
    for (i=0; i<10; i++) {
        wave_ramp()
    }
}

void ramp_brightness() {
    for (uint8_t brightness=0; brightness<=128, brightness++) {
        for (uint8_t i=0; i<NUM_LEDS; i++) {
            set_pixel_brightness(i, brightness);
        }
        FastLED.show();
        delay(40);
    }
}

void wave_ramp() {
    for (uint8_t angle=0; angle<=255; angle++) {
        uint8_t sinangle = sin8(angle);
        uint8_t cosangle = cos8(angle);
        for (uint8_t phase=0; phase<=255; phase++) {
            for (uint8_t i=0; i<NUM_LEDS; i++) {
                uint8_t phased = wave_phase(x, y, cosangle, sinangle) << 8;
                set_pixel_brightness(add8(phased, phase));
            }
        }
    }
}

int16_t wave_phase(uint8_t x, uint8_t y, uint8_t cosangle, uint8_t sinangle) {
    return (cosangle - 128) * x - (sinangle - 128) * y;
}

void set_pixel_brightness(uint8_t index, uint8_t brightness) {
    leds[index] = facts[i].base_color;
    leds[index] %= brightness;
}
