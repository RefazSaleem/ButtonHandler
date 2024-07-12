#include "ButtonHandler.h"

ButtonHandler::ButtonHandler(const int pins[], int numPins) 
: buttonPins(pins), numPins(numPins), debounceDelay(50), multiPressInterval(350), longPressTime(500) {
    memset(buttonStates, 0, sizeof(buttonStates));
    memset(lastButtonStates, 0, sizeof(lastButtonStates));
    memset(lastDebounceTimes, 0, sizeof(lastDebounceTimes));
    memset(pressStartTimes, 0, sizeof(pressStartTimes));
    memset(multiPressCounts, 0, sizeof(multiPressCounts));
    memset(lastPressTimes, 0, sizeof(lastPressTimes));
    memset(printBuffers, false, sizeof(printBuffers));
}

void ButtonHandler::begin(unsigned long debounceDelay, unsigned long multiPressInterval, unsigned long longPressTime) {
    this->debounceDelay = debounceDelay;
    this->multiPressInterval = multiPressInterval;
    this->longPressTime = longPressTime;

    for (int i = 0; i < numPins; i++) {
        pinMode(buttonPins[i], INPUT_PULLUP);
    }
}

void ButtonHandler::read(String& output) {
    bool updated = false;
    output = "";

    for (int i = 0; i < numPins; i++) {
        if (checkButton(i, output)) {
            updated = true;
        }
    }

    for (int i = 0; i < numPins; i++) {
        if (printBuffers[i] && (millis() - lastPressTimes[i] > multiPressInterval)) {
            if (multiPressCounts[i] > 0) {
                output = String(buttonPins[i]) + "x" + String(multiPressCounts[i]);
                updated = true;
                multiPressCounts[i] = 0;
                printBuffers[i] = false;
            }
        }
    }

    if (!updated) {
        output = "";
    }
}

bool ButtonHandler::checkButton(int index, String& output) {
    bool updated = false;
    int reading = digitalRead(buttonPins[index]);

    if (reading != lastButtonStates[index]) {
        lastDebounceTimes[index] = millis();
    }

    if ((millis() - lastDebounceTimes[index]) > debounceDelay) {
        if (reading != buttonStates[index]) {
            buttonStates[index] = reading;

            if (buttonStates[index] == LOW) {
                pressStartTimes[index] = millis();
            } else {
                unsigned long pressDuration = millis() - pressStartTimes[index];
                if (pressDuration > longPressTime) {
                    output = String(buttonPins[index]) + "l";
                    updated = true;
                    multiPressCounts[index] = 0;
                    printBuffers[index] = false;
                } else if (pressDuration > 0) {
                    if ((millis() - lastPressTimes[index]) <= multiPressInterval) {
                        multiPressCounts[index]++;
                    } else {
                        multiPressCounts[index] = 1;
                    }
                    lastPressTimes[index] = millis();
                    printBuffers[index] = true;
                    updated = true;
                }
            }
        }
    }

    lastButtonStates[index] = reading;
    return updated;
}
