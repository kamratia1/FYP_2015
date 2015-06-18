% This script iterates through the simulink model at different voltgages 
% to estimate mirror deflection. A graph is also plotted. 

% Calculate the costants for the mirror based on the following properties
L = 17;     % length of mirror in mm
W = 8;      % width of mirror in mm    
d = 0.2;    % depth of mirror in mm
Q = 60;     % Quality factor of mirror
f = 45.7;   % Resonant Frequency of mirror

[I C K] = calculate_constants(L, W, d, Q, f);

V_start = 30;   % Start Voltage 
V_end = 100;    % End Voltage
V_delta = 10;   % Step size of the Voltage

V_array = V_start:V_delta:V_end;
range = 1:1:length(V_array);

str = 'FYP_Model_ClosedLoop';   % Name of Simulink Model

i = 1;

% Iterate through the model to calculate the mirror deflection
while(i<=length(V_array)),
    
    V = V_array(i);
    sim(str)
    data = simout.signals.values;
    range(i) = max(data) - min(data);
    i = i+1;
end

% plot Mirro Deflection against Voltage
plot(V_array, range/2);
grid on;
xlabel('Voltage (V)');
ylabel('Mirror Deflection angle (degrees)');
title('Mirror Deflection against Voltage')
hold on;
