// Date and time functions using just software, based on millis() & timer

#include <Arduino.h>
#include <Wire.h>         // this #include still required because the RTClib depends on it
#include "RTClib.h"
#include <SPI.h>
#include <SD.h>
#include <SparkFun_MMA8452Q.h> // Includes the SFE_MMA8452Q library



// On the Ethernet Shield, CS is pin 4. Note that even if it's not
// used as the CS pin, the hardware CS pin (10 on most Arduino boards,
// 53 on the Mega) must be left as an output or the SD library
// functions will not work.
const int chipSelect = 4;

File dataFile;


// Begin using the library by creating an instance of the MMA8452Q
//  class. We'll call it "accel". That's what we'll reference from
//  here on out.
MMA8452Q accel;

float ax;
float last_ax;
float d_ax;

float ay;
float last_ay;
float d_ay;

float az;
float last_az;
float d_az;

float threshold = 25;

int loopcount = 0;
int loops2ave = 10;



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



  Serial.println("----- TinyWPA Vibration Sensor ------");
  Serial.println("Based Upon: MMA8452Q Test Code!");
  
  // Choose your adventure! There are a few options when it comes
  // to initializing the MMA8452Q:
  //  1. Default init. This will set the accelerometer up
  //     with a full-scale range of +/-2g, and an output data rate
  //     of 800 Hz (fastest).
  //accel.init();
  //  2. Initialize with FULL-SCALE setting. You can set the scale
  //     using either SCALE_2G, SCALE_4G, or SCALE_8G as the value.
  //     That'll set the scale to +/-2g, 4g, or 8g respectively.
  //accel.init(SCALE_4G); // Uncomment this out if you'd like
  //  3. Initialize with FULL-SCALE and DATA RATE setting. If you
  //     want control over how fast your accelerometer produces
  //     data use one of the following options in the second param:
  //     ODR_800, ODR_400, ODR_200, ODR_100, ODR_50, ODR_12,
  //     ODR_6, or ODR_1. 
  //     Sets to 800, 400, 200, 100, 50, 12.5, 6.25, or 1.56 Hz.
  accel.init(SCALE_2G, ODR_800);

  //Initi loop varibles
  ax = 0;
  last_ax = 0;
  d_ax = 0;
  ay = 0;
  last_ay = 0;
  d_ay = 0;
  az = 0;
  last_az = 0;
  d_az = 0;




    
}





void loop () {

    
//    //Serial.println();
//    delay(3000);

  // Use the accel.available() function to wait for new data
  //  from the accelerometer.
  if (accel.available())
  {
    // First, use accel.read() to read the new variables:
    accel.read();

    ax = accel.x;
    ay = accel.y;
    az = accel.z;
    
    d_ax = abs(ax - last_ax);
    d_ay = abs(ay - last_ay);
    d_az = abs(az - last_az);

    if (d_ax > threshold || d_ay > threshold || d_az > threshold) {
          // make a string for assembling the data to log:
      String dataString = "";
      DateTime now = rtc.now();
      
      //dataString += String(loopcount);
      //dataString += String(',');
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
      dataString += String(',');
      dataString += String(d_ax,0);
      dataString += String(',');
      dataString += String(d_ay,0);
      dataString += String(',');
      dataString += String(d_az,0);

      //Write the serial screen
      Serial.println(dataString);

      //Write to the data file
      dataFile.println(dataString);
      dataFile.flush();

      //Increment the loop counter
      loopcount++;

      //Wait 
      delay(1000);
    };


    //Store current accells to last accells
    last_ax = ax;
    last_ay = ay;
    last_az = az;
  }
}




