/******************************************************************************
  A configurator for changing the I2C address on the Qwiic Button/Switch that walks
  the user through finding the address of their button, and then changing it!

  Fischer Moseley @ SparkFun Electronics
  Original Creation Date: July 30, 2019

  This code is Lemonadeware; if you see me (or any other SparkFun employee) at the
  local, and you've found our code helpful, please buy us a round!

  Hardware Connections:
  Attach the Qwiic Shield to your Arduino/Photon/ESP32 or other
  Plug the button into the shield
  Print it to the serial monitor at 115200 baud.

  Distributed as-is; no warranty is given.
******************************************************************************/
#include "Keyboard.h"
#include <SparkFun_Qwiic_Button.h>

QwiicButton button1;
QwiicButton button2;
QwiicButton button3;

void setup() {
    Keyboard.begin();
    
    Wire.begin(); //Join I2C bus
    button1.begin(0x6F);
    button2.begin(0x6D);
    button3.begin(0x6C);
}

enum State { PRESSED, RELEASED, WAITING };
State buttonstate[3] = { WAITING, WAITING, WAITING };


void do_fade(void)   { Keyboard.press(KEY_LEFT_ALT); Keyboard.press('F'); delay(100); Keyboard.releaseAll();}
void select_scene(char s) { Keyboard.press(KEY_LEFT_ALT); Keyboard.press(s); delay(100); Keyboard.releaseAll(); }

void select_S1(void) { select_scene('1'); do_fade(); }
void select_S2(void) { select_scene('2'); do_fade(); }
void select_S3(void) { select_scene('3'); do_fade(); }

void (*sendkeys[])(void) = {select_S1, select_S2, select_S3 };

void loop() {
    bool pressed[3];
    pressed[0] = button1.isPressed();
    pressed[1] = button2.isPressed();
    pressed[2] = button3.isPressed();

    for (int x = 0; x < 3; x++) {
        switch (buttonstate[x]) {
            case WAITING: if ( pressed[x]) { buttonstate[x] = PRESSED;  } 
                          break;
            case PRESSED: if (!pressed[x]) { buttonstate[x] = RELEASED; } 
                          break;
            case RELEASED:
                          (sendkeys[x])();
                          buttonstate[x] = WAITING;
                          break;
        }
    }
    delay(20);
}
