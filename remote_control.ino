/*
   Pratik Deshmukh
   Task Submission for MQTT communication
   Code for Remote control
*/
#include <ESP8266WiFi.h> // Library for esp
#include <PubSubClient.h> // Library for mqtt protocols, publish subscribe
const int pbutton = 13; // pin declaration for button
const char* ssid = "ssid";  // network ssid 
const char* wifi_password = "paswd"; // network password
const char* mqtt_server = "server"; // IP or dns of server
const char* mqtt_fromremote = "pratik/fromremote"; // publishing topic name
const char* mqtt_fromdevice = "pratik/fromdevice"; // subscribing topic name
const char* mqtt_username = "usrname"; // broker username
const char* mqtt_password = "paswd"; // broker password
const char* clientID = "client id";  // client ID 
String payload; // string variable declaration to print subscribed msg

WiFiClient wifiClient; // Creating a client that can connect to IP address and port 
PubSubClient client(mqtt_server, 1883, wifiClient); // declaration of IP and port number(1883 default for mqtt)
void setup() { 
  pinMode(pbutton, INPUT); // declaring button as INTPUT peripheral
  Serial.begin(115200); // baud rate for serial monitor
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, wifi_password);  // start wifi connection with mentioned ssid
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  // print ip of esp8266

  // need to connect broker to initialize communication
  if (client.connect(clientID, mqtt_username, mqtt_password)) {
    Serial.println("Connected to MQTT Broker!");
  }
  else {
    Serial.println("Connection to MQTT Broker failed...");
  }
}

 // function for publishing msg from required topic
void sendmsg() {
  if (pbutton == HIGH) {
    if (client.publish(mqtt_fromremote, "Button pressed!")) { // check if msg is sent 
      Serial.println("Button pushed and message sent!");
    }
    else {
      Serial.println("Communication failed , retrying"); // retry
      client.connect(clientID, mqtt_username, mqtt_password); // reconnect
      delay(10);
      client.publish(mqtt_fromremote, "Button pressed!");  // send instruction that button is pressed
    }
  }
  else if (pbutton == LOW) {
    if (client.publish(mqtt_fromremote, "Button released!")) { // check if msg is sent
      Serial.println("Button released and message sent!");
    }
    else {
      Serial.println("Communication failed , retrying"); // retry
      client.connect(clientID, mqtt_username, mqtt_password); // reconnect
      delay(10);
      client.publish(mqtt_fromremote, "Button released!"); // sent send instruction that button is released
    }
  }
}
// function for subscribing msg from required topic
void receivemsg() {
  payload = client.subscribe(mqtt_fromdevice); // command to subscribe from a said topic 
  Serial.print("Received message : ");
  Serial.println(payload); // print received message
}

void loop() {
  sendmsg(); // call function to send command
  receivemsg(); // call function to receive response
  delay(1000); // run this loop continuosly with 1 second delay
}
