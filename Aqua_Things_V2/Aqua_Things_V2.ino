#include <WiFi.h>
#include <String.h>
#include "ConfigManager.h"
#include "ADS_functions.h"
#include "Preferences_functions.h"
#include "SocketClient.h"

// === Config Variables: Used in other header files ===
String STA_SSID = "";
String STA_PASS = "";
String STA_HOST = "";
String STA_PORT = "";
String AP_SSID = "";
String AP_PASS = "";
String PKT_HEAD = "";
String PKT_TAIL = "";
bool P0_ACTU_EN = false;
bool P1_ACTU_EN = false;
bool P2_ACTU_EN = false;
bool P3_ACTU_EN = false;
bool P4_ACTU_EN = false;
bool P5_ACTU_EN = false;
bool P6_ACTU_EN = false;
bool P7_ACTU_EN = false;
int P0_ACTU_L = -9999;  // Port 0 Actuation Value Low (actuate if measurement is below)
int P1_ACTU_L = -9999;  // Port 1                         ''
int P2_ACTU_L = -9999;  // Port 2                         ''
int P3_ACTU_L = -9999;  // Port 3                         ''
int P4_ACTU_L = -9999;  // Port 4                         ''
int P5_ACTU_L = -9999;  // Port 5                         ''
int P6_ACTU_L = -9999;  // Port 6                         ''
int P7_ACTU_L = -9999;  // Port 7                         ''
int P0_ACTU_H = 9999;   // Port 0 Actuation Value High (actuate if measurement is above)
int P1_ACTU_H = 9999;   // Port 1                         ''
int P2_ACTU_H = 9999;   // Port 2                         ''
int P3_ACTU_H = 9999;   // Port 3                         ''
int P4_ACTU_H = 9999;   // Port 4                         ''
int P5_ACTU_H = 9999;   // Port 5                         ''
int P6_ACTU_H = 9999;   // Port 6                         ''
int P7_ACTU_H = 9999;   // Port 7                         ''
uint64_t DEVICE_ADDRESS = 0;
int SETUP_STATUS = DEVICE_SETUP_INIT;

#define PIN_DET0 32
#define PIN_DET1 33
#define PIN_DET2 25
#define PIN_DET3 26
#define PIN_DET4 27
#define PIN_DET5 14
#define PIN_DET6 12
#define PIN_DET7 13
#define ACTUATE_DURATION 300000
#define PIN_ACTU 17
bool actuate_Trig = false;
unsigned long actuate_Start_Time = 0;
unsigned long prev_send_Time = 0;

// === External ADC Objects ===
int adsCh0 = -1;
int adsCh1 = -1;
int adsCh2 = -1;
int adsCh3 = -1;
int adsCh4 = -1;
int adsCh5 = -1;
int adsCh6 = -1;
int adsCh7 = -1;


