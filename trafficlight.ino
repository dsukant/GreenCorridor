#include<SPI.h>
#include<nRF24L01.h>
#include<RF24.h>
#include<SoftwareSerial.h>
#include<String.h>

RF24 radio(9,10);


byte pip;
byte newdata;
int rx;
int amb;

unsigned char ADDRESS2[1]= {0xb2};  
unsigned char ADDRESS3[1]= {0xb3};  
unsigned char ADDRESS4[1]= {0xb4};  
unsigned char ADDRESS5[1]= {0xb5}; 

unsigned char ADDRESS1[5]  = 
{
   0xb1,0x43,0x88,0x99,0x45
}; 

unsigned char ADDRESS0[5]  = 
{
  0xb0,0x43,0x88,0x99,0x45
};




void setup() {
  Serial.begin(9600);

  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  
  radio.begin();
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_250KBPS);
  radio.openWritingPipe(ADDRESS0);
  radio.openReadingPipe(0,ADDRESS0);
  radio.openReadingPipe(1,ADDRESS1);
  radio.openReadingPipe(2,ADDRESS2);
  radio.openReadingPipe(3,ADDRESS3);
  radio.openReadingPipe(4,ADDRESS4);
  radio.openReadingPipe(5,ADDRESS5);
  radio.startListening();
  delay(1000);
}

void loop() 
{
      if ( radio.available(&pip) )
      {
        Serial.print("ff");
        radio.read(&rx,sizeof(rx));
        newdata = 1;
        Serial.println(pip);
      }
      else
      {
        Serial.println("Red\tOrange\tGreen");
        digitalWrite(4,HIGH);
        digitalWrite(5,LOW);
        digitalWrite(6,LOW);
        Serial.println("ON \t OFF \t OFF");      
        Serial.println("Waiting for ambulance");
      }
  if(newdata == 1)
  {
    newdata = 0;
    if(pip==1)
    {
      
      amb = rx;
      Serial.print("Amb = ");
      Serial.println(amb);
    }
    if(amb==2)
    {
         Serial.println("------------------------------------");
         Serial.println("Ambulance is coming\n\nSwitching Red Light to Green");
         Serial.println("Red\tOrange\tGreen");
         digitalWrite(4,LOW);
         digitalWrite(5,LOW);
         digitalWrite(6,HIGH);
         Serial.println("OFF \t OFF \t ON");
         Serial.println("--------------------");  
         delay(10000);
    }
   }

}

 
  
