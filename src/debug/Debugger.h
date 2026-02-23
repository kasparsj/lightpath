#pragma once

#include <cstdint>

class TopologyObject;

#define AVG_FPS_FRAMES 120

class Debugger
{

  public:

    Debugger(TopologyObject &object);
    ~Debugger();
    
    void update(unsigned long millis);
    void countEmit();
    float getFPS();
    float getNumEmits();
    bool isModelWeight(uint8_t id, uint16_t i);
    bool isIntersection(uint16_t i);
    bool isConnection(uint16_t i);
    void dumpConnections();
    void dumpIntersections();

  private:
    TopologyObject &object;
    bool **weightPixels;
    bool *interPixels;
    bool *connPixels;
    float fps[AVG_FPS_FRAMES] = {0.f};
    uint16_t numEmits[AVG_FPS_FRAMES] = {0};
    uint8_t fpsIndex = 0;
    uint8_t emitsIndex = 0;
    unsigned long prevMillis = 0;

};
