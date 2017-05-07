// Date and time functions using just software, based on millis() & timer

#include <Arduino.h>
#include <Wire.h>         // this #include still required because the RTClib depends on it
#include "RTClib.h"
#include <SPI.h>
#include <SD.h>



// On the Ethernet Shield, CS is pin 4. Note that even if it's not
// used as the CS pin, the hardware CS pin (10 on most Arduino boards,
// 53 on the Mega) must be left as an output or the SD library
// functions will not work.
const int chipSelect = 4;

File dataFile;



#if defined(ARDUINO_ARCH_SAMD)
// for Zero, output on USB Serial console, remove line below if using programming port to program the Zero!
   #define Serial SerialUSB
#endif

RTC_Millis rtc;

void setup () {
    Serial.begin(115200);
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.begin(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));




  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }


  Serial.print("Initializing SD card...");
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(SS, OUTPUT);
  
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1) ;
  }
  Serial.println("card initialized.");
  
  // Open up the file we're going to log to!
  dataFile = SD.open("datalog.txt", FILE_WRITE);
  if (! dataFile) {
    Serial.println("error opening datalog.txt");
    // Wait forever since we cant write data
    while (1) ;
  }



    
}





void loop () {
    // make a string for assembling the data to log:
    String dataString = "";
    DateTime now = rtc.now();
    
    dataString += String(now.year(), DEC);
    dataString += String('/');
    dataString += String(now.month(), DEC);
    dataString += String('/');
    dataString += String(now.day(), DEC);
    dataString += String(' ');
    dataString += String(now.hour(), DEC);
    dataString += String(':');
    dataString += String(now.minute(), DEC);
    dataString += String(':');
    dataString += String(now.second(), DEC);
    Serial.println(dataString);
    
    dataFile.println(dataString);
    dataFile.flush();
    
    //Serial.println();
    delay(3000);
}
