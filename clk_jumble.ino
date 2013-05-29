//show time and date and use a random jumble of letters transition each time the time changes.
void jumble() {
  
  char days[7][4] = {"SON", "MON", "DIE", "MIT", "DON", "SAM","SON"}; //DS1307 outputs 1-7
  char allchars[37] = {"ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890"};
  char endchar[16];
  byte counter[16];
  byte mins = time[1];
  byte seq[16];
  
  cls();
  
  for (int show = 0; show < SHOWCLOCK ; show++) {
    
    getDate(time);
    //flash led for seconds on arduino
    if ( (time[0] % 2) == 0) { 
      buttons.ledOn(1);
    }else{ 
      buttons.ledOff(1);
    }
    
    if(checkEvents())
      return;

    
    if ( show == 0 || mins != time[1]  ) {  
      
      //fill an arry with 0-15 and randomize the order so we can plot letters in a jumbled pattern rather than sequentially
      for (int i=0; i<16; i++) {
        seq[i] = i;  // fill the array in order
      }
      //randomise array of numbers 
        for (int i=0; i<(16-1); i++) {
            int r = i + (rand() % (16-i)); // Random remaining position.
            int temp = seq[i]; seq[i] = seq[r]; seq[r] = temp;
        }

      
      //reset these for comparison next time
      mins = time[1];
      byte hours = time[2];   
      byte dow   = time[3] - 1; // the DS1307 outputs 1 - 7. 
      byte date  = time[4];
   
      byte alldone = 0;
   
      //set counters to 50
      for(byte c=0; c<16 ; c++) {
        counter[c] = 3 + random (0,20);
      }
  
      //set final characters
      char buffer[3];
      itoa(hours,buffer,10);
  
      //fix - as otherwise if num has leading zero, e.g. "03" hours, itoa coverts this to chars with space "3 ". 
      if (hours < 10) {
        buffer[1] = buffer[0];
        buffer[0] = '0';
      }
      
      endchar[0] = buffer[0];
      endchar[1] = buffer[1];
      endchar[2] = ':';
      
      itoa (mins, buffer, 10);
      if (mins < 10) {
        buffer[1] = buffer[0];
        buffer[0] = '0';
      }
  
      endchar[3] = buffer[0];
      endchar[4] = buffer[1];
  
      itoa (date, buffer, 10);
      if (date < 10) {
        buffer[1] = buffer[0];
        buffer[0] = '0';
      }
 
      //then work out date 2 letter suffix - eg st, nd, rd, th etc
      /*
    char suffix[4][3]={"st", "nd", "rd", "th"};
      byte s = 3; 
      if(date == 1 || date == 21 || date == 31) {
        s = 0;
      } 
      else if (date == 2 || date == 22) {
        s = 1;
      } 
      else if (date == 3 || date == 23) {
        s = 2;
      }
      */
      //set topline
      endchar[5] = ' ';
      endchar[6] = ' ';
      endchar[7] = ' ';
 
      //set bottom line
      endchar[8] = days[dow][0];
      endchar[9] = days[dow][1];
      endchar[10] = days[dow][2];
      endchar[11] = ' ';
      endchar[12] = buffer[0];
      endchar[13] = buffer[1];
      endchar[14] = '.';
      endchar[15] = ' ';
      //endchar[14] = suffix[s][0];
      //endchar[15] = suffix[s][1];
 
      byte x = 0;
      byte y = 0;
 
      //until all counters are 0
      while (alldone < 16){
   
        //for each char    
        for(byte c=0; c<16 ; c++) {
      
          if (seq[c] < 8) { 
            x = 0;
            y = 0; 
          } else {
            x = 8;
            y = 8;   
          }
      
          //if counter > 1 then put random char
          if (counter[ seq[c] ] > 1) {
            ht1632_putchar( ( seq[c] -x) * 6, y, allchars[random(0,36)]); //random
            disp.syncDisplays();
            counter[ seq[c] ]--;
          }
 
          //if counter == 1 then put final char 
          if (counter[ seq[c] ] == 1) {
            ht1632_putchar( (seq[c]-x) * 6, y, endchar[seq[c]]); //final char
            disp.syncDisplays();
            counter[seq[c]] = 0;
            alldone++;
          } 
              
          //if counter == 0 then just pause to keep update rate the same
          if (counter[seq[c]] == 0) {
            delay(4);
          }

    
          byte event = buttons.buttonCheck();
          if(event == BlinkPlug::ON1) {
            switch_mode();
            return;      
          }
        }
      }
    }
    delay(50);
  } //showclock
  fade_down();
}
