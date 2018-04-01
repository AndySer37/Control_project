clear;
% ~~~~~~~ load zero input data ~~~~~~~~~

[angle_smooth, omega_smooth2, omega_dot_smooth] = zeroInputData('no_motor.mat');

% ~~~~ start of the least square method ~~~~~

% range = 15:70;   % stable
range = 30:40;  % fit
% range = 1:20;  % fit
% range = 1:40;
% range = 15:35;


psi = angle_smooth(range, 1);
w = omega_smooth2(range, 1);
W = omega_dot_smooth(range, 1);

A = zeros(length(w),2);
A(:,1) = w;
A(:,2) = sin(psi);

b = W;

x = (transpose(A)*A)\transpose(A)*b;

rho3 = x(1);    
rho4 = x(2);    

% ~~~~~~~~~~ Vs input data ~~~~~~~~~~~~~

Vs = 100;

[angle_smooth, omega_smooth2, omega_dot_smooth, ...
            theta_l, theta_dot_smooth2, theta_ddot_smooth] = VInputData('PWM50.mat');

% range = 4:45; % stable
% range = 4:30; % stable
% range = 7:40;
range = 25:35;

% range = 9:17;

psi = angle_smooth(range);
w = omega_smooth2(range);
W = omega_dot_smooth(range);

theta = theta_l(range);
theta_dot = theta_dot_smooth2(range);
theta_ddot = theta_ddot_smooth(range);

A = zeros(length(theta_ddot),2);
A(:,1) = -theta_ddot;
A(:,2) = -(theta_ddot.*cos(psi));

tempB = [(theta_dot+w) sin(psi)];
tempC = [-rho3;-rho4];
b = W + (tempB * tempC);

x = (transpose(A)*A)\transpose(A)*b;

rho1 = x(1);    %
rho2 = x(2);    %

rho = [rho1 rho2 rho3 rho4];

A = zeros(length(w), 5);
A(:, 1) = -W;
A(:, 2) = -(2.*cos(psi).*W-sin(2.*psi).*sec(psi).*w);
A(:, 3) = w;
A(:, 4) = -theta_dot;
A(:, 5) = Vs.*ones(length(w), 1);

b = theta_ddot;
sig = (transpose(A)*A)\transpose(A)*b;

display(rho);
display(sig);

save('parameters.mat', 'sig', 'rho');