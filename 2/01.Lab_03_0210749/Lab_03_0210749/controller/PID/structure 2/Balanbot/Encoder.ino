void Encoder_L()   //讀取左輪的Encoder值
{
  if(digitalRead(SPD_INT_L2) == HIGH) 
  {
    encoderPosL = encoderPosL - 1; //反轉
  }
  else
  {
    encoderPosL = encoderPosL + 1; //正轉
  }

  if (encoderPosL >= 32000)
    encoderPosL = -32000;
  else if (encoderPosL <= -32000)
    encoderPosL = 32000;
}

void Encoder_R()   //讀取右輪的Encoder值
{
  if(digitalRead(SPD_INT_R2) == LOW)
  {
    encoderPosR = encoderPosR - 1; //反轉
  }
  else
  {
    encoderPosR = encoderPosR + 1; //正轉
  }

  if (encoderPosR >= 32000)
    encoderPosR = -32000;
  else if (encoderPosR <= -32000)
    encoderPosR = 32000;
}
