#include <ESP8266WiFi.h>
#include<SPI.h>
#include<nRF24L01.h>
#include<RF24.h>
#include<SoftwareSerial.h>
#include<String.h>
const char* ssid = "*******";
const char* password = "********";
WiFiServer server(80);
int ambulance1 = 2;
int val = 0;
int i;
int ble;
int indication = D3;
RF24 radio(D4,D8);
String amb = {"***********"};
unsigned char ADDRESS0[5] = {0xb1, 0x43, 0x88, 0x99, 0x45};
SoftwareSerial ss(D1,D2);

void setup() 
{
  
  ESP.wdtDisable();
  Serial.begin(115200);   //initialise Serial
  delay(10);
  ss.begin(9600);        //initialise UART
  pinMode(indication,OUTPUT);


  /*_________Wifi Network__________*/

  Serial.println();
  Serial.println();
  Serial.print("Connecting to : \t");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
   delay(500);
   Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  server.begin();
  Serial.println("Server started");
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/"); 
  digitalWrite(indication,LOW);
  for(i=0;i<=2;i++)
  {
  digitalWrite(indication,HIGH);
  delay(200);
  digitalWrite(indication,LOW);
  delay(200);
  }
  /*______________________________________*/
  
  
/*__________NRF Interfacing______________*/
 
  radio.begin();
  radio.setRetries(15,15);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_MAX);
  radio.openWritingPipe(ADDRESS0);
  radio.openReadingPipe(0,ADDRESS0);
  radio.stopListening();

/*_______________________________________*/  


}

void loop() 
{ 
  
  
          WiFiClient client = server.available();
          
          if (!client) 
          {
            ESP.wdtFeed();
            return;
          }
          
          Serial.println("Waiting for new data");
          
          while(!client.available())
          {
            ESP.wdtFeed();
            delay(1);
          }
          String request = client.readStringUntil('\r');
          Serial.println(request);
          client.flush();
          int value = LOW;

          if(request.indexOf("/EMERGENCY")!= -1)
          {  
            ble=7;

            value = HIGH;
           }
  
          if (request.indexOf("/NON_EMERGENCY") != -1)  
          {
            value = LOW;
          }

/*_________________________WEBPAGE________________________*/

         client.println("HTTP/1.1 200 OK");
         client.println("Content-Type: text/html");
         client.println("");
         client.println("<!DOCTYPE HTML>");
         client.println("<html>");
         client.println("<head>");
         client.println("<script>");
         client.println("function startTime() ");
         client.println("{");
         client.println ("var today = new Date();");
         client.println("var h = today.getHours();");
         client.println("var m = today.getMinutes();");
         client.println("var s = today.getSeconds();");
         client.println("m = checkTime(m);");
         client.println("s = checkTime(s);");
         client.println("document.getElementById('txt').innerHTML = h + ':' + m + ':' + s;");
         client.println("var t = setTimeout(startTime, 500);");
         client.println("}");
         client.println("function checkTime(i)"); 
         client.println("{");
         client.println("if (i < 10) {i = '0' + i};");  
         client.println("return i;");
         client.println("}");
         client.println("</script>"); 
         client.println("</head>");
         client.println("<body style='background-color:powderblue;' onload='startTime()'>");
         client.print("<h1 style=text-align:center;><font size = 12 color = black>AMBULANCE SYSTEM</style></font></h1>");
         client.println("<br><br>");
         client.println("<a href=\"/EMERGENCY\"\"><button style='float:left'><font size = 8 color = red>EMERGENCY</style></font></p></button></a>");
         client.println("<a href=\"/NON_EMERGENCY\"\"><button style='float:right'><font size = 9 color = green>NON_EMERGENCY</style></font></button></a>"); 
         client.println("<br><br><br><br><br><br><br><br><br><br><br><br><br><br><br>");
         client.println("<details>");
         client.println("<summary>Developed by :</summary>");
         client.println("<p> Sukant Dhamija</p>");
         client.println("<p> Sumit Negi</p>");
         client.println("<p> Sushant Singh</p>");
         client.println("</details>");
         client.println(" <p align = 'right'><font size = 4>Current time :<div align = 'right' id='txt'></div></font></p>");
         client.println("<p align = center><font size = 12 color =  #707b7c > DRIVE SAFE !!</font></p>");
         client.println("</html>");
         
/*__________________________OVER____________________________*/
  
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
  if(ble==7)
  {
                Serial.println("Waiting for ambulance\n");
            while(!ambulance())
             {
                ESP.wdtFeed();
             };
            radio.write(&ambulance1,sizeof(ambulance1));  
            Serial.println("Ambulance is coming\n");
            Serial.println("Sending data to traffic light system");
            Serial.println("------------------------------------");
           }
           else
           {}
}
 
 
 
 int ambulance()
  {
   String msg;
   char c;
   int val=0; 
  if(ss.available()>0)
  {
    while(ss.available())
  {
    c=ss.read();
    msg=msg+c;
    delay(10);
  }
  msg=msg.substring(0,11);
  
  if(amb.indexOf(msg)<0)
  {
    Serial.print("Unauthorized ambulance");
    val=0;
  }
  else val=1;
  
  }
  return val;
  }
 
  
