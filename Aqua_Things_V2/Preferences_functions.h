#ifndef PREFERENCES_FUNCTIONS_H
#define PREFERENCES_FUNCTIONS_H

#include <Preferences.h>

#define DEVICE_SETUP_INIT 255   //  If device has not been setup at all (freshly loaded program, no data in non-volatile memory)
#define DEVICE_SETUP_NONE 0     //  Device has been not been setup manually (this will be the mode after the progran is run for the first time)
#define DEVICE_SETUP_DONE 1     //  Device has been setup manually

Preferences pf;
extern String STA_SSID;
extern String STA_PASS;
extern String STA_HOST;
extern String STA_PORT;
extern String AP_SSID;
extern String AP_PASS;
extern String PKT_HEAD;
extern String PKT_TAIL;
extern bool P0_ACTU_EN;
extern bool P1_ACTU_EN;
extern bool P2_ACTU_EN;
extern bool P3_ACTU_EN;
extern bool P4_ACTU_EN;
extern bool P5_ACTU_EN;
extern bool P6_ACTU_EN;
extern bool P7_ACTU_EN;
extern int P0_ACTU_L;
extern int P1_ACTU_L;
extern int P2_ACTU_L;
extern int P3_ACTU_L;
extern int P4_ACTU_L;
extern int P5_ACTU_L;
extern int P6_ACTU_L;
extern int P7_ACTU_L;
extern int P0_ACTU_H;
extern int P1_ACTU_H;
extern int P2_ACTU_H;
extern int P3_ACTU_H;
extern int P4_ACTU_H;
extern int P5_ACTU_H;
extern int P6_ACTU_H;
extern int P7_ACTU_H;
extern uint64_t DEVICE_ADDRESS;
extern int SETUP_STATUS;

int is_Setup_Done = DEVICE_SETUP_INIT;
bool __pf_Print_En = true;    // will print all config variables on update/get when true

uint64_t randomUint64();
void pf_Init();
void pf_Update();
void pf_Get();
void pf_Reset();
void pf_Is_Init();
void pf_Print();

void pf_Init() {
  pf.begin("Aqua-Things", false);

  if(pf.getInt("initialized", DEVICE_SETUP_INIT) == DEVICE_SETUP_INIT){   // If preferences does not exist
    pf.end();
    pf_Reset();                                           // initialize preferences with default values
    is_Setup_Done = DEVICE_SETUP_NONE;
    SETUP_STATUS = is_Setup_Done;
    return;
  }

  pf.end();
  pf_Get();
}

