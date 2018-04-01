
% clear;
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
A = [  0 0 1;
      A1 A2 A3;
      B1 B2 B3;];
B = [0;C1;C2];

C = eye(3);
D = [0;0;0];
sys = ss(A, B, C, D);

K = place(A, B, [-1 -2 -13])

% figure(1)
% plot(1:length(angle), angle, 1:length(simData.Data(:,2)), simData.Data(:,2))
% figure(2)
% plot(1:length(theta_l), theta_l, 1:length(simData.Data(:,1)), simData.Data(:,1))
% figure(3)
% plot(1:length(theta_dot_smooth), theta_dot_smooth, 1:length(simData.Data(:,3)), simData.Data(:,3))
% figure(4)
% plot(1:length(omega_smooth2), omega_smooth2, 1:length(simData.Data(:,4)), simData.Data(:,4))
