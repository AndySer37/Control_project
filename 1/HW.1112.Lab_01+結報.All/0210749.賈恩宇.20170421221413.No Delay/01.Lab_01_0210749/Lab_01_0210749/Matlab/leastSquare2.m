clear;
Vs = 100;

[angle_smooth, omega_smooth2, omega_dot_smooth, ...
            theta_l, theta_dot_smooth2, theta_ddot_smooth] = VInputData('V100_test1.mat');

r = 3.45;
x = theta_l * r;
x_dot_smooth = theta_dot_smooth2 * r;
x_ddot_smooth = theta_ddot_smooth * r;
        
range = 8:20;

psi = angle_smooth(range);
w = omega_smooth2(range);
W = omega_dot_smooth(range);

theta = x(range);
x_dot = x_dot_smooth(range);
x_ddot = x_ddot_smooth(range);

A = zeros(length(range), 4);

A(:, 1) = -x_dot;
A(:, 2) = -sin(psi);
A(:, 3) = -x_ddot.*cos(psi);
A(:, 4) = Vs.*ones(length(range), 1);

b = W;

rho = (transpose(A)*A)\transpose(A)*b;

A = zeros(length(range), 4);
A(:, 1) = -x_dot;
A(:, 2) = -W.*cos(psi);
A(:, 3) = -psi.^2.*sin(psi);
A(:, 4) = Vs.*ones(length(range), 1);

b = x_ddot;

sig = (transpose(A)*A)\transpose(A)*b;

display(rho);
display(sig);