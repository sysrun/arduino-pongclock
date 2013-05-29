static byte bin2bcd (byte val) {
    return val + 6 * (val / 10);
}

static byte bcd2bin (byte val) {
    return val - 6 * (val >> 4);
}


static void setDate (byte yy, byte mm, byte dd, byte dow, byte h, byte m, byte s) {
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
}

static void getDate (byte* buf) {
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
}
