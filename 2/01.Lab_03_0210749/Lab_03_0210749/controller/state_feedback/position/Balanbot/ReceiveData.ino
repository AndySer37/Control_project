void ReceiveData(int interval_receive) {
    if( (millis() - SendTimer) > interval_receive)
    {
        char val;
        // 若收到藍牙模組的資料，則送到「序列埠監控視窗」
        while (BTSerial2.available()) {
          val = BTSerial2.read();
          str_receive[len_receive++] = val;
          if(len_receive>=128)
          {
            len_receive = 0;
            break;
          }
          if(val == ']')
          {
              //BTSerial2.println(str_receive);
              //CheckIfJoystick();
              CheckIfPosition();
              for (int i = 0;i < len_receive;i++)
              {
                str_receive[i] = 0;
              }
              len_receive = 0;//歸零
          }
          else
              ;
              //Serial.print(val);
        }
    }
}