void pf_Update(){
  pf.begin("Aqua-Things", false);

  if( pf.getInt("initialized", DEVICE_SETUP_NONE) !=  DEVICE_SETUP_DONE)  pf.putInt("initialized", DEVICE_SETUP_DONE);
  is_Setup_Done = DEVICE_SETUP_DONE;
  SETUP_STATUS = is_Setup_Done;
  
  if(STA_SSID!=pf.getString("STA_SSID", ""))              pf.putString("STA_SSID", STA_SSID);
  if(STA_PASS!=pf.getString("STA_PASS", ""))              pf.putString("STA_PASS", STA_PASS);
  if(STA_HOST!=pf.getString("STA_HOST", ""))              pf.putString("STA_HOST", STA_HOST);
  if(STA_PORT!=pf.getString("STA_PORT", ""))              pf.putString("STA_PORT", STA_PORT);
  if(AP_SSID!=pf.getString("AP_SSID", ""))                pf.putString("AP_SSID", AP_SSID);
  if(AP_PASS!=pf.getString("AP_PASS", ""))                pf.putString("AP_PASS", AP_PASS);
  if(PKT_HEAD!=pf.getString("PKT_HEAD", ""))              pf.putString("PKT_HEAD", PKT_HEAD);
  if(PKT_TAIL!=pf.getString("PKT_TAIL", ""))              pf.putString("PKT_TAIL", PKT_TAIL);
  if(P0_ACTU_EN!=pf.getBool("ACT_EN_0", false))    pf.putBool("ACT_EN_0", P0_ACTU_EN); 
  if(P1_ACTU_EN!=pf.getBool("ACT_EN_1", false))    pf.putBool("ACT_EN_1", P1_ACTU_EN); 
  if(P2_ACTU_EN!=pf.getBool("ACT_EN_2", false))    pf.putBool("ACT_EN_2", P2_ACTU_EN); 
  if(P3_ACTU_EN!=pf.getBool("ACT_EN_3", false))    pf.putBool("ACT_EN_3", P3_ACTU_EN); 
  if(P4_ACTU_EN!=pf.getBool("ACT_EN_4", false))    pf.putBool("ACT_EN_4", P4_ACTU_EN); 
  if(P5_ACTU_EN!=pf.getBool("ACT_EN_5", false))    pf.putBool("ACT_EN_5", P5_ACTU_EN); 
  if(P6_ACTU_EN!=pf.getBool("ACT_EN_6", false))    pf.putBool("ACT_EN_6", P6_ACTU_EN); 
  if(P7_ACTU_EN!=pf.getBool("ACT_EN_7", false))    pf.putBool("ACT_EN_7", P7_ACTU_EN); 
  if(P0_ACTU_L!=pf.getInt("P0_ACTU_L", -9999))      pf.putInt("P0_ACTU_L", P0_ACTU_L);
  if(P1_ACTU_L!=pf.getInt("P1_ACTU_L", -9999))      pf.putInt("P1_ACTU_L", P1_ACTU_L);
  if(P2_ACTU_L!=pf.getInt("P2_ACTU_L", -9999))      pf.putInt("P2_ACTU_L", P2_ACTU_L);
  if(P3_ACTU_L!=pf.getInt("P3_ACTU_L", -9999))      pf.putInt("P3_ACTU_L", P3_ACTU_L);
  if(P4_ACTU_L!=pf.getInt("P4_ACTU_L", -9999))      pf.putInt("P4_ACTU_L", P4_ACTU_L);
  if(P5_ACTU_L!=pf.getInt("P5_ACTU_L", -9999))      pf.putInt("P5_ACTU_L", P5_ACTU_L);
  if(P6_ACTU_L!=pf.getInt("P6_ACTU_L", -9999))      pf.putInt("P6_ACTU_L", P6_ACTU_L);
  if(P7_ACTU_L!=pf.getInt("P7_ACTU_L", -9999))      pf.putInt("P7_ACTU_L", P7_ACTU_L);
  if(P0_ACTU_H!=pf.getInt("P0_ACTU_H", 9999))      pf.putInt("P0_ACTU_H", P0_ACTU_H);
  if(P1_ACTU_H!=pf.getInt("P1_ACTU_H", 9999))      pf.putInt("P1_ACTU_H", P1_ACTU_H);
  if(P2_ACTU_H!=pf.getInt("P2_ACTU_H", 9999))      pf.putInt("P2_ACTU_H", P2_ACTU_H);
  if(P3_ACTU_H!=pf.getInt("P3_ACTU_H", 9999))      pf.putInt("P3_ACTU_H", P3_ACTU_H);
  if(P4_ACTU_H!=pf.getInt("P4_ACTU_H", 9999))      pf.putInt("P4_ACTU_H", P4_ACTU_H);
  if(P5_ACTU_H!=pf.getInt("P5_ACTU_H", 9999))      pf.putInt("P5_ACTU_H", P5_ACTU_H);
  if(P6_ACTU_H!=pf.getInt("P6_ACTU_H", 9999))      pf.putInt("P6_ACTU_H", P6_ACTU_H);
  if(P7_ACTU_H!=pf.getInt("P7_ACTU_H", 9999))      pf.putInt("P7_ACTU_H", P7_ACTU_H);
  pf.end();
  pf_Print();
}

