
for i=1:(length(theta_dot)-1)
    theta_dot2(i) = (theta_dot(i+1) - theta_dot(i))/0.1;
end
theta_dot2(length(theta_dot)) = theta_dot2(length(theta_dot)-1);

for i=1:(length(phi_dot)-1)
    phi_dot2(i) = (phi_dot(i+1) - phi_dot(i))/0.1;
end
phi_dot2(length(phi_dot)) = phi_dot2(length(phi_dot)-1);

A = zeros(length(theta_dot),5);
A(:,1) = -theta_dot2';
A(:,2) = -(2*cos(theta')*theta_dot2'-((sin(2*theta'))*(sec(theta))*(theta_dot)));
A(:,3) = theta_dot;
A(:,4) = -phi_dot;
A(:,5) = 3.0706;



b = transpose(phi_dot2);

x = inv(transpose(A)*A)*transpose(A)*b;

rho1 = x(1) 
rho2 = x(2) 
rho3 = x(3)    
rho4 = x(4)
rho5 = x(5)