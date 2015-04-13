function out = matoutp( inp )
%UNTITLED4 Summary of this function goes here
%   Detailed explanation goes here

fprintf(evalin('base','fidout'),'%f\n',inp);
%fclose(evalin('base','fidout'));
%evalin('base','fidout = fopen(''forsyde_out'',''w'');');
out=0;
end