void pf_Get(){
  pf.begin("Aqua-Things", false);
  STA_SSID  = pf.getString("STA_SSID", "");
  STA_PASS  = pf.getString("STA_PASS", "");
  STA_HOST  = pf.getString("STA_HOST", "");
  STA_PORT  = pf.getString("STA_PORT", "");
  AP_SSID   = pf.getString("AP_SSID", "");
  AP_PASS   = pf.getString("AP_PASS", "");
  PKT_HEAD  = pf.getString("PKT_HEAD", "");
  PKT_TAIL  = pf.getString("PKT_TAIL", "");
  P0_ACTU_EN = pf.getBool("ACT_EN_0", false);
  P1_ACTU_EN = pf.getBool("ACT_EN_1", false);
  P2_ACTU_EN = pf.getBool("ACT_EN_2", false);
  P3_ACTU_EN = pf.getBool("ACT_EN_3", false);
  P4_ACTU_EN = pf.getBool("ACT_EN_4", false);
  P5_ACTU_EN = pf.getBool("ACT_EN_5", false);
  P6_ACTU_EN = pf.getBool("ACT_EN_6", false);
  P7_ACTU_EN = pf.getBool("ACT_EN_7", false);
  P0_ACTU_L = pf.getInt("P0_ACTU_L", -9999);
  P1_ACTU_L = pf.getInt("P1_ACTU_L", -9999);
  P2_ACTU_L = pf.getInt("P2_ACTU_L", -9999);
  P3_ACTU_L = pf.getInt("P3_ACTU_L", -9999);
  P4_ACTU_L = pf.getInt("P4_ACTU_L", -9999);
  P5_ACTU_L = pf.getInt("P5_ACTU_L", -9999);
  P6_ACTU_L = pf.getInt("P6_ACTU_L", -9999);
  P7_ACTU_L = pf.getInt("P7_ACTU_L", -9999);
  P0_ACTU_H = pf.getInt("P0_ACTU_H", 9999);
  P1_ACTU_H = pf.getInt("P1_ACTU_H", 9999);
  P2_ACTU_H = pf.getInt("P2_ACTU_H", 9999);
  P3_ACTU_H = pf.getInt("P3_ACTU_H", 9999);
  P4_ACTU_H = pf.getInt("P4_ACTU_H", 9999);
  P5_ACTU_H = pf.getInt("P5_ACTU_H", 9999);
  P6_ACTU_H = pf.getInt("P6_ACTU_H", 9999);
  P7_ACTU_H = pf.getInt("P7_ACTU_H", 9999);
  DEVICE_ADDRESS     = pf.getULong64("DEVICE_ADDR", 0);
  is_Setup_Done = pf.getInt("initialized", DEVICE_SETUP_INIT);
  SETUP_STATUS = is_Setup_Done;
  pf.end();
  pf_Print();
}

void pf_Reset(){
  pf.begin("Aqua-Things", false);

  uint64_t dev_addr = pf.getULong64("DEVICE_ADDR", 0);  // preserve device address

  pf.clear();
  AP_SSID = "AQUACULTURE";
  AP_PASS = "AQUACULTURE";
  pf.putString("AP_SSID", AP_SSID);
  pf.putString("AP_PASS", AP_PASS);

  pf.putInt("P0_ACTU_L", -9999);
  pf.putInt("P1_ACTU_L", -9999);
  pf.putInt("P2_ACTU_L", -9999);
  pf.putInt("P3_ACTU_L", -9999);
  pf.putInt("P4_ACTU_L", -9999);
  pf.putInt("P5_ACTU_L", -9999);
  pf.putInt("P6_ACTU_L", -9999);
  pf.putInt("P7_ACTU_L", -9999);
  pf.putInt("P0_ACTU_H", 9999);
  pf.putInt("P1_ACTU_H", 9999);
  pf.putInt("P2_ACTU_H", 9999);
  pf.putInt("P3_ACTU_H", 9999);
  pf.putInt("P4_ACTU_H", 9999);
  pf.putInt("P5_ACTU_H", 9999);
  pf.putInt("P6_ACTU_H", 9999);
  pf.putInt("P7_ACTU_H", 9999);



  pf.putInt("initialized", DEVICE_SETUP_NONE);
  is_Setup_Done = DEVICE_SETUP_NONE;
  SETUP_STATUS = is_Setup_Done;
  
  if(dev_addr == 0)                                     // if no address
    pf.putULong64("DEVICE_ADDR", randomUint64());   // generate adress
  else                                                  // else
    pf.putULong64("DEVICE_ADDR", dev_addr);             // store preserved address
  
  pf.end();

  pf_Print();
}

