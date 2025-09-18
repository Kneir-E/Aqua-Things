#ifndef ADS_FUNCTIONS_H
#define ADS_FUNCTIONS_H

#include <ADS1115_WE.h>
#include <Wire.h>

#define __ADS_READ_TIMEOUT 200

#define I2C_SDA 22  // Custom SDA pin
#define I2C_SCL 21  // Custom SCL pin

#define I2C_ADDRESS_1  0x48
#define I2C_ADDRESS_2  0x49

ADS1115_WE __ads_obj0 = ADS1115_WE(I2C_ADDRESS_1);
ADS1115_WE __ads_obj1 = ADS1115_WE(I2C_ADDRESS_2);


bool __ads_obj0_exists = false;
bool __ads_obj1_exists = false;

extern int adsCh0;
extern int adsCh1;
extern int adsCh2;
extern int adsCh3;
extern int adsCh4;
extern int adsCh5;
extern int adsCh6;
extern int adsCh7;

void ads_Init();
int __ads_Read_Ch(int adsNum, ADS1115_MUX channel);
void ads_Read_All();
void __ads_Start_Connect(int adsNum);
void ads_Reconnect();

void ads_Init() {
  // Initialize I2C protocol
  Wire.begin(I2C_SDA, I2C_SCL);

  // Initialize the first ADS1115
  __ads_Start_Connect(0);

  // Initialize the second ADS1115
  __ads_Start_Connect(1);
}

int __ads_Read_Ch(int adsNum, ADS1115_MUX channel) {
  // ADS1115_WE _ads;
  // if (adsNum == 0) _ads = __ads_obj0;
  // else if (adsNum == 1) _ads = __ads_obj1;
  // else return -1;

  int voltage = 0;
  bool is_success = true;
  unsigned long int time_start = millis();

  if(adsNum==0){
    
    __ads_obj0.setCompareChannels(channel);
    __ads_obj0.startSingleMeasurement();
    while (__ads_obj0.isBusy()) {
      if ((millis() - time_start) >= __ADS_READ_TIMEOUT) {
        __ads_obj0_exists = false;
        is_success = false;
        break;
      }
    }
    if (!is_success) {
      return -1;
    } else {
      voltage = __ads_obj0.getResult_mV();
      while(voltage<=0) voltage+=50;
      return voltage;
    }

  }else if(adsNum==1){

    __ads_obj1.setCompareChannels(channel);
    __ads_obj1.startSingleMeasurement();
    while (__ads_obj1.isBusy()) {
      if ((millis() - time_start) >= __ADS_READ_TIMEOUT) {
        __ads_obj1_exists = false;
        is_success = false;
        break;
      }
    }
    if (!is_success) {
      return -1;
    } else {
      voltage = __ads_obj1.getResult_mV();
      while(voltage<=0) voltage+=50;
      return voltage;
    }

  }else{
    return -1;
  }
}

// Read all channels and store to global variables
void ads_Read_All() {
  if (__ads_obj0_exists) {
    if(adsCh0==-1)  adsCh0 =            __ads_Read_Ch(0, ADS1115_COMP_0_GND);
    else            adsCh0 =  (adsCh0 + __ads_Read_Ch(0, ADS1115_COMP_0_GND))/2;
    if(adsCh1==-1)  adsCh1 =            __ads_Read_Ch(0, ADS1115_COMP_1_GND);
    else            adsCh1 =  (adsCh1 + __ads_Read_Ch(0, ADS1115_COMP_1_GND))/2;
    if(adsCh2==-1)  adsCh2 =            __ads_Read_Ch(0, ADS1115_COMP_2_GND);
    else            adsCh2 =  (adsCh2 + __ads_Read_Ch(0, ADS1115_COMP_2_GND))/2;
    if(adsCh3==-1)  adsCh3 =            __ads_Read_Ch(0, ADS1115_COMP_3_GND);
    else            adsCh3 =  (adsCh3 + __ads_Read_Ch(0, ADS1115_COMP_3_GND))/2;
    // adsCh0 += __ads_Read_Ch(0, ADS1115_COMP_0_GND);
    // adsCh1 += __ads_Read_Ch(0, ADS1115_COMP_1_GND);
    // adsCh2 += __ads_Read_Ch(0, ADS1115_COMP_2_GND);
    // adsCh3 += __ads_Read_Ch(0, ADS1115_COMP_3_GND);
    if (__ads_obj0_exists) Serial.printf("Successful read on ADC0-%d-%d-%d-%d\n", adsCh0, adsCh1, adsCh2, adsCh3);
    else if (!__ads_obj0_exists) Serial.printf("No Response from External ADC0!\n");
  } else {
    adsCh0 = -1;
    adsCh1 = -1;
    adsCh2 = -1;
    adsCh3 = -1;
  }

  if (__ads_obj1_exists) {
    if(adsCh4==-1)  adsCh4 =            __ads_Read_Ch(1, ADS1115_COMP_0_GND);
    else            adsCh4 =  (adsCh4 + __ads_Read_Ch(1, ADS1115_COMP_0_GND))/2;
    if(adsCh5==-1)  adsCh5 =            __ads_Read_Ch(1, ADS1115_COMP_1_GND);
    else            adsCh5 =  (adsCh5 + __ads_Read_Ch(1, ADS1115_COMP_1_GND))/2;
    if(adsCh6==-1)  adsCh6 =            __ads_Read_Ch(1, ADS1115_COMP_2_GND);
    else            adsCh6 =  (adsCh6 + __ads_Read_Ch(1, ADS1115_COMP_2_GND))/2;
    if(adsCh7==-1)  adsCh7 =            __ads_Read_Ch(1, ADS1115_COMP_3_GND);
    else            adsCh7 =  (adsCh7 + __ads_Read_Ch(1, ADS1115_COMP_3_GND))/2;
    // adsCh4 += __ads_Read_Ch(1, ADS1115_COMP_0_GND);
    // adsCh5 += __ads_Read_Ch(1, ADS1115_COMP_1_GND);
    // adsCh6 += __ads_Read_Ch(1, ADS1115_COMP_2_GND);
    // adsCh7 += __ads_Read_Ch(1, ADS1115_COMP_3_GND);
    if (__ads_obj1_exists) Serial.printf("Successful read on ADC1-%d-%d-%d-%d\n", adsCh4, adsCh5, adsCh6, adsCh7);
    else if (!__ads_obj1_exists) Serial.printf("No Response from External ADC1!\n");
  } else {
    adsCh4 = -1;
    adsCh5 = -1;
    adsCh6 = -1;
    adsCh7 = -1;
  }
}

void ads_Reconnect() {
  if (!__ads_obj0_exists) {
    __ads_Start_Connect(0);
  }
  if (!__ads_obj1_exists) {
    __ads_Start_Connect(1);
  }
}

// Connect to ADS
void __ads_Start_Connect(int adsNum) {

  if (adsNum == 0) {
    if (!__ads_obj0.init()) {
      __ads_obj0_exists = false;
      Serial.printf("External ADC%d not found!\n", adsNum);
    } else {
      __ads_obj0_exists = true;
      __ads_obj0.setVoltageRange_mV(ADS1115_RANGE_6144);
      Serial.printf("External ADC%d verified!\n", adsNum);
    }
  } else if (adsNum==1){
    if (!__ads_obj1.init()) {
      __ads_obj1_exists = false;
      Serial.printf("External ADC%d not found!\n", adsNum);
    } else {
      __ads_obj1_exists = true;
      __ads_obj1.setVoltageRange_mV(ADS1115_RANGE_6144);
      Serial.printf("External ADC%d verified!\n", adsNum);
    }
  }

}

#endif