#include <ArduinoJson.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <WebSocketClient.h>
#include <WebSocketServer.h>
#include <Wire.h>
#include <SPI.h>

/* Libraries to include Bluetooth connection in case WiFi does not connect to server */
//#include "esp_bt_main.h"
//#include "esp_bt_device.h"
//#include "BluetoothSerial.h"

/* Create the LCD object for display */
LiquidCrystal_I2C lcd(0x27, 16, 2);

/* Define the input and output pins for FireBeetle connection to Power Module*/
#define POWER_OUT D3
#define POWER_IN D2

/* Global varibales used in main loop */
int voltage, current, port;
int power,powero;
int cost,emission;
int sum;

/* WiFi SSID and PASSWORD for connection */
//char* ssid = "ece design lab2";
//const char* password =  "ecedesignlab12345";
char* ssid = "Slimeville 2.4G";
const char* password = "230PD$limes";
 
/* Function to connect FireBeetle ESEP32 to chosen network */
void connectToNetwork() {
  Serial.println("Beginning of connectToNetwork");
  
  /* Disconnect from any previous networks and reconnect to network given SSID and PASSWORD */
  WiFi.disconnect(true);
  WiFi.begin(ssid, password);
 
  /* Loop to check if connection has been established */
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Establishing connection to WiFi...");
  }
  
  /* Printing IP ADDRESS for the local network connection */
  Serial.println("Connected to network");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
 
}

/* Function to scan networks in the range of FireBeetle ESP32 
 * which allows for you to see the networks availbe for connection
 * MUST CHANGE SSID AND PASSWORD TO MATCH NETWORKS SCANNED */
void scanNetworks() {

  Serial.println("Start of scanNetworks");
  
  /* Functoin to scan networks */
  int numberOfNetworks = WiFi.scanNetworks();
 
  Serial.print("Number of networks found: ");
  Serial.println(numberOfNetworks);
 
  for (int i = 0; i < numberOfNetworks; i++) {
 
    Serial.print("Network name: ");
    Serial.println(WiFi.SSID(i));
 
    Serial.print("Signal strength: ");
    Serial.println(WiFi.RSSI(i));
 
    Serial.print("MAC address: ");
    Serial.println(WiFi.BSSIDstr(i));s
 
    Serial.println("-----------------------");
 
  }
}

/* Variables for the Web Socket Server connection */
WebSocketClient webSocketClient;
WiFiClient client;

char path[] = "/";
//char host[] = "136, 142, 159, 44"; //"10.215.31.172" or "10.215.16.1" <- Other potential addresses to be connected to
IPAddress host(136,142,159,44);
//IPAddress google(74,125,115,105);  // Google test address

//Create a function that connects to the servers
// connected with the server and completes a handshake between the server.
// Refer to the google tabs open.
void connectToServer(){
  Serial.println("Connecting to:");
  Serial.println(host);
  
  /* Call function connect to connect to host with port availabe (8080 for server; 80 for Google) */
  if(client.connect(host,8080)){
    Serial.println("Connection success");
  }else{
    Serial.println("Connection failed");
  }
/* Handshake betweeen the Client and Server for secure transfer of data */
  webSocketClient.path = path;
  webSocketClient.host = host;
  if (webSocketClient.handshake(client)) {
    Serial.println("Handshake successful");
  } else {
    Serial.println("Handshake failed");
  }
}


/* Fucntion to handle the received messages in JSON format */
void handleReceivedMessage(String message){
  /*message handling code */
  StaticJsonBuffer<500> JSONBuffer; //Memory pool

  JsonObject& parsed = JSONBuffer.parseObject(message);
  if (!parsed.success()){
    Serial.println("Parsing Failed");
    return;
  }

  //Variables for JSON message structure
  int port = parsed["port"]; //Get value of sensor measurement
}

