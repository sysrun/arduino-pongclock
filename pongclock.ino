/// @dir rtc_demo
/// Hooking up a DS1307 (5V) or DS1340Z (3V) real time clock via I2C.
// 2009-09-17 <jc@wippler.nl> http://opensource.org/licenses/mit-license.php


//#define JEENODE
  


// the real-time clock is connected to port 1 in I2C mode (AIO = SCK, dIO = SDA)
#include "config.h"
#include <JeeLib.h>
#include "MatrixDisplay.h"
#include "DisplayToolbox.h"
#include "Clockfont.h"
#include "fontLarge.h"

// Macro to make it the initDisplay function a little easier to understand
#define setMaster(dispNum, CSPin) initDisplay(dispNum,CSPin,true)
#define setSlave(dispNum, CSPin) initDisplay(dispNum,CSPin,false)

PortI2C myport (3 /*, PortI2C::KHZ400 */);
DeviceI2C rtc (myport, 0x68);

BlinkPlug buttons (2);
MilliTimer everySecond;

// Init Matrix
MatrixDisplay disp(2, CLKPIN, DATAPIN, true);
// Pass a copy of the display into the toolbox
DisplayToolbox toolbox(&disp);

// Prepare boundaries
uint8_t X_MAX = 0;
uint8_t Y_MAX = 0;
uint8_t time[7];
uint8_t timetemp[7];

int random_mode = 0;
int mode_time_up;                        // Holds hour where clock mode will next change if in random mode
int mode_changed = 0;                    // Flag if mode changed.
int clock_mode = 2;                      // Default clock mode (1 = pong)
int textscrolltime = DEFAULTTEXTSCROLLTIME;

char msgLine[200] = "        #sysrun: Das ist ein Test!";
String uartMsgTemp = "        ";         // a string to hold incoming data
boolean uartStringComplete = false;  // whether the string is complete


void setup() {
  Serial.begin(57600);
  Serial.println("\n[pongclock]");

  // Fetch bounds (dynamically work out how large this display is)
  X_MAX = disp.getDisplayCount() * (disp.getDisplayWidth());
  Y_MAX = disp.getDisplayHeight();
  // Prepare displays
  disp.setMaster(0,CS1PIN); // INT
  disp.setSlave(1,CS2PIN); // P4, DATA
  getDate(time);
  mode_time_up = time[3];                 // Set first time to change clock mode
  //setDate(13,05,25,7,22,52,0);
  uartMsgTemp.reserve(200);
}

void loop() {
  if (random_mode){
    getDate(time);
    //set counter to change clock type every 3 or 4 hours
    if (mode_time_up == time[2]) {
      mode_time_up = time[2] + random (2,4);   //set next time to change - add 2 to 3 hours
      if (mode_time_up >= 24) { mode_time_up = random (1,2); }   //if time is over 24, set to 0 + random
      clock_mode = random(0,MAX_CLOCK_MODE - 1);     //pick new random mode
    }
  }
  
  if (uartStringComplete) {
    text_scroller();
  }
  
  switch(clock_mode) {
    case 0: normal_clock();break;
    case 1: pong(); break;
    case 2: jumble(); break;
    case 3: text_scroller(); break;
    default: normal_clock();break;
  }
}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read(); 

    // add it to the inputString:
    if (inChar != '\n') {
      uartMsgTemp += inChar;
    }
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      int timeSep = uartMsgTemp.indexOf(':');
      if (timeSep >= 1 && timeSep <= 3) {
        textscrolltime = uartMsgTemp.substring(0, timeSep).toInt();
        uartMsgTemp = uartMsgTemp.substring(timeSep+1, uartMsgTemp.length()+1);
      }
      uartMsgTemp = "        " + uartMsgTemp;
      uartMsgTemp.toCharArray(msgLine, uartMsgTemp.length()+1);
      uartMsgTemp = "";
      uartStringComplete = true;
    } 
  }
}
