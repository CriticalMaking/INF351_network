/*
 * INF351_Control 
 * Matt Ratto 
 * Nov, 2022
 *
 * control code for INF351 Network 
 * connect all arduinos SDA/SCL/GND to shared breadboard 
 * Run this code on one arduino, run peripheral code on all others 
 * assign each peripheral a unique sequential address: 1,2,3, etc 
 * This code will check each peripheral for messages 
 * if message is available, control will send to correct peripheral
 * messages must be in format [peripheral address]+6 characters
 * if no message available, peripheral must send * as heartbeat
 * if any peripheral does not respond with message or heartbeat, 
 * network will hang
 */

#include <Wire.h>
char message[10]; // use global to avoid issues with char arrays
int devices[] = {1,2,3,4,5,6,7}; 
int numDevices = 7; 
int deviceNum = 0; 

void setup() {
  Wire.begin();        // join i2c bus (address optional for control)
  Serial.begin(9600);  // start serial for output
}

void loop() {
Serial.print("requesting from:"); 
Serial.println(devices[deviceNum]);
int received = requestMsg(devices[deviceNum],8); 
 if (strstr (message, "*")){
  Serial.print("Heartbeat from:"); 
  Serial.println(devices[deviceNum]); 
}else{ 
  int address = message[1]-48;
writePeriph(address, message); //send message to peripheral
} 
delay(1000); 
deviceNum++; 
if (deviceNum==numDevices) deviceNum=0; 
}

int requestMsg(int address, int num){
   int i=0;
   int received = 0; 
   Wire.requestFrom(address, num);    // request num bytes from peripheral device #8
   while (Wire.available()) { // peripheral may send less than requested
    char c = Wire.read(); // receive a byte as character
    //Serial.print(c);         // print the character
    message[i] = c;
    i++; 
   }
Serial.print("Received:"); 
Serial.println(message); 
return i; 
}

void writePeriph(int address, char sendData[7]){
 Serial.print("Sending:");
 Serial.print(sendData);
 Serial.print(" to:"); 
 Serial.println(address);  
 Wire.beginTransmission(address); // transmit to peripheral device
  for (int i=0;i<7;i++){
  Serial.print(sendData[i]); 
  Wire.write(sendData[i]);
  }
  Serial.println("end send data"); 
  Wire.endTransmission();    // stop transmitting 
}