/* Function to handle the sent messages in JSON format */
void handleSendMessage(float p, float v, float c){
	/* Testing format of JSON parameters */
//  JSON message
//  Serial.print(p);
//  Serial.print(v);
//  Serial.print(c);
  
  //char JSONMessage[] = " {\"Port\": p, \"Voltage\": v, \"Current\": 2}"; Test of JSON format

  StaticJsonBuffer<300> JSONBuffer;                         //Memory pool
  JsonObject& JSONencoder = JSONBuffer.createObject();
  
  JSONencoder["Port"] = p;
  JSONencoder["Voltage"] = v;
  JSONencoder["Current"] = c;

  JSONencoder.printTo(Serial);
  Serial.println("\n\n");

//  JsonObject& parsed = JSONBuffer.parseObject(data); //Parse message
//
// 
//  if (!parsed.success()) {   //Check for errors in parsing
//    Serial.println("Parsing failed");
//    delay(5000);
//    return;
// 
//  }
//  
//  String port = parsed["Port"]; //Get sensor type value
//  String voltage = parsed["Voltage"];  
//  String current = parsed["Current"];
// 
//  Serial.print("Port: ");
//  Serial.println(port);
//  Serial.print("Voltage: ");
//  Serial.println(voltage);
//  Serial.print("Current: ");
//  Serial.println(current);
 
  Serial.println();
  
  delay(5000);
}
/* Function to initiate Bluetooth in microcontroller */
/*
bool initBluetooth()
{
  if (!btStart()) {
    Serial.println("Failed to initialize controller");
    return false;
  } else {
    Serial.println("Initialized Controller");
  }
 
  if (esp_bluedroid_init() != ESP_OK) {
    Serial.println("Failed to initialize bluedroid");
    return false;
  } else {
    Serial.println("Initialized bluedroid");
  }
 
  if (esp_bluedroid_enable() != ESP_OK) {
    Serial.println("Failed to enable bluedroid");
    return false;
  } else {
    Serial.println("enabled bluedroid");
  }
 
}
*/

/* Function to print the Bluetooth address so host can connect to FireBeetle ESP32 */
/*
void printDeviceAddress() {
 
  const uint8_t* point = esp_bt_dev_get_address();
 
  for (int i = 0; i < 6; i++) {
 
    char str[3];
 
    sprintf(str, "%02X", (int)point[i]);
    Serial.print(str);
 
    if (i < 5){
      Serial.print(":");
    }
 
  }
}
*/

void setup() {
  //Set up functions
  Serial.begin(9600);
  
  /* Starting the connection to WiFi and starting server if needed */
  //scanNetworks(); 				// Function to scan networks in the area
  //connectToNetwork(); 			// Function to connect to selected network in area
  //connectToServer(); 				// connecting the FireBeetle as Client to a server
  //server.begin(); 				// activating server on FireBeetle
  //Serial.println(WiFi.localIP()); // Printing the local IP Address of FireBeetle ESP32

  /* Starting the connection to Bluetooth if needed */
  //initBluetooth();
  //printDeviceAddress();

  /* Starting the LCD display */
  lcd.begin();
  lcd.backlight();
  lcd.clear();

  /*set up pins for the Power Module */
  pinMode(POWER_OUT,OUTPUT);
  pinMode(POWER_IN,INPUT);

  /* back up ADC code */
  //pinMode(A0,INPUT);
  //pinMode(A1,INPUT);
  //analogSetClockDiv(60);

  /* digital read Code */
  pinMode(D9,INPUT);

  /* analog read code */
  pinMode(A0,INPUT);
}

/* Count Variable */
int count;

