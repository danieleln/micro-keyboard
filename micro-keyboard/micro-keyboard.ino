// See the library https://github.com/NicoHood/HID
#include "HID-Project.h"
#include "Button.hpp"


#define NUM_OF_PAGES 3
#define NUM_OF_KEYS 7

// Keycodes for each of the 3 pages x 7 keys
constexpr KeyboardKeycode KEYCODES[NUM_OF_PAGES][NUM_OF_KEYS] = {
  {KEY_ESC,         KEY_BACKSPACE,   KEY_ENTER,        KEY_LEFT_ARROW,   KEY_DOWN_ARROW,   KEY_UP_ARROW,     KEY_RIGHT_ARROW},
  {HID_KEYBOARD_F1, HID_KEYBOARD_F2, HID_KEYBOARD_F3,  HID_KEYBOARD_F4,  HID_KEYBOARD_F5,  HID_KEYBOARD_F6,  HID_KEYBOARD_F7},
  {HID_KEYBOARD_F8, HID_KEYBOARD_F9, HID_KEYBOARD_F10, HID_KEYBOARD_F11, HID_KEYBOARD_F12, HID_KEYBOARD_F13, HID_KEYBOARD_F14},
};


// Page button variables
Button page_btn = Button(4);
uint8_t current_page_index = 0;

// Key buttons variables
Button key_btns[NUM_OF_KEYS] = {
  Button(5), Button(6), Button(7), Button(8), Button(9), Button(10), Button(11)
};
KeyboardKeycode last_active_keycode[NUM_OF_KEYS];




// pulses LED_BUILTIN
void pulse_LED(uint16_t delay_ms, uint8_t num_of_bursts);



void setup() {
  // Initializes the last active keys
  for (uint8_t i = 0; i < NUM_OF_KEYS; i++) {
    last_active_keycode[i] = KEYCODES[current_page_index][i];
  }


  // Initializes the builtin LED
  pinMode(LED_BUILTIN, OUTPUT);
}



void loop() {
  // Reads page button
  Button::Event evt = page_btn.wait_event();
  if (evt == Button::Event::PRESS) {

    // Changes page
    if (current_page_index < NUM_OF_PAGES-1)
      current_page_index += 1;
    else
      current_page_index = 0;

    // Pulses the LED one time for the first page, two for the second, etc
    pulse_LED(80, current_page_index+1);
  }




  // Reads key buttons
  for (uint8_t i = 0; i < NUM_OF_KEYS; i++) {

    Button::Event evt = key_btns[i].wait_event();
    switch (evt) {

      case Button::Event::PRESS:
        // Sends the keycode associated to the current page and key
        KeyboardKeycode keycode = KEYCODES[current_page_index][i];
        BootKeyboard.press(keycode);

        // Stores the key that is been pressed. Required when switching
        // page while another key is still being pressed
        last_active_keycode[i] = keycode;

        // pulse_LED(10);


      case Button::Event::RELEASE:
        // Releases the last pressed key
        BootKeyboard.release(last_active_keycode[i]);
        // pulse_LED(10);
      }
  }

}


// Single LED pulse
void pulse_LED(uint16_t delay_ms) {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(delay_ms);
  digitalWrite(LED_BUILTIN, LOW);
}

// Multiple LED pulses
// If num_of_bursts = 1 *always*, use the previous implementation,
// because it avoids introducing a further (blocking) delay at the end
void pulse_LED(uint16_t delay_ms, uint8_t num_of_bursts) {
  for (uint8_t i = 0; i < num_of_bursts; i++) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(delay_ms);
    digitalWrite(LED_BUILTIN, LOW);
    delay(delay_ms);
  }
}
