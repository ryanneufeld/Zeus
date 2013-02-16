#include "DHT.h"
#include "SPI.h"
#include "Ethernet.h"
#include "WebServer.h"

// no-cost stream operator as described at 
// http://sundial.org/arduino/?page_id=119
template<class T>
inline Print &operator <<(Print &obj, T arg)
{ obj.print(arg); return obj; }

static uint8_t MAC[] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 };

#define PORT    80
#define PREFIX  ""
#define DHTTYPE DHT21 /* DHT 21 (AM2301) */

DHT dht1(2, DHTTYPE);
DHT dht2(3, DHTTYPE);


WebServer webserver(PREFIX, PORT);

float readTemperature(DHT dht, int sensorNum) {
  float temperature = dht.readTemperature();
  float calibratedTemperature = 0.0;

  /* calibrate temperature sensors */
  if (sensorNum == 1) {
    calibratedTemperature = (temperature-0.8);
  }
  else if (sensorNum == 2){
    calibratedTemperature = (temperature-0.5);
  }

  return calibratedTemperature;
}

float readHumidity(DHT dht, int sensorNum) {
  float humidity = dht.readHumidity();
  float calibratedHumidity = 0.0;

  /* calibrate humidity sensors */
  if (sensorNum == 1) {
    calibratedHumidity = (humidity+0.5);
  }
  else if (sensorNum == 2){
    calibratedHumidity = (humidity+1.2);
  }

  return humidity;
}

void sensorCmd(WebServer &server, WebServer::ConnectionType type)
{
  P(htmlHead) =
    "<html>"
    "<head>"
    "<title>Arduino Web Server</title>"
    "<style type=\"text/css\">"
    "BODY { font-family: Tahoma }"
    "H1 { font-size: 14pt; text-decoration: underline }"
    "P  { font-size: 10pt; }"
    "</style>"
    "<meta http-equiv=\"refresh\" content=\"10\">"
    "</head>"
    "<body>";

   P(htmlFoot) = 
     "</body>"
     "</html>";

  server.httpSuccess();
  server.printP(htmlHead);

  float temp1   = readTemperature(dht1,1);
  float temp2   = readTemperature(dht2,2);
  float humid1  = readHumidity(dht1,1);
  float humid2  = readHumidity(dht2,2);

  server << "<h1>Sensor:</h1><ul>";
  server << "<li>temperature 1: " << temp1 << "</li>";
  server << "<li>humidity 1: " << humid1 << "</li>";
  server << "<li>temperature 2: " << temp2 << "</li>";
  server << "<li>humidity 2: " << humid2 << "</li>";
  server << "</ul>";

  server.printP(htmlFoot);
}


void defaultCmd(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{
  sensorCmd(server, type);  
}

void setup()
{
  Ethernet.begin(MAC);
  webserver.begin();

  webserver.setDefaultCommand(&defaultCmd);
  webserver.addCommand("index.html", &defaultCmd);
}

void loop()
{
  // process incoming connections one at a time forever
  webserver.processConnection();

  // if you wanted to do other work based on a connecton, it would go here
}
