% ~~~~ preprocessing of zero input data ~~~~
function [angle_smooth, omega_smooth2, omega_dot_smooth] = zeroInputData(fileName)   
    
    % load data
    load(fileName);
    Data_cangle = buf(:,1);
    angle = Data_cangle*pi/180;

    % set sampling period
    t = 0.02;

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

end