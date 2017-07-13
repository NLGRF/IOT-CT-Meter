char inData[25]; // Allocate some space for the string
char inChar = -1; // Where to store the character read
byte index = 0; // Index into array; where to store the character
String SetMode, sMode, nTag;
void SetDATA() {
  getString();

  if (index > 0)
  {
    SetMode = String(inData[0]) + String(inData[1]) + String(inData[2]);
    //nTag = String(inData[4]) + String(inData[5]);
    //int nt = 10 * String(inData[4]).toInt() + String(inData[5]).toInt();
    //int hr = 10 * String(inData[7]).toInt() + String(inData[8]).toInt();
    //int mn = 10 * String(inData[10]).toInt() + String(inData[11]).toInt();
    //int lt = 10 * String(inData[13]).toInt() + String(inData[14]).toInt();
    if (SetMode == "stt" && index == 26)
    {
      index = 0;
      int yy = 1000 * String(inData[4]).toInt() + 100 * String(inData[5]).toInt() + 10 * String(inData[6]).toInt() + String(inData[7]).toInt();
      int mm = 10 * String(inData[9]).toInt() + String(inData[10]).toInt();
      int dd = 10 * String(inData[12]).toInt() + String(inData[13]).toInt();
      int wk = 10 * String(inData[15]).toInt() + String(inData[16]).toInt();
      int HH = 10 * String(inData[18]).toInt() + String(inData[19]).toInt();
      int MM = 10 * String(inData[21]).toInt() + String(inData[22]).toInt();
      int SS = 10 * String(inData[24]).toInt() + String(inData[25]).toInt();
      SetClock(yy % 1000, mm, dd, wk, HH, MM, SS);
      Serial.println("SET TIME OK");
    }
    else
    {
      index = 0;
      Serial.println();
      Serial.println("Error Enter Again");
      Serial.println();
    }
  }
}

void getString() {
  //while ((Serial.available() < 1));
  while ((Serial.available() > 0)) {
    delay(5);
    inChar = Serial.read(); // Read a character
    inData[index] = inChar; // Store it
    index++; // Increment where to write next
    inData[index] = '\0'; // Null terminate the string
  }
}
