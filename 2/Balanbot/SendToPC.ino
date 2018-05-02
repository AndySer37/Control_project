void SendToPC(int interval_send)
{
  if( (millis() - SendToPCTimer) > interval_send)
    {
      SendToPCTimer = millis();
    
      String BTSerialData1="";
      String BTSerialData2="";
      String BTSerialData3="";
      
      BTSerialData1 = String(Angle_Car,2);
      BTSerialData2 = String(Speed_R,DEC);
      BTSerialData3 = String(Speed_L,DEC);

      //Serial.print(Angle_Car);Serial.print(" ");
      //Serial.print(Speed_R);Serial.print(" ");
      //Serial.println(Speed_L);
      
      char* buf1 = (char*)malloc(sizeof(char)*BTSerialData1.length()+1);
      char* buf2 = (char*)malloc(sizeof(char)*BTSerialData2.length()+1);
      char* buf3 = (char*)malloc(sizeof(char)*BTSerialData3.length()+1);
      
      BTSerialData1.toCharArray(buf1, BTSerialData1.length()+1);
      BTSerialData2.toCharArray(buf2, BTSerialData2.length()+1);
      BTSerialData3.toCharArray(buf3, BTSerialData3.length()+1);
      
      BTSerial2.write(buf1);BTSerial2.write(" ");
      BTSerial2.write(buf2);BTSerial2.write(" ");
      BTSerial2.write(buf3);BTSerial2.write("\n");
      
      free(buf1);free(buf2);free(buf3);
    }
}
void SendToPC2(int interval_send)
{
  if( (millis() - SendToPCTimer) > interval_send)
    {
      SendToPCTimer = millis();
    
      String BTSerialData1="";
      String BTSerialData2="";
      String BTSerialData3="";
      String BTSerialData4="";
      
      BTSerialData1 = String(Angle_Car,2);
      BTSerialData2 = String(Speed_R,DEC);
      BTSerialData3 = String(Speed_L,DEC);
      BTSerialData4 = String(pos,DEC);
      //Serial.print(Angle_Car);Serial.print(" ");
      //Serial.print(Speed_R);Serial.print(" ");
      //Serial.println(Speed_L);
      
      char* buf1 = (char*)malloc(sizeof(char)*BTSerialData1.length()+1);
      char* buf2 = (char*)malloc(sizeof(char)*BTSerialData2.length()+1);
      char* buf3 = (char*)malloc(sizeof(char)*BTSerialData3.length()+1);
      char* buf4 = (char*)malloc(sizeof(char)*BTSerialData4.length()+1);
      
      BTSerialData1.toCharArray(buf1, BTSerialData1.length()+1);
      BTSerialData2.toCharArray(buf2, BTSerialData2.length()+1);
      BTSerialData3.toCharArray(buf3, BTSerialData3.length()+1);
      BTSerialData4.toCharArray(buf4, BTSerialData4.length()+1);
      
      BTSerial2.write(buf1);BTSerial2.write(" ");
      BTSerial2.write(buf2);BTSerial2.write(" ");
      BTSerial2.write(buf3);BTSerial2.write(" ");
      BTSerial2.write(buf4);BTSerial2.write("\n");
      free(buf1);free(buf2);free(buf3);free(buf4);
    }
}
