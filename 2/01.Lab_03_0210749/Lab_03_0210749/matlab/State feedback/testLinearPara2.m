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

C = eye(4);
D = [0;0;0;0];
sys = ss(A, B, C, D);

% poles = [-1 -2 -4 -16];
poles = [-1 -3 -4 -16];
% poles = [-1 -3 -5 -16];

K = place(A, B, poles);
K_deg = K;
K_deg(1, 3:4) = K_deg(1, 3:4).*(pi/180);
display(K_deg);


% LQR
% Q = diag([0.00001, 0.0001, 100, 0.01]);
% R = 0.0001;
% Q = diag([10, 0, 10000, 100]);
% R = 1;
% 
% [K_lqr,S,e] = lqr(A,B,Q,R);
% K_lqr_deg = K_lqr;
% K_lqr_deg(1, 3:4) = K_lqr_deg(1, 3:4).*(pi/180);
% display(K_lqr_deg);