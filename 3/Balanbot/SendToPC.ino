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
      String BTSerialData5="";
      String BTSerialData6="";
      
      BTSerialData1 = String(Angle_Car,2);
      BTSerialData2 = String(Speed_R,2);
      BTSerialData3 = String(Speed_L,2);
      BTSerialData4 = String(pos,2);
      BTSerialData5 = String(Et_turn);  //pos_R
      BTSerialData6 = String(pos_L,2);  //pos_L
      //Serial.print(Angle_Car);Serial.print(" ");
      //Serial.print(Speed_R);Serial.print(" ");
      //Serial.println(Speed_L);
      
      char* buf1 = (char*)malloc(sizeof(char)*BTSerialData1.length()+1);
      char* buf2 = (char*)malloc(sizeof(char)*BTSerialData2.length()+1);
      char* buf3 = (char*)malloc(sizeof(char)*BTSerialData3.length()+1);
      char* buf4 = (char*)malloc(sizeof(char)*BTSerialData4.length()+1);
      char* buf5 = (char*)malloc(sizeof(char)*BTSerialData5.length()+1);
      char* buf6 = (char*)malloc(sizeof(char)*BTSerialData6.length()+1);
      
      BTSerialData1.toCharArray(buf1, BTSerialData1.length()+1);
      BTSerialData2.toCharArray(buf2, BTSerialData2.length()+1);
      BTSerialData3.toCharArray(buf3, BTSerialData3.length()+1);
      BTSerialData4.toCharArray(buf4, BTSerialData4.length()+1);
      BTSerialData5.toCharArray(buf5, BTSerialData5.length()+1);
      BTSerialData6.toCharArray(buf6, BTSerialData6.length()+1);
      
      BTSerial2.write(buf1);BTSerial2.write(" ");
      BTSerial2.write(buf2);BTSerial2.write(" ");
      BTSerial2.write(buf3);BTSerial2.write(" ");
      BTSerial2.write(buf4);BTSerial2.write(" ");
      BTSerial2.write(buf5);BTSerial2.write(" ");
      BTSerial2.write(buf6);BTSerial2.write("\n");
      free(buf1);free(buf2);free(buf3);free(buf4);free(buf5);free(buf6);
    }
}
