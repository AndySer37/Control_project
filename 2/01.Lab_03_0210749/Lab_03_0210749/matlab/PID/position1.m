
clear;
load('parameters.mat');


base = (1-(rho(1)+rho(2))*(sig(1)+2*sig(2)));

A1 = (-(sig(1)+2*sig(2))*rho(4))/base;
A2 = (-(sig(4)+(sig(1)+2*sig(2))*rho(3)))/base;
A3 = (sig(3)-(sig(1)+2*sig(2))*rho(3))/base;
C1 = sig(5)/base;

B1 = rho(4)/base;
B2 = (rho(3)+(rho(1)+rho(2))*sig(4))/base;
B3 = (rho(3)-(rho(1)+rho(2))*sig(3))/base;
C2 = (-(rho(1)+rho(2))*sig(5))/base;

% x_dot = Ax + Bu
% y = Cx + Du
A = [ 0 0 1 0;
      0 0 0 1;
      0 A1 A2 A3;
      0 B1 B2 B3;];
B = [0;0;C1;C2];

C = [1 0 0 0;
     0 1 0 0];
D = [0;0];
sys = ss(A, B, C, D);
[num, den] = ss2tf(A, B, C, D);

subSys_pos = tf(num(1,:) ,den);

poles = [-12 -32 -64];

Kd = -(1/(num(2,3)+num(2,4))*(poles(1)+poles(2)+poles(3))+den(1,2)/(num(2,3)+num(2,4)))*pi/180;
Kp = -(den(1,3)/(num(2,3)+num(2,4))-1/(num(2,3)+num(2,4))*(poles(1)*poles(2)+poles(1)*poles(3)+poles(2)*poles(3)))*pi/180;
Ki = -(1/(num(2,3)+num(2,4))*(poles(1)*poles(2)*poles(3))+den(1,4)/(num(2,3)+num(2,4)))*pi/180;


b3 = num(1, 3);
b2 = num(1, 4);
b1 = num(1, 5);
a3 = den(1, 2);
a2 = den(1, 3);
a1 = den(1, 4);
close_num = [b3*Kd b2*Kd+b3*Kp (b1*Kd+b3*Ki+b2*Kp) b2*Ki+b1*Kp b1*Ki];
close_den = [1+b3*Kd (a3+b2*Kd+b3*Kp) (a2+b1*Kd+b3*Ki+b2*Kp) (a1+b2*Ki+b1*Kp) b1*Ki 0];
M = tf(close_num, close_den);


Kp_p = 16.8;
Ki_p = 0;
Kd_p = 0;
