theta = Data_rad(5:30);
w = omega_smooth2(5:30);

for i=1:(length(w)-1)
    W(i) = (w(i+1) - w(i))/0.01;
end
W(length(w)) = W(length(w)-1);

A = zeros(length(w),2);
A(:,1) = w';
A(:,2) = sin(theta');

b = transpose(W);

x = inv(transpose(A)*A)*transpose(A)*b;

rho3 = x(1)    %-10.932019402580760
rho4 = x(2)    %-39.402306014934040