#pragma once
#include "Arduino.h"

#ifndef PINMODE
#define PINMODE INPUT_PULLUP
#endif

#ifndef ACTIVE_STATE
#define ACTIVE_STATE LOW
#endif

#ifndef DEBOUNCE_DELAY_MS
#define DEBOUNCE_DELAY_MS 50
#endif

class Button {
private:
  const uint8_t pin;
  bool state;
  unsigned long timestamp;

public:
  enum class Event { NONE, PRESS, RELEASE };

  Event wait_event(void);
  Button(uint8_t);
};
