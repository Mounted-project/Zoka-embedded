
#include <Arduino.h>
#include "MCP23017.h"
#include "bq24296m.h"
#include "../lib/MAX17048/MAX17048.h"
#include <Adafruit_VCNL4040.h>
#include <LIS2DW12Sensor.h>

#include <Wire.h>
#include "pins.hpp"



bool already_configured = false;
void scanI2C();

void setup()
{
  pinMode(ESP_POWER_EN, OUTPUT);
  digitalWrite(ESP_POWER_EN, HIGH);
  Serial.begin(115200);


  Wire.begin(I2C_SDA, I2C_SCL, 100000);
  MCP23017 MCP = MCP23017 (MCP23017_ADDR, Wire);
  BQ24296M bq24296m = BQ24296M (BQ24296_ADDR, Wire);
  MAX17048 max17048 = MAX17048 (0x36, Wire);
  // Adafruit_VCNL4040 vcnl4040 = Adafruit_VCNL4040();
//  
  // MAX17048.attatch(&i2c);
  // Serial.println("\nI2C Scanner");
  // Serial.println("\nI2C Scanner 1");
  // scanI2C();
  MCP.init();
  // Serial.println("\nI2C Scanner 2");
  // scanI2C();
  MCP.pinMode(BQ_CHARGE_STATUS,1);
  MCP.pinMode(BQ_PGOOD,1);
  
  MCP.pinMode(RGB_LED_R,0);
  MCP.pinMode(RGB_LED_G,0);
  MCP.pinMode(RGB_LED_B,0);
  MCP.pinMode(LEDS_ENABLE,0);

  MCP.pinMode(ENABLE_1_8V,0);
  MCP.pinMode(ENABLE_10V,0);
  MCP.pinMode(RGB_LCD_LSB,0);

  MCP.pinMode(BUTTON_ENCODER,1);
  MCP.pinMode(BUTTON1,1);
  MCP.pinMode(BUTTON2,1);

  MCP.digitalWrite(ENABLE_1_8V,0);
  MCP.digitalWrite(ENABLE_10V,0);
  MCP.digitalWrite(RGB_LCD_LSB,0);
  MCP.digitalWrite(RGB_LED_R,1);
  MCP.digitalWrite(RGB_LED_G,1);
  MCP.digitalWrite(RGB_LED_B,1);

  MCP.digitalWrite(LEDS_ENABLE,0);
  MCP.digitalWrite(ENABLE_10V,1);
  delay(5000);
  bool charge_enable = true; 
  bq24296m.setWatchDogTimerStatus(WATCHDOG_DISABLE);
  bq24296m.setInputControl(VINDPM_3880mV_OFFSET,IINLIM_500mA);
  Serial.print(" Set Charge Enable: "); Serial.println(charge_enable, HEX); 
  bq24296m.setChargeEnable(charge_enable); 

  // while(1){}
  while(1){
    delay(500);
    Serial.println("Device : BQ2429x");
    bq24296m.setWatchDogTimerStatus(WATCHDOG_DISABLE);
    SYSTEM_STATUS_REG status; 
    status.raw = bq24296m.getStatus(); 
    Serial.print(" VBUS_STAT : 0x"); Serial.println(status.vbus_stat, HEX); 
    Serial.print(" CHRG_STAT : 0x"); Serial.println(status.chrg_stat, HEX); 
    Serial.print(" DPM_STAT  : 0x"); Serial.println(status.chrg_stat, HEX); 
    Serial.print(" PG_STAT   : 0x"); Serial.println(status.pg_stat, HEX); 
    Serial.print(" THERM_STAT: 0x"); Serial.println(status.therm_stat, HEX); 
    Serial.print(" VSYS_STAT : 0x"); Serial.println(status.vsys_stat, HEX); 
    Serial.println("\n");

    NEW_FAULT_REG fault; 
    fault.raw = bq24296m.getFaults(); 
    Serial.print(" NTC_FAULT      : 0x"); Serial.println(fault.ntc_fault, HEX); 
    Serial.print(" RSV_FAULT      : 0x"); Serial.println(fault.rsv, HEX); 
    Serial.print(" BAT_FAULT      : 0x"); Serial.println(fault.bat_fault, HEX); 
    Serial.print(" CHRG_FAULT     : 0x"); Serial.println(fault.chrg_fault, HEX); 
    Serial.print(" OTG_FAULT      : 0x"); Serial.println(fault.otg_fault, HEX); 
    Serial.print(" WATCHDOG_FAULT : 0x"); Serial.println(fault.watchdog_fault, HEX); 
    delay(1000); 

    Serial.println("Device : MAX17048");
    Serial.print("VCELL ADC : ");
    Serial.println(max17048.adc());
    Serial.print("VCELL V   : ");
    Serial.println(max17048.voltage());
    Serial.print("VCELL SOC : ");
    Serial.print(max17048.percent());
    Serial.println(" \%");
    Serial.print("VCELL SOC : ");
    Serial.print(max17048.accuratePercent());
    Serial.println(" \%");
    Serial.println();
    delay(1000);
    }
    
}

