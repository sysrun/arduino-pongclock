void normal_clock() {
  Serial.println("clockmode:normal");
  cls();
  byte hours = time[2];
  byte mins = time[1];
  byte secs = time[0];
  //loop to display the clock for a set duration of SHOWCLOCK
  for (int show = 0; show < SHOWCLOCK ; show++) {
    getDate(time);

    flashSecondsLED();

    if(checkEvents())
      return;


    //update the clock if this is the first run of the show clock loop,
    // or if the time has changed from what we had stored in mins and hors vars.
    if ( show == 0 || (mins != time[1] ) ) {
      mins = time[1];
      hours = time[2];
      char buffer[3];
      
      itoa(hours,buffer,10);
      if (hours<10) {
        buffer[1]=buffer[0];
        buffer[0]='0';
      }
      ht1632_putbigchar(0,1, buffer[0]);
      ht1632_putbigchar(12,1, buffer[1]);
      
      toolbox.setPixel(23,4,1);
      toolbox.setPixel(23,5,1);
      toolbox.setPixel(24,4,1);
      toolbox.setPixel(24,5,1);
      
      toolbox.setPixel(23,10,1);
      toolbox.setPixel(23,11,1);
      toolbox.setPixel(24,10,1);
      toolbox.setPixel(24,11,1);
      
      itoa (mins, buffer, 10);
      if (mins < 10) {
        buffer[1] = buffer[0];
        buffer[0] = '0';
      }
      ht1632_putbigchar(26, 1, buffer[0]);
      ht1632_putbigchar(38, 1, buffer[1]);
      disp.syncDisplays();
    }
    delay(50);
  }
  fade_down();

}
