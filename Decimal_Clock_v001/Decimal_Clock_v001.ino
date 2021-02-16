// This is a small sketch for a decimal clock 
// by: Florian Ellsäßer 29.01.2021
// please check repository at: 
// if you are looking for more information on decimal time,
// please check: https://en.wikipedia.org/wiki/Decimal_time 
// Happy clock building!

// This script was created for the ESP32 Dev Module
// Please check the wiring diagram at: 
// ***** Include the necessary libraries **************************************
#include <WiFi.h>
#include <WebServer.h>
#include <AutoConnect.h>
#include <Stepper.h>
#include <WiFi.h>
#include <Adafruit_GFX.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

// ***** Define the variables ******************************************
#define NTP_OFFSET  7200 
#define NTP_INTERVAL 60 * 1000   
// change this to your current location
#define NTP_ADDRESS  "de.pool.ntp.org"

// You can define the wifi credentials already here, if they remain 
// empty, the credentials that might be saved on your ESP32 are erased
const char* ssid     = "";
const char* password = "";
// change this to the number of steps on your stepper motor
int SPU = 2038; 
// initialize the light switch pin
int sensorPin = 34;
// set the light switch value to zero
int sensorValue = 0; 
// create the stepmetric variables and set them to zero
int stepmetric_old = 0;
int stepmetric = 0; 
// initialize the time variables
int hour = 0;
int minute = 0;
int second = 0;
// create a decimal variable
float metric = 0;

// initialize ntpUDP and timeClient
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTP_ADDRESS, NTP_OFFSET, NTP_INTERVAL);

// initialize the stepper library on pins 15,2,0,4
Stepper myStepper(SPU, 15,2,5,4);

// ***** Functions ***********************************************************

//create a function to move Motor 1 step
void Move() {
  myStepper.step(-1);
  stepmetric_old = stepmetric;
  Serial.println("Move motor one step forward.");
  delay(1000);
}
// create a move fast function
void MoveFast() {
  myStepper.step(-1);
}
// create a calibration function
void CalibrateClockhand(){
  // move motor to zero position
  // first st the speed of the motor
  myStepper.setSpeed(200);
  // while the switch is not activated, move the motor
  // move the motor until the switch is activated
  while (analogRead(sensorPin)>= 1) {     
    myStepper.step(-10);
    delay(5);
  }
  // inform the attentive Serial Monitor observer
  Serial.println("Clockhand reached 0 position.");
  delay(1000);
}
// create a function to get the current time
void GetCurrentTime(){
  // first get an update of the time function
  timeClient.forceUpdate();
  // get a time string
  String time = timeClient.getFormattedTime();
  // print the time to Serial just in case you have the Serial Monitor on
  Serial.println(time);
  // get the values for each time variable
  hour = time.substring(0,2).toInt();
  minute = time.substring(3,5).toInt();
  second = time.substring(6,8).toInt();
}
// recalculate for decimal time
void RecalculateMetricTime(){
  // runt the GetCurrentTime() function again
  GetCurrentTime();
  // calculate the decimal time from the time variales
  metric = hour/2.4 + minute/1.44/100 + second/0.864/10000;
  // print the metric time to the Serial
  Serial.println(metric);
  // convert decimal time to a number of steps
  stepmetric = metric/10 * SPU;
  delay(1000);
}
// create a Server instance to input your wifi credentials
WebServer Server;
// create a Portal instance using AutoConnect
AutoConnect      Portal(Server);

// This creates a root page if you access the device after it is connected
// to the Wifi
void rootPage() {
  char content[] = "Hi, I'm this Decimal Clock in your Wifi!";
  Server.send(200, "text/plain", content);
}
// start Millis
unsigned long lastMillis;

// ***** start setup ********************************************************
void setup() {
  // give it a short break, then start Serial
  delay(1000);
  Serial.begin(115200);
  // Start the server
  Server.on("/", rootPage);
  if (Portal.begin()) {
    Serial.println("HTTP server:" + WiFi.localIP().toString());
  }
  // set light switch pin mode to input
  pinMode(sensorPin, INPUT);
  // start the timeClient
  timeClient.begin();
  while(!timeClient.update()) {
    timeClient.forceUpdate();
  }
  // set the speed of the motor 
  myStepper.setSpeed(5);
  // startup clockhand calibration
  CalibrateClockhand();
  // figure out current time and step position
  GetCurrentTime();
  // recalculate for decimal time
  RecalculateMetricTime();
  // move the clockhand to the right spot
  for (int i=0; i <= stepmetric; i++){
    MoveFast();
    delay(5);
   }
  // give it a break before starting the loop  
  delay(5000);
}

// ***** start the loop *****************************************************
void loop() {
  // start the Portal's handleClient
  Portal.handleClient();

  // ***** Recalibrate the Clockhand from time to time **********************
  // check millis if it is time to recalibrate-> every 2h (120 min)
  if (millis() - lastMillis >= 120*60*1000UL)
  {   
   //define a new millis for the next calibration cycle
   lastMillis = millis();  
   // startup clockhand calibration:
   CalibrateClockhand();
   // get the current time
   GetCurrentTime();
   // recalculate for decimal time
   RecalculateMetricTime();
   // move the clock in the right position 
   for (int i=0; i <= stepmetric; i++){
      MoveFast();
      delay(5);
   }
  }
  // ***** Run the regular clockhand movement ********************************
  GetCurrentTime();
  RecalculateMetricTime();
  if (stepmetric_old != stepmetric) {
    Move();
    delay(5000);  
 }
}
