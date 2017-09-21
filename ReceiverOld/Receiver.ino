 #define rfReceivePin A0  //RF Receiver pin = Analog pin 0
 #define ledPin 13        //Onboard LED = digital pin 13

 // 11-12 millis per cycle

 unsigned int rawData = 0;   // variable used to store received data
 unsigned int averagedData = 0;   // variable used to store received data
 bool dataBool = false;
 
 const unsigned int threshold = 300;  //upper threshold value

 void setup(){
   pinMode(ledPin, OUTPUT);
   Serial.begin(9600);
 }

 void loop(){
  rawData=analogRead(rfReceivePin);    //listen for data on Analog pin 0
  averagedData
  if(data < threshold)
  {
    dataBool = true;
  }
  else
  {
    dataBool = false;
  }
      
   
  if(dataBool)
  {
     digitalWrite(ledPin, HIGH);   //If a LOW signal is received, turn LED OFF
     Serial.println(data);  
   }
   else
   {
     digitalWrite(ledPin, LOW);   //If a HIGH signal is received, turn LED ON
     Serial.println(data);
   }
   
 }
