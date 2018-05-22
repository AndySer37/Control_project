void ReceiveData(int interval_receive) {
  if ( (millis() - SendTimer) > interval_receive)
  {
    char val;
    // 若收到藍牙模組的資料，則送到「序列埠監控視窗」
    if (BTSerial.available()) {
      val = BTSerial.read();
      str_receive[len_receive++] = val;
      if (len_receive >= 128)len_receive = 0;
      if (val == ']')
      {
        Serial.println(val);
        CheckIfJoystick();
        CheckIfPID();

        len_receive = 0;//歸零
      }
      else if (val == '~') {
        Serial.println(val);
        CheckIfJoystick();
        CheckIfPID();
        //Car_Control_bt();
        len_receive = 0;//歸零
      }
      else if (val == '[') {
        Serial.println(val);
        CheckIfJoystick();
        CheckIfPID();
        //Car_Control_bt();
        len_receive = 0;//歸零
      }
      else if (val == '+') {
        if (state == 1) {
          pos = (pos_L + pos_R ) / 2, Et_wheel = 0, encoderPosR = 0, encoderPosL = 0, encoderL_past = 0, encoderR_past = 0; //Et_total = 0;pos = 0;Et_wheel = 0;encoderPosR = 0;encoderPosL = 0;
          KA_P = 16;   
          KA_I = 140;
        }
        else if (state == 0) {
          pos_it = 0, pos_itL = 0, pos_itR = 0,  Et_wheel = 0, encoderPosR = 0, encoderPosL = 0, encoderL_past = 0, encoderR_past = 0; //Et_total = 0;pos = 0;Et_wheel = 0;encoderPosR = 0;encoderPosL = 0;
        }
        state = 0;
        pos_count = 0.15;
      }
      else if (val == '-') {
        if (state == 1) {
          pos = (pos_L + pos_R ) / 2, Et_wheel = 0, encoderPosR = 0, encoderPosL = 0, encoderL_past = 0, encoderR_past = 0; //Et_total = 0;pos = 0;Et_wheel = 0;encoderPosR = 0;encoderPosL = 0;
          KA_P = 16;   
          KA_I = 140;
        }
        else if (state == 0) {
          pos_it = 0, pos_itL = 0, pos_itR = 0,  Et_wheel = 0, encoderPosR = 0, encoderPosL = 0, encoderL_past = 0, encoderR_past = 0; //Et_total = 0;pos = 0;Et_wheel = 0;encoderPosR = 0;encoderPosL = 0;
        }
        state = 0;
        pos_count = -0.15;
      }
      else if (val == 'R') {
        if (state == 1) {
          pos_it = 0, pos_itL = 0, pos_itR = 0, Et_wheel = 0, encoderPosR = 0, encoderPosL = 0, encoderL_past = 0, encoderR_past = 0; //Et_total = 0;pos = 0;Et_wheel = 0;encoderPosR = 0;encoderPosL = 0;
        }
        else if (state == 0) {
          pos_it = 0, pos_L = pos, pos_R = pos, Et_wheel = 0, encoderPosR = 0, encoderPosL = 0, encoderL_past = 0, encoderR_past = 0; //Et_total = 0;pos = 0;Et_wheel = 0;encoderPosR = 0;encoderPosL = 0;
          KA_P = 18;   
          KA_I = 155;
        }
        state = 1;
        //trun_Radius = -8;
        trun_direction2 = -1;
      }
      else if (val == 'L') {
        if (state == 1) {
          pos_it = 0, pos_itL = 0, pos_itR = 0, Et_wheel = 0, encoderPosR = 0, encoderPosL = 0, encoderL_past = 0, encoderR_past = 0; //Et_total = 0;pos = 0;Et_wheel = 0;encoderPosR = 0;encoderPosL = 0;
        }
        else if (state == 0) {
          pos_it = 0, pos_L = pos, pos_R = pos, Et_wheel = 0, encoderPosR = 0, encoderPosL = 0, encoderL_past = 0, encoderR_past = 0; //Et_total = 0;pos = 0;Et_wheel = 0;encoderPosR = 0;encoderPosL = 0;
          KA_P = 18;   
          KA_I = 155;
        }
        state = 1;
        //trun_Radius = 8;
        trun_direction2 = 1;
      }
      else if (val == 'S') {
        if (state == 1) {
          pos = (pos_L + pos_R ) / 2, Et_wheel = 0, encoderPosR = 0, encoderPosL = 0, encoderL_past = 0, encoderR_past = 0; //Et_total = 0;pos = 0;Et_wheel = 0;encoderPosR = 0;encoderPosL = 0;
          KA_P = 16;   
          KA_I = 140;
        }
        else if (state == 0) {
          pos_it = 0, pos_itL = 0, pos_itR = 0,  Et_wheel = 0, encoderPosR = 0, encoderPosL = 0, encoderL_past = 0, encoderR_past = 0; //Et_total = 0;pos = 0;Et_wheel = 0;encoderPosR = 0;encoderPosL = 0;
        }
        state = 0;
        pos_count = 0;
      }
      else {
        Serial.print(val);
      }
    }
  }
  if(1)
  {
    Wire.begin(0x04);                // join i2c bus with address #8
    Wire.onRequest(requestEvent); // register event
    Wire.onReceive(receiveEvent);

    
  }
}

/*void receiveEvent(int howMany) {
  while (1 < Wire.available()) { // loop through all but the last
    char c = Wire.read(); // receive byte as a character
    if(c=='s'){
       if (state == 1) {
          pos = (pos_L + pos_R ) / 2, Et_wheel = 0, encoderPosR = 0, encoderPosL = 0, encoderL_past = 0, encoderR_past = 0; //Et_total = 0;pos = 0;Et_wheel = 0;encoderPosR = 0;encoderPosL = 0;
        }
        else if (state == 0) {
          pos_it = 0, pos_itL = 0, pos_itR = 0,  Et_wheel = 0, encoderPosR = 0, encoderPosL = 0, encoderL_past = 0, encoderR_past = 0; //Et_total = 0;pos = 0;Et_wheel = 0;encoderPosR = 0;encoderPosL = 0;
        }
        state = 0;
        pos_count = 0;
    }
  }
}*/
void receiveEvent(int howMany) {
  while (1 < Wire.available()) { // loop through all but the last
    char c = Wire.read(); // receive byte as a character
    Serial.print(c);         // print the character
  }
  int x = Wire.read();    // receive byte as an integer
  Serial.println(x);         // print the integer
}
void requestEvent() {
  Wire.write(9); // respond with message of 6 bytes
  // as expected by master
}
