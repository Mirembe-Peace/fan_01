#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>


#define DHTPIN 2
#define DHTTYPE DHT11

  //instaciate a DHT sensor instance
DHT dht(DHTPIN, DHTTYPE);

//initialize all the pins
const int potPin = A0;
const int fanPin_1 = 3;
const int fanPin_2 = 5;


                  //address, columns, rows
                  //instaciate an LCD instance

LiquidCrystal_I2C lcd(0x27, 16, 2); // Set the LCD address and dimensions


void setup() {
  Serial.begin(9600); // Initialize Serial for debugging
  dht.begin();// initialize the DHT sensor
  pinMode(fanPin_1, OUTPUT);
  pinMode(fanPin_2, OUTPUT);// set those pins as output pins
  lcd.init();                      // Initialize the LCD
  lcd.backlight();                 // Turn on the backlight
  lcd.setCursor(0, 0);              // set cursor to 1st row 1st column
  lcd.print("Temp Fan Control");   // displaying the name of my project
  lcd.setCursor(0, 1);            // set the cursor to the next row
lcd.print("by:sMirembe Peace");   // prints my name as owner of the project
  Serial.println("Setup complete"); // Debug statement
  delay(2000); // waits for 2 seconds as you read the message
  lcd.clear(); // clears the LCD for the next statement
}


void loop() {
                                      
  int threshold = map(analogRead(potPin), 0, 1023, 13, 29); // Map potentiometer value 0-1023 to temperature range 13-20


  float temperature = dht.readTemperature();//reads the the temp from the DHT sensor 


  // Check if the DHT sensor reading failed
  if (isnan(temperature)) {   // Check if the DHT sensor reading valid, nan, not a real number
    Serial.println("Failed to read from DHT sensor!"); //if so it diplays this message
    lcd.clear();//then clears the screen
    lcd.setCursor(0, 0);// sets it back to first position
    lcd.print("Sensor Error!");//prints sensor error
    delay(1000); // waits for one second
    return; // Skip the rest of the loop because the reading did not happen
  }


  // Control the fan speed using PWM based on temperature
  int fanSpeed = 0; // Default fan speed (off)
  if (temperature > threshold) {
    fanSpeed = map(temperature, threshold, 13, 50, 255); // Map the temperature to a PWM value (0 to 255) and the threshold is 29 degrees celcius
    fanSpeed = constrain(fanSpeed, 0, 255); // Ensure fanSpeed is within 0-255
  }


  // Set the fan speed duty cycles using PWM
  analogWrite(fanPin_1, fanSpeed);
  analogWrite(fanPin_2, fanSpeed);


  // Display on LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temperature);
  lcd.print("C");


  lcd.setCursor(0, 1);
  lcd.print("Fan Speed: ");
  lcd.print(map(fanSpeed, 0, 255, 0, 100)); // Display fan speed as a percentage
  lcd.print("%");


  // Display on Serial Monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print("C | Threshold: ");
  Serial.print(threshold);
  Serial.print("C | Fan Speed: ");
  Serial.print(map(fanSpeed, 0, 255, 0, 100)); // Display fan speed as a percentage
  Serial.println("%");


  delay(1000); // Wait 1 second before the next reading
}