void cl_Handle_Msg() {
  if(millis() - prev_send_Time<1000)  //If 1 second has not passed since previous send time
    return;
  else
    prev_send_Time=millis();
  if (cl.available()) {
    String rcvmsg = cl.readStringUntil('\n');
    Serial.print("Received: ");
    Serial.println(rcvmsg);
    rcvmsg.trim();    //remove leading or trailing white space
    String command = rcvmsg.substring(0, 7);
    if (command == "Actu_On") {
      actuate_Start();
      cl.println("Actuating");
    } else if (command == "ActuOff") {
      actuate_End();
      cl.println("Actuation End");
    } else if (command == "SetActu") {
      Serial.println("Received command: Set Actuation");
      bool success = true;
      String params = rcvmsg.substring(7, rcvmsg.length());
      // Serial.println("Params: " + params);
      // Serial.println(params.charAt(0) == '-');
      while (params.charAt(0) == '-') {
        //  "-CH0H:9999-CH0L:0000-CH1H:0000-"
        //  "0123456789"
        String tmp = params.substring(1, 5);
        String val = params.substring(6, 10);
        // Serial.println("tmp: " + tmp);
        // Serial.println("val: " + val);
        int valInt;
        if (!(isDigit(val.charAt(0)) && isDigit(val.charAt(1)) && isDigit(val.charAt(2)) && isDigit(val.charAt(3)))) {
          success = false;
          break;
        }
        valInt = val.toInt();
        if (tmp.charAt(4) == 'L' && valInt == 0) valInt = -9999;
        if (tmp == "CH0L") P0_ACTU_L = valInt;
        else if (tmp == "CH1L") P1_ACTU_L = valInt;
        else if (tmp == "CH2L") P2_ACTU_L = valInt;
        else if (tmp == "CH3L") P3_ACTU_L = valInt;
        else if (tmp == "CH4L") P4_ACTU_L = valInt;
        else if (tmp == "CH5L") P5_ACTU_L = valInt;
        else if (tmp == "CH6L") P6_ACTU_L = valInt;
        else if (tmp == "CH7L") P7_ACTU_L = valInt;
        else if (tmp == "CH0H") P0_ACTU_H = valInt;
        else if (tmp == "CH1H") P1_ACTU_H = valInt;
        else if (tmp == "CH2H") P2_ACTU_H = valInt;
        else if (tmp == "CH3H") P3_ACTU_H = valInt;
        else if (tmp == "CH4H") P4_ACTU_H = valInt;
        else if (tmp == "CH5H") P5_ACTU_H = valInt;
        else if (tmp == "CH6H") P6_ACTU_H = valInt;
        else if (tmp == "CH7H") P7_ACTU_H = valInt;
        else {
          success = false;
          break;
        }
        params = params.substring(10, params.length());
      }
      if (success) {
        Serial.println("Successfuly Updated Actuation");
        cl.println("Successful");
        pf_Update();
      } else {
        Serial.println("Unsuccessful Update in Actuation");
        cl.println("Unuccessful");
        pf_Get();
      }
    } else if (command == "SetAcEn") {
      Serial.println("Received command: Set Actuation");
      bool success = true;
      String params = rcvmsg.substring(7, rcvmsg.length());
      Serial.println("Params: " + params);
      Serial.println(params.charAt(0) == '-');
      while (params.charAt(0) == '-') {
        //  "-CH0:1-"
        //  "0123456"
        String chn = params.substring(1, 4);
        char val = params.charAt(5);
        Serial.println("chn: " + chn);
        Serial.println("val: " + val);
  
        bool result;
        if(val=='1') result = true;
        else if(val=='0') result = false;
        else{
          success = false;
          break;
        }
        if      (chn == "CH0") P0_ACTU_EN = result;
        else if (chn == "CH1") P1_ACTU_EN = result;
        else if (chn == "CH2") P2_ACTU_EN = result;
        else if (chn == "CH3") P3_ACTU_EN = result;
        else if (chn == "CH4") P4_ACTU_EN = result;
        else if (chn == "CH5") P5_ACTU_EN = result;
        else if (chn == "CH6") P6_ACTU_EN = result;
        else if (chn == "CH7") P7_ACTU_EN = result;
        else {
          success = false;
          break;
        }
        params = params.substring(6, params.length());
      }
      if (success) {
        Serial.println("Successfuly Updated Actuation Enable");
        cl.println("Successful");
        pf_Update();
      } else {
        Serial.println("Unsuccessful Update in Actuation Enable");
        cl.println("Unuccessful");
        pf_Get();
      }
    }
  }
  String msg = String("{ ")
               + "\"CH0\": " + String(adsCh0) + ", " + "\"DET0\": " + String(digitalRead(PIN_DET0) == LOW) + ", "
               + "\"CH1\": " + String(adsCh1) + ", " + "\"DET1\": " + String(digitalRead(PIN_DET1) == LOW) + ", "
               + "\"CH2\": " + String(adsCh2) + ", " + "\"DET2\": " + String(digitalRead(PIN_DET2) == LOW) + ", "
               + "\"CH3\": " + String(adsCh3) + ", " + "\"DET3\": " + String(digitalRead(PIN_DET3) == LOW) + ", "
               + "\"CH4\": " + String(adsCh4) + ", " + "\"DET4\": " + String(digitalRead(PIN_DET4) == LOW) + ", "
               + "\"CH5\": " + String(adsCh5) + ", " + "\"DET5\": " + String(digitalRead(PIN_DET5) == LOW) + ", "
               + "\"CH6\": " + String(adsCh6) + ", " + "\"DET6\": " + String(digitalRead(PIN_DET6) == LOW) + ", "
               + "\"CH7\": " + String(adsCh7) + ", " + "\"DET7\": " + String(digitalRead(PIN_DET7) == LOW) + ", "
               + "\"ActuStat\": " + String(actuate_Trig) + "} ";

  cl.println(msg);
  Serial.println("Sent to Server: " + msg);
}

void config_Callback_On_Submit() {
  Serial.println("Callback Triggered on Submit");
  pf_Update();
  WiFi.softAPdisconnect();
  WiFi.disconnect(false, true);
  Serial.println(WiFi.status());
  Serial.println(WiFi.getMode());
  wifi_Init();
}

void port_Init() {
  Serial.println("Port Initialized");
  pinMode(PIN_DET0, INPUT_PULLUP);
  pinMode(PIN_DET1, INPUT_PULLUP);
  pinMode(PIN_DET2, INPUT_PULLUP);
  pinMode(PIN_DET3, INPUT_PULLUP);
  pinMode(PIN_DET4, INPUT_PULLUP);
  pinMode(PIN_DET5, INPUT_PULLUP);
  pinMode(PIN_DET6, INPUT_PULLUP);
  pinMode(PIN_DET7, INPUT_PULLUP);
  pinMode(PIN_ACTU, OUTPUT);
  digitalWrite(PIN_ACTU, HIGH);
}

