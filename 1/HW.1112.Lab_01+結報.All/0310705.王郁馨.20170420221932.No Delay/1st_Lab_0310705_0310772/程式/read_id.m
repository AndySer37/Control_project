if (exist('S','var'))              %�ˬdS��data�ܼƬO�_�s�b�A�p���N�|�۰ʲM��
    fclose(S);
    clear S;
end
clear;
close all;clc;
S = serial('COM9');                %***S��serial port
S.BaudRate = 38400;
fopen(S);
buf = [];                     %�NData�M��
    for i=1:100
        while(S.BytesAvailable == 0)        %read from the input buffer,�P�_s�O�_��Data�i��
        end
i
         buf(i,:)=fscanf(S, '%g %g %g',[1 3])
    end
fclose(S);