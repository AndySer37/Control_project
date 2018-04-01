if (exist('S','var'))
    fclose(S);
    clear S;
end
clear;
close all;clc;
S = serial('COM9');                % serial port
% S = serial('/dev/cu.wchusbserial1420');
S.BaudRate = 38400;
fopen(S);
buf = [];                     % Data

while(S.BytesAvailable == 0)        %read from the input buffer
end
disp('Connect!!');

% wait for stable connection
dummyLength = 200;
for i = 1:dummyLength
    A = fgets(S);
end
disp('End of waiting');
fprintf(S, 's');

totalLength = 150;
buf = zeros(totalLength, 3);
disp('~~~~~~~~~~~Start!!~~~~~~~~~~~~~~');
for i=1:totalLength
%     while(S.BytesAvailable == 0)        %read from the input buffer
%     end
    i
    temp = fscanf(S, '%g %g %g',[1 3]);
    buf(i,:) = temp;
end
fclose(S);

figure(1);
subplot(1, 3, 1);
plot(1:totalLength, buf(:, 1));
xlabel('n');
ylabel('degree');
title('Car Angle');
grid on;
subplot(1, 3, 2);
plot(1:totalLength, buf(:, 2));
xlabel('n');
ylabel('cm/sec');
title('Left Wheel Speed');
grid on;
subplot(1, 3, 3);
plot(1:totalLength, buf(:, 3));
xlabel('n');
ylabel('cm/sec');
title('Right Wheel Speed');
grid on;