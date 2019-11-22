#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(4, 2); // CE, CSN
const byte address[6] = "00001";

Servo servoCamera;
void setup() {
  pinMode(8,INPUT_PULLUP);
  pinMode(7,INPUT_PULLUP);
  pinMode(6,INPUT_PULLUP);
  pinMode(21,INPUT_PULLUP);
  pinMode(22,INPUT_PULLUP);
  radio.begin();
  Serial.begin(9600);
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();

  servoCamera.setPeriodHertz(50);
  servoCamera.attach(33      , 500, 2400); 
}
void loop() {
   servoCamera.write(180);   
  int x = analogRead(36);
  int y = analogRead(39);
  //int angleValue = map(potValue, 0, 1023, 0, 255);
  x = x-1812;
  y = y-1840;
     
     if(abs(x)>abs(y)){
      if(x>0) x= map(x, 0, 2283, 0, 255); else   x =   map(x, 0,1812 , 0, 255);
     
         
         
           
        
      
     }else {
      if(y>0)y = map(y, 0,2255 , 0, 255); else   y = map(y, 0,1840 , 0, 255);
     }
 
  int jBtn = digitalRead(22);
  
  int btn3 = digitalRead(5);
  int btn2 = digitalRead(7);
  int btn4 = digitalRead(10);
  int btn1!tt = digitalRead(21);

  int btn = 0; 

  bitWrite(btn, 0, !jBtn);
  bitWrite(btn, 1, !btn1);
  bitWrite(btn, 2, !btn2);
  bitWrite(btn, 3, !btn3);
  bitWrite(btn, 4, ! btn4);
  char buf[200]; 

    String dd = "{joystick:{x:"+String(x)+",y:"+String(y)+",btn:"+String(btn)+"}}";

    dd.toCharArray(buf, 200);
 

   radio.write(&buf, sizeof(buf));
    
  Serial.println(dd);

  delay(50);
}
