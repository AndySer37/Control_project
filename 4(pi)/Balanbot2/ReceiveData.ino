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
      else 
        control(val);
    }
  }
  //rpi connection
  if ((millis() - piTimer) > 1000) {
    pi_data[0] = 1;
    Wire.beginTransmission(9); // transmit to device #9
    Wire.write(1);           // sends 16 bytes
    Wire.requestFrom(9, 8);//address and bytes
    if (Wire.available()) {
      pi_data[0] = Wire.read();
    }
    Wire.endTransmission();    // stop transmitting
    control(pi_data[0]);
    Serial.println(pi_data[0]);
    Serial.println(10);
    piTimer = millis();
  }
}

void control(char val)
{
  if (val == '+') {
    if (state == 1) {
      pos = tempPos / -17, Et_wheel = 0, encoderPosR = 0, encoderPosL = 0, encoderL_past = 0, encoderR_past = 0; //pos = tempPos /-17;pos = 0;Et_wheel = 0;encoderPosR = 0;encoderPosL = 0;
      KA_P = 17;
      Et_total = 0;
      KA_I = 130;
      KA_D = 0.3;
      KP_P = 1.3;
    }
    else if (state == 0) {
      pos_it = 0, pos_itL = 0, pos_itR = 0,  Et_wheel = 0, encoderPosR = 0, encoderPosL = 0, encoderL_past = 0, encoderR_past = 0; //Et_total = 0;pos = 0;Et_wheel = 0;encoderPosR = 0;encoderPosL = 0;
    }
    state = 0;
    pos_count = 0.08;
  }
  else if (val == '-') {
    if (state == 1) {
      pos = tempPos / -17, Et_wheel = 0, encoderPosR = 0, encoderPosL = 0, encoderL_past = 0, encoderR_past = 0; //Et_total = 0;pos = 0;Et_wheel = 0;encoderPosR = 0;encoderPosL = 0;
      KA_P = 17;
      Et_total = 0;
      KA_I = 130;
      KA_D = 0.3;
      KP_P = 1.3;
    }
    else if (state == 0) {
      pos_it = 0, pos_itL = 0, pos_itR = 0,  Et_wheel = 0, encoderPosR = 0, encoderPosL = 0, encoderL_past = 0, encoderR_past = 0; //Et_total = 0;pos = 0;Et_wheel = 0;encoderPosR = 0;encoderPosL = 0;
    }
    state = 0;
    pos_count = -0.08;
  }
  else if (val == 'R')  {
    if (state == 1) {
      pos_it = 0, pos_itL = 0, pos_itR = 0, Et_wheel = 0, encoderPosR = 0, encoderPosL = 0, encoderL_past = 0, encoderR_past = 0; //Et_total = 0;pos = 0;Et_wheel = 0;encoderPosR = 0;encoderPosL = 0;
    }
    else if (state == 0) {
      pos_it = 0, pos_L = pos, pos_R = pos, Et_wheel = 0, encoderPosR = 0, encoderPosL = 0, encoderL_past = 0, encoderR_past = 0; //Et_total = 0;pos = 0;Et_wheel = 0;encoderPosR = 0;encoderPosL = 0;
      KA_P = 14;
      KA_I = 110;
      KA_D = 0.2;
      KP_P = 1.5;
    }
    state = 1;
    trun_Radius =45
    ;
    trun_direction2 = -1;
  }
  else if (val == 'L') {
    if (state == 1) {
      pos_it = 0, pos_itL = 0, pos_itR = 0, Et_wheel = 0, encoderPosR = 0, encoderPosL = 0, encoderL_past = 0, encoderR_past = 0; //Et_total = 0;pos = 0;Et_wheel = 0;encoderPosR = 0;encoderPosL = 0;
    }
    else if (state == 0) {
      pos_it = 0, pos_L = pos, pos_R = pos, Et_wheel = 0, encoderPosR = 0, encoderPosL = 0, encoderL_past = 0, encoderR_past = 0; //Et_total = 0;pos = 0;Et_wheel = 0;encoderPosR = 0;encoderPosL = 0;
      KA_P = 14;
      KA_I = 110;
      KA_D = 0.2;
      KP_P = 1.5;
    }
    state = 1;
    trun_Radius =16;
    trun_direction2 = 1;
  }
  else if (val == 'r') {
    if (state == 1) {
      pos_it = 0, pos_itL = 0, pos_itR = 0, Et_wheel = 0, encoderPosR = 0, encoderPosL = 0, encoderL_past = 0, encoderR_past = 0; //Et_total = 0;pos = 0;Et_wheel = 0;encoderPosR = 0;encoderPosL = 0;
    }
    else if (state == 0) {
      pos_it = 0, pos_L = pos, pos_R = pos, Et_wheel = 0, encoderPosR = 0, encoderPosL = 0, encoderL_past = 0, encoderR_past = 0; //Et_total = 0;pos = 0;Et_wheel = 0;encoderPosR = 0;encoderPosL = 0;
      KA_P = 14;
      KA_I = 110;
      KA_D = 0.2;
      KP_P = 1.5;
    }
    state = 1;
    trun_Radius = 0;
    trun_direction2 = -1;
  }
  else if (val == 'l') {
    if (state == 1) {
      pos_it = 0, pos_itL = 0, pos_itR = 0, Et_wheel = 0, encoderPosR = 0, encoderPosL = 0, encoderL_past = 0, encoderR_past = 0; //Et_total = 0;pos = 0;Et_wheel = 0;encoderPosR = 0;encoderPosL = 0;
    }
    else if (state == 0) {
      pos_it = 0, pos_L = pos, pos_R = pos, Et_wheel = 0, encoderPosR = 0, encoderPosL = 0, encoderL_past = 0, encoderR_past = 0; //Et_total = 0;pos = 0;Et_wheel = 0;encoderPosR = 0;encoderPosL = 0;
      KA_P = 14;
      KA_I = 110;
      KA_D = 0.2;
      KP_P = 1.5;
    }
    state = 1;
    trun_Radius = 0;
    trun_direction2 = 1;
  }
  else if (val == 'S')  {
    if (state == 1) {
      pos = tempPos / -17, Et_wheel = 0, encoderPosR = 0, encoderPosL = 0, encoderL_past = 0, encoderR_past = 0, Et_total = 0; //Et_total = 0;pos = 0;Et_wheel = 0;encoderPosR = 0;encoderPosL = 0;
      KA_P = 17;
      KA_I = 130;
      KA_D = 0.3;
      KP_P = 1.3;
    }
    else if (state == 0) {
      pos_it = 0, pos_itL = 0, pos_itR = 0,  Et_wheel = 0, encoderPosR = 0, encoderPosL = 0, encoderL_past = 0, encoderR_past = 0; //Et_total = 0;pos = 0;Et_wheel = 0;encoderPosR = 0;encoderPosL = 0;
    }
    state = 0;
    pos_count = 0;
  }
  else if (state == 1 && val == '0') {
    pos_it = 0, pos_itL = 0, pos_itR = 0, Et_wheel = 0, encoderPosR = 0, encoderPosL = 0, encoderL_past = 0, encoderR_past = 0;
    trun_Radius = 0;
  }
  else if (state == 1 && val == '8') {
    pos_it = 0, pos_itL = 0, pos_itR = 0, Et_wheel = 0, encoderPosR = 0, encoderPosL = 0, encoderL_past = 0, encoderR_past = 0;
    trun_Radius = 8;
  }
}
