//print a clock using words rather than numbers
void word_clock() {
 
  cls();
  
  char numbers[19][10]   = { "one", "two", "three", "four","five","six","seven","eight","nine","ten",
                             "eleven","twelve", "thirteen","fourteen","fifteen","sixteen","seventeen","eighteen","nineteen"};              
  char numberstens[5][7] = { "ten","twenty","thirty","forty","fifty" };

  byte hours_y, mins_y; //hours and mins and positions for hours and mins lines  
  
  byte hours = time[2];
  byte mins  = time[1];
   
  //loop to display the clock for a set duration of SHOWCLOCK
  for (int show = 0; show < SHOWCLOCK ; show++) {

    getDate(time); //get the time from the clock chip
            
    //flash led for seconds on arduino
    if ( (time[0] % 2) == 0) { 
      buttons.ledOn(1);
    }else{ 
      buttons.ledOff(1);
    }
    
    if(checkEvents())
      return;

    //print the time if it has changed or if we have just come into the subroutine
    if ( show == 0 || mins != time[1] ) {  
      
      //reset these for comparison next time
      mins = time[1];   
      hours = time[2];
      
      //make hours into 12 hour format
      if (hours > 12){ hours = hours - 12; }
      if (hours == 0){ hours = 12; } 
   
      //split mins value up into two separate digits 
      int minsdigit = mins % 10;
      byte minsdigitten = (mins / 10) % 10;
      
      char str_top[12];
      char str_bot[12];
      
      //if mins <= 10 , then top line has to read "minsdigti past" and bottom line reads hours
      if (mins < 10) {     
        strcpy (str_top,numbers[minsdigit - 1]);
        strcat (str_top," PAST");
        strcpy (str_bot,numbers[hours - 1]);
      }
      //if mins = 10, cant use minsdigit as above, so soecial case to print 10 past /n hour.
      if (mins == 10) {     
        strcpy (str_top,numbers[9]);
        strcat (str_top," PAST");
        strcpy (str_bot,numbers[hours - 1]);
      }
      
      //if time is not on the hour - i.e. both mins digits are not zero, 
      //then make top line read "hours" and bottom line ready "minstens mins" e.g. "three /n twenty one"
      else if (minsdigitten != 0 && minsdigit != 0  ) {
     
        strcpy (str_top,numbers[hours - 1]); 
        
        //if mins is in the teens, use teens from the numbers array for the bottom line, e.g. "three /n fifteen"
        if (mins >= 11 && mins <= 19) {
           strcpy (str_bot, numbers[mins - 1]);
   
        //else bottom line reads "minstens mins" e.g. "three \n twenty three"
        } else {     
          strcpy (str_bot, numberstens[minsdigitten - 1]);
          strcat (str_bot, " "); 
          strcat (str_bot, numbers[minsdigit - 1]); 
        }
      }
      // if mins digit is zero, don't print it. read read "hours" "minstens" e.g. "three /n twenty"
      else if (minsdigitten != 0 && minsdigit == 0  ) {
        strcpy (str_top, numbers[hours - 1]);     
        strcpy (str_bot, numberstens[minsdigitten - 1]);
      }
        
      //if both mins are zero, i.e. it is on the hour, the top line reads "hours" and bottom line reads "o'clock"
      else if (minsdigitten == 0 && minsdigit == 0  ) {
        strcpy (str_top,numbers[hours - 1]);     
        strcpy (str_bot, "O'CLOCK");
      }
     
      //work out offset to center top line on display. 
      byte len = 0;
      while(str_top[len]) { len++; }; //get length of message
      byte offset_top = (X_MAX - ((len - 1)*4)) / 2; //
      
      //work out offset to center bottom line on display. 
      len = 0;
      while(str_bot[len]) { len++; }; //get length of message
      byte offset_bot = (X_MAX - ((len - 1)*4)) / 2; //

      fade_down();
      disp.syncDisplays();
    
      //plot hours line
      byte i = 0;
      while(str_top[i]) {
        ht1632_puttinychar((i*4) + offset_top, 2, str_top[i]); 
        i++;
      }
  
      i = 0;
      while(str_bot[i]) {
        ht1632_puttinychar((i*4) + offset_bot, 9, str_bot[i]); 
        i++;
      }
      disp.syncDisplays();

    }
  delay (50); 
  }
  fade_down();
}
