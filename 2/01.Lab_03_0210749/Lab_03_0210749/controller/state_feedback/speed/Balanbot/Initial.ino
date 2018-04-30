void Init()
{
  pinMode(PWM_L, OUTPUT);
  pinMode(PWM_R, OUTPUT);
  pinMode(DIR_L1, OUTPUT);
  pinMode(DIR_L2, OUTPUT);
  pinMode(DIR_R1, OUTPUT);
  pinMode(DIR_R2, OUTPUT); 

  pinMode(SPD_INT_L2, INPUT);
  pinMode(SPD_INT_R2, INPUT);

  //initial variables
  Speed_L = 0;
  Speed_R = 0;
  car_position = 0;
  pwm = 0; pwm_l = 0; pwm_r = 0;
  Speed_Diff = 0; Speed_Diff_ALL = 0;
  Angle_Car_Adjustment = 0.7;
  Gyro_Car_Adjustment = 2.5;
  Speed_Need = 0; //速度 0~1.0 -->轉換
  Turn_Need = 0;  //旋轉角度 -179~+179 -->轉換
  encoderPosL = 0;
  encoderPosR = 0;
  
  ANG_OFFSET = 0;
  Et_wheel = 0;
  

  k1 = 3.1271;
  k2 = 9.1414;
  k3 = 0.2283;

  target_car_speed = 0;

    
  Et_total = 0;


  i2cData[0] = 7;    // Set the sample rate to 1000Hz - 8kHz/(7+1) = 1000Hz
  i2cData[1] = 0x00; // Disable FSYNC and set 260 Hz Acc filtering, 256 Hz Gyro filtering, 8 KHz sampling
  i2cData[2] = 0x00; // Set Gyro Full Scale Range to ±250deg/s
  i2cData[3] = 0x00; // Set Accelerometer Full Scale Range to ±2g
}
