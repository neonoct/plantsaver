

#include <SPI.h>
#include <Ethernet.h>
byte mac[] = {  0xA8, 0x61, 0x0A, 0xAE, 0x83, 0x71 }; //Setting MAC Address
float lightData=0;
float x=0;
int y=0;
float xxData=0;
int Ledstate= 2;
float Time0;
float minute=60000/10; //normally 60000ms=60sec but for demo 6sec
//float hour=minute*60; 
float day=minute*14; //normally hour*14
float night=minute*10;//normally hour*10
float intervals[6]; // 



char server1[] = "192.168.0.134"; // ip of my pc as it is used as server by XAMPP and so MYSQL
IPAddress ip(192,168,0,21); // ip of httpserver arduino that lets us to turn LED on and off (html)
EthernetClient client; 
EthernetServer server(80); // port number that is used by arduino for httpserver arduino that lets us to turn LED on and off (html)
String readstring;

/* Setup for Ethernet and RFID */
void setup() {
  
  for(int i=0;i<6;i++){     // 6 conditions for average ldr values whether to open or close the led
  intervals[i]=minute*i*2;
  }
  pinMode(5,OUTPUT);
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  if (Ethernet.begin(mac) == 0) {
  Serial.println("Failed to configure Ethernet using DHCP");}
  Ethernet.begin(mac, ip);
   if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    while (true) {
      delay(1); // do nothing, no point running without Ethernet hardware
    }
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
  }

  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
  delay(1000);
  
}
//------------------------------------------------------------------------------


