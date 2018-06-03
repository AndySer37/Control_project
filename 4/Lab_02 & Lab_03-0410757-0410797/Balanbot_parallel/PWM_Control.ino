void PWM_Calculate()
{
  Speed_L = (encoderPosL - encoderL_past) / CPR * PI * 2 * TIRE_RADIUS / (millis() - SpeedTimer) * 1000; //cm/sec
  Speed_R = (encoderPosR - encoderR_past) / CPR * PI * 2 * TIRE_RADIUS / (millis() - SpeedTimer) * 1000; //cm/sec

  SpeedTimer = millis();

  encoderR_past = encoderPosR;
  encoderL_past = encoderPosL;

  Speed_LR = (Speed_L + Speed_R) * 0.5;

  Speed_Diff = Speed_L - Speed_R;
  Speed_Diff_ALL += Speed_Diff;


  double temp;
  double Pt = 0, It = 0, Dt = 0;

  Pt = KA_P * Angle_Car;
  It = KA_I * (Et_total);
  Dt = KA_D * Gyro_Car;
  Et_total += Angle_Car * (micros() - angle_dt) * 0.000001; //micros --> sec
  if (Et_total > 2)
    Et_total = 2;
  else if (Et_total < -2)
    Et_total = -2;
  angle_dt = micros();

  temp = Pt + It + Dt;
  pwm = int(temp);
  //Serial.println(pwm);
  //Serial.println(Et_total);
  //Speed_Need  //Turn_Need
  pwm_r = pwm;
  pwm_l = pwm ;
  if (pwm_r > 80)
    pwm_r = 80;
  else if (pwm_r < -80)
    pwm_r = -80;
  if (pwm_l > 80)
    pwm_l = 80;
  else if (pwm_l < -80)
    pwm_l = -80;
}
void PWM_cal_by_angle()
{
  Speed_L = (encoderPosL - encoderL_past) / CPR * PI * 2 * TIRE_RADIUS / (millis() - SpeedTimer) * 1000; //cm/sec
  Speed_R = (encoderPosR - encoderR_past) / CPR * PI * 2 * TIRE_RADIUS / (millis() - SpeedTimer) * 1000; //cm/sec

  SpeedTimer = millis();

  encoderR_past = encoderPosR;
  encoderL_past = encoderPosL;

  Speed_LR = (Speed_L + Speed_R) * 0.5;

  Speed_Diff = Speed_L - Speed_R;
  Speed_Diff_ALL += Speed_Diff;


  double temp;
  double Pt = 0, It = 0, Dt = 0;
  double pt_L = 0, it_L = 0, dt_L = 0;
  double pt_R = 0, it_R = 0, dt_R = 0;
  double deno =  abs(trun_Radius + car_half_distance) + abs(trun_Radius - car_half_distance);

  pos_L += (micros() - angle_dt) * Speed_L / 1000000;
  pos_L += (trun_Radius + trun_direction2 * car_half_distance) * trun_direction / (deno * 12);
  //Serial.println(pos);
  pt_L = -KP_P * pos_L;
  it_L = KP_I * (pos_itL);
  dt_L = KP_D * Speed_L;
  pos_itL += pos_L * (micros() - angle_dt) * 0.000001; //micros --> sec

  pos_R += (micros() - angle_dt) * Speed_R / 1000000;
  pos_R += (trun_Radius - trun_direction2 * car_half_distance) * trun_direction / (deno * 12);
  //Serial.println(pos);
  pt_R = -KP_P * pos_R;
  it_R = KP_I * (pos_itR);
  dt_R = KP_D * Speed_R;
  pos_itR += pos_R * (micros() - angle_dt) * 0.000001; //micros --> sec

  double diff =  trun_direction2 * (pt_L - pt_R) / 2;
  double tempPos = pt_R;

  Pt = -KA_P * (tempPos - Angle_Car);
  It = -KA_I * (Et_total);
  Dt = KA_D * Gyro_Car;
  Et_total += (0.2 * tempPos - Angle_Car) * (micros() - angle_dt) * 0.000001; //micros --> sec

  double P_wheel = 0, I_wheel = 0;
  //Et_turn += (diff * 2 / (-KP_P) - car_half_distance * trun_direction / (deno * 10)) * (micros() - angle_dt) * 0.000001;
  P_wheel = 2 * diff;
  //I_wheel = 0.1 * Et_turn;
  int temp2 = int(P_wheel + I_wheel);

  if (Et_total > 2)
    Et_total = 2;
  else if (Et_total < -2)
    Et_total = -2;
  angle_dt = micros();

  temp = Pt + It + Dt;
  pwm = int(temp);
  //Serial.println(pwm);
  //Serial.println(Et_total);
  //Speed_Need  //Turn_Need
  if ( trun_direction2 == 1) {
    pwm_r = pwm ;
    pwm_l = pwm + 2* temp2;
  }
   else {
    pwm_r = pwm + 2* temp2;
    pwm_l = pwm ;
  }
  if (pwm_r > 160)
    pwm_r = 160;
  else if (pwm_r < -160)
    pwm_r = -160;
  if (pwm_l > 160)
    pwm_l = 160;
  else if (pwm_l < -160)
    pwm_l = -160;
}