void pf_Print(){
  if(!__pf_Print_En){
    return;
  }
  Serial.println("STA_SSID: " + String(STA_SSID));
  Serial.println("STA_PASS: " + String(STA_PASS));    
  Serial.println("STA_HOST: " + String(STA_HOST));    
  Serial.println("STA_PORT: " + String(STA_PORT));    
  Serial.println("AP_SSID: " + String(AP_SSID));      
  Serial.println("AP_PASS: " + String(AP_PASS));      
  Serial.println("PKT_HEAD: " + String(PKT_HEAD));    
  Serial.println("PKT_TAIL: " + String(PKT_TAIL));    
  Serial.println("P0_ACTU_EN: " + String(P0_ACTU_EN));
  Serial.println("P1_ACTU_EN: " + String(P1_ACTU_EN));
  Serial.println("P2_ACTU_EN: " + String(P2_ACTU_EN));
  Serial.println("P3_ACTU_EN: " + String(P3_ACTU_EN));
  Serial.println("P4_ACTU_EN: " + String(P4_ACTU_EN));
  Serial.println("P5_ACTU_EN: " + String(P5_ACTU_EN));
  Serial.println("P6_ACTU_EN: " + String(P6_ACTU_EN));
  Serial.println("P7_ACTU_EN: " + String(P7_ACTU_EN));
  Serial.println("P0_ACTU_L: " + String(P0_ACTU_L));  
  Serial.println("P1_ACTU_L: " + String(P1_ACTU_L));  
  Serial.println("P2_ACTU_L: " + String(P2_ACTU_L));  
  Serial.println("P3_ACTU_L: " + String(P3_ACTU_L));  
  Serial.println("P4_ACTU_L: " + String(P4_ACTU_L));  
  Serial.println("P5_ACTU_L: " + String(P5_ACTU_L));
  Serial.println("P6_ACTU_L: " + String(P6_ACTU_L));
  Serial.println("P7_ACTU_L: " + String(P7_ACTU_L));
  Serial.println("P0_ACTU_H: " + String(P0_ACTU_H));
  Serial.println("P1_ACTU_H: " + String(P1_ACTU_H));
  Serial.println("P2_ACTU_H: " + String(P2_ACTU_H));
  Serial.println("P3_ACTU_H: " + String(P3_ACTU_H));
  Serial.println("P4_ACTU_H: " + String(P4_ACTU_H));
  Serial.println("P5_ACTU_H: " + String(P5_ACTU_H));
  Serial.println("P6_ACTU_H: " + String(P6_ACTU_H));
  Serial.println("P7_ACTU_H: " + String(P7_ACTU_H));
}

uint64_t randomUint64() {
  // Generate two 32-bit random numbers
  uint32_t high = random(0xFFFFFFFF); // Upper 32 bits
  uint32_t low = random(0xFFFFFFFF);  // Lower 32 bits
  
  // Combine the two 32-bit numbers into one 64-bit number
  uint64_t random64 = ((uint64_t)high << 32) | low;


  Serial.print("Generated 64bit num: ");
  Serial.println(random64);
  return random64;
}

#endif