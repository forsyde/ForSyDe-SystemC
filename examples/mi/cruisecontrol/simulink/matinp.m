function outp = matinp( ~ )
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here

persistent res;
if ~evalin('base','feof(fidin)')
    temp = fgetl (evalin('base','fidin'));
    res = sscanf(temp,'%f %f');
    %fclose(evalin('base','fidin'));
    %evalin('base','fidin = fopen(''mat2mod'',''r'');');
else
    set_param(gcs, 'SimulationCommand', 'stop');
end
outp = res;
end

