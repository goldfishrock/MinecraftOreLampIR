#include <Arduino.h>
#include <FastLED.h>
#include <IRremote.hpp>

#define LED_PIN 5
#define PIXELS 5
#define IR_RECEIVE_PIN 2

const long flashInterval = 1000;
unsigned long previousMillis = 0;

CRGB leds[PIXELS];

uint8_t brightness = 127;
uint8_t hue = 0;
byte colour = 0;
byte inputCode;

bool isOn = false;
bool featureRunning = false;
bool randomLamps = false;
bool rainbow = false;

const uint32_t colourPalette[] = {
    0x00FF0000, 0x0000FF00, 0x000000FF, 0x00FFFFFF,
    0x00FF2121, 0x001DAD1D, 0x003B3BF7, 0x00FA6969,
    0x00006AFF, 0x002400A6, 0x00FF8000, 0x00014694,
    0x007037CC, 0x00FFFF00, 0x000D574D, 0x00FF00FF
};

const uint32_t commandPalette[] = {50, 51, 52, 53, 1, 2, 3, 4, 5, 6, 7, 54, 8, 9, 10, 55, 11, 12, 13, 56, 14, 15, 16, 57};

void updateLeds(const CRGB& color, byte bright) {
    fill_solid(leds, PIXELS, color);
    FastLED.setBrightness(bright);
    FastLED.show();
}

void changeColour(byte bright) {
    updateLeds(CRGB(colourPalette[colour]), bright);
}

void randomColour(byte bright) {
    randomSeed(analogRead(A0));
    for (byte pixelCount = 0; pixelCount < PIXELS; pixelCount++) {
        leds[pixelCount] = CRGB(colourPalette[random(0, 16)]);
    }
    FastLED.setBrightness(bright);
    FastLED.show();
}

void changeBrightness(bool brighter) {
    if (isOn) {
        brightness = constrain(brightness + (brighter ? 5 : -5), 1, 255);
        if (randomLamps) randomColour(brightness);
        else changeColour(brightness);
    }
}

void lampOff() {
    isOn = false;
    for (byte fadeCounter = brightness; fadeCounter > 0; fadeCounter--) {
        FastLED.setBrightness(fadeCounter - 1);
        FastLED.show();
        delay(10);
    }
}

void lampOn() {
    isOn = true;
    for (byte fadeCounter = 0; fadeCounter < brightness; fadeCounter++) {
        changeColour(fadeCounter);
        delay(10);
    }
}

void flashLeds() {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= flashInterval) {
        previousMillis = currentMillis;
        isOn = !isOn;
        FastLED.setBrightness(isOn ? brightness : 0);
        FastLED.show();
    }
}

void randomLeds() {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= flashInterval) {
        previousMillis = currentMillis;
        randomColour(brightness);
    }
}

void rainbowtime() {
    for (int pixelCount = 0; pixelCount < PIXELS; pixelCount++) {
        leds[pixelCount] = CHSV(hue + (pixelCount * 10), 255, 255);
    }
    FastLED.show();
    EVERY_N_MILLISECONDS(15) { hue++; }
}

void pulseLeds() {
    for (int fadeCount = 0; fadeCount < brightness * 2; fadeCount++) {
        int dimness = (fadeCount < brightness) ? fadeCount : (brightness * 2 - fadeCount - 1);
        FastLED.setBrightness(dimness);
        FastLED.show();
        delay(30);
    }
}

void handleFeature(byte feature) {
    switch (feature) {
        case 54: flashLeds(); break;
        case 55: randomLeds(); break;
        case 56: pulseLeds(); break;
        case 57: rainbowtime(); break;
    }
}

void setup() {
    Serial.begin(115200);
    IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
    FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, PIXELS);
    FastLED.setBrightness(0);
    FastLED.show();
}

void loop() {
    if (IrReceiver.decode()) {
        delay(300);
        inputCode = IrReceiver.decodedIRData.command;
        uint8_t selectedValue = commandPalette[inputCode];

        if (selectedValue < 50) {
            colour = selectedValue - 1;
            changeColour(brightness);
        } else if (selectedValue == 50) {
            changeBrightness(true);
        } else if (selectedValue == 51) {
            changeBrightness(false);
        } else if (selectedValue == 52) {
            lampOff();
        } else if (selectedValue == 53) {
            lampOn();
        } else {
            featureRunning = !featureRunning;
            if (!featureRunning) changeColour(brightness);
        }
        
        IrReceiver.resume();
    }

    if (featureRunning) {
        handleFeature(commandPalette[inputCode]);
    }
}
