#include <SoftwareSerial.h>
#include <Wire.h>
#include <math.h>
#include "Kalman.h"
#include "I2C.h"
#include "MPU6050.h"
MPU6050 Gyro6050;

//bluetooth receive text
#define MAX_BTCMDLEN 128
char str_receive[MAX_BTCMDLEN]; // received 128 bytes from an Android system
byte str_send[MAX_BTCMDLEN]; // sent 128 bytes from an Arduino system
int len_send = 0; //要傳送的字串長度
int len_receive = 0; //要接收的字串長度
SoftwareSerial BTSerial(A0, 13); // Arduino RX/TX    //to Phone
SoftwareSerial BTSerial2(A0, 13); // Arduino RX/TX   //to PC
Kalman kalmanX; // Create the Kalman instances
Kalman kalmanY;

/* IMU Data */
double accX, accY, accZ;
double gyroX, gyroY, gyroZ;
int16_t tempRaw;

double gyroXangle, gyroYangle; // Angle calculate using the gyro only
double compAngleX, compAngleY; // Calculated angle using a complementary filter
double kalAngleX, kalAngleY; // Calculated angle using a Kalman filter

double k1;
double k2;
double k3;
double k4;

uint32_t timer;
uint8_t i2cData[14]; // Buffer for I2C data

//motor  define
#define PWM_L 10  //PWMA
#define PWM_R 5   //PWMB
#define DIR_L1 4
#define DIR_L2 12
#define DIR_R1 7
#define DIR_R2 8
#define STBY 6 //standby

//encoder define
#define SPD_INT_R 3   //interrupt 
#define SPD_INT_L 2   //interrupt
#define SPD_INT_R2 11
#define SPD_INT_L2 9

#define CPR 374.0 //count per round
#define TIRE_RADIUS 3.45 //3.45cm
double Et_wheel;
float Speed_L, Speed_R;
int pwm, pwm_l, pwm_r;
int pwm_ll, pwm_rr;
int trun_direction;    //+1 OR -1
double Angle_Car, trun_Radius, car_half_distance; //車子傾斜角
double Angle_Car_Adjustment;
double Gyro_Car;
double Gyro_Car_Adjustment;
float Speed_LR;
float pos, pos_L , pos_R ,pos_count;
double KA_P, KA_I, KA_D;
double KP_P, KP_I, KP_D;
double ANG_OFFSET;
double Et_total, pos_it, Et_turn, pos_itL, pos_itR;

float Speed_Need, Turn_Need;
int Speed_Diff, Speed_Diff_ALL;
int state;
uint32_t angle_dt;
uint32_t SendTimer;
uint32_t ReceiveTimer;
uint32_t SendToPCTimer;
uint32_t SpeedTimer;

int encoderPosL, encoderPosR;
int encoderR_past, encoderL_past;

bool First_Time = true;

void setup() {

  Serial.begin(9600);
  BTSerial.begin(38400); // NANO 沒辦法同時處理兩個BT UART傳輸  //bt1
  //BTSerial2.begin(38400);   //bt2
  Init();
  Wire.begin();
  TWBR = ((F_CPU / 400000L) - 16) / 2; // Set I2C frequency to 400kHz

  while (i2cWrite(0x19, i2cData, 4, false)); // Write to all four registers at once
  while (i2cWrite(0x6B, 0x01, true)); // PLL with X axis gyroscope reference and disable sleep mode
  while (i2cRead(0x75, i2cData, 1));

  if (i2cData[0] != 0x68) { // Read "WHO_AM_I" register
    Serial.print(F("Error reading sensor"));
    while (1);
  }
  delay(200); // Wait for sensor to stabilize

  /* Set kalman and gyro starting angle */
  while (i2cRead(0x3B, i2cData, 6));

  accX = (i2cData[0] << 8) | i2cData[1];
  accY = (i2cData[2] << 8) | i2cData[3];
  accZ = (i2cData[4] << 8) | i2cData[5];

  // Source: http://www.freescale.com/files/sensors/doc/app_note/AN3461.pdf eq. 25 and eq. 26
  // atan2 outputs the value of -π to π (radians) - see http://en.wikipedia.org/wiki/Atan2
  // It is then converted from radians to degrees
#ifdef RESTRICT_PITCH // Eq. 25 and 26
  double roll  = atan2(accY, accZ) * RAD_TO_DEG;
  double pitch = atan(-accX / sqrt(accY * accY + accZ * accZ)) * RAD_TO_DEG;
#else // Eq. 28 and 29
  double roll  = atan(accY / sqrt(accX * accX + accZ * accZ)) * RAD_TO_DEG;
  double pitch = atan2(-accX, accZ) * RAD_TO_DEG;
#endif

  kalmanX.setAngle(roll); // Set starting angle
  kalmanY.setAngle(pitch);
  gyroXangle = roll;
  gyroYangle = pitch;
  compAngleX = roll;
  compAngleY = pitch;

  attachInterrupt(0, Encoder_L, RISING);
  attachInterrupt(1, Encoder_R, RISING);

  timer = micros();
  angle_dt = micros();
  SendTimer = millis();
  ReceiveTimer = millis();
  SendToPCTimer = millis();
  SpeedTimer = millis();
}

void loop() {

  double DataAvg[3];
  double AngleAvg = 0;

  DataAvg[0] = 0; DataAvg[1] = 0; DataAvg[2] = 0;

  while (1)
  {
    if (UpdateAttitude())
    {
      DataAvg[2] = DataAvg[1];
      DataAvg[1] = DataAvg[0];
      DataAvg[0] = Angle_Car;
      AngleAvg = (DataAvg[0] + DataAvg[1] + DataAvg[2]) / 3;

      if (First_Time)
      {
        Angle_Car_Adjustment = -AngleAvg;
        DataAvg[2] = 0;
        DataAvg[1] = 0;
        DataAvg[0] = 0;
        Angle_Car = 0;
        First_Time = false;
      }
      else if (AngleAvg < 45 || AngleAvg > -45) {
        //PWM_Calculate();
        if (state == 0 )
          PWM_state_feedback();
         else if (state == 1)
          PWM_cal_by_angle();
        
        Car_Control();
      }
  
    }
    Serial.println(Et_total);
    SendData(250); //250ms     //bt1
    ReceiveData(200); //200ms  //bt1

    //SendToPC(50);  //bt2
    //SendToPC2(50);
  }
}