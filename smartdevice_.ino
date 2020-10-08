/*
   Pratik Deshmukh
   Task Submission for MQTT communication
   Code for Smart device
*/
#include <ESP8266WiFi.h>  // Library for esp
#include <PubSubClient.h> // Library for mqtt protocols, publish subscribe
const int ledpin = 12; // pin declaration for led 
const char* ssid = "ssid"; // network ssid 
const char* wifi_password = "paswd"; // network password
const char* mqtt_server = "server";   // IP or dns of server
const char* mqtt_fromdevice = "pratik/fromdevice"; // publishing topic name
const char* mqtt_fromremote = "pratik/fromremote"; // subscribing topic name
const char* mqtt_username = "usrname"; // broker username
const char* mqtt_password = "paswd"; // broker password
const char* clientID = "client id"; // client ID 
String payload; // string variable declaration to print subscribed msg

WiFiClient wifiClient; // Creating a client that can connect to IP address and port 
PubSubClient client(mqtt_server, 1883, wifiClient); // declaration of IP and port number(1883 default for mqtt)
void setup() {
  pinMode(ledpin, OUTPUT); // declaring ledpin as OUTPUT peripheral
  Serial.begin(115200); // baud rate for serial monitor
  Serial.print("Connecting to "); 
  Serial.println(ssid); 
  WiFi.begin(ssid, wifi_password); // start wifi connection with mentioned ssid
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP()); // print ip of esp8266

  // need to connect broker to initialize communication
  if (client.connect(clientID, mqtt_username, mqtt_password)) {
    Serial.println("Connected to MQTT Broker!");
  }
  else {
    Serial.println("Connection to MQTT Broker failed...");
  }
}
 // function for subscribing msg from required topic
void receivemsg() {
  payload = client.subscribe(mqtt_fromremote); // command to subscribe from a said topic 
  Serial.print("Received message : "); 
  Serial.println(payload); // print received message
  if (payload == "Button pressed!") { // check and compare msg 
    digitalWrite(ledpin, HIGH); // turning on LED if received msg is button pressed
    if (client.publish(mqtt_fromdevice, "LED ON")) { // print succesfull msg if feedback is sent
      Serial.println("LED TURNED ON and message sent!");
    }
    else {
      Serial.println("Communication failed , retrying"); // retry to send feedback
      client.connect(clientID, mqtt_username, mqtt_password); // reconnect 
      delay(10); 
      client.publish(mqtt_fromdevice, "LED ON"); // send feedback
    }
  }
  else if (payload == "Button released!") { // check and compare msg 
    digitalWrite(ledpin, LOW); // turning off LED if received msg is button released
    if (client.publish(mqtt_fromdevice, "LED OFF")) { // print succesfull msg if feedback is sent
      Serial.println("LED TURNED ON and message sent!");
    }
    else {
      Serial.println("Communication failed , retrying"); // retry to send feedback
      client.connect(clientID, mqtt_username, mqtt_password); // reconnect 
      delay(10);
      client.publish(mqtt_fromdevice, "LED OFF"); // send feedback
    }
  }
}
void loop() {
  receivemsg(); // call function
  delay(1000); // 1 second delay to execute function again
}
