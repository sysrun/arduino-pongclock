#define DEMOTIME 30000  // 30 seconds max on each demo is enough.
#define DISPDELAY 100    // Each "display" lasts this long
#define LONGDELAY 1000  // This delay BETWEEN demos
#define DEFAULTTEXTSCROLLTIME 50
#define PONGSPEEDMS 20//35

#define FADEDELAY 60                     // Time to fade display to black
#define SHOWCLOCK 5000                   // How long to show clocktype for
#define MAX_CLOCK_MODE 5                 // Number of clock modes

#define BAT1_X 2                         // Pong left bat x pos (this is where the ball collision occurs, the bat is drawn 1 behind these coords)
#define BAT2_X 45                        // Pong right bat x pos (this is where the ball collision occurs, the bat is drawn 1 behind these coords)

#define JEENODE

#ifdef JEENODE
  #define CLKPIN 17
  #define DATAPIN 7
  #define CS1PIN 4
  #define CS2PIN 14
  #define DCF77PIN 3
  #define DCF_INTERRUPT 1		 // Interrupt number associated with pin

#else
  #define CS1PIN 10
  #define CS2PIN 11
  #define CLKPIN 12
  #define DATAPIN 13
  #define DCF77PIN 2
  #define DCF_INTERRUPT 0		 // Interrupt number associated with pin
#endif
