[I C K] = calculate_constants(17, 8, 0.2, 60, 45.7);

V_start = 30;
V_end = 100;
V_delta = 10;

V_array = V_start:V_delta:V_end;
range = 1:1:length(V_array);

i = 1;

while(i<=length(V_array)),
    
    V = V_array(i);
    sim('FYP_Model7')
    data = simout.signals.values;
    range(i) = max(data) - min(data);
    i = i+1;
end

plot(V_array, range/2);
grid on;
xlabel('Voltage (V)');
ylabel('Mirror Deflection angle (degrees)');
title('Mirror Deflection against Voltage (17L, 6W, 0.2d, 100Hz, Varying Q)')
hold on;

error = abs((range./2) - def50);
error_sq = error.^2;
MSE = (1/length(V_array))*sum(error_sq)