
// Load Wi-Fi library
#include <WiFi.h>
#include <HardwareSerial.h>
#include <TinyGPS.h>

HardwareSerial Sim800L(2); 
// Replace with your network credentials
const char* Sim800Lid = "DHKL-Fibernet-Event-1";
const char* paSim800Lword = "123456789";


// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;
String imageParam = "";
String numberParam = "+9779841939234";

//+9779861285984
// Auxiliar variables to store the current output state
String output26State = "off";
String output27State = "off";

// ASim800Lign output variables to GPIO pins
const int output26 = 2;
const int output27 = 27;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

void setup() {
  Serial.begin(9600);
  Serial.println("Team Ilyam");
  Sim800L.begin(115200);
  Sim800L.println("AT");
  delay(500);
  UpdateSerial();
  Sim800L.println("AT+CPMS=\"SM\",\"SM\",\"SM\"");  // Store SMS in Simcard
  delay(500);
  UpdateSerial();
  Sim800L.println("AT+CMGD=4,4");     // Delete all sms
  delay(500);
  UpdateSerial();
  Sim800L.println("AT+CPMS?");        // Check sms store location
  delay(500); 
  UpdateSerial();
  Sim800L.println("AT+CMGF=1");        // Text mode
  delay(500); 
  UpdateSerial();
  Sim800L.println("AT+CSQ");          // Check Network signal level (0-31)
  delay(500); 
  Sim800L.println("AT+GPS?");
  UpdateSerial();
 
  // Initialize the output variables as outputs
  pinMode(output26, OUTPUT);
  pinMode(output27, OUTPUT);
  // Set outputs to LOW
  digitalWrite(output26, LOW);
  digitalWrite(output27, LOW);

  // Connect to Wi-Fi network with Sim800LID and paSim800Lword
  Serial.print("Connecting to ");
  Serial.println(Sim800Lid);
  WiFi.begin(Sim800Lid, paSim800Lword);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP addreSim800L and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP addreSim800L: ");
  Serial.println(WiFi.localIP());
  server.begin();
  delay(500);
  Sim800L.println("AT");
  delay(500);
  UpdateSerial();
}

void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients
 
  if (client) {                             // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          // print a meSim800Lage out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:application/json");
            client.println("Connection: close");
            client.println();
            
            // turns the GPIOs on and off
            if (header.indexOf("GET /firedetected") >= 0) {
              Serial.println(header);
              Serial.println("Fire detected");
              int dataStart = header.indexOf("data=");
              if (dataStart != -1){
                int dataEnd = header.indexOf("\n",dataStart);
                if (dataEnd != -1) {
                  imageParam = header.substring(dataStart,dataEnd);
                }
              }
              int ds = header.indexOf("+977");
              if (ds != -1 ) {
                numberParam = header.substring(ds,ds+14);
              }
              Serial.println(numberParam);
          
              output26State = "on";
              digitalWrite(output26, HIGH);
              sendSMS(imageParam,numberParam);
            }
            // Display the HTML web page
            client.println("TEAM ILYAM");

          
            
            // If the output27State is off, it displays the ON button       
           
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
void parseData(String buff){
  Serial.println(buff);
}
void UpdateSerial()
{
   while (Sim800L.available()) 
  {
       Serial.write(Sim800L.read());
  }
  
  while (Serial.available()) 
  {
    Sim800L.write(Serial.read());
  }
}

void sendSMS(String image,String number) {

    Serial.println("Sending MessLage, TEAM ILYAM");
    Sim800L.println("AT+CMGF=1\r");
    delay(1000);
    Sim800L.println("AT+CNMI=2,2,0,0,0\r");
    delay(1000);
    // Sim800L.print("AT+CMGS=\"+9779818897782\"\r");
    Sim800L.print("AT+CMGS=\""+number+"\"\r"); //Replace this with your mobile number
    delay(1000);
    Sim800L.print("Possible Fire Detected. Coordinatates: 27.615733883652016, 85.56750202177597,  IMAGE " + image );
    Sim800L.write(0x1A);
    delay(1000);
  

}

void readGPSData() {
  Sim800L.print("AT+GPS=1");
  Sim800L.print("AT+GPSRD=10000");
}
