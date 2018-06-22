void Path_control()
{
   path++;
  if(path==1){  
      control('s');
      PWM_Cal();
   }
   else if(path<300)
      PWM_Cal();
   else if(path==300){
      control('+');
      PWM_Cal();
   }
   else if(path>300&&path<500)
      PWM_Cal();
      
   else if(path==500){
      control('S');
      PWM_Cal();
   }
   else if(path>500&&path<550)
      PWM_Cal();
    
   else if(path==550){
      control('R');
      PWM_cal_by_angle();
   }
   else if(path>550&&path<950)
      PWM_cal_by_angle();

   else if(path==950){
      control('S');
      PWM_Cal();
   }
   else if(path>950&&path<1000)
      PWM_Cal();
   
   else if(path==1000){
      control('+');
      PWM_Cal();
   }
   else if(path>1000&&path<1200)
      PWM_Cal();
  
   else if(path==1200){
      control('S');
      PWM_Cal();
   }
   else if(path>1200&&path<1250)
      PWM_Cal();
      
   else if(path==1250){
      control('l');
      PWM_cal_by_angle();
   }
   else if(path>1250&&path<2050)
      PWM_cal_by_angle();
      
   else if(path==2050){
      control('S');
      PWM_Cal();
   }
   else
      PWM_Cal();
}