/* Infinite Loop */
void loop(){
  Ledstate=2;
  Time0=millis();
  Serial.println("Gunduz");
  while((millis()-Time0 <day)){ //send data during the day
    x=analogRead(A0);
    lightData=map(x,0,1023,0,255);
    Sending_To_phpmyadmindatabase(); 


    if(Ledstate==1)
            digitalWrite(5,HIGH);
          else
            digitalWrite(5,LOW);
    }

  Serial.println("Gece");
  Ledstate=2; 
  float Time1=millis();  
  int a=Receiving_from_phpmyadmindatabase(); //receive the data after day ends and calculate
  delay(10);
  while((millis()-Time1 <night)){ //led by systemonoff during the night
    float Timex=millis();
    
    if(a>230 && (Ledstate==0 || Ledstate==2 )){
      Serial.println("a>230 (Ledstate==0 || Ledstate==2 )");

      
      while(millis()-Timex <=night){
      LedOnOff();
      
      if(Ledstate==1)
        digitalWrite(5,HIGH);
       else
        digitalWrite(5,LOW);
      
      }
     }
     else if(a>210 && a<=230 && (Ledstate==1 || Ledstate==2 ))
     {
      Serial.println("a>210 && a<=230 && (Ledstate==1 || Ledstate==2 )");
      while(millis()-Timex <=intervals[1]){
      LedOnOff(); 
    


      if(Ledstate==0)
        digitalWrite(5,LOW);
       else
        digitalWrite(5,HIGH);
        
        
      } 
      Ledstate=2;
      while(millis()-Timex <=night)
      {
       LedOnOff();
      
        if(Ledstate==1)
        digitalWrite(5,HIGH);
        else
        digitalWrite(5,LOW);  
      
      }
     }
     else if(a>190 && a<=210 && (Ledstate==1 || Ledstate==2 ))
     {
      Serial.println("a>190 && a<=210 && (Ledstate==1 || Ledstate==2 )");
      
        while(millis()-Timex <=intervals[2]){
          LedOnOff();   
         

       if(Ledstate==0)
        digitalWrite(5,LOW);
       else
        digitalWrite(5,HIGH);
          
          
        } 
        Ledstate=2;
        while(millis()-Timex <=night){
          LedOnOff(); 
        
          digitalWrite(5,LOW); 

        if(Ledstate==1)
          digitalWrite(5,HIGH);
        else
          digitalWrite(5,LOW); 
        }
     }
     else if(a>170 && a<=190 && (Ledstate==1 || Ledstate==2 ))
     {
        Serial.println("a>170 && a<=190 && (Ledstate==1 || Ledstate==2 )");
        while(millis()-Timex <=intervals[3]){
          LedOnOff();   
      
         

          if(Ledstate==0)
            digitalWrite(5,LOW);
          else
            digitalWrite(5,HIGH);

          
          }
          Ledstate=2; 
        while(millis()-Timex <=night){
          LedOnOff(); 
         

           if(Ledstate==1)
            digitalWrite(5,HIGH);
          else
            digitalWrite(5,LOW); 
       
        }
     }else if(a>150 && a<=170 && (Ledstate==1 || Ledstate==2 )){
       Serial.println("a>150 && a<=170 && (Ledstate==1 || Ledstate==2 )");
      
      while(millis()-Timex <=intervals[4]){
        LedOnOff(); 
      
         if(Ledstate==0)
            digitalWrite(5,LOW);
          else
            digitalWrite(5,HIGH);
              
      } 
      Ledstate=2;
      while(millis()-Timex <=night){
        LedOnOff(); 
        
        if(Ledstate==1)
            digitalWrite(5,HIGH);
          else
            digitalWrite(5,LOW);
      }
     }
     else if(a>0 && a<=150 && (Ledstate==1 || Ledstate==2 ))
     {
      Serial.println("a>0 && a<=150 && (Ledstate==1 || Ledstate==2 )"); 
      Serial.println(Ledstate,DEC); 
      while(millis()-Timex <=intervals[5]){
      LedOnOff(); 
   

      if(Ledstate==0)
        digitalWrite(5,LOW);
       else
        digitalWrite(5,HIGH);
         
      } 
        
     
      Ledstate=2; //burda low vardi ? 
      while(millis()-Timex <= night){
      LedOnOff(); 
      
      if(Ledstate==1)
        digitalWrite(5,HIGH);
       else
        digitalWrite(5,LOW);
    
      }
     }
  }
    
    delay(10);
}

  void Sending_To_phpmyadmindatabase()   //CONNECTING WITH MYSQL sending the data
 {
  LedOnOff();
  delay(10);
   if (client.connect(server1, 8012)) {
    Serial.println("connected");
    // Make a HTTP request:
    Serial.print("GET /deneme/dd.php?Light=");
    client.print("GET /deneme/dd.php?Light=");     //YOUR URL
    if(Ledstate==1){
      lightData=255;
      xxData=1;
    }
    if(Ledstate==0 || Ledstate==2){
      xxData=0;
    }
    Serial.println(lightData);
    client.print(lightData);
    client.print("&xx=");
    client.print(xxData);
    Serial.println(xxData);
    client.print(" ");      //SPACE BEFORE HTTP/1.1
    client.print("HTTP/1.1");
    client.println();
    client.println("Host: 192.168.0.134");
    client.println("Connection: close");
    client.println();
  } else {
    // if you didn't get a connection to the server1:
    Serial.println("connection failed");
  }
  delay(1000); // interval
    LedOnOff();
  delay(10);
 }

 int Receiving_from_phpmyadmindatabase()   //CONNECTING WITH MYSQL receiving the data
 {
   if (client.connect(server1, 8012)) {
    Serial.println("connected");
    // Make a HTTP request:
    Serial.print("GET /deneme/read.php?");
    client.print("GET /deneme/read.php?");     //YOUR URL
    client.print(" ");      //SPACE BEFORE HTTP/1.1
    client.print("HTTP/1.1");
    client.println();
    client.println("Host: 192.168.0.134");
    client.println("Connection: close");
    client.println(); 
    unsigned long timeout = millis();
    while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
   } 
  }
  while (client.available()) {
    String line = client.readStringUntil('\r');
    y=line.toInt();
    Serial.print(line);
  }
  Serial.println();
  Serial.println("closing connection");
  
 }else {
    // if you didn't get a connection to the server1:
    Serial.println("connection failed");
  }

  return y;
 }


 void LedOnOff()  // putting up http server on IPAddress ip(192,168,0,21) to change the state of led that means turning it off or on manually or letting system decide 
 { 
  // listen for incoming clients
  EthernetClient client2 = server.available();
  if (client2) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client2.connected()) {
      if (client2.available()) {
        char c = client2.read();
        Serial.write(c);
        if(readstring.length()<100){
          readstring +=c;
        }
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          Serial.print(readstring);
          client2.println("HTTP/1.1 200 OK");
          client2.println("Content-Type: text/html");
          client2.println("Connection: close");  // the connection will be closed after completion of the response
          client2.println();
          client2.println("<!DOCTYPE HTML>");
          client2.println("<html>");
          client2.println("<a href=\"/?button1on\"\"><button>LED ON</button></a>");
          client2.println("<a href=\"/?button2off\"\"><button>LED OFF</button></a>");
          client2.println("<a href=\"/?sysControl\"\"><button>sysControl</button></a>");
          client2.println("<body style=background-color:powderblue>");
          client2.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(100);
    // close the connection:
    client2.stop();

    if(readstring.indexOf("?button1on")>0){
            digitalWrite(5,HIGH);
            Ledstate=1;
          }

          if(readstring.indexOf("?button2off")>0){
            digitalWrite(5,LOW);
            Ledstate=0;
          }
           if(readstring.indexOf("?sysControl")>0){
            
            Ledstate=2;
          }
          
    readstring = "";
    Serial.println("client disconnected");
  }
}