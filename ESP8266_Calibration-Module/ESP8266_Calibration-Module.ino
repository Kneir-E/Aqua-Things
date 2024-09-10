#include <ADS1115_WE.h> 
#include <Wire.h>
#define ADS_ADD 0x48
#define ADS_EN0 D6
#define ADS_EN1 D7

ADS1115_WE _ads = ADS1115_WE(ADS_ADD);
bool adsNotFound = true;

void setup() {
  Serial.begin(9600);
  delay(2000);
  Serial.println("Calibration Module Started");

  pinMode(ADS_EN0, INPUT_PULLUP);
  pinMode(ADS_EN1, INPUT_PULLUP);

  Wire.begin();
  if(!_ads.init()){
    Serial.println("Error: External ADC not found!");
    adsNotFound = true;
  }else{
    Serial.println("External ADC found!");
    adsNotFound = false;
    _ads.setVoltageRange_mV(ADS1115_RANGE_6144);
    _ads.setCompareChannels(ADS1115_COMP_0_GND);
    _ads.setCompareChannels(ADS1115_COMP_1_GND);
    _ads.setMeasureMode(ADS1115_SINGLE);
  }
}

void loop() {
  if(!Serial.available())
    return;
  
  String inst = Serial.readString();

  if(adsNotFound){
    Serial.println("NA-ERROR: External ADC isn't found. Try reconnecting the calibration module.");
    return;
  }

  String instCode = inst.substring(0, 7);

  if(instCode=="AutoDet"){
    int det0 = digitalRead(ADS_EN0);
    int det1 = digitalRead(ADS_EN1);
    if(det0 && det1){
      Serial.println("Not-Detected");
    }else if(!det0 && det1){
      Serial.println("Detected-CH0");
    }else if(det0 && !det1){
      Serial.println("Detected-CH1");
    }else if(!det0 && !det1){
      Serial.println("Detected-CH0-Detected-CH1");
    }
    return;
  }else if(instCode=="ReqData"){
    int channel;
    if(inst.c_str()[8]=='0'){
      channel = 0;
    }else if (inst.c_str()[8]=='1'){
      channel = 1;
    }else{
      Serial.println("NA-Channel is not recognized");
      return;
    }

    int sample_size = inst.substring(10, inst.length()).toInt(); 

    if(sample_size==0){
      Serial.println("NA-Error in input. Expected: \"ReqData-<channel (0 or 1)>-<sample size(int)>\"");
      return;
    }

    Serial.println("Affirmative");

    int i=1;
    while(i<=sample_size){
      int milliVolt;
      if(channel==0)      milliVolt = ads_Read(ADS1115_COMP_0_GND);
      else if(channel==1) milliVolt = ads_Read(ADS1115_COMP_1_GND);
      String data = String(i++) + '-' + String(milliVolt);
      Serial.println(data);
    }

  }else{
    Serial.println("NA-Command is not recognized");
  }
}

int ads_Read(ADS1115_MUX channel) {
  int voltage = 0;
  _ads.setCompareChannels(channel);
  _ads.startSingleMeasurement();
  while(_ads.isBusy()){}
  voltage = _ads.getResult_mV(); //_ads.getResult_V();
  return voltage;
}
