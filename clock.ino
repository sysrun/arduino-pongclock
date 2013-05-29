
void flashSecondsLED() {
  //flash led for seconds on arduino
  if ( (time[0] % 2) == 0) { 
    buttons.ledOn(1);
  }else{ 
    buttons.ledOff(1);
  }
}

boolean checkEvents() {
  if (Serial.available()) {
    return true;
  }

  byte event = buttons.buttonCheck();
  if(event == BlinkPlug::ON1) {
    switch_mode();
    return true;
  }
  if(event == BlinkPlug::ON2) {
    display_date();
    fade_down();
    //pong();
    return true;
  }
  return false;
}


//print menu to change the mode
void switch_mode() {
  
  char* modes[] = {"Numbers", "Pong", "Words", "Jumble", "Text", "Random", "Set Clk" };
  
  byte next_clock_mode;
  byte firstrun = 1;
  
  mode_changed = 1; //set this flag so we don't show the date when we get out the menu, we just go straigh to the new mode.

  //loop waiting for button (timeout after X loops to return to mode X)
  for(int count=0; count< 40 ; count++) {
     
    //if user hits button, change the clock_mode
    byte event = buttons.buttonCheck();

    if(event == BlinkPlug::ON1 || firstrun == 1) {
      
      count = 0;
      cls();
        
      if (firstrun == 0) { clock_mode++; } 
      if (clock_mode > MAX_CLOCK_MODE) { clock_mode = 0; }
       
      //print arrown and current clock_mode name on line one and print next clock_mode name on line two
      char str_top[9];
      char str_bot[9];
      
      strcpy (str_top, ">");
      strcat (str_top, modes[clock_mode]);
  
      next_clock_mode = clock_mode + 1;
      if (next_clock_mode > MAX_CLOCK_MODE) { next_clock_mode = 0; }
  
      strcpy (str_bot, " ");
      strcat (str_bot, modes[next_clock_mode]);
  
      byte i = 0;
      while(str_top[i]) {
        ht1632_putchar(i*6, 0, str_top[i]); 
        i++;
      }
  
      i = 0;
      while(str_bot[i]) {
        ht1632_putchar(i*6, 8, str_bot[i]); 
        i++;
      }
      disp.syncDisplays();

      firstrun = 0;
    }
    delay(50); 
  }
  //if random clock_mode set, set next hour to change clock type 
  if (clock_mode == MAX_CLOCK_MODE - 1 ){ 
    random_mode = 1;  
    mode_time_up = time[2]; 
    clock_mode = 0;
  } else {
    random_mode = 0;
  } 
}



/*
 * display_date
 * print the day of week, date and month 
 */
void display_date() {
  cls();
  //read the date from the DS1307
  //it returns the month number, day number, and a number representing the day of week - 1 for Tue, 2 for Wed 3 for Thu etc.
  byte dow = time[3]-1; //we  take one off the value the DS1307 generates, as our array of days is 0-6 and the DS1307 outputs  1-7.
  byte date = time[4];
  byte month = time[5]-1; 

  Serial.print("DOW:");
  Serial.print(dow);
  Serial.print(" - DATE:");
  Serial.print(date);
  Serial.print(" - MONTH:");
  Serial.println(month);
  //array of day and month names to print on the display. Some are shortened as we only have 8 characters across to play with 
  char daynames[7][9]={
    "Sonntag", "Montag", "Dienstag", "Mittwoch", "Dnrstag", "Freitag", "Samstag"};
  char monthnames[12][9]={
    "Januar", "Februar", "Maerz", "April", "Mai", "Juni", "Juli", "August", "Sept", "Oktober", "November", "Dezember"};

  //call the flashing cursor effect for one blink at x,y pos 0,0, height 5, width 7, repeats 1
  flashing_cursor(0,0,5,7,1);
  disp.syncDisplays();

  //print the day name
  int i = 0;
  while(daynames[dow][i]) {
    flashing_cursor(i*6,0,5,7,0);
    ht1632_putchar(i*6 , 0, daynames[dow][i]); 
    disp.syncDisplays();
    i++;
    byte event = buttons.buttonCheck();
    //check for button press and exit if there is one.
    if(event == BlinkPlug::ON1 || event == BlinkPlug::ON2){
      return;
    }
  }

  //pause at the end of the line with a flashing cursor if there is space to print it.
  //if there is no space left, dont print the cursor, just wait.
  if (i*6 < 48){
    flashing_cursor(i*6,0,5,7,1);  
    disp.syncDisplays();
  } else {
    delay(300);
  }

  //flash the cursor on the next line  
  flashing_cursor(0,8,5,7,0);
  disp.syncDisplays();

  //print the date on the next line: First convert the date number to chars so we can print it with ht1632_putchar
  char buffer[3];
  itoa(date,buffer,10); 
 
  //print the 1st date number
  ht1632_putchar(0, 8, buffer[0]);
  disp.syncDisplays();

  //if date is under 10 - then we only have 1 digit so set positions of sufix etc one character nearer
  byte suffixposx = 6;
  
  //if date over 9 then print second number and set xpos of suffix to be 1 char further away
  if (date > 9){
    suffixposx = 12;
    flashing_cursor(6,8,5,7,0); 
    ht1632_putchar(6, 8, buffer[1]);
    disp.syncDisplays();
  }
 
  //print the 2 suffix characters
  flashing_cursor(suffixposx, 8,5,7,0);
  ht1632_putchar(suffixposx, 8, '.'); 
  //delay(70);
  disp.syncDisplays();
  //blink cursor after 
  flashing_cursor(suffixposx + 6,8,5,7,1);  
  disp.syncDisplays();

  //replace day name with date on top line - effectively scroll the bottom line up by 8 pixels
  cls();   
  ht1632_putchar(0, 0, buffer[0]);  //date first digit
  ht1632_putchar(6, 0, buffer[1]);  //date second digit - this may be blank and overwritten if the date is a single number
  ht1632_putchar(suffixposx, 0, '.');   //date suffix
  disp.syncDisplays();

  //flash the cursor for a second for effect
  flashing_cursor(suffixposx + 6,0,5,7,0);  
  disp.syncDisplays();

  //print the month name on the bottom row
  i = 0;
  while(monthnames[month][i]) {  
    flashing_cursor(i*6,8,5,7,0);
    disp.syncDisplays();
    ht1632_putchar(i*6, 8, monthnames[month][i]); 
    i++;
    disp.syncDisplays();

    byte event = buttons.buttonCheck();
    //check for button press and exit if there is one.
    if(event == BlinkPlug::ON1 || event == BlinkPlug::ON2){
        return;
    }
  }

  //blink the cursor at end if enough space after the month name, otherwise juts wait a while
  if (i*6 < 48){
    flashing_cursor(i*6,8,5,7,2);  
    disp.syncDisplays();
  } else {
    delay(1000);
  }
  delay(3000);
}
