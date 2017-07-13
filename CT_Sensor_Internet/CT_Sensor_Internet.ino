#define SDA_PORT PORTD
#define SDA_PIN 4
#define SCL_PORT PORTD
#define SCL_PIN 5
#include <SoftI2CMaster.h>
#include <avr/io.h>
#include "EmonLib.h"                   // Include Emon Library
#include <SoftwareSerial.h>
#include <stdlib.h>

// replace with your channel's thingspeak API key
String apiKey ="XLXP8LMOQHFXMNLZ"; //Write
//String apiKey =""; //Read

// connect 10 to TX of Serial USB
// connect 11 to RX of serial USB
SoftwareSerial ser(10, 11); // RX, TX

long i=1L,j=0L,k;
double unit,sunit,ssum1,ssum2;

#define DS1307      0xD0
#define RTC_SECOND  0
#define RTC_MINUTE  1
#define RTC_HOUR    2
#define RTC_WKDAY   3
#define RTC_DATE    4
#define RTC_MONTH   5
#define RTC_YEAR    6
#define RTC_CTRL    7

//DS1307 Clock Buffer
byte second_BCD;
byte minute_BCD;
byte hour_BCD;
byte wkDay_BCD;
byte date_BCD;
byte month_BCD;
byte year_BCD;
byte ctrl_BCD;

EnergyMonitor emon1;
//EnergyMonitor emon2;
//EnergyMonitor emon3;
//EnergyMonitor emon4;
//EnergyMonitor emon5;
//EnergyMonitor emon6;

double Irms1 = 0.0; ////ตัวแปรกระแสที่วัดได้
//double Irms2 = 0.0;
//double Irms3 = 0.0;
//double Irms4 = 0.0;
//double Irms5 = 0.0;
//double Irms6 = 0.0;

double a,b,c,d,e,f,x,y;

double vat1,vat2,s1,s2,ft,k1,k2,k3,k4,k5,k6,k7,k8,k9,k10,sum1,sum2,fee1=8.19,fee2=38.22,Amount1,Amount2,sumb1,sumb2,sAmount1,sAmount2;

void setup()                                    //Setup Function
{
 

  i2c_init();
  emon1.current(0, 50.0);
//  emon2.current(1, 50.0);
//  emon3.current(2, 50.0);
//  emon4.current(3, 50.0);
//  emon5.current(4, 50.0);
//  emon6.current(5, 50.0);
  Irms1 = emon1.calcIrms(2000);  // Calculate Irms only
//  Irms2 = emon2.calcIrms(2000);
//  Irms3 = emon3.calcIrms(2000);
//  Irms4 = emon4.calcIrms(2000);
//  Irms5 = emon5.calcIrms(2000);
//  Irms6 = emon6.calcIrms(2000);

  // enable debug serial
  Serial.begin(9600); 
  // enable software serial
  ser.begin(9600);
  
  // reset ESP8266
  ser.println("AT+RST");
}

