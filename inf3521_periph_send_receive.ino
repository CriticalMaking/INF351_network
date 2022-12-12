/* 
 *  INF351 Peripheral Send and Receive
 *  Matt Ratto 
*   Nov, 2022
*   
*   peripheral code for INF351 Network
*   connect all arduinos SDA/SCL/GND to shared breadboard
*   this peripheral code requires use of one arduino running control
*   assign each peripheral a unique sequential address: 1,2,3, etc 
*   this code responds to message requests from control
*   messages must be in format [pheripheral address]+6 characters
*   if no message available, peripheral must send * as heartbeat
*   if any peripheral does not respond with message or heartbeat, 
 *  network will hang
*/

#include <Wire.h>
int myAddress = 2; 
char * myMsg;
char receiveMsg[6];  

void setup() {
  Serial.begin(9600); 
  Wire.begin(myAddress);                // join i2c bus with address #8
  Wire.onRequest(requestEvent); // register event
  Wire.onReceive(receiveEvent); 
}

void loop() {
  delay(100);
  myMsg = "3hello"; //change myMsg when want to send to another peripheral
  delay(1000); 
}

// function that executes whenever data is requested by control
// this function is registered as an event, see setup()
void requestEvent() {
  Serial.print("Sending:");
  Serial.print(myMsg); 
  Serial.print(" from:"); 
  Serial.println(myAddress); 
  Wire.write(myAddress); 
  for (int i=0; i<7;i++){
  Wire.write(myMsg[i]);
  }
}

// function that executes whenever data is sent by control
// this function is registered as an event, see setup()
void receiveEvent(int howMany)
{
  int i = 0; 
  while(Wire.available()) // loop through all but the last
  {
    char c = Wire.read(); // receive byte as a character
    //Serial.print(c);         // print the character
    receiveMsg[i] = c;
    i++;
    //Serial.print(i);   
  }
  Serial.print("received:"); 
  Serial.println(receiveMsg);
  // Put code to execute when receive message here
  if (strstr (receiveMsg, "hello")){
  Serial.println("hello received");     
  }
}
