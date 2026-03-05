#include "Debugger.h"

Debugger::Debugger() = default;

Debugger::~Debugger() = default;

void Debugger::update(unsigned long millis) {
    fps[fpsIndex] = 1000.f / float(millis - prevMillis);
    fpsIndex = (fpsIndex + 1) % AVG_FPS_FRAMES;
    emitsIndex = (emitsIndex + 1) % AVG_FPS_FRAMES;
    numEmits[emitsIndex] = 0;
    prevMillis = millis;
}

void Debugger::countEmit() {
    numEmits[emitsIndex]++;
}

float Debugger::getFPS() {
    float avg = 0;
    for (uint8_t i=0; i<AVG_FPS_FRAMES; i++) {
        avg += fps[i];
    }
    return avg / AVG_FPS_FRAMES;
}

float Debugger::getNumEmits() {
    float sum = 0;
    for (uint8_t i=0; i<AVG_FPS_FRAMES; i++) {
        sum += numEmits[i];
    }
    return sum / AVG_FPS_FRAMES;
}
