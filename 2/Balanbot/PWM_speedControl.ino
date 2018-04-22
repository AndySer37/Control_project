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
  
  Pt = KA_P * Angle_Car;
  It = KA_I * (Et_total);
  Dt = KA_D * Gyro_Car;
  Et_total += Angle_Car * (micros() - angle_dt) * 0.000001; //micros --> sec
  if (Et_total > 2)
    Et_total = 2;
  else if (Et_total < -2)
    Et_total = -2;
  

  //Serial.println(Angle_Car);
  //Serial.println(Gyro_Car);
  //Serial.println(pwm);
  //Serial.println(Et_total);
  //Speed_Need  //Turn_Need

  pos += (micros() - angle_dt) * Speed_LR / 1000000;
  Serial.println(pos);
  pt = -KP_P * pos;
  it = KP_I * (pos_it);
  dt = KP_D * Speed_LR;
  pos_it += pos * (micros() - angle_dt) * 0.000001; //micros --> sec

  temp = Pt + It + Dt + pt + it + dt;
  pwm = int(temp);
  pwm_r = pwm ;
  pwm_l = pwm ;
  if (pwm_r > 80)
    pwm_r = 80;
  else if (pwm_r < -80)
    pwm_r = -80;
  if (pwm_l > 80)
    pwm_l = 80;
  else if (pwm_l < -80)
    pwm_l = -80;
  angle_dt = micros();
  SpeedTimer = millis();
}
