

/*
 * flashing_cursor
 * print a flashing_cursor at xpos, ypos and flash it repeats times 
 */
void flashing_cursor(byte xpos, byte ypos, byte cursor_width, byte cursor_height, byte repeats) {
  for (byte r = 0; r <= repeats; r++) {    
    for (byte x = 0; x <= cursor_width; x++) {
      for (byte y = 0; y <= cursor_height; y++) {
        toolbox.setPixel(x+xpos, y+ypos, 1);
      }
    }
    
    if (repeats > 0) {
      delay(400);
    } else {
      delay(70);
    }
        
    for (byte x = 0; x <= cursor_width; x++) {
      for (byte y = 0; y <= cursor_height; y++) {
        toolbox.setPixel(x+xpos, y+ypos, 0);
      }
    }   
    //if cursor set to repeat, wait a while
    if (repeats > 0) {
     delay(400); 
    }
  }
}

void fadeTo(char value) {
  value = constrain(value, 0, 14);
  toolbox.setBrightness(value);
}


/*
 * fade_down
 * fade the display to black
 */
void fade_down() {
  char intensity;
  for (intensity=14; intensity >= 0; intensity--) {
    toolbox.setBrightness(intensity);
    delay(FADEDELAY);
  }
  //clear the display and set it to full brightness again so we're ready to plot new stuff
  cls();
  toolbox.setBrightness(15);
}

void cls() {
  disp.clear(0,true,false);
  disp.clear(1,true,false);
}
