clear;
load('parameters.mat');

base = sig(5)*(1-rho(3)/(rho(1)+rho(2)));
A3 = (sig(1)+2*sig(2)-1/(rho(1)+rho(2)))/base;
A2 = (rho(3)/(rho(1)+rho(2))-sig(3)-rho(3)/(rho(1)+rho(2))*(sig(1)+2*sig(2)+sig(4)/rho(3)))/base;
A1 = (rho(4)/(rho(1)+rho(2))+rho(3)/(rho(1)+rho(2))*(sig(3)+sig(4)))/base;
A0 = (sig(4)*rho(4)/(rho(1)+rho(2)))/base;


poles = [-8 -32 -64];
% poles = [-16 -32 -64];

Kd = (A3*(poles(1)+poles(2)+poles(3))+A2)*pi/180;
Kp = (A1-A3*(poles(1)*poles(2)+poles(1)*poles(3)+poles(2)*poles(3)))*pi/180;
Ki = (A3*(poles(1)*poles(2)*poles(3))+A0)*pi/180;

display(Kd);
display(Kp);
display(Ki);



sys = tf([1/A3 0], [1 A2/A3 A1/A3 A0/A3]);
pole(sys)
contr = tf([Kd Kp Ki],[1 0]);
% g = sys*contr;
% rlocus(sys)