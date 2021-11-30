#include <BleKeyboard.h>
BleKeyboard bleKeyboard("Apple IIgs BLE", "Je&F Manufacturer", 100);

#include "Adafruit_Keypad.h"
// https://www.arduino.cc/en/Reference/KeyboardModifiers
#define K_ESC  KEY_ESC
#define K_TAB  KEY_TAB
#define K_BS   KEY_BACKSPACE
#define K_DEL  KEY_DELETE
#define K_RTN  KEY_RETURN
#define K_LCTL KEY_LEFT_CTRL
#define K_LSHF KEY_LEFT_SHIFT
#define K_RSHF KEY_RIGHT_SHIFT
#define K_CAPS KEY_CAPS_LOCK
#define K_LALT KEY_LEFT_ALT
#define K_LCMD KEY_LEFT_GUI
#define K_LEFT KEY_LEFT_ARROW
#define K_DOWN KEY_DOWN_ARROW
#define K_UP   KEY_UP_ARROW
#define K_RIGH KEY_RIGHT_ARROW
#define K_PAUP KEY_PAGE_UP
#define K_PADN KEY_PAGE_DOWN
const byte ROWS = 5;
const byte COLS = 17;
byte rowPins[ROWS] = { 13, 14,  15, 0, 21 };
byte colPins[COLS] = { 27, 4, 26, 25,    32, 22,  /*39,*/ 2, 3, 1,   5, 23, 19, 18,   17, 16,   33,  12 };
char keys[ROWS][COLS] = {
  { K_ESC,  '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', K_BS, K_DEL, '=', '/' },
  { K_TAB,  'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '*', '7', '8', '9' },
  { K_LCTL, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', K_RTN, '+', '4', '5', '6' },
  { K_LSHF, 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', K_RSHF, K_RTN, '-', '1', '2', '3' },
  { K_CAPS, K_LALT, K_LCMD, '`', ' ', '\\', K_LEFT, K_DOWN, K_UP, K_RIGH, '0', '.', 0,0,0,0,0 }
};
char alt_keys[ROWS][COLS] = {
  { K_ESC,  '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', K_DEL, K_DEL, '=', '/' },
  { K_TAB,  '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '*', '7', K_UP, '9' },
  { K_LCTL, 'a', 's', 'd', 'f', 'g', K_LEFT, K_DOWN, K_UP, K_RIGH, ';', '\'', K_RTN, '+', K_LEFT, K_DOWN, K_RIGH },
  { K_LSHF, 'z', 'x', 'c', 'v', 'b', K_PADN, K_PAUP, ',', '.',   '/', K_RSHF, K_RTN, '-', '1', '2', '3' },
  { K_CAPS, K_LALT, K_LCMD, '`', ' ', '\\', K_LEFT, K_DOWN, K_UP, K_RIGH, '0', '.', 0,0,0,0,0 }
};
Adafruit_Keypad keypad = Adafruit_Keypad(
  makeKeymap(keys), rowPins, colPins, ROWS, COLS
);
bool useAltLayer = false;

void setup() {
//  Serial.begin(115200);
  keypad.begin();
  bleKeyboard.begin();
  bleKeyboard.setBatteryLevel(100);
//  Serial.println("ready.");
}

void loop() {
  keypad.tick();
  if (keypad.available()) { 
    keypadEvent e = keypad.read();
    if (bleKeyboard.isConnected()) {
      char key = useAltLayer ? alt_keys[e.bit.ROW][e.bit.COL] : (char)e.bit.KEY;
      if (key == K_CAPS) {
        useAltLayer = (e.bit.EVENT == KEY_JUST_PRESSED);
      }
      else if (e.bit.EVENT == KEY_JUST_PRESSED) {
        bleKeyboard.press(key);
      }
      else if(e.bit.EVENT == KEY_JUST_RELEASED) {
        bleKeyboard.release(key);
      }
    }
  }
  delay(1);
}
