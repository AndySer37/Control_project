void PWM_state_feedback()
{
  Speed_L = (encoderPosL - encoderL_past) / CPR * PI * 2 * TIRE_RADIUS / (millis() - SpeedTimer) * 1000; //cm/sec
  Speed_R = (encoderPosR - encoderR_past) / CPR * PI * 2 * TIRE_RADIUS / (millis() - SpeedTimer) * 1000; //cm/sec


  encoderR_past = encoderPosR;
  encoderL_past = encoderPosL;

  Speed_LR = (Speed_L + Speed_R) * 0.5;

  Speed_Diff = Speed_L - Speed_R;
  Speed_Diff_ALL += Speed_Diff;
  
  double temp;

  pos += (micros() - angle_dt) * Speed_LR / 1000000;
  //Serial.println(pos);
  double P_wheel = 0, I_wheel = 0;
  double encoderError = (encoderPosL - encoderPosR);
  Et_wheel += encoderError * (micros() - angle_dt) * 0.000001;
  P_wheel = 2 * encoderError;
  I_wheel = 4 * Et_wheel;
  int temp2 = int(P_wheel + I_wheel);


  temp = (k1 * (pos - 0) + k2 * Speed_LR + k3 * Angle_Car + k4 * Gyro_Car);     // state_feedback control
  pwm = int(temp);



  pwm_r = pwm   + temp2;
  pwm_l = pwm   - temp2;
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
