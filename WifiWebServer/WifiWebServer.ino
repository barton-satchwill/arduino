/*
 Web  Server 
 A simple web server that shows the value of the analog input pins.
 using a WiFi shield.
*/

#include <SPI.h>
#include <WiFi.h>

char ssid[] = "***";//  your network SSID (name) 
char pass[] = "***"; // your network password
int status = WL_IDLE_STATUS;
WiFiServer server(80);
char line[256];
int lineIndex = 0;
char url[25];
IPAddress ip(192, 168, 0, 108);

int redPin = 5;
int whitePin = 6;
int greenPin = 8;

void setup() {
  pinMode(redPin, OUTPUT);     
  pinMode(whitePin, OUTPUT);     
  pinMode(greenPin, OUTPUT);     
  Serial.begin(9600); 
  digitalWrite(redPin, LOW);
  digitalWrite(whitePin, LOW);
  digitalWrite(greenPin, LOW);
  setupConnection();
}


void loop() {
  // listen for incoming clients
  WiFiClient client = server.available();
  if (client) {
// Serial.println("------------ new client ----------------------");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
// Serial.print("a..");    
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
// Serial.print(c);
// Serial.print(".");
        line[lineIndex++] = c;
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          toggleLights(); // <------------- this is new
          sendResponse(client);
          lineIndex = 0;
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
          line[lineIndex-1]='\0';
          lineIndex = 0;
          if (strncmp(line, "GET", 3) == 0) {
            strncpy(url, strstr(line, "/"), 25);
            for (int i=0; i<strlen(url); i++) {
              if (url[i] == ' ') {
                url[i] = '\0';
              }
            }
          }
        } 
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(10);
    // close the connection:
    client.stop();
// Serial.println("client disonnected\n\n");
  }
}

void setupConnection(){
  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present"); 
    while(true)
      ; // don't continue:
  } 
  
  WiFi.config(ip);

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) { 
// Serial.print("Attempting to connect to SSID ");
// Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    delay(100);
  } 
  server.begin();
  printWifiStatus();
}  


void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("Connected to SSID ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void sendResponse(WiFiClient client) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connnection: close");
  client.println();
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<body>");
  client.println("<table align='center'>");
  client.println("<th>Light Control</th>");
  client.println("<tr><td>");
  printLightStatus(client, "red", digitalRead(redPin));
  client.println("</td></tr>");
  client.println("<tr><td>");
  printLightStatus(client, "white", digitalRead(whitePin));
  client.println("</td></tr>");
  client.println("<tr><td>");
  printLightStatus(client, "green", digitalRead(greenPin));
  client.println("</td></tr>");
  client.println("</table>");
  client.println("<br />");
  client.println("<br />");
  client.print("<center>");
  client.print("The arduino has been running for: ");
  client.print(millis()/1000/60);
  client.print(" minutes");  
  client.println("</center>");
  client.println("</body>");
  client.println("</html>");
}


void toggleLights() {
 if (strncmp(url, "/red", 4) == 0) { digitalWrite(redPin, digitalRead(redPin) ^ 1); }
 if (strncmp(url, "/white", 6) == 0) { digitalWrite(whitePin, digitalRead(whitePin) ^ 1); }
 if (strncmp(url, "/green", 6) == 0) { digitalWrite(greenPin, digitalRead(greenPin) ^ 1); }
}

void printLightStatus( WiFiClient client, String lightName, int lightState) {
  client.print("<FORM METHOD='link' ACTION='");
  client.print(lightName);
  client.println("'>");
  client.print("    <INPUT TYPE='submit' style='width:100%; background-color:#99CCFF' ");

  if(lightState ==0) {
    client.println("VALUE='Turn " + lightName + " light ON'>");    
  } else {
    client.println("VALUE='Turn " + lightName + " light OFF'>");
  }
  client.println("</FORM>");
}




/*
<FORM METHOD="LINK" ACTION="red">
<INPUT TYPE="submit" VALUE="Red">
</FORM>
*/
