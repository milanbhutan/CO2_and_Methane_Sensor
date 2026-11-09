#include <Wire.h>
#include <LiquidCrystal_I2C.h>
//#include "BlueDot_BME280.h"
#include <SoftwareSerial.h>
#define TRIGGER_PIN 9
#define ECHO_PIN 10
#define A0 A0
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

unsigned long start_time = millis();
unsigned long reading_time = 0;
unsigned long time_since_boot = 0;

Adafruit_BME280 bme1; // Sensor 1
Adafruit_BME280 bme2; // Sensor 2

//BlueDot_BME280 bme1;                                     //Object for Sensor 1
//BlueDot_BME280 bme2;                                     //Object for Sensor 2

//int bme1Detected = 0;                                    //Checks if Sensor 1 is available
//int bme2Detected = 0;                                    //Checks if Sensor 2 is available

LiquidCrystal_I2C lcd1(0x27, 16, 2); // I2C address 0x27, 16 column and 2 rows
LiquidCrystal_I2C lcd2(0x26, 16, 2); // I2C address 0x27, 16 column and 2 rows
LiquidCrystal_I2C lcd3(0x25, 16, 2); // I2C address 0x25, 16 column and 2 rows

SoftwareSerial co2_serial(6, 7);
SoftwareSerial methane(12, 13);

void co2_read();
void methane_read();

bool fill_buffer(void);
bool read_co2_value(char command, uint32_t &value);

void format_co2_output(uint32_t raw);
void format_temperature_output(uint32_t raw);
void format_humidity_output(uint32_t raw);
void format_pressure_output(uint32_t raw);

// CO2 sensor
double multiplier = 0.0;
uint8_t buffer[25];
uint8_t ind = 0;

//Methane Sensor
const uint8_t request[] = {0x11, 0x01, 0x01, 0xED};
uint8_t reply[8];

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

  //CO2 Sensor
  co2_serial.begin(9600);
  co2_serial.setTimeout(1000);
  Serial.println("Waiting for CO2 sensor warm-up...");
  delay(2000);

  //Methane Sensor
  methane.begin(9600);
  methane.setTimeout(1000);
  Serial.println("Waiting for methane sensor warm-up...");
  delay(2000);
//   if (!bme1.begin(0x76)) {
//     Serial.println("Could not find BME280 sensor 1 at 0x76!");
//     while (1);
//   }

//   if (!bme2.begin(0x77)) {
//     Serial.println("Could not find BME280 sensor 2 at 0x77!");
//     while (1);
//   }

//   Serial.println("Both BME280 sensors initialized.");



// //Ping
//   pinMode(TRIGGER_PIN, OUTPUT);
//   pinMode(ECHO_PIN, INPUT);


// //LCD
//   lcd1.init(); // initialize the lcd
//   lcd1.backlight();
//   lcd1.clear();                 // clear display
//   lcd1.setCursor(0, 0);         // move cursor to   (0, 0)
//   lcd1.print("hi world :)");
//   delay(2000);
//   lcd1.clear();
//   delay(2000);

