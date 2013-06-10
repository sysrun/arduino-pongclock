#include "config.h"
#ifdef JEENODE 
 PortI2C myport (2 /*, PortI2C::KHZ400 */);
 DeviceI2C rtc (myport, 0x68);
#endif
static byte bin2bcd (byte val) {
    return val + 6 * (val / 10);
}

static byte bcd2bin (byte val) {
    return val - 6 * (val >> 4);
}


static void setDate (byte yy, byte mm, byte dd, byte dow, byte h, byte m, byte s) {
  #ifdef JEENODE

    rtc.send();
    rtc.write(0);
    rtc.write(bin2bcd(s));
    rtc.write(bin2bcd(m));
    rtc.write(bin2bcd(h));
    rtc.write(bin2bcd(dow));
    rtc.write(bin2bcd(dd));
    rtc.write(bin2bcd(mm));
    rtc.write(bin2bcd(yy));
    rtc.write(0);
    rtc.stop();
    #endif
}

static void getDate (byte* buf) {
  time_t DCFtime = DCF.getTime(); // Check if new DCF77 time is available
  if (DCFtime) {
    tmElements_t tm;   
    breakTime(DCFtime, tm);
  
    Serial.println("");
    Serial.print("Time: ");
    Serial.print(tm.Hour);
    Serial.print(":");
    printDigits(tm.Minute);
    Serial.print(":");
    printDigits(tm.Second);
    Serial.print(" Date: ");
    Serial.print(tm.Day);
    Serial.print(".");
    Serial.print(tm.Month);
    Serial.print(".");
    Serial.print(tm.Year+1970);
    Serial.print(" - ");
    Serial.println(tm.Wday);
    setDate(tm.Year+1970-2000, tm.Month, tm.Day, tm.Wday, tm.Hour, tm.Minute, tm.Second); 
  }
#ifdef JEENODE
    rtc.send();
    rtc.write(0);	
    rtc.stop();
    rtc.receive();
    buf[0] = bcd2bin(rtc.read(0)); // SEC
    buf[1] = bcd2bin(rtc.read(0)); // MIN
    buf[2] = bcd2bin(rtc.read(0)); // HOUR
    buf[3] = rtc.read(0)&0x07; // DOW
    buf[4] = bcd2bin(rtc.read(0)); // DAY
    buf[5] = bcd2bin(rtc.read(0)); // MON
    buf[6] = bcd2bin(rtc.read(1)); // YEAR
    rtc.stop();
#else
    buf[0] = 1;
    buf[1] = 2;
    buf[2] = 13;
    buf[3] = 3;
    buf[4] = 24;
    buf[5] = 1;
    buf[6] = 80;
#endif

}

void printDigits(int digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}
