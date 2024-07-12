#ifndef ButtonHandler_h
#define ButtonHandler_h

#include "Arduino.h"

class ButtonHandler {
public:
    ButtonHandler(const int pins[], int numPins);
    void begin(unsigned long debounceDelay, unsigned long multiPressInterval, unsigned long longPressTime);
    void read(String& output);

private:
    const int* buttonPins;
    int numPins;
    unsigned long debounceDelay;
    unsigned long multiPressInterval;
    unsigned long longPressTime;

    int buttonStates[10];
    int lastButtonStates[10];
    unsigned long lastDebounceTimes[10];
    unsigned long pressStartTimes[10];
    int multiPressCounts[10];
    unsigned long lastPressTimes[10];
    bool printBuffers[10];

    bool checkButton(int index, String& output);
};

#endif
