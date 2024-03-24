#include "Button.hpp"

Button::Button(uint8_t pin) : pin(pin), state(HIGH), timestamp(0) {
  pinMode(pin, PINMODE);
}

Button::Event Button::wait_event(void) {
  bool new_state = digitalRead(pin);
  unsigned long new_timestamp = millis();

  if (new_state == state)
    // Button is still being pressed / released
    return Button::Event::NONE;

  if (new_timestamp - timestamp <= DEBOUNCE_DELAY_MS)
    // Ignores changes within the DEBOUNCE_DELAY_MS time window
    return Button::Event::NONE;

  // Button changed state after the DEBOUNCE_DELAY_MS time window
  // => updates the state of the Button
  state = new_state;
  timestamp = new_timestamp;

  // Returns the corresponding event
  if (new_state == ACTIVE_STATE)
    return Button::Event::PRESS;

  return Button::Event::RELEASE;
}
