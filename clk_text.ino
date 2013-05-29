

int crtPos = 0;
int nSpeed = 4;

void text_scroller() {
  Serial.println("clockmode:textscroller");
  cls();
  for (int show = 0; show < textscrolltime ; show++) {
    getDate(time);
    flashSecondsLED();

    if(checkEvents())
      return;
 
    if (crtPos > strlen(msgLine)) 
      crtPos = 0;
    
    if (crtPos == 0)
      cls();
    if (uartStringComplete) {
      crtPos = 0;
      uartStringComplete = false;
    }
    displayLargeScrollingLine();
  }
  fade_down();
}

void displayLargeScrollingLine(){
  int leftCol, tmpPos;
  uint8_t x;
  // shift the 14 bit high characters one character to the left, 1 character is 2 - 11 bits wide;
  for (leftCol = 0; leftCol > -12; --leftCol) {
    x = leftCol;
    tmpPos = crtPos;
    do {
      x = ht1632_putLargeChar(x, 1, ((tmpPos < strlen(msgLine)) ? msgLine[tmpPos] : ' '))+1;
      ++tmpPos;
    } while ((x < X_MAX + 1) && (x != 0));		// fill display with 32 dots and stop if 1 first char is moved out of the display;	
    disp.syncDisplays();
    delay(40 - nSpeed * 10);

    if (x == 0)
      break;									// we have moved the first complete character so stop;
  }
  ++crtPos;
}

int ht1632_putLargeChar(int x, int y, char c) {
  // fonts defined for ascii 32 and beyond (index 0 in font array is ascii 32);
  byte charIndex, col, row;

  // replace undisplayable characters with blank;
  if (c == 0xFFFFFFA1) {
    charIndex = 127 - 32;
  } else if (c < 32 || c > 127) {
    charIndex	=	0;
  } else {
    charIndex	=	c - 32;
  }

  // move character definition, pixel by pixel, onto the display;
  // Fonts are defined as up to 14 bit per row and max. 11 columns;
  // first row is always zero to create the space between the characters;

  for (col=0; col < 11; ++col) {				// max 11 columns;
    uint16_t dots = pgm_read_word_near(&largeFont[charIndex][col]);
    if (dots == 0)  // stop if all bits zero;
      break;

    for (row=0; row < 14; row++) {
      if (dots & (0x4000 >> row))    			// max 14 rows;
        toolbox.setPixel(x+col, y+row, 1);
      else 
        toolbox.setPixel(x+col, y+row, 0);
    }
  }
  return x+col;
}
