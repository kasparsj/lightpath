#include "Globals.h"
#include "runtime/EmitParams.h"

#include <algorithm>
#include <cmath>

FastNoise gPerlinNoise;
unsigned long gMillis = 0;

static LightgraphAllocationFailureObserver gLightgraphAllocationFailureObserver = nullptr;
static bool gLightgraphHasFrameTiming = false;
static unsigned long gLightgraphLastFrameMillis = 0;
static float gLightgraphFrameElapsedMillis = static_cast<float>(EmitParams::frameMs());
static float gLightgraphCurrentStepMillis = static_cast<float>(EmitParams::frameMs());

void lightgraphSetAllocationFailureObserver(LightgraphAllocationFailureObserver observer) {
#if LIGHTGRAPH_ALLOCATION_FAILURE_HOOK_ENABLED
  gLightgraphAllocationFailureObserver = observer;
#else
  (void) observer;
#endif
}

void lightgraphReportAllocationFailure(
    LightgraphAllocationFailureSite site,
    uint16_t detail0,
    uint16_t detail1) {
#if LIGHTGRAPH_ALLOCATION_FAILURE_HOOK_ENABLED
  if (gLightgraphAllocationFailureObserver == nullptr) {
    return;
  }
  gLightgraphAllocationFailureObserver(site, detail0, detail1);
#else
  (void) site;
  (void) detail0;
  (void) detail1;
#endif
}

void lightgraphAdvanceFrameTiming(unsigned long nowMillis) {
#if LIGHTGRAPH_FPS_INDEPENDENT_SPEED
  if (!gLightgraphHasFrameTiming || nowMillis < gLightgraphLastFrameMillis) {
    gLightgraphHasFrameTiming = true;
    gLightgraphLastFrameMillis = nowMillis;
    gLightgraphFrameElapsedMillis = static_cast<float>(EmitParams::frameMs());
    gLightgraphCurrentStepMillis = gLightgraphFrameElapsedMillis;
    return;
  }

  gLightgraphFrameElapsedMillis = static_cast<float>(nowMillis - gLightgraphLastFrameMillis);
  gLightgraphLastFrameMillis = nowMillis;
  gLightgraphCurrentStepMillis = gLightgraphFrameElapsedMillis;
#else
  (void) nowMillis;
#endif
}

void lightgraphResetFrameTiming() {
  gLightgraphHasFrameTiming = false;
  gLightgraphLastFrameMillis = 0;
  gLightgraphFrameElapsedMillis = static_cast<float>(EmitParams::frameMs());
  gLightgraphCurrentStepMillis = gLightgraphFrameElapsedMillis;
}

uint8_t lightgraphSimulationSubsteps() {
#if LIGHTGRAPH_FPS_INDEPENDENT_SPEED
  const float referenceFrameMillis = static_cast<float>(EmitParams::frameMs());
  if (referenceFrameMillis <= 0.0f) {
    return 1;
  }

  const float desiredSteps = std::ceil(gLightgraphFrameElapsedMillis / referenceFrameMillis);
  const uint8_t steps = static_cast<uint8_t>(std::max(1.0f, desiredSteps));
  return std::min<uint8_t>(steps, LIGHTGRAPH_MAX_SIMULATION_SUBSTEPS);
#else
  return 1;
#endif
}

void lightgraphSetSimulationSubstep(uint8_t stepCount) {
#if LIGHTGRAPH_FPS_INDEPENDENT_SPEED
  if (stepCount == 0) {
    gLightgraphCurrentStepMillis = gLightgraphFrameElapsedMillis;
    return;
  }
  gLightgraphCurrentStepMillis =
      gLightgraphFrameElapsedMillis / static_cast<float>(std::max<uint8_t>(stepCount, 1));
#else
  (void) stepCount;
#endif
}

float lightgraphConfiguredSpeedPixelsPerSecond(float speed) {
#if LIGHTGRAPH_FPS_INDEPENDENT_SPEED
  return speed * EmitParams::DURATION_FPS;
#else
  return speed;
#endif
}

float lightgraphMotionDistance(float speed) {
#if LIGHTGRAPH_FPS_INDEPENDENT_SPEED
  return lightgraphConfiguredSpeedPixelsPerSecond(speed) *
         (gLightgraphCurrentStepMillis / 1000.0f);
#else
  return speed;
#endif
}
