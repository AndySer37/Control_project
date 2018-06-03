void PWM_Cal()
{
  Speed_L = (encoderPosL - encoderL_past) / CPR * PI * 2 * TIRE_RADIUS / (millis() - SpeedTimer) * 1000; //cm/sec
  Speed_R = (encoderPosR - encoderR_past) / CPR * PI * 2 * TIRE_RADIUS / (millis() - SpeedTimer) * 1000; //cm/sec


  encoderR_past = encoderPosR;
  encoderL_past = encoderPosL;

  Speed_LR = (Speed_L + Speed_R) * 0.5;

  Speed_Diff = Speed_L - Speed_R;
  Speed_Diff_ALL += Speed_Diff;


  double temp;
  double Pt = 0, It = 0, Dt = 0;
  double pt = 0, it = 0, dt = 0;
  
  pos += (micros() - angle_dt) * Speed_LR / 1000000;
  pos += pos_count;
  //Serial.println(pos);
  pt = -KP_P * pos;
  it = KP_I * (pos_it);
  dt = KP_D * Speed_LR;
  pos_it += pos * (micros() - angle_dt) * 0.000001; //micros --> sec
  double tempPos = pt;
Serial.println(Angle_Car);
  Pt = -KA_P * (tempPos - Angle_Car);
  It = -KA_I * (Et_total);
  Dt = KA_D * Gyro_Car;
  Et_total += (0.2 * tempPos - Angle_Car) * (micros() - angle_dt) * 0.000001; //micros --> sec
 // Et_total += (- Angle_Car) * (micros() - angle_dt) * 0.000001; //micros --> sec
  if (Et_total > 2)
    Et_total = 2;
  else if (Et_total < -2)
    Et_total = -2;


  double P_wheel = 0, I_wheel = 0;
  double encoderError = (encoderPosL - encoderPosR);
  Et_wheel += encoderError * (micros() - angle_dt) * 0.000001;
  P_wheel = 2 * encoderError;
  I_wheel = 4 * Et_wheel;
  int temp2 = int(P_wheel + I_wheel);
  //temp2 = 0;
  //Serial.println(Angle_Car);
  //Serial.println(Gyro_Car);
  //Serial.println(pwm);
  //Serial.println(Et_total);
  //Speed_Need  //Turn_Need

  temp = Pt + It + Dt ;
  pwm = int(temp);
  
  //int t = 2 / 3;
  
//  if (pwm > 12)
//    pwm_r =  (pwm - 12) * t + 12  ;
//  else if (pwm < -12)
//    pwm_r =  (pwm + 12) * t - 12  ;
//  else
//    pwm_r = pwm;

  pwm_r = pwm   + temp2;
  pwm_l = pwm   - temp2;
  if (pwm_r > 160)
    pwm_r = 160;
  else if (pwm_r < -160)
    pwm_r = -160;
  if (pwm_l > 160)
    pwm_l = 160;
  else if (pwm_l < -160)
    pwm_l = -160;
  angle_dt = micros();
  SpeedTimer = millis();
}
