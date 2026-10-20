#include <Wire.h>
#include <LiquidCrystal_I2C.h>
//#include "BlueDot_BME280.h"
#include <SoftwareSerial.h>
#define TRIGGER_PIN 9
#define ECHO_PIN 10
#define A0 A0
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

Adafruit_BME280 bme1; // Sensor 1
Adafruit_BME280 bme2; // Sensor 2

//BlueDot_BME280 bme1;                                     //Object for Sensor 1
//BlueDot_BME280 bme2;                                     //Object for Sensor 2

//int bme1Detected = 0;                                    //Checks if Sensor 1 is available
//int bme2Detected = 0;                                    //Checks if Sensor 2 is available

LiquidCrystal_I2C lcd1(0x27, 16, 2); // I2C address 0x27, 16 column and 2 rows
LiquidCrystal_I2C lcd2(0x26, 16, 2); // I2C address 0x27, 16 column and 2 rows


void setup() {
Serial.begin(9600);
/*//BMEs
    //*************BASIC SETUP - READ BEFORE GOING ON!********************* 
  //Choose between Software and Hardware SPI protocol for all sensors

    bme1.parameter.communication = 1;                    //Hardware SPI for Sensor 1 (bme1)
    bme2.parameter.communication = 1;                    //Hardware SPI for Sensor 2 (bme2)

      

  //*********************************************************************
  //*************BASIC SETUP - READ BEFORE GOING ON!*********************
  
  //Set the Chip Select pins for the SPI Communication.
  //Each sensor must have a different Chip Select pin.
  //Connect the Chip Select pins from the sensors to regular digital pins on the Arduino.

    bme1.parameter.SPI_cs = 6;                          //Chip Select Pin for Sensor 1
    bme2.parameter.SPI_cs = 5;                          //Chip Select Pin for Sensor 2


    

  //*********************************************************************
  //*************BASIC SETUP - READ BEFORE GOING ON!*********************
  
  //If you are using Hardware SPI, then the MOSI, MISO and SCK pins are predefined by the Arduino Board (ICSP Header).
  //The ICSP Header looks like this:
  //                ICSP 
  //          MISO |x  x| VCC
  //           SCK |x  x| MOSI
  //        /RESET |x  x| GND
  //Connect the SDO (sensor) to MISO (Arduino).
  //Connect the SDI (sensor) to MOSI (Arduino).
  //Connect the SCK (sensor) to SCK (Arduino). This one was obvious!
  //In this case you can comment this section out.
  
  //But if you are using Software SPI you need to set the pins for the MOSI, MISO and SCK lines.
  //Per default the MOSI line is on the pin 11, the MISO line on the pin 12 and the SCK line on the pin 13.
  //You may change those pins if you like, but remember to connect that pin to all sensors.

  
    bme1.parameter.SPI_mosi = 11;                       //The MOSI signal must be connected to the same pin for all sensors
    bme2.parameter.SPI_mosi = 11;                       //The MOSI signal must be connected to the same pin for all sensors


    bme1.parameter.SPI_miso = 12;                       //The MISO signal must be connected to the same pin for all sensors
    bme2.parameter.SPI_miso = 12;                       //The MISO signal must be connected to the same pin for all sensors


    bme1.parameter.SPI_sck = 13;                       //The SCK signal must be connected to the same pin for all sensors
    bme2.parameter.SPI_sck = 13;                        //The SCK signal must be connected to the same pin for all sensors
 
  
  //*********************************************************************
  //*************ADVANCED SETUP - SAFE TO IGNORE*************************
   
  //Now choose on which mode your device will run
  //On doubt, just leave on normal mode, that's the default value

  //0b00:     In sleep mode no measurements are performed, but power consumption is at a minimum
  //0b01:     In forced mode a single measured is performed and the device returns automatically to sleep mode
  //0b11:     In normal mode the sensor measures continually (default value)
  
    bme1.parameter.sensorMode = 0b11;                    //Setup Sensor mode for Sensor 1
    bme2.parameter.sensorMode = 0b11;                    //Setup Sensor mode for Sensor 2


              
    
  //*********************************************************************
  //*************ADVANCED SETUP - SAFE TO IGNORE*************************
  
  //Great! Now set up the internal IIR Filter
  //The IIR (Infinite Impulse Response) filter suppresses high frequency fluctuations
  //In short, a high factor value means less noise, but measurements are also less responsive
  //You can play with these values and check the results!
  //In doubt just leave on default

  //0b000:      factor 0 (filter off)
  //0b001:      factor 2
  //0b010:      factor 4
  //0b011:      factor 8
  //0b100:      factor 16 (default value)

    bme1.parameter.IIRfilter = 0b100;                   //IIR Filter for Sensor 1
    bme2.parameter.IIRfilter = 0b100;                   //IIR Filter for Sensor 2

    

  //*********************************************************************
  //*************ADVANCED SETUP - SAFE TO IGNORE*************************

  //Next you'll define the oversampling factor for the humidity measurements
  //Again, higher values mean less noise, but slower responses
  //If you don't want to measure humidity, set the oversampling to zero

  //0b000:      factor 0 (Disable humidity measurement)
  //0b001:      factor 1
  //0b010:      factor 2
  //0b011:      factor 4
  //0b100:      factor 8
  //0b101:      factor 16 (default value)

    bme1.parameter.humidOversampling = 0b101;            //Humidity Oversampling for Sensor 1
    bme2.parameter.humidOversampling = 0b101;            //Humidity Oversampling for Sensor 2

    

  //*********************************************************************
  //*************ADVANCED SETUP - SAFE TO IGNORE*************************
  
  //Now define the oversampling factor for the temperature measurements
  //You know now, higher values lead to less noise but slower measurements
  
  //0b000:      factor 0 (Disable temperature measurement)
  //0b001:      factor 1
  //0b010:      factor 2
  //0b011:      factor 4
  //0b100:      factor 8
  //0b101:      factor 16 (default value)

    bme1.parameter.tempOversampling = 0b101;              //Temperature Oversampling for Sensor 1
    bme2.parameter.tempOversampling = 0b101;              //Temperature Oversampling for Sensor 2

    

  //*********************************************************************
  //*************ADVANCED SETUP - SAFE TO IGNORE*************************
  
  //Finally, define the oversampling factor for the pressure measurements
  //For altitude measurements a higher factor provides more stable values
  //On doubt, just leave it on default
  
  //0b000:      factor 0 (Disable pressure measurement)
  //0b001:      factor 1
  //0b010:      factor 2
  //0b011:      factor 4
  //0b100:      factor 8
  //0b101:      factor 16 (default value)  

    bme1.parameter.pressOversampling = 0b101;             //Pressure Oversampling for Sensor 1
    bme2.parameter.pressOversampling = 0b101;             //Pressure Oversampling for Sensor 2

       
  
  //*********************************************************************
  //*************ADVANCED SETUP - SAFE TO IGNORE*************************
  
  //For precise altitude measurements please put in the current pressure corrected for the sea level
  //On doubt, just leave the standard pressure as default (1013.25 hPa);
  
    bme1.parameter.pressureSeaLevel = 1013.25;            //default value of 1013.25 hPa (Sensor 1)
    bme2.parameter.pressureSeaLevel = 1013.25;            //default value of 1013.25 hPa (Sensor 2)


  //Also put in the current average temperature outside (yes, really outside!)
  //For slightly less precise altitude measurements, just leave the standard temperature as default (15°C);
  
    bme1.parameter.tempOutsideCelsius = 15;               //default value of 15°C
    bme2.parameter.tempOutsideCelsius = 15;               //default value of 15°C

    
    bme1.parameter.tempOutsideFahrenheit = 59;            //default value of 59°F
    bme2.parameter.tempOutsideFahrenheit = 59;            //default value of 59°F


  
  //*********************************************************************
  //*************ADVANCED SETUP IS OVER - LET'S CHECK THE CHIP ID!*******
  
  // Initialize Sensor 1
  if (bme1.init() != 0x60)
  {    
    Serial.println(F("Ops! First BME280 Sensor not found!"));
    bme1Detected = 0;
  }
  else
  {
    Serial.println(F("First BME280 Sensor detected!"));
    bme1Detected = 1;
  }

 // Initialize Sensor 2
  if (bme2.init() != 0x60)
  {    
    Serial.println(F("Ops! Second BME280 Sensor not found!"));
    bme2Detected = 0;
  }
  else
  {
    Serial.println(F("Second BME280 Sensor detected!"));
    bme2Detected = 1;
  }
  
  if ((bme1Detected == 0)&(bme2Detected == 0))
  {
    Serial.println();
    Serial.println();
    Serial.println(F("Something is loose"));
    Serial.println();
    
    while(1);
  }
*/
  while (!Serial); // Wait for Serial to be ready

  if (!bme1.begin(0x76)) {
    Serial.println("Could not find BME280 sensor 1 at 0x76!");
    while (1);
  }

  if (!bme2.begin(0x77)) {
    Serial.println("Could not find BME280 sensor 2 at 0x77!");
    while (1);
  }

  Serial.println("Both BME280 sensors initialized.");



//Ping
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
//Methane
  float sensor_volt = 0;
  float RL = 20.0;        // Load resistance in kilo-ohms (10 or 20 kohm)
  float VCC = 5.0;        // Supply voltage of 5V from Arduino
  float RS_air = 0;       // Rs in clean air
  float R0 = 0;           // Baseline Rs (used as R0)
  float sensorValue = 0;

  // --- Averaging 100 analog readings for better stability ---
  for (int x = 0; x < 100; x++) {
    sensorValue += analogRead(A0);
  }
  sensorValue = sensorValue / 100.0;

  // --- Convert ADC value to voltage ---
  sensor_volt = (sensorValue / 1024.0) * VCC;

  // --- Calculate Rs using the voltage divider formula ---
  RS_air = RL * (VCC - sensor_volt) / sensor_volt;

  // --- Save Rs in clean air as R0 ---
  R0 = RS_air; /*part of an old code to use RO. Wanted to keep option*/

  Serial.print("Sensor Voltage: ");
  Serial.print(sensor_volt);
  Serial.println(" V");

  Serial.print("Rs: ");
  Serial.print(R0);
  Serial.println(" kohms");

//LCD
  lcd1.init(); // initialize the lcd
  lcd1.backlight();
  lcd1.clear();                 // clear display
  lcd1.setCursor(0, 0);         // move cursor to   (0, 0)
  lcd1.print("hi world :)");
  delay(2000);
  lcd1.clear();
  delay(2000);

  lcd2.init(); // initialize the lcd
  lcd2.backlight();
  lcd2.clear();                 // clear display
  lcd2.setCursor(0, 0);         // move cursor to   (0, 0)
  lcd2.print("hi again :)");
  delay(2000);
  lcd2.clear();
  delay(2000);
}
void loop() {
  // put your main code here, to run repeatedly:
//Ping
  long duration;
  float distance;

  // Send pulse to trigger pin
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);

  // Read the echo pin
  duration = pulseIn(ECHO_PIN, HIGH);

  // Calculate distance
  distance = (duration / 2.0) * 0.0344; // Distance in cm


  lcd1.clear();                 // clear display
  lcd1.setCursor(0, 0);         // move cursor to   (0, 0)
  lcd1.print("D:");             // print message at (0, 1,)
  lcd1.setCursor(2, 0);         // move cursor to   (2, 0)
  lcd1.print(distance);         // print message at (2, 1)
   
 //BME1
  float temperature1 = bme1.readTemperature();
  float humidity1 = bme1.readHumidity();
  float pressure1 = bme1.readPressure();
  lcd1.setCursor(7, 1);         // move cursor to   (0, 0)
  lcd1.print("P:");        // print message at (0, 0)
  lcd1.setCursor(9, 1);         // move cursor to   (2, 1)
  lcd1.print(pressure1/100);
  lcd1.setCursor(0, 1);         // move cursor to   (0, 0)
  lcd1.print("H:");        // print message at (0, 0)
  lcd1.setCursor(2, 1);         // move cursor to   (2, 1)
  lcd1.print(humidity1);
  lcd1.setCursor(9, 0);         // move cursor to   (0, 0)
  lcd1.print("T:");        // print message at (0, 0)
  lcd1.setCursor(11,0);         // move cursor to   (2, 1)
  lcd1.print(temperature1);