void loop()
{

  // scanI2C();






  // Serial.println("Device : VCNL4040");
  // if(already_configured == false){
  //   Serial.println("Getting VCNL4040 parameters : ");
  //   Serial.print("Proximity LED current set to: ");
  //   switch(vcnl4040.getProximityLEDCurrent()) {
  //     case VCNL4040_LED_CURRENT_50MA: Serial.println("50 mA"); break;
  //     case VCNL4040_LED_CURRENT_75MA: Serial.println("75 mA"); break;
  //     case VCNL4040_LED_CURRENT_100MA: Serial.println("100 mA"); break;
  //     case VCNL4040_LED_CURRENT_120MA: Serial.println("120 mA"); break;
  //     case VCNL4040_LED_CURRENT_140MA: Serial.println("140 mA"); break;
  //     case VCNL4040_LED_CURRENT_160MA: Serial.println("160 mA"); break;
  //     case VCNL4040_LED_CURRENT_180MA: Serial.println("180 mA"); break;
  //     case VCNL4040_LED_CURRENT_200MA: Serial.println("200 mA"); break;
  //   }
  //   //vcnl4040.setProximityLEDDutyCycle(VCNL4040_LED_DUTY_1_40);
  //   Serial.print("Proximity LED duty cycle set to: ");
  //   switch(vcnl4040.getProximityLEDDutyCycle()) {
  //     case VCNL4040_LED_DUTY_1_40: Serial.println("1/40"); break;
  //     case VCNL4040_LED_DUTY_1_80: Serial.println("1/80"); break;
  //     case VCNL4040_LED_DUTY_1_160: Serial.println("1/160"); break;
  //     case VCNL4040_LED_DUTY_1_320: Serial.println("1/320"); break;
  //   }

  //   //vcnl4040.setAmbientIntegrationTime(VCNL4040_AMBIENT_INTEGRATION_TIME_80MS);
  //   Serial.print("Ambient light integration time set to: ");
  //   switch(vcnl4040.getAmbientIntegrationTime()) {
  //     case VCNL4040_AMBIENT_INTEGRATION_TIME_80MS: Serial.println("80 ms"); break;
  //     case VCNL4040_AMBIENT_INTEGRATION_TIME_160MS: Serial.println("160 ms"); break;
  //     case VCNL4040_AMBIENT_INTEGRATION_TIME_320MS: Serial.println("320 ms"); break;
  //     case VCNL4040_AMBIENT_INTEGRATION_TIME_640MS: Serial.println("640 ms"); break;
  //   }


  //   //vcnl4040.setProximityIntegrationTime(VCNL4040_PROXIMITY_INTEGRATION_TIME_8T);
  //   Serial.print("Proximity integration time set to: ");
  //   switch(vcnl4040.getProximityIntegrationTime()) {
  //     case VCNL4040_PROXIMITY_INTEGRATION_TIME_1T: Serial.println("1T"); break;
  //     case VCNL4040_PROXIMITY_INTEGRATION_TIME_1_5T: Serial.println("1.5T"); break;
  //     case VCNL4040_PROXIMITY_INTEGRATION_TIME_2T: Serial.println("2T"); break;
  //     case VCNL4040_PROXIMITY_INTEGRATION_TIME_2_5T: Serial.println("2.5T"); break;
  //     case VCNL4040_PROXIMITY_INTEGRATION_TIME_3T: Serial.println("3T"); break;
  //     case VCNL4040_PROXIMITY_INTEGRATION_TIME_3_5T: Serial.println("3.5T"); break;
  //     case VCNL4040_PROXIMITY_INTEGRATION_TIME_4T: Serial.println("4T"); break;
  //     case VCNL4040_PROXIMITY_INTEGRATION_TIME_8T: Serial.println("8T"); break;
  //   }
  //     //vcnl4040.setProximityHighResolution(false);
  //   Serial.print("Proximity measurement high resolution? ");
  //   Serial.println(vcnl4040.getProximityHighResolution() ? "True" : "False");
  //   already_configured = true;
  // }

  // Serial.print("Proximity:"); Serial.println(vcnl4040.getProximity());
  // Serial.print("Ambient light:"); Serial.println(vcnl4040.getLux());
  // Serial.print("Raw white light:"); Serial.println(vcnl4040.getWhiteLight());


  // Serial.println("");
  // Serial.println("Device : LIS2DW12");
  // LIS2DW12Sensor Accelero(&Wire);
  // Accelero.begin();
  // Accelero.Enable_X();
  // int32_t accelerometer[3];
  // Accelero.Get_X_Axes(accelerometer); 
  // Serial.print("X Axis:"); Serial.println(String(accelerometer[0]));
  // Serial.print("Y Axis:"); Serial.println(String(accelerometer[1]));
  // Serial.print("Z Axis:"); Serial.println(String(accelerometer[2]));
}

void scanI2C() {
  byte error, address;
  int devices = 0;

  Serial.println("Scanning...");

  for (address = 8; address < 127; ++address) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("Found device at address 0x");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.print(address, HEX);
      Serial.println(" !");
      devices++;
    } else if (error == 4) {
      Serial.print("Unknown error at address 0x");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.println(address, HEX);
    }
  }

  if (devices == 0) {
    Serial.println("No I2C devices found");
  } else {
    Serial.println("Scan complete");
  }
}