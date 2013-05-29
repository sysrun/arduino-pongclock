/* ht1632_putchar
 * Copy a 5x7 character glyph from the myfont data structure to display memory, with its upper left at the given coordinate
 * This is unoptimized and simply uses plot() to draw each dot.
 */
int ht1632_putchar(byte x, byte y, char c) {
  byte dots;
  if (c >= 'A' && c <= 'Z' || (c >= 'a' && c <= 'z') ) {
    c &= 0x1F;   // A-Z maps to 1-26
  } else if (c >= '0' && c <= '9') {
    c = (c - '0') + 31;
  } else if (c == ' ') {
    c = 0; // space
  } else if (c == '.') {
    c = 27; // full stop
  } else if (c == '\'') {
    c = 28; // single quote mark
  } else if (c == ':') {
    c = 29; // clock_mode selector arrow
  } else if (c == '>') {
    c = 30; // clock_mode selector arrow
  }

  int retX = 0;
  for (char col=0; col< 5; col++) {
    dots = pgm_read_byte_near(&myfont[c][col]);
    for (char row=0; row < 7; row++) {
      if (dots & (64>>row))   	     // only 7 rows.
        toolbox.setPixel(x+col, y+row, 1);
      else 
        toolbox.setPixel(x+col, y+row, 0);
    }
    retX = x+col;
  }
  return retX;
}

/* ht1632_putbigchar
 * Copy a 10x14 character glyph from the myfont data structure to display memory, with its upper left at the given coordinate
 * This is unoptimized and simply uses plot() to draw each dot.
 */
void ht1632_putbigchar(byte x, byte y, char c) {
  byte dots;
  if (c >= 'A' && c <= 'Z' || (c >= 'a' && c <= 'z') ) {
    return;   //return, as the 10x14 font contains only numeric characters 
  } 
  if (c >= '0' && c <= '9') {
    c = (c - '0');
    c &= 0x1F;
  } 

  for (char col=0; col< 10; col++) {
    dots = pgm_read_byte_near(&mybigfont[c][col]);
    for (char row=0; row < 8; row++) {
      if (dots & (128>>row))   	   
        toolbox.setPixel(x+col, y+row, 1);
      else 
        toolbox.setPixel(x+col, y+row, 0);
    }

    dots = pgm_read_byte_near(&mybigfont[c][col+10]);
    for (char row=0; row < 8; row++) {
      if (dots & (128>>row))   	   
        toolbox.setPixel(x+col, y+row+8, 1);
      else 
        toolbox.setPixel(x+col, y+row+8, 0);
    } 
  }  
}


/* ht1632_puttinychar
 * Copy a 3x5 character glyph from the myfont data structure to display memory, with its upper left at the given coordinate
 * This is unoptimized and simply uses plot() to draw each dot.
 */
uint8_t ht1632_puttinychar(byte x, byte y, char c) {
  byte dots;
  if (c >= 'A' && c <= 'Z' || (c >= 'a' && c <= 'z') ) {
    c &= 0x1F;   // A-Z maps to 1-26
  } else if (c >= '0' && c <= '9') {
    c = (c - '0') + 31;
  } else if (c == ' ') {
    c = 0; // space
  } else if (c == '.') {
    c = 27; // full stop
  } else if (c == '\'') {
    c = 28; // single quote mark
  } else if (c == '!') {
    c = 29; // single quote mark
  } else if (c == '?') {
    c = 30; // single quote mark
  }
  uint8_t lastX = 0;
  for (char col=0; col< 3; col++) {
    dots = pgm_read_byte_near(&mytinyfont[c][col]);
    for (char row=0; row < 5; row++) {
      if (dots & (16>>row))   	   
        toolbox.setPixel(x+col, y+row, 1);
      else 
        toolbox.setPixel(x+col, y+row, 0);
    }
    lastX = x+col;
  }
  return lastX;
}

/*
 * Copy a character glyph from the myfont data structure to
 * display memory, with its upper left at the given coordinate
 * This is unoptimized and simply uses setPixel() to draw each dot.
 */
void drawChar(uint8_t x, uint8_t y, char c) {
  uint8_t dots;
  if (c >= 'A' && c <= 'Z' ||
    (c >= 'a' && c <= 'z') ) {
    c &= 0x1F;   // A-Z maps to 1-26
  } else if (c >= '0' && c <= '9') {
    c = (c - '0') + 27;
  } else if (c == ' ') {
    c = 0; // space
  }
  for (char col=0; col< 5; col++) {
    dots = pgm_read_byte_near(&myfont[c][col]);
    for (char row=0; row < 7; row++) {
      if (dots & (64>>row))   	     // only 7 rows.
        toolbox.setPixel(x+col, y+row, 1);
      else 
        toolbox.setPixel(x+col, y+row, 0);
    }
  }
}