void loop() {
  //Variables
  int temp;
  int temp2;
  int port_value;
  int port_in;
  int i;
  
  //Testing the JSON reading and writing formats
  // currently not working and I dont know why. It does not like 
  // handleSendMessage(6,9.9,0.7);

  /* This is the WiFi server connection where I create a client and connect to a server
   * via host and port location */
/*
  //WiFi Server connection
  String data;

  if (client.connected()){
    //Communication with client
    webSocketClient.sendData("Info");

    webSocketClient.getData(data);
      if (data.length() > 0) {
        Serial.println("Received Data: ");
        Serial.println(data);
        //handleReceivedMessage(data);
        //webSocketServer.sendData(handleSendMessage(port,voltage,current));
      }
      delay(100);
    } else {
    Serial.println("The client disconnected");
    delay(100);
  }
  
  delay(1000);
*/

  //Backup ADC code to read in values
  temp = digitalRead(D9);
/*
  temp = analogRead(A0);

  if(temp>500){
    temp2 = 1;
  } else {
    temp2 = 0;
  }
*/

  if (temp == LOW){
    sum += 1;
  }
  
  Serial.print("Digital Value: ");
  Serial.println(temp);
  
  Serial.print("Sum: ");
  Serial.println(sum);

  
  //Power Module integration
  power = digitalRead(POWER_IN);
  delay(500);

  while (power == HIGH){
    digitalWrite(POWER_OUT, HIGH);
    delay(100);
    digitalWrite(POWER_OUT,LOW);
    delay(100);
  }
  
  port_value = (port_in + port)%6;
    for (i = 0; i < port_value; i++){
      digitalWrite(POWER_OUT, LOW);
      delay(50);
      digitalWrite(POWER_OUT,HIGH);
      delay(100);
  }

  /*
  switch (port){
    case 2:
      for (i = 0; i < port_value; i++)[
        digitalWrite(POWER, HIGH);
        delay(100);
        digitalWrite(POWER,LOW);
        delay(100);
      }
      break;
    case 3:
      for (i = 0; i < port_value; i++)[
        digitalWrite(POWER, HIGH);
        delay(100);
        digitalWrite(POWER,LOW);
        delay(100);
      }
      break;
    case 4:
      for (i = 0; i < port_value; i++)[
        digitalWrite(POWER, HIGH);
        delay(100);
        digitalWrite(POWER,LOW);
        delay(100);
      }
      break;
    case 5:
      for (i = 0; i < port_value; i++)[
        digitalWrite(POWER, HIGH);
        delay(100);
        digitalWrite(POWER,LOW);
        delay(100);
      }
      break;
    default:
  
  if (power == HIGH){
    Serial.println("HIGH");
    digitalWrite(POWER_OUT, HIGH);
  } else {
    Serial.println("LOW");
    digitalWrite(POWER_OUT, LOW);
  }
  delay(500);
  */
  
  /* Display functions based off input readings online with dummy vairables
   * Works properly, need to replace the variables with the read JSON values */
  /*
  lcd.setCursor(0,0);
  lcd.print("Savings:");
  lcd.setCursor(0,9);
  lcd.print();

  lcd.setCursor(1,0);
  lcd.print("E-Reduc:");
  lcd.setCursor(1,9);
  lcd.print();
  */

  /* Using serial communication to trasnfer data from ATMEGA to FireBeetle
   * Not implementing this into final design so leave commented out */
/*
  while (!Serial.available()){}
  delay(100);
  byte b1 = Serial.read();
  while (!Serial.available()){}
  delay(100);
  byte b2 = Serial.read();

  voltage = b2 + b1 *256;
 
  Serial.print("\nVoltage:");
  Serial.print(voltage);
  delay(500);
*/
  
  /* Code to display the connection to the I2C address
   * Not needed in final demonstration, just used for testing */
/*
  Serial.println("Scanning...");
  for (address = 1; address < 127; address++){
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0){
      Serial.print("I2C device found at address 0x\n");
      if(address < 16){
        Serial.print("0");
        Serial.print(address,HEX);
        Serial.println(" !");
        Devices++;
      }
      else if(error == 4){
        Serial.print("Unknown error at address 0x");
      }
      if (Devices == 0){
        Serial.println("No I2C devices found\n");
      }
      else{
        Serial.println("done\n");
      }
      delay(5000);
    }
  }
*/

/* Checking if the power module is charging or not */
  count += 1;
  Serial.print("Count: ");
  Serial.print(count);
  Serial.print("\n");
  
  if (count == 10){
    if (sum == 0){
      digitalWrite(POWER_OUT,HIGH);
      delay(100);
      digitalWrite(POWER_OUT,LOW);
      delay(100);

      Serial.println("Iterated to next port");
    } else{
      sum = 0;
      Serial.println("Still charging...");
    }
  count = 0;
  }
}
