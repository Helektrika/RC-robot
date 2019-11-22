#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#include <ArduinoJson.h>

//#include <ESP32Servo.h>
RF24 radio(17, 5); // CE, CSN
const byte address[6] = "00001";

const int PWM_M1 = 26;
const int IN1_M1 = 13;
const int IN2_M1 = 12;

const int PWM_M2 = 25; 
const int IN1_M2 = 14;
const int IN2_M2 = 27;


const int freq = 5000;
const int PWMChannel1 = 0;
const int PWMChannel2 = 1;
const int servoChanne = 2;
int pos = 4000;
int speed = 150;

//Servo servoCamera;
void setup() {
  
   pinMode(PWM_M1,OUTPUT);
   pinMode(PWM_M2,OUTPUT);
  
   pinMode(IN1_M1,OUTPUT);
   pinMode(IN2_M1,OUTPUT);
  
   pinMode(IN1_M2,OUTPUT);
   pinMode(IN2_M2,OUTPUT);

   Serial.begin(115200);

   ledcSetup(PWMChannel1, freq, 8);  
   ledcSetup(PWMChannel2, freq, 8); 
   ledcSetup(servoChanne, 50, 16); 
   
   ledcAttachPin(PWM_M1, PWMChannel1);
   ledcAttachPin(PWM_M2, PWMChannel2);
   ledcAttachPin(33, servoChanne);

  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
 // servoCamera.setPeriodHertz(50);
 // servoCamera.attach(33      , 500, 2400); 
  Serial.println("Start");
ledcWrite(servoChanne,pos ); 
}



void loop() {
   
            // sweep servo 1
     // Serial.print("pos:"); Serial.println(pos);
   
   //servoCamera.write(90); 
  DynamicJsonDocument doc(1024);
  if (radio.available()) {
    char text[400] = "";
    radio.read(&text, sizeof(text));

    Serial.println(text);
    deserializeJson(doc, text);
     JsonObject obj = doc.as<JsonObject>();

     int x = obj["joystick"]["x"];
     int y = obj["joystick"]["y"];
     

     int btn = obj["joystick"]["btn"];
    
     if(bitRead(btn, 1)){ pos+=100; if(pos>8000) pos = 8000; ledcWrite(servoChanne,pos ); }
     if(bitRead(btn, 3)){ pos-=100; if(pos<0) pos = 0;ledcWrite(servoChanne,pos ); }
     Serial.print("btn:"); Serial.println(btn);
     if(abs(x)>abs(y)){
      if(x>0) backward( abs(x)); else   forward( abs(x));
     }else {
      if(y>0) turnLeft( abs(y)); else turnRight  ( abs(y));
     }
     

     
   //  Serial.print("X:"); Serial.println(x);
    // Serial.print("Y:"); Serial.println(y);
     /*Serial.print("btn1:"); Serial.println(btn1);
  Serial.print("btn2:"); Serial.println(btn2);
  Serial.print("btn3:"); Serial.println(btn3);
  Serial.print("btn4:"); Serial.println(btn4);*/
     
    //Serial.println(text);
  }
}



void forward(int speed){

  Serial.println("forward");
 
  ledcWrite(PWMChannel1, speed);
  ledcWrite(PWMChannel2, speed);
  digitalWrite(IN1_M1, HIGH);
  digitalWrite(IN2_M1, LOW);
  
  digitalWrite(IN1_M2, LOW);
  digitalWrite(IN2_M2, HIGH);
}

void backward(int speed){
 
  Serial.println("backward");
  
  ledcWrite(PWMChannel1, speed);
  ledcWrite(PWMChannel2, speed);
  
  digitalWrite(IN1_M1, LOW);
  digitalWrite(IN2_M1, HIGH);
  
  digitalWrite(IN1_M2, HIGH);
  digitalWrite(IN2_M2, LOW);
}

void turnLeft(int speed){

 
  Serial.println("turnLeft");
 
  ledcWrite(PWMChannel1, speed);
  ledcWrite(PWMChannel2, speed);
  
  digitalWrite(IN1_M1, HIGH);
  digitalWrite(IN2_M1, LOW);
  
  digitalWrite(IN1_M2, HIGH);
  digitalWrite(IN2_M2, LOW);
}

void turnRight(int speed){

  Serial.println("turnRight");
  ledcWrite(PWMChannel1, speed);
  ledcWrite(PWMChannel2, speed);
  
  digitalWrite(IN1_M1, LOW);
  digitalWrite(IN2_M1, HIGH);
  
  digitalWrite(IN1_M2, LOW);
  digitalWrite(IN2_M2, HIGH);
}

void stop(){
  Serial.println("stop");
  digitalWrite(IN1_M1, LOW);
  digitalWrite(IN2_M1, LOW);
  
  digitalWrite(IN1_M2, LOW);
  digitalWrite(IN2_M2, LOW);
}

void speedUp(){
  
  speed+=10;
  if(speed>255) speed =255;
  ledcWrite(PWMChannel1, speed);
  ledcWrite(PWMChannel2, speed);
  Serial.print("speedUp"); Serial.println(speed);
}

void speedDown(){
  Serial.println("speedDown");
  speed-=10;
  if(speed<0) speed =0;
  ledcWrite(PWMChannel1, speed);
  ledcWrite(PWMChannel2, speed);

  Serial.print("speedUp"); Serial.println(speed);
}
