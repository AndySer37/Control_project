//This is master pc
#include <SoftwareSerial.h>
SoftwareSerial BTSerial(10,11);
String comdata = "";
void setup()
{
  Serial.begin(9600); 
  BTSerial.begin(38400); 
}
void loop(){
 
  
  
  if(BTSerial.available()){
   
    char BTSerial_Buffer = BTSerial.read();
    if(BTSerial_Buffer != '\n')
    {
        comdata += BTSerial_Buffer;
    }
    else
    {
      Serial.println(comdata);
      comdata = "";
    }
  }
  
}


