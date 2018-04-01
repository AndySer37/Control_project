%theta = [0 0 0 0 0];
%w = [0 0 0 0 0];
%theta=[-0.114226222;-0.297637111;-0.565897778;-0.879304667;-1.091638444;]

%theta_dot=[-1.834108889	-2.682606667	-2.331038311	-2.331038311	-2.331038311]
%phi=[-8.376811594;-7.826086957;-7.362318841;-3.971014493;-2.086956522;]
%phi_dot=[5.507246377	4.637681159	11.83768116	11.83768116	11.83768116]

    rho3 = 3.0437;
    rho4 = -7.6662;
    
for i=1:(length(theta_dot)-1)
    theta_dot2(i) = (theta_dot(i+1) - theta_dot(i))/0.1;
end
theta_dot2(length(theta_dot)) = theta_dot2(length(theta_dot)-1);

for i=1:(length(phi_dot)-1)
    phi_dot2(i) = (phi_dot(i+1) - phi_dot(i))/0.1;
end
phi_dot2(length(phi_dot)) = phi_dot2(length(phi_dot)-1);

A = zeros(length(theta_dot),2);
A(:,1) = -phi_dot2';
A(:,2) = -cos(theta')*phi_dot2';
%A(:,3) = phi_dot + theta_dot;
%A(:,4) = sin(theta');

B=zeros(length(phi),2);
	B(:,1)=phi_dot + theta_dot;
	B(:,2)=sin(theta');
	
	C(:,1)=[rho3,rho4];
b = transpose(theta_dot2)-B*C;
%b = transpose(theta_dot2);

x = inv(transpose(A)*A)*transpose(A)*b;

rho3 = b(1) 
rho4 = b(2) 
%rho3 = x(3)    
%rho4 = x(4)