//   lcd2.init(); // initialize the lcd
//   lcd2.backlight();
//   lcd2.clear();                 // clear display
//   lcd2.setCursor(0, 0);         // move cursor to   (0, 0)
//   lcd2.print("hi again :)");
//   delay(2000);
//   lcd2.clear();
//   delay(2000);

  lcd3.init(); // initialize the lcd
  lcd3.backlight();
  lcd3.clear();                 // clear display
  lcd3.setCursor(0, 0);         // move cursor to   (0, 0)
  lcd3.print("hi again :)");
  delay(2000);
  lcd3.clear();
  delay(2000);

}
void loop() {
  // put your main code here, to run repeatedly:
 
  // Methane Sensor Reading
  while(1){
  methane.listen();
  methane_read();
 
  // CO2 Sensor Reading
  co2_serial.listen();
  co2_read();
  reading_time = millis();
  time_since_boot = reading_time - start_time;
  time_since_boot = float(time_since_boot)/1000;
  Serial.print("Time Since Boot: ");
  Serial.print(time_since_boot);
  Serial.println(" seconds");
  Serial.println();
  delay(1000);
  }
 


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

  Serial.println("before LCD clear");

  lcd1.clear();                 // clear display
  lcd1.setCursor(0, 0);         // move cursor to   (0, 0)
  lcd1.print("D:");             // print message at (0, 1,)
  lcd1.setCursor(2, 0);         // move cursor to   (2, 0)
  lcd1.print(distance);         // print message at (2, 1)
 
  Serial.println("after LCD clear");
 //BME1
  // float temperature1 = bme1.readTemperature();
  // float humidity1 = bme1.readHumidity();
  // float pressure1 = bme1.readPressure();
  lcd1.setCursor(7, 1);         // move cursor to   (0, 0)
  lcd1.print("P:");        // print message at (0, 0)
  lcd1.setCursor(9, 1);         // move cursor to   (2, 1)
  //lcd1.print(pressure1/100);
  lcd1.setCursor(0, 1);         // move cursor to   (0, 0)
  lcd1.print("H:");        // print message at (0, 0)
  lcd1.setCursor(2, 1);         // move cursor to   (2, 1)
  //lcd1.print(humidity1);
  lcd1.setCursor(9, 0);         // move cursor to   (0, 0)
  lcd1.print("T:");        // print message at (0, 0)
  lcd1.setCursor(11,0);         // move cursor to   (2, 1)
  //lcd1.print(temperature1);




  lcd2.clear();                 // clear display
  lcd2.setCursor(0, 0);         // move cursor to   (0, 0)
  lcd2.print("R:");        // print message at (0, 0)
  lcd2.setCursor(2, 0);         // move cursor to   (2, 0)
  //lcd2.print(R0,2); // print message at (2, 1)*/


  // float temperature2 = bme2.readTemperature();
  // float humidity2 = bme2.readHumidity();
  // float pressure2 = bme2.readPressure();

  lcd2.setCursor(7, 1);         // move cursor to   (0, 0)
  lcd2.print("P:");        // print message at (0, 0)
  lcd2.setCursor(9, 1);         // move cursor to   (2, 1)
  //lcd2.print(pressure2/100);
  lcd2.setCursor(0, 1);         // move cursor to   (0, 0)
  lcd2.print("H:");        // print message at (0, 0)
  lcd2.setCursor(2, 1);         // move cursor to   (2, 1)
  //lcd2.print(humidity2);
  lcd2.setCursor(9, 0);         // move cursor to   (0, 0)
  lcd2.print("T:");        // print message at (0, 0)
  lcd2.setCursor(11, 0);         // move cursor to   (2, 1)
  //lcd2.print(temperature2);

// Read sensor 1
  Serial.print ("Distance to bag surface:");
  Serial.println(distance);
  Serial.println("BME280 out of digester (0x76):");
  Serial.print("Temp: ");
  //Serial.print(bme1.readTemperature());
  Serial.print(" °C, Hum: ");
  //Serial.print(bme1.readHumidity());
  Serial.print(" %, Press: ");
  //Serial.print(bme1.readPressure() / 100.0F);
  Serial.println(" hPa");

  // Read sensor 2
  Serial.println("BME280 with MQ-4 (0x77):");
  Serial.print("Temp: ");
  //Serial.print(bme2.readTemperature());
  Serial.print(" °C, Hum: ");
  //Serial.print(bme2.readHumidity());
  Serial.print(" %, Press: ");
  //Serial.print(bme2.readPressure() / 100.0F);
  Serial.println(" hPa");
  Serial.println("----");
  Serial.print("MQ-4 Sensor Resistance: ");
  //Serial.print(R0);
  Serial.print(" kohms, Sensor Voltage: ");
  //Serial.print(sensor_volt);
  Serial.println(" V");
  Serial.println("-------------------");
  delay(2000); // Wait 2 seconds
}


