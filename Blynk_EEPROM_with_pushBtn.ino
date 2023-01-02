//Paste yout blynk crediantials here 
#define BLYNK_TEMPLATE_ID "TMPL1ZnQC4Fy"
#define BLYNK_DEVICE_NAME "EEpRom"
#define BLYNK_AUTH_TOKEN "Dn_22NVwV9-ypByvVAbr8QHUSTM4X7uA"

#define BLYNK_PRINT Serial

#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

BlynkTimer timer;

#define led1 D5
#define led2 D8
#define wifiLED D1

#define btn1 D0
#define btn2 D2

int wifiState = 1;
int load1, load2;

char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "HaveFun";
char pass[] = "";

void isBlynkConnected(){
  bool isConnected = Blynk.connected();
  if (isConnected == false){
    wifiState = 1;
    digitalWrite(wifiLED, HIGH);
    Serial.println("NOT Connected");
  }
  if(isConnected == true){
    if(wifiState == 1){
      wifiState = 0;
      update_blynk();
    }
    digitalWrite(wifiLED, LOW);
        Serial.println("Connected");
  }
}

void setup()
{
  //initializing EEPROM with alocating 512 byte
  EEPROM.begin(512);
  Serial.begin(115200);

  //reading EEPROM
  load1 = EEPROM.read(1);
  load2 = EEPROM.read(2);

  //Output pins
    pinMode(led1, OUTPUT); 
    pinMode(led2, OUTPUT);

  //wifi indicator
    pinMode(wifiLED, OUTPUT); 

  //btn pins
    pinMode(btn1, INPUT); 
    pinMode(btn2, INPUT); 
    

  updateLED();

  WiFi.begin(ssid, pass);
  timer.setInterval(5000L, isBlynkConnected);
  Blynk.config(auth);
  delay(200);
 
}

void loop()
{
  if ( WiFi.status() != WL_CONNECTED){
    Serial.println("WiFi Not Connected!");
  }else{
    //Serial.println("WiFi Connected!");
    Blynk.run();
  }

if (wifiState == 0){
  online(); 
}else{
  offline();
}
  timer.run();
}
  BLYNK_WRITE(V0){
  load1 = param.asInt();
  updateLED();
  }

  BLYNK_WRITE(V1){
  load2 = param.asInt();
  updateLED();
  }

  void online(){
    if(digitalRead(btn1)== true){
      load1 = !load1;
      updateLED();
      update_blynk();
      delay(200);
    }else if(digitalRead(btn2)== true){
      load2 = !load2;
      updateLED();
      update_blynk();
      delay(200);
    }
  }

    void offline(){
    if(digitalRead(btn1)== true){
      load1 = !load1;
      updateLED();
      delay(200);
    }else if(digitalRead(btn2)== true){
      load2 = !load2;
      updateLED();
      delay(200);
    }
  }
  
void updateLED(){
digitalWrite(led1, load1);
digitalWrite(led2, load2);
updateEEPROM();
}

void update_blynk(){
  Blynk.virtualWrite(V0, load1);
  Blynk.virtualWrite(V1, load2);
}

void updateEEPROM(){
  EEPROM.put(1, load1);
  EEPROM.put(2, load2);
  EEPROM.commit(); 
}
