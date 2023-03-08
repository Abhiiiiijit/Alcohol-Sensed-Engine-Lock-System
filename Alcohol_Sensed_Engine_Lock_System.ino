#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
String agAdi = "OPPO F15";                 // We write the name of our network here.    
String agSifresi = "sneha@14";           // We write the password of our network here.
int rxPin = 10;                                               //ESP8266 RX pini
int txPin = 13;
String ip = "184.106.153.149"; 

SoftwareSerial esp(rxPin, txPin); 
// #define sensorDigital 10
#define sensorAnalog A0
const int rs = 2,en = 3, d4 = 7, d5 = 6, d6 = 5, d7 = 4;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int motorpin1 = 8;
int motorpin2 = 9;
int enA = 12;
 
void setup() {
  Serial.begin(9600);  
  // pinMode(sensorDigital, INPUT);
  

  Serial.println("Started");
  esp.begin(115200);                                          // We are starting serial communication with ESP8266.
  esp.println("AT");                                         // We do the module control with the AT command.
  Serial.println("AT  sent ");
  while(!esp.find("OK")){                                     // We wait until the module is ready.
    esp.println("AT");
    Serial.println("ESP8266 Not Find.");
  }
  Serial.println("OK Command Received");
  esp.println("AT+CWMODE=1");                                 // We set the ESP8266 module as a client.
  while(!esp.find("OK")){                                     // We wait until the setting is done.
    esp.println("AT+CWMODE=1");
    Serial.println("Setting is ....");
  }
  Serial.println("Set as client");
  Serial.println("Connecting to the Network ...");
  esp.println("AT+CWJAP=\""+agAdi+"\",\""+agSifresi+"\"");    // We are connecting to our network.
  while(!esp.find("OK"));                                     // We wait until it is connected to the network.
  Serial.println("connected to the network.");
  delay(1000);
  // lcd.print("Alcohol Detected");
  
  pinMode(enA, OUTPUT);
  delay(100);  
  lcd.begin(16, 2);
  pinMode(motorpin1, OUTPUT);
  pinMode(motorpin2, OUTPUT);
  


}

void loop() {

   esp.println("AT+CIPSTART=\"TCP\",\""+ip+"\",80");           // We connect to Thingspeak.
  if(esp.find("Error")){                                      // We check the connection error.
    Serial.println("AT+CIPSTART Error");
  }
  bool digital;
  int analog = analogRead(sensorAnalog);
  String veri = "GET https://api.thingspeak.com/update?api_key=WT3BDKMAZ92Q5EOY";   // Thingspeak command. We write our own api key in the key part.                                   
  veri += "&field1=";
  veri += String(analog);
  // veri += "&field2=";
  // veri += String(digital);  
                        
  digitalWrite(motorpin1, HIGH);
  digitalWrite(motorpin2, LOW);
  analogWrite(enA,255) ;
  // analog=824;
  if(analog>500)
  {
    digital=0;
    veri += "&field2=";
    veri += String(digital); 
    veri += "\r\n\r\n"; 
    esp.print("AT+CIPSEND=");                                   // We give the length of data that we will send to ESP.
    esp.println(veri.length()+2);
    delay(200);
    if(esp.find(">")){
      esp.print(veri);                                          // We send the data.
      Serial.println(veri);
      Serial.println("Data sent.");
      // delay(100);
    }
    Serial.println("Connection Closed.");
    esp.println("AT+CIPCLOSE");                                // we close the link
    delay(100);
  }
  else{
    digital=1;
    veri += "&field2=";
    veri += String(digital);
    veri += "\r\n\r\n"; 
    esp.print("AT+CIPSEND=");                                   // We give the length of data that we will send to ESP.
    esp.println(veri.length()+2);
    delay(200);
    if(esp.find(">")){                                          // The commands in it are running when ESP8266 is ready..
      esp.print(veri);                                          // We send the data.
      Serial.println(veri);
      Serial.println("Data sent.");
      // delay(100);
    }
    Serial.println("Connection Closed.");
    esp.println("AT+CIPCLOSE");                                // we close the link
    delay(100);

  }
  
  
  // veri += "\r\n\r\n"; 
  // esp.print("AT+CIPSEND=");                                   // We give the length of data that we will send to ESP.
  // esp.println(veri.length()+2);
  // delay(200);
  // if(esp.find(">")){                                          // The commands in it are running when ESP8266 is ready..
  //   esp.print(veri);                                          // We send the data.
  //   Serial.println(veri);
  //   Serial.println("Data sent.");
  //   // delay(100);
  // }
  // Serial.println("Connection Closed.");
  // esp.println("AT+CIPCLOSE");                                // we close the link
  // delay(100);

 
  if (digital == 0) {
    digitalWrite(motorpin1, LOW);
    digitalWrite(motorpin2, LOW);

   
    
    
    
    lcd.print("Alcohol Detected");
    delay(2000);
    lcd.clear(); 
    // SendMessage();  
    delay(2000);   
  } 
  else 
  {
    digitalWrite(motorpin1, HIGH);
    digitalWrite(motorpin2, LOW);   
  }
}// The commands in it are running when ESP8266 is ready..