void methane_read(){
  // Discard any leftover bytes before starting a new request.
  while (methane.available()) methane.read();


  methane.write(request, sizeof(request));  // No CR/LF

  size_t received = methane.readBytes(reply, sizeof(reply));

  // Serial.print("RX: ");
  // for (size_t j = 0; j < received; j++) {
  //   if (reply[j] < 0x10) Serial.print('0');
  //   Serial.print(reply[j], HEX);
  //   Serial.print(' ');
  // }
  // Serial.println();

  if (received != 8) {
    Serial.println("Missing or incomplete response");
  } else {
    uint8_t sum = 0;
    for (uint8_t j = 0; j < 8; j++) sum += reply[j];

    if (reply[0] != 0x16 || reply[1] != 0x05 ||
        reply[2] != 0x01 || sum != 0) {
      Serial.println("Invalid response or checksum");
    } else if (reply[5] != 0) {
      Serial.print("Sensor status flags: 0x");
      Serial.println(reply[5], HEX);
    } else {
      uint16_t count = ((uint16_t)reply[3] << 8) | reply[4];

      // SJH-100A: documented resolution is 0.01% volume.
      Serial.print("Methane: ");
      Serial.print(count / 100.0, 2);
      Serial.println(" % vol");
    }
  }

}

void co2_read() {
  uint32_t raw;

  // Ask the sensor for its CO2-to-ppm multiplier.
  // A returned multiplier code of 0 means 0.1.
  bool have_multiplier = read_co2_value('.', raw);

  if (have_multiplier) {
    multiplier = (raw == 0) ? 0.1 : (double)raw;
  }

  // Z = filtered CO2.
  if (read_co2_value('Z', raw)) {
    if (have_multiplier) {
      format_co2_output(raw);
    } else {
      Serial.println(F("CO2: multiplier read failed"));
    }
  }

  // Lowercase t = onboard temperature sensor.
  if (read_co2_value('t', raw)) {
    format_temperature_output(raw);
  }

  if (read_co2_value('H', raw)) {
    format_humidity_output(raw);
  }

  if (read_co2_value('B', raw)) {
    format_pressure_output(raw);
  }

}


bool read_co2_value(char command, uint32_t &value) {
  // Clear leftover data before requesting another reading.
  while (co2_serial.available()) {
    co2_serial.read();
  }

  co2_serial.println(command);

  if (!fill_buffer()) {
    Serial.print(F("MX200 "));
    Serial.print(command);
    Serial.println(F(": incomplete response or response too long"));
    return false;
  }

  // Expected response: command + space + five digits.
  // Example: H 00351
  if (ind != 7 ||
      buffer[0] != (uint8_t)command ||
      buffer[1] != ' ') {
    Serial.print(F("MX200 "));
    Serial.print(command);
    Serial.println(F(": unexpected response"));
    return false;
  }

  uint32_t parsed = 0;

  for (uint8_t j = 2; j < 7; j++) {
    if (buffer[j] < '0' || buffer[j] > '9') {
      Serial.print(F("MX200 "));
      Serial.print(command);
      Serial.println(F(": invalid numeric response"));
      return false;
    }

    parsed = parsed * 10UL + (buffer[j] - '0');
  }

  value = parsed;
  return true;
}


bool fill_buffer(void) {
  ind = 0;
  buffer[0] = '\0';

  unsigned long started = millis();

  while (millis() - started < 1000UL) {
    if (!co2_serial.available()) {
      continue;
    }

    char received = (char)co2_serial.read();

    if (received == '\r') {
      continue;
    }

    if (received == '\n') {
      if (ind == 0) {
        continue;
      }

      buffer[ind] = '\0';
      return true;
    }

    if (ind >= sizeof(buffer) - 1) {
      return false;
    }

    buffer[ind++] = (uint8_t)received;
  }

  return false;
}


void format_co2_output(uint32_t raw) {
  double ppm = (double)raw * multiplier;

  Serial.print(F("CO2 (filtered): "));
  Serial.print(ppm, multiplier < 1.0 ? 1 : 0);
  Serial.println(F(" ppm"));
}


void format_temperature_output(uint32_t raw) {
  // Convert to signed before subtracting for negative temperatures.
  double temperature_c = ((int32_t)raw - 1000L) / 10.0;

  Serial.print(F("Temperature: "));
  Serial.print(temperature_c, 1);
  Serial.println(F(" C"));
}


void format_humidity_output(uint32_t raw) {
  double humidity_percent = raw / 10.0;

  Serial.print(F("Humidity: "));
  Serial.print(humidity_percent, 1);
  Serial.println(F(" % RH"));
}


void format_pressure_output(uint32_t raw) {
  double pressure_mbar = raw / 10.0;

  Serial.print(F("Pressure: "));
  Serial.print(pressure_mbar, 1);
  Serial.println(F(" mbar"));
}

