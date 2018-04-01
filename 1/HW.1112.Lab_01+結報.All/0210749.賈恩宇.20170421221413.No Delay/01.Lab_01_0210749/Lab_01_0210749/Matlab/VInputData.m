
% ~~~~~ with input Vs ~~~~~~~
function [angle_smooth, omega_smooth2, omega_dot_smooth, ...
            theta_l, theta_dot_smooth, theta_ddot_smooth] = VInputData(fileName)

    % load data
    load(fileName);

    % set sampling period
    t = 0.02;

    % ~~~~~~~~~~~ psi ~~~~~~~~~~~ 
    
    Data_cangle = buf(:,1);
    angle = Data_cangle*pi/180;
    

    % assign psi
    angle_smooth = angle;

    % calculate angular speed of psi (omega) rad/s
    omega = zeros(length(angle_smooth), 1);
    for i=2:length(angle_smooth)
        omega(i-1) = (angle_smooth(i)-angle_smooth(i-1))*1.0/t;
    end
    omega(length(angle_smooth)) = omega(length(angle_smooth)-1);
    
    omega_smooth = zeros(length(angle_smooth), 1);
    % smoothen angular speed of psi
    omega_smooth(1) = omega(1);
    omega_smooth(2) = omega(2);
    for i=3:length(omega)-2
        omega_smooth(i) = (omega(i-2)+omega(i-1)+omega(i)+omega(i+1)+omega(i+2))/5.0;
    end
    omega_smooth(length(omega)-1) = omega_smooth(length(omega)-2);
    omega_smooth(length(omega)) = omega_smooth(length(omega)-1);

    % smoothen twice
    omega_smooth2 = omega_smooth;
    for i=3:length(omega)-2
        omega_smooth2(i) = (omega_smooth2(i-2)+omega_smooth2(i-1)+omega_smooth2(i)+omega_smooth2(i+1)+omega_smooth2(i+2))/5.0;
    end
    omega_smooth2(length(omega)-1) = omega_smooth2(length(omega)-2);
    omega_smooth2(length(omega)) = omega_smooth2(length(omega)-1);

    % calculate angular acceleration of psi (omega_dot)
    omega_dot = zeros(length(omega_smooth), 1);
    for i=2:length(omega_smooth)
        omega_dot(i-1) = (omega_smooth2(i)-omega_smooth2(i-1))*1.0/t;
    end
    omega_dot(length(omega_smooth2)) = omega_dot(length(omega_smooth2)-1);

    % smoothen
    omega_dot_smooth = zeros(length(omega_smooth), 1);
    for i=3:length(omega_dot)-2
        omega_dot_smooth(i) = (omega_dot(i-2)+omega_dot(i-1)+omega_dot(i)+omega_dot(i+1)+omega_dot(i+2))/5.0;
    end
    omega_dot_smooth(length(omega_dot)-1) = omega_dot_smooth(length(omega_dot)-2);
    omega_dot_smooth(length(omega_dot)) = omega_dot_smooth(length(omega_dot)-1);

    % ~~~~~~~~~~~ theta ~~~~~~~~~~~ 
    
    % get the informatin from the encoder of the 'Left' wheel
    theta_l = buf(:,2);
    
    % calculate angular speed of wheel (theta_dot) rad/s
    theta_dot = zeros(length(theta_l), 1);
    for i=2:length(theta_l)
        theta_dot(i-1) = (theta_l(i)-theta_l(i-1))*1.0/t;
    end
    theta_dot(length(theta_l)) = theta_dot(length(theta_l)-1);
    
    % smoothen
    theta_dot_smooth = zeros(length(theta_dot), 1);
    theta_dot_smooth(1) = theta_dot(1);
    theta_dot_smooth(2) = theta_dot(2);
    for i=3:length(omega)-2
        theta_dot_smooth(i) = (theta_dot(i-2)+theta_dot(i-1)+theta_dot(i)+theta_dot(i+1)+theta_dot(i+2))/5.0;
    end
    theta_dot_smooth(length(theta_dot)-1) = theta_dot_smooth(length(theta_dot)-2);
    theta_dot_smooth(length(theta_dot)) = theta_dot_smooth(length(theta_dot)-1);
    
    
    % calculate angular acceleration of wheel (theta_ddot)
    theta_ddot = zeros(length(theta_dot_smooth), 1);
    for i=2:length(theta_dot_smooth)
        theta_ddot(i-1) = (theta_dot_smooth(i)-theta_dot_smooth(i-1))*1.0/t;
    end
    theta_ddot(length(theta_dot_smooth)) = theta_ddot(length(theta_dot_smooth)-1);
    
    % smoothen
    theta_ddot_smooth = zeros(length(theta_ddot), 1);
    for i=3:length(theta_ddot)-2
        theta_ddot_smooth(i) = (theta_ddot(i-2)+theta_ddot(i-1)+theta_ddot(i)+theta_ddot(i+1)+theta_ddot(i+2))/5.0;
    end
    theta_ddot_smooth(length(theta_ddot)-1) = theta_ddot_smooth(length(theta_ddot)-2);
    theta_ddot_smooth(length(theta_ddot)) = theta_ddot_smooth(length(theta_ddot)-1);
end
