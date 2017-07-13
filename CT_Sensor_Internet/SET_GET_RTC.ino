void SetClock(int yy, int mm, int dd, int wk, int HH, int MM, int SS)
{
  i2c_start(DS1307 | I2C_WRITE);
  i2c_write(RTC_SECOND);
  i2c_write(decToBcd(SS));
  i2c_write(decToBcd(MM));
  i2c_write(decToBcd(HH));
  i2c_write(decToBcd(wk));
  i2c_write(decToBcd(dd));
  i2c_write(decToBcd(mm));
  i2c_write(decToBcd(yy));
  i2c_write(0x00);
  i2c_stop();
}

void GetClock()
{
  i2c_start(DS1307 | I2C_WRITE);
  i2c_write(RTC_SECOND);
  i2c_stop();
  i2c_rep_start(DS1307 | I2C_READ);
  second_BCD = i2c_read(false);
  minute_BCD = i2c_read(false);
  hour_BCD   = i2c_read(false);
  wkDay_BCD  = i2c_read(false);
  date_BCD   = i2c_read(false);
  month_BCD  = i2c_read(false);
  year_BCD   = i2c_read(false);
  ctrl_BCD  = i2c_read(true);
  i2c_stop();
}

byte BCDtoDecimal(byte bcdVal)
{
  return bcdVal / 16 * 10 + bcdVal % 16;
}

void Print2DigitHex(byte hexVal)
{
  if (hexVal < 0x10)
  {
    Serial.print("0");
  }
  Serial.print(hexVal, HEX);
}
byte decToBcd(byte val){
// Convert normal decimal numbers to binary coded decimal
  return ( (val/10*16) + (val%10) );
}

byte bcdToDec(byte val)  {
// Convert binary coded decimal to normal decimal numbers
  return ( (val/16*10) + (val%16) );
}
