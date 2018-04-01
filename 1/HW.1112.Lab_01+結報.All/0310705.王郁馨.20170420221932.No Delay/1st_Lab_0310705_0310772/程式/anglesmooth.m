 angle = [-6.45;
-9.916;
-16.668;
-30.368;
-52.33
];
 t = 0.1;

 
 angle_smooth = angle;
 
%  angle_smooth2 = angle_smooth;
%  for i=3:length(angle_smooth)-2
%     angle_smooth2(i) = (angle_smooth2(i-2)+angle_smooth2(i-1)+angle_smooth2(i)+angle_smooth2(i+1)+angle_smooth2(i+2))/5.0;
%  end
%  angle_smooth2(length(angle_smooth)-1) = angle_smooth2(length(angle_smooth)-2);
%  angle_smooth2(length(angle_smooth)) = angle_smooth2(length(angle_smooth)-1);

for i=2:length(angle_smooth)
%     omega(i-1) = (Data_rad(i)-Data_rad(i-1))*1.0/(Data_t(i)-Data_t(i-1));
    omega(i-1) = (angle_smooth(i)-angle_smooth(i-1))*1.0/t;
end
 omega(length(angle_smooth)) = omega(length(angle_smooth)-1);
 
 %¥­·Æ
 omega_smooth(1) = omega(1);
 omega_smooth(2) = omega(2);
 for i=3:length(omega)-2
    omega_smooth(i) = (omega(i-2)+omega(i-1)+omega(i)+omega(i+1)+omega(i+2))/5.0;
 end
 omega_smooth(length(omega)-1) = omega_smooth(length(omega)-2);
 omega_smooth(length(omega)) = omega_smooth(length(omega)-1);
 
 omega_smooth2 = omega_smooth;
 for i=3:length(omega)-2
    omega_smooth2(i) = (omega_smooth2(i-2)+omega_smooth2(i-1)+omega_smooth2(i)+omega_smooth2(i+1)+omega_smooth2(i+2))/5.0;
 end
 omega_smooth2(length(omega)-1) = omega_smooth2(length(omega)-2);
 omega_smooth2(length(omega)) = omega_smooth2(length(omega)-1);
 
 for i=2:length(omega_smooth2)
%     omega_dot(i-1) = (omega_smooth2(i)-omega_smooth2(i-1))*1.0/(Data_t(i)-Data_t(i-1));
    omega_dot(i-1) = (omega_smooth2(i)-omega_smooth2(i-1))*1.0/t;
end
 omega_dot(length(omega_smooth2)) = omega_dot(length(omega_smooth2)-1);
 
 for i=3:length(omega_dot)-2
    omega_dot_smooth(i) = (omega_dot(i-2)+omega_dot(i-1)+omega_dot(i)+omega_dot(i+1)+omega_dot(i+2))/5.0;
 end
 omega_dot_smooth(length(omega_dot)-1) = omega_dot_smooth(length(omega_dot)-2);
 omega_dot_smooth(length(omega_dot)) = omega_dot_smooth(length(omega_dot)-1);

figure
plot(angle);
figure
plot(omega);
figure
plot(omega_smooth2);
figure
plot(omega_dot);
figure
plot(omega_dot_smooth);
