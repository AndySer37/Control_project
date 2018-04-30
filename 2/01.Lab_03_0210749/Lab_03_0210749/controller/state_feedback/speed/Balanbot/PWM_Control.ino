void PWM_Calculate()
{  

  // x_dot 速度 = (目前Encder值 - 前一刻Encder值) / 一圈Encoder數 * 輪子圓周長 / 取樣時間
  Speed_L = (encoderPosL - encoderL_past) / CPR  * PI * TIRE_RADIUS * 2 / (millis() - SpeedTimer) * 1000;  // cm/sec
  Speed_R = (encoderPosR - encoderR_past) / CPR * PI * TIRE_RADIUS * 2 / (millis() - SpeedTimer) * 1000;  // cm/sec


  
  SpeedTimer = millis();

  // 因為此次的Speed已經算完，所以時間要推進至下一刻
  encoderL_past = encoderPosL;
  encoderR_past = encoderPosR;
  
  Speed_LR = (Speed_L + Speed_R) * 0.5;  // 把左右輪速度相加的一半設為車子整體速度

  // Position
  car_position += Speed_LR*(micros()-dt)*0.000001;

  double P_wheel = 0, I_wheel = 0;
  double encoderError = (encoderPosL - encoderPosR);
  Et_wheel += encoderError*(micros()-dt)*0.000001;
  P_wheel = 2*encoderError;
  I_wheel = 4*Et_wheel;

  dt = micros();
  

  int temp2 = int(P_wheel + I_wheel);
  
  double temp;
  
  temp = (k1*(Speed_LR - target_car_speed) + k2*Angle_Car + k3*Gyro_Car);
  
  pwm = int(temp); // PWM = -K*x;
  //Serial.println(pwm);
  //Speed_Need  //Turn_Need  
  pwm_r = pwm+temp2;
  pwm_l = pwm-temp2;  
}
