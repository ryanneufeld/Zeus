#include <DHT.h>
#include <SD.h>

/*
  SD card datalogger

 This example shows how to log data from three analog sensors
 to an SD card using the SD library.

 The circuit:
 * analog sensors on analog ins 0, 1, and 2
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4

 created  24 Nov 2010
 modified 9 Apr 2012
 by Tom Igoe

 This example code is in the public domain.

 */

#define DEBUG
// MUST BE 8.3 FILENAME FORMAT!!
#define LOGFILE "sensor.csv"
//#define DHTTYPE DHT11   // DHT 11 
//#define DHTTYPE DHT22   // DHT 22  (AM2302)
#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Sensor polling time is 5 seconds
#define DELAY 5000

DHT dht1(2, DHTTYPE);
DHT dht2(3, DHTTYPE);

// On the Ethernet Shield, CS is pin 4. Note that even if it's not
// used as the CS pin, the hardware CS pin (10 on most Arduino boards,
// 53 on the Mega) must be left as an output or the SD library
// functions will not work.
const int chipSelect = 4;

void setup()
{
 // Open serial communications and wait for port to open:
  Serial.begin(28800);
  
  while (!Serial); // wait for serial port to connect. Needed for Leonardo only
  
  // Start reading from sensors
  dht1.begin();
  dht2.begin();

  #ifdef DEBUG
  Serial.print("Initializing SD card...");
  #endif /* DEBUG */
  
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(10, OUTPUT);
  
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    // TODO Add LED notification of card failure.
    #ifdef DEBUG
    Serial.println("Card failed, or not present");
    #endif /* DEBUG */
    
    // don't do anything more:
    return;
  }
  
  #ifdef DEBUG
  Serial.println(" done!");
  #endif /* DEBUG */
}

void loop()
{
  // make a string for assembling the data to log:
  String dataString = "";
  String dataString1 = "";
  String dataString2 = "";
  
  // Collect temp an humidity values
  int h1 = (int) (dht1.readHumidity() * 100);
  int t1 = (int) (dht1.readTemperature() * 100);

  // Collect temp an humidity values  
  int h2 = (int) (dht2.readHumidity() * 100);
  int t2 = (int) (dht2.readTemperature() * 100);

  // check if returns are valid, if they are NaN (not a number) then something went wrong!
  if (isnan(t1) || isnan(h1)) {
    #ifdef DEBUG
    Serial.println("Failed to read from DHT1");
    #endif /* DEBUG */
    
    dataString1 += "NaN,NaN";
  } else {
    dataString1 = String(t1) + "," + String(h1);
  }

  // check if returns are valid, if they are NaN (not a number) then something went wrong!
  if (isnan(t2) || isnan(h2)) {
    #ifdef DEBUG
    Serial.println("Failed to read from DHT2");
    #endif /* DEBUG */

    dataString2 += "NaN,NaN";
  } else {
    dataString2 = String(t2) + "," + String(h2);
  }
  
  dataString = dataString1 + "," + dataString2;

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open(LOGFILE, FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
  }
  // if the file isn't open, pop up an error:
  else {
    #ifdef DEBUG
    Serial.println("error opening " + String(LOGFILE));
    #endif /* DEBUG */
  }

  #ifdef DEBUG
  Serial.println(dataString);
  #endif /* DEBUG */
  
  // TODO Replace this with RTC check or possibly config file.
  delay(DELAY);
}









