if (exist('S','var'))              %檢查S的data變數是否存在，如有就會自動清除
    fclose(S);
    clear S;
end
clear;
close all;clc;
S = serial('COM9');                %***S為serial port
S.BaudRate = 38400;
fopen(S);
buf = [];                     %將Data清空
    for i=1:100
        while(S.BytesAvailable == 0)        %read from the input buffer,判斷s是否有Data進來
        end
i
         buf(i,:)=fscanf(S, '%g %g %g',[1 3])
    end
fclose(S);