void loop()
{
  Irms1 = emon1.calcIrms(2000);  // Calculate Irms only
//  Irms2 = emon2.calcIrms(2000);
//  Irms3 = emon3.calcIrms(2000);
//  Irms4 = emon4.calcIrms(2000);
//  Irms5 = emon5.calcIrms(2000);
//  Irms6 = emon6.calcIrms(2000);
  SetDATA();
  GetClock();
  Serial.print(BCDtoDecimal(hour_BCD));
  Serial.print(":");
  Serial.print(BCDtoDecimal(minute_BCD));
  Serial.print(":");
  Serial.print(BCDtoDecimal(second_BCD));
  Serial.print(" ");
  Serial.print(BCDtoDecimal(date_BCD));
  Serial.print("/");
  Serial.print(BCDtoDecimal(month_BCD));
  Serial.print("/20");
  Serial.print(BCDtoDecimal(year_BCD));
  Serial.print(" ");
  Serial.print("CH1:");
  Serial.print(Irms1);
//  Serial.print(" ");
//  Serial.print("CH2:");
//  Serial.print(Irms2);
//  Serial.print(" ");
//  Serial.print("CH3:");
//  Serial.print(Irms3);
//  Serial.print(" ");
//  Serial.print("CH4:");
//  Serial.print(Irms4);
//  Serial.print(" ");
//  Serial.print("CH5:");
//  Serial.prin(Irms5);
//  Serial.print(" ");
//  Serial.print("CH6:");
//  Serial.print(Irms6);
  Serial.println();

  unit = (((Irms1*0.220)/3600)*15); // per 15 s
  sunit += unit;
  Serial.print("unit : ");
  Serial.println(unit);
  Serial.print("sunit : ");
  Serial.println(sunit);
  
  
  ft = sunit * 0.048;
  Serial.print("ft : ");
  Serial.println(ft);
  
if(sunit>=0 && sunit<=150)
{
  Serial.print("k1 : ");
    if(sunit>=15)
  {
    k1 = 15 * 2.3488;
    Serial.println(k1);
  }
    else
  {
    k1 = sunit * 2.3488;
    Serial.println(k1);
  } 
    
    Serial.print("k2 : ");
    a = sunit - 15;
  if(a>=10)
  {
    k2 = 10 * 2.9882; 
  }
    else if(a>=0)
  {
    k2 = a * 2.9882;
    Serial.println(k2);
  }
    else
    {
      k2 = 0;
      Serial.println(k2);
    }
  
    Serial.print("k3 : ");
    b = a - 10;
    if(b>=10)
  {
    k3 = 10 * 3.2405;
  Serial.println(k3);  
  }
    else if(b>=0)
  {
    k3 = b * 3.2405;
    Serial.println(k3);
  }
  else
    {
      k3 = 0;
      Serial.println(k3);
    }
  
    Serial.print("k4 : ");
    c = b - 10;
    if(c>=65)
  {
    k4 = 65 * 3.6237;
    Serial.println(k4);
  }
     else if(c>=0)
  {
    k4 = c * 3.6237;
    Serial.println(k4);
  }
  else
    {
      k4 = 0;
      Serial.println(k4);
    }
  
    Serial.print("k5 : ");
    d = c - 65;
  if(d>=50)
  {
    k5 = 50 * 3.7171;
  Serial.println(k5);
  }
     else if(d>=0)
  {
    k5 = d * 3.7171;
    Serial.println(k5);
  }
  else
    {
      k5 = 0;
      Serial.println(k5);
    }
    
    Serial.print("k6 : ");
    e = d - 50;
  if(e>=250)
  {
    k6 = 250 * 4.2218;
    Serial.println(k6);
  }
     else if(e>=0)
  {
    k6 = e * 4.2218;
    Serial.println(k6);
  }
  else
    {
      k6 = 0;
      Serial.println(k6);
    }
   
    Serial.print("k7 : ");
    f = e - 250;
  if(f>=0)
  {
    k7 = f * 4.4217;
    Serial.println(k7);
  }
     else
  {
    k7 = 0;
    Serial.println(k7);
  }
  
    Serial.print("s1 :");
  s1 = k1 + k2 + k3 + k4 + k5 + k6 + k7;
  Serial.println(s1);
  
    Serial.print("sum1 :");
  sum1 = k1 + k2 + k3 + k4 + k5 + k6 + k7 - ft;
  Serial.println(sum1);
  sumb1 = s1+fee1;
  Serial.print("sumb1 :");
  Serial.println(sumb1);
  vat1 = (sum1 + fee1) * 0.07;
  Serial.print("vat1 :");
  Serial.println(vat1);
  Amount1 = sum1+fee1+vat1;
  Serial.print("Amount1 :"); 
  Serial.println(Amount1);
  Serial.println(" ");

  Serial.print("sAmount1 :");
  sAmount1 += Amount1;
  Serial.println(sAmount1);
  Serial.print("ssum1 :");
  ssum1 += sum1;
  Serial.println(ssum1);
  Serial.println();
  
  
  }
  
  
  
else if(sunit>150)
{
  Serial.print("k8 : ");
    k8 = 150 * 3.2484;
    Serial.println(k8);
    
    Serial.print("k9 : ");
    x = sunit - 150;
if(x>=250)
  {
    k9 = 250 * 4.2218;
    Serial.println(k9);
  }
     else if(x>=0)
  {
    k9 = x * 4.2218;
    Serial.println(k9);
  }
  else
    {
      k9 = 0;
      Serial.println(k9);
    }
  
    Serial.print("k10 : ");
    y = x - 250;
    if(y>=0)
  {
    k10 = y * 4.4217;
    Serial.println(k10);
  }
     else
  {
    k10 = 0;
    Serial.println(k10);
  }
    
    Serial.print("s2 :");
  s2 = k8 + k9 + k10;
  Serial.println(s2);
  
    Serial.print("sum2 :");
  sum2 = k8 + k9 + k10 - ft;
  Serial.println(sum2);
  sumb2 = s2+fee2;
  Serial.print("sumb2 :");
  Serial.println(sumb2);
  vat2 = (sum2 + fee2) * 0.07;
  Serial.print("vat2 :");
  Serial.println(vat2);
  Amount2 = sum2+fee2+vat2;
  Serial.print("Amount2 :");
  Serial.println(Amount2);
  Serial.println(" ");

   Serial.print("sAmount2 :");
   sAmount2 += Amount2;
   Serial.println(sAmount2);
   Serial.print("ssum2 :");
   ssum2 += sum2;    
   Serial.println(ssum2);
   Serial.println();

}

  j++;
  Serial.print("Count :");
  Serial.println(j);
  k = ((j*15)/3600);
  Serial.print("Total Time hr :");
  Serial.println(k);
  Serial.println(" ");
  
  String state1=String(sum1);

  String state2=String(ssum1);

  String state3=String(sum2);

  String state4=String(ssum2);

//  String state1=String(Amount1);
//  
//  String state2=String(sAmount1);
//  
//  String state3=String(Amount2);
//  
//  String state4=String(sAmount2);

  String state5=String(unit);

  String state6=String(sunit);

  String state7=String(j);

  String state8=String(k);
  

//  String state1=String(Irms);
  
  
//  String state2=String(Irms2);
//  
  
//  String state3=String(Irms3);
//  
//  
//  String state4=String(Irms4);
//
//  
//  String state5=String(Irms5);
//  
//  
//  String state6=String(Irms6);
  
  // TCP connection
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += "184.106.153.149"; // api.thingspeak.com
  cmd += "\",80";
  ser.println(cmd);
  Serial.println(cmd);
   
  if(ser.find("Error")){
    Serial.println("AT+CIPSTART error");
    return;
  }
  
  // prepare GET string
  String getStr = "GET /update?api_key=";
  getStr += apiKey;
  /*if(i==0)
  {
    Serial.println("1");
  getStr +="&field1=";
  getStr += String(state1);
  i++;
  }
  else if(i==1)
  {Serial.println("2");
  getStr +="&field2=";
  getStr += String(state2);
  i++;
  }
  else if(i==2)
  {
    Serial.println("3");
  getStr +="&field3=";
  getStr += String(state3);
  i++;
  }
  else if(i==3)
  {
    Serial.println("4");
  getStr +="&field4=";
  getStr += String(state4);
  i=0;
  }
  
  */
  
  if(sunit>=0 && sunit<=150)
{
  getStr +="&field1=";
  getStr += String(state1);
  getStr +="&field2=";
  getStr += String(state2);
  getStr +="&field5=";
  getStr += String(state5);
  getStr +="&field6=";
  getStr += String(state6);
  getStr +="&field7=";
  getStr += String(state7);
  getStr +="&field8=";
  getStr += String(state8);
  getStr += "\r\n\r\n";
}
else if(sunit>150)
{
  getStr +="&field3=";
  getStr += String(state3);
  getStr +="&field4=";
  getStr += String(state4);
  getStr +="&field5=";
  getStr += String(state5);
  getStr +="&field6=";
  getStr += String(state6);
  getStr +="&field7=";
  getStr += String(state7);
  getStr +="&field8=";
  getStr += String(state8);
  getStr += "\r\n\r\n";
}


  // send data length
  cmd = "AT+CIPSEND=";
  cmd += String(getStr.length());
  ser.println(cmd);
  Serial.println(cmd);

  if(ser.find(">")){
    ser.print(getStr);
    Serial.print(getStr);
  }
  else{
    ser.println("AT+CIPCLOSE");
    // alert user
    Serial.println("AT+CIPCLOSE");
  }


      
  // thingspeak needs 15 sec delay between updates
  delay(15000);  
}
