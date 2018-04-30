clear;
load('parameters2.mat');

base = (1 - rho(3)*sig(2));
A1 = -((sig(1) - sig(2)*rho(1))/base);
A2 = (sig(2)*rho(2))/base;
B1 = (sig(4) - sig(2)*rho(4))/base;
A3 = -((rho(1) - rho(3)*sig(1))/base);
A4 = -((rho(2))/base);
B2 = (rho(4) - rho(3)*sig(4))/base;

A = [0 1 0 0;
     0 A1 A2 0;
     0 0 0 1;
     0 A3 A4 0];

B = [0;B1;0;B2];


C = [1 0 0 0;
     0 0 1 0];

D = [0;0];
sys = ss(A, B, C, D);
[num, den] = ss2tf(A, B, C, D);

subSys_pos = tf(num(1,:) ,den);

% poles = [-10 -50 -70];
poles = [-7 -50 -70];

Kd = (1/(num(2,3)+num(2,4))*(poles(1)+poles(2)+poles(3))+den(1,2)/(num(2,3)+num(2,4)))*pi/180;
Kp = (den(1,3)/(num(2,3)+num(2,4))-1/(num(2,3)+num(2,4))*(poles(1)*poles(2)+poles(1)*poles(3)+poles(2)*poles(3)))*pi/180;
Ki = (1/(num(2,3)+num(2,4))*(poles(1)*poles(2)*poles(3))+den(1,4)/(num(2,3)+num(2,4)))*pi/180;

b2 = num(1, 3);
b1 = num(1, 5);
a3 = den(1, 2);
a2 = den(1, 3);
a1 = den(1, 4);
close_num = [b2*Kd b2*Kp (b1*Kd+b2*Ki) b1*Kp b1*Ki];
close_den = [1+b2*Kd (b2*Kp+a3) (b1*Kd+b2*Ki+a2)  (b1*Kp+a1) b1*Ki 0];
M = tf(close_num, close_den);


Kp_p = 1.96;
Ki_p = 0.197;
Kd_p = 4.87;
nGc = tf([Kd_p Kp_p Ki_p], [1 0]);