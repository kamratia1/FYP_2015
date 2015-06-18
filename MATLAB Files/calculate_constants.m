% This function calculates the constants of the mirror to be used on the
% simulink Model. 
% I is the moment of Inertia of the mirror
% C is the rotational Friction
% K is the torsion coeddicient

function [I, C, K] = calculate_constants(l,w,d,Q,Wn)
% Q is the q-factor
% Wn is the undamped natural frequency in Hz.
% l, w and d in mm

Wn = Wn*2*pi; % convert frequency to rad/s
density = 2.3290; % density of silicon in g/cm3

%convert the dimensions into cm
l=l/10;
w=w/10;
d=d/10;

%Wn = Wn*2*pi;    % convert Wn from Hz to rad/s
m = l*w*d*density/1000; % mass of mirror in kg

w = w/100; % convert from cm to m

I = (1/12)*m*w*w;  % moment of inertia
K = Wn*Wn*I;       % calculating torsion coefficient
C = sqrt(I*K)/Q;   % calculating rotational friction

end