///Methane
  float sensor_volt = 0;
  float RL = 20.0;        // Load resistance in kilo-ohms (10 or 20 kohm)
  float VCC = 5.0;        // Supply voltage of 5V from Arduino
  float RS_air = 0;       // Rs in clean air
  float R0 = 0;           // Baseline Rs (used as R0)
  float sensorValue = 0;
  sensorValue = analogRead(A0);

  // --- Convert ADC value to voltage ---
  sensor_volt = (sensorValue / 1023.0) * VCC;

  // --- Calculate Rs using the voltage divider formula ---
  RS_air = RL * ((VCC - sensor_volt) / sensor_volt);

  // --- Save Rs in clean air as R0 ---
  R0 = RS_air;


  lcd2.clear();                 // clear display
  lcd2.setCursor(0, 0);         // move cursor to   (0, 0)
  lcd2.print("R:");        // print message at (0, 0)
  lcd2.setCursor(2, 0);         // move cursor to   (2, 0)
  lcd2.print(R0,2); // print message at (2, 1)*/


  float temperature2 = bme2.readTemperature();
  float humidity2 = bme2.readHumidity();
  float pressure2 = bme2.readPressure();

  lcd2.setCursor(7, 1);         // move cursor to   (0, 0)
  lcd2.print("P:");        // print message at (0, 0)
  lcd2.setCursor(9, 1);         // move cursor to   (2, 1)
  lcd2.print(pressure2/100);
  lcd2.setCursor(0, 1);         // move cursor to   (0, 0)
  lcd2.print("H:");        // print message at (0, 0)
  lcd2.setCursor(2, 1);         // move cursor to   (2, 1)
  lcd2.print(humidity2);
  lcd2.setCursor(9, 0);         // move cursor to   (0, 0)
  lcd2.print("T:");        // print message at (0, 0)
  lcd2.setCursor(11, 0);         // move cursor to   (2, 1)
  lcd2.print(temperature2);

// Read sensor 1
  Serial.print ("Distance to bag surface:");
  Serial.println(distance);
  Serial.println("BME280 out of digester (0x76):");
  Serial.print("Temp: ");
  Serial.print(bme1.readTemperature());
  Serial.print(" °C, Hum: ");
  Serial.print(bme1.readHumidity());
  Serial.print(" %, Press: ");
  Serial.print(bme1.readPressure() / 100.0F);
  Serial.println(" hPa");

  // Read sensor 2
  Serial.println("BME280 with MQ-4 (0x77):");
  Serial.print("Temp: ");
  Serial.print(bme2.readTemperature());
  Serial.print(" °C, Hum: ");
  Serial.print(bme2.readHumidity());
  Serial.print(" %, Press: ");
  Serial.print(bme2.readPressure() / 100.0F);
  Serial.println(" hPa");
  Serial.println("----");
  Serial.print("MQ-4 Sensor Resistance: ");
  Serial.print(R0);
  Serial.print(" kohms, Sensor Voltage: ");
  Serial.print(sensor_volt);
  Serial.println(" V");
  Serial.println("-------------------");
  delay(2000); // Wait 2 seconds
}


