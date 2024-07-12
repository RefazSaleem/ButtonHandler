#include <Arduino.h>
#include "ButtonHandler.h"

#define MAX_PINS 2

const int buttonPins[MAX_PINS] = {6, 7};

ButtonHandler buttonHandler(buttonPins, MAX_PINS);

void setup() {
    Serial.begin(115200);
    buttonHandler.begin(50, 500, 500);
}

void loop() {
    String buttonPressOutput;
    buttonHandler.read(buttonPressOutput);
    if (buttonPressOutput.length() > 0) {
        Serial.println(buttonPressOutput); // Print the output only if it's not empty
    }
}
