/* 
 *  LDR Lux Meter
 *  https://www.allaboutcircuits.com/projects/design-a-luxmeter-using-a-light-dependent-resistor/
 *  
 *  This sketch calculates the lux from a voltage reading
 *  
 *  The hardware connected to Analog Pin 0 should be a voltage divider circuit between an LDR and a 
 *  resistor (nominally 5kohm).  The resistor should be connected to ground and the LDR should be
 *  connected to 5V.  The point in between should be connected to Analog Pin 0.
 *  1) Calculate voltage based on reading
 *  2) Calculate resistance of LDR based on voltage
 *  3) Calculate the lux that must be falling on LDR based on the resistance
 *  
 *  The lux reading is then output to an LCD shield.
 *  Created Dec. 4, 2015
 *  By David Williams
 *  
 *  Portions modified 2017-06-15 
 *  by Ron KIerstead
 * 
*/


#include <Wire.h>
////  I2C and LCD Shield libraries
#include <LiquidCrystal_I2C.h>

// The LCD I2C uses the I2C SCL and SDA pins. On classic Arduinos
// this is Analog 4 and 5 so you can't use those for analogRead() anymore
// However, you can connect other I2C sensors to the I2C bus and share
// the I2C bus.


//// These constants, define values needed for the LDR readings and ADC
#define LDR_PIN                   A0
#define MAX_ADC_READING           1023
#define ADC_REF_VOLTAGE           5.0 //Original Value 5.0
#define REF_RESISTANCE            4600  // measure 5k resistor for best results org value 5000
#define LUX_CALC_SCALAR           12518931
#define LUX_CALC_EXPONENT         -1.405

//set the LCD address to 0x27 
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  


void setup() {
  lcd.begin(20,4); // initialize the lcd for 20 chars 4 lines, turn on backlight 
  lcd.backlight();
  lcd.setCursor(0,0); //start at character 1 on line 0
  Serial.begin(9600);
  lcd.print(" Light Sensor Test");
  Serial.println(F("Light Sensor Test"));
  delay(3000);
  lcd.clear();



  Serial.print("Raw Data");
  Serial.print(",");
  Serial.print("LDR Voltage");
  Serial.print(",");
  Serial.print("LDR Resistance");
  Serial.print(",");
  Serial.println("LDR Lux");

}

void loop() 
{  
  
  int   ldrRawData;
  float resistorVoltage, ldrVoltage;
  float ldrResistance;
  float ldrLux;
  
  // Perform the analog to digital conversion  
  ldrRawData = analogRead(LDR_PIN);
  
  // RESISTOR VOLTAGE_CONVERSION
  // Convert the raw digital data back to the voltage that was measured on the analog pin
  resistorVoltage = (float)ldrRawData / MAX_ADC_READING * ADC_REF_VOLTAGE;

  // voltage across the LDR is the 5V supply minus the 5k resistor voltage
  ldrVoltage = ADC_REF_VOLTAGE - resistorVoltage;
  
  // LDR_RESISTANCE_CONVERSION
  // resistance that the LDR would have for that voltage  
  ldrResistance = ldrVoltage/resistorVoltage * REF_RESISTANCE;
  
  // LDR_LUX
  // Change the code below to the proper conversion from ldrResistance to
  // ldrLux
  ldrLux = LUX_CALC_SCALAR * pow(ldrResistance, LUX_CALC_EXPONENT);
 
  // print out the results in comma seperated value CSV to import into a spreadsheet
  Serial.print(ldrRawData);
  Serial.print(",");
  Serial.print(ldrVoltage);
  Serial.print(",");
  Serial.print(ldrResistance);
  Serial.print(",");
  Serial.println(ldrLux);

  lcd.setCursor(0,0); //start at character 1 on line 0
  lcd.print("Raw    : "); lcd.print(ldrRawData);
  lcd.setCursor(0,1);
  lcd.print("LDR V  : "); lcd.print(ldrVoltage); lcd.print("  Volts");
  lcd.setCursor(0,2);
  lcd.print("LDR R  : "); lcd.print(ldrResistance); lcd.print(" Ohms");
  lcd.setCursor(0,3);
  lcd.print("LDR Lux: "); lcd.print(ldrLux); lcd.print(" lux");
  
  delay(1000);
}
