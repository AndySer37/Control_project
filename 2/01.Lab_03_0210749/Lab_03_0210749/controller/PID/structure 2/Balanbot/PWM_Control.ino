void PWM_Calculate()
{ 
  // 速度 = (目前Encder值 - 前一刻Encder值) / 一圈Encoder數 * 輪子圓周長 / 取樣時間
  Speed_L = (encoderPosL - encoderL_past) / CPR * PI * TIRE_RADIUS * 2 / (millis() - SpeedTimer) * 1000;  // cm/sec
  Speed_R = (encoderPosR - encoderR_past) / CPR * PI * TIRE_RADIUS * 2 / (millis() - SpeedTimer) * 1000;  // cm/sec

  SpeedTimer = millis();

  // 因為此次的Speed已經算完，所以時間要推進至下一刻
  encoderL_past = encoderPosL;
  encoderR_past = encoderPosR;
  
  Speed_LR = (Speed_L + Speed_R) * 0.5;  // 把左右輪速度相加的一半設為車子整體速度
  
  Speed_Diff = Speed_L - Speed_R;
  Speed_Diff_ALL += Speed_Diff;
 
  double temp;
  double Pt = 0, It = 0, Dt = 0;
  double Pt_p = 0, It_p = 0, Dt_p = 0;

  // 位置控制
  // Car Position
  car_position += Speed_LR*(micros()-angle_dt)*0.000001;
  Pt_p = -KP_P * (car_position);
  //It_p = KP_I * car_position_int;
  //Dt_p = KP_D * Speed_LR;
  //car_position_int += (car_position)*(micros()-angle_dt)*0.000001;
  double tempPos = Pt_p;

  // 角度控制
  Pt = -KA_P * (tempPos - Angle_Car);
  It = -KA_I * (Et_total);
  Dt = KA_D * Gyro_Car;
  Et_total += (tempPos - Angle_Car)*(micros()-angle_dt)*0.000001;  // 單位換算:micros --> sec

  double P_wheel = 0, I_wheel = 0;

  double encoderError = (encoderPosL - encoderPosR);
  Et_wheel += encoderError*(micros()-angle_dt)*0.000001;
  P_wheel = 2*encoderError;
  I_wheel = 4*Et_wheel;
  
  angle_dt = micros();

  int temp2 = int(P_wheel + I_wheel);
  temp = Pt + It + Dt;
  pwm = int(temp); // PWM = Pt+It+Dt
  //Serial.println(pwm);
  //Speed_Need  //Turn_Need  
  pwm_r = pwm+temp2;
  pwm_l = pwm-temp2;  
}