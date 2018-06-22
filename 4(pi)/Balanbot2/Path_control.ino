void Path_control()
{
  path++;
  //1300//475//4150
  if (path == 1)
    control('S');
  else if (path == 500)
    control('+');
  else if (path == 1800)
    control('S');
  else if (path == 2000)
    control('l');
  else if (path == 2475)
    control('S');
  else if (path == 2975)
    control('+');
  else if (path == 4275)
    control('S');
  else if (path == 4475)
    control('l');
  else if (path == 4950)
    control('S');
  else if (path == 5450)
    control('+');
  else if (path == 6100)
    control('S');
  else if (path == 6300)
    control('r');
  else if (path == 6775)
    control('S');
  else if (path == 7275)
    control('+');
  else if (path == 7925)
    control('S');
  else if (path == 8125)
    control('r');
  else if (path == 8600)
    control('S');
  else if (path == 9100)
    control('R');
  else if (path == 13250)
    control('S');
}