void handle_Actu() {
  if (actuate_Trig) {
    if (millis() - actuate_Start_Time >= ACTUATE_DURATION) {
      actuate_End();
    }
    return;
  }
  if (P0_ACTU_EN && adsCh0 <= P0_ACTU_L) actuate_Start();
  if (P1_ACTU_EN && adsCh1 <= P1_ACTU_L) actuate_Start();
  if (P2_ACTU_EN && adsCh2 <= P2_ACTU_L) actuate_Start();
  if (P3_ACTU_EN && adsCh3 <= P3_ACTU_L) actuate_Start();
  if (P4_ACTU_EN && adsCh4 <= P4_ACTU_L) actuate_Start();
  if (P5_ACTU_EN && adsCh5 <= P5_ACTU_L) actuate_Start();
  if (P6_ACTU_EN && adsCh6 <= P6_ACTU_L) actuate_Start();
  if (P7_ACTU_EN && adsCh7 <= P7_ACTU_L) actuate_Start();

  if (P0_ACTU_EN && adsCh0 >= P0_ACTU_H) actuate_Start();
  if (P1_ACTU_EN && adsCh1 >= P1_ACTU_H) actuate_Start();
  if (P2_ACTU_EN && adsCh2 >= P2_ACTU_H) actuate_Start();
  if (P3_ACTU_EN && adsCh3 >= P3_ACTU_H) actuate_Start();
  if (P4_ACTU_EN && adsCh4 >= P4_ACTU_H) actuate_Start();
  if (P5_ACTU_EN && adsCh5 >= P5_ACTU_H) actuate_Start();
  if (P6_ACTU_EN && adsCh6 >= P6_ACTU_H) actuate_Start();
  if (P7_ACTU_EN && adsCh7 >= P7_ACTU_H) actuate_Start();
}

void actuate_Start() {
  if (actuate_Trig != true) {
    Serial.println("Actuation Started");
    digitalWrite(PIN_ACTU, LOW);
    actuate_Trig = true;
    actuate_Start_Time = millis();
  }
}

void actuate_End() {
  Serial.println("Actuation Ended");
  digitalWrite(PIN_ACTU, HIGH);
  actuate_Trig = false;
  actuate_Start_Time = 0;
}

void setup() {
  Serial.begin(115200);

  pf_Init();    // Retrieve Config Variables from ROM and store in RAM
  ads_Init();   // Initialize extenal ADC
  port_Init();  // Initialize port detection pins and actuation pin

  conf_Init(cl_Stop, config_Callback_On_Submit);  // Initialize Config Mode
                                                  // =======================ADD CALLBACKS... SHOULD BE AFTER WIFI===========================
                                                  // on trigger: close client
                                                  // on submit: pf_Update(), wifi_Init()

  if (is_Setup_Done == DEVICE_SETUP_NONE)  // IF DEVICE SETUP HAS NOT BEEN DONE
    __conf_trig_flag = true;               // Trigger Config Mode
}

void loop() {
  if (is_Setup_Done != DEVICE_SETUP_DONE) {  // IF DEVICE SETUP HAS NOT BEEN DONE
    Serial.println("No Setup Found. Entering Initial Setup Mode");
    config_Mode_Loop(9999, true);  // Enter Config Mode with no Expiry Time.
    delay(1000);
    return;  // Do Nothing else
  } else
    config_Mode_Loop();  // Required function for setup mode async - process config mode flags and functions


  if (__conf_enabled) {  // IF IN SETUP MODE
    // CODE TO DO WHILE IN CONFIG MODE

  } else if (!__conf_enabled) {  // IF IN OPERATION MODE

    // if WiFi not Connected, Connect WiFi
    if (WiFi.status() != WL_CONNECTED)
      wifi_Init();

    // if WiFi connected but Client not connected, Connect Client
    if (WiFi.status() == WL_CONNECTED && !cl.connected())
      cl_Connect();

    // if WiFi connected and Client connected, Handle comms with client
    else if (WiFi.status() == WL_CONNECTED && cl.connected()) {
      cl_Handle_Msg();
    }
  }

  // ========== FOREGROUND ROUTINE ==========
  Serial.println("Foreground Routine");
  // ALWAYS DO IN ANY MODE
  ads_Reconnect();
  ads_Read_All();  // Read all channels and update global variables
  handle_Actu();
  if (conf_Reset_Flag) {
    Serial.println("Clearing Config");
    pf_Reset();
    Serial.println("Clear Complete. Restarting...");
    ESP.restart();
  }
  // delay(500);
}