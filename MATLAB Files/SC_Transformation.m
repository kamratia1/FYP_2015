% This script calclates the force in the gap between two electrodes and
% using the force, the torque on the mirror is calculated. The script
% assumes that only one side of the mirror is driven and the electrodes are
% along the full length of the mirror. 
% A graph of torque against deflection angle is plotted 

% permittivity of free space and relative permittivity
e0 = 8.85418782e-12;
er = 1;
    
%number of field lines to plot
lines = 40;
equipotential = 20;

V = 30; %potential difference (in volts) between the two electrodes

l = 240;  % overlap length in um

w = 22;  % width of each electrode
h = 200;  % height of an electrode
h1 = 180;    % height of 2nd electrode 
g = 28;   % gap between the electrodes 

W = 8;     % width of mirror in mm
L = 15;    % length of mirror in mm

num_of_gaps = (L*1e-3)/((w+g)*1e-6);   % number of gaps in the electrode array

x_start = -400;    % Start displacement between electrodes in um
x_end = 400;       % End displacement between electrodes in um

% The resolution of the analysis. 
% Higher resolution will lead to smoother results but may take much longer
delta_x = 20;      

x = x_start:delta_x:x_end;

i = 1;
E = 0:1:length(x)-1;
F = 0:1:length(x)-2;

while ( i<=length(x) ),
    
    axis_lim = [-40 ((w)+g+40) h+50 (h+x(i))];
    
    % Geometry of the electrodes
    vertices = [1i*h, (w/2)+(1i*h), (w/2), 0, Inf, (w+g)+(1i*x(i)), ((w/2)+g)+ (1i)*x(i), ((w/2)+g)+ (1i*(x(i)+h1)), ((w)+g)+(1i*(x(i)+h1)), Inf];
    ang = [1/2, 3/2, 3/2, 1/2, 0, 1/2, 3/2, 3/2, 1/2, 0];

    %Create the polygon in the physical domain (z-plane)
    p = polygon(vertices, ang);
    % Transform into the rectangular domain
    f = rectmap(p, [1 4 6 9]);
    
    ww = evalinv(f, [vertices(4), vertices(9)]);
     
    A = imag(ww(1));
    B = imag(ww(2));
    d = abs(real(ww(1)) - real(ww(2)));
    
    %capacitance per unit length (F/m) of the whole region bounded by field
    %lines A and B
    Cap = e0*er*(abs(A-B))/abs(d);
    
    %Capacitance at every displacment 
    E(i) = Cap;
    i = i+1;
end

Capacitance = E*(l*1e-6)*num_of_gaps;   % capacitance in Farads
E_smooth = smooth(E);                  

%The force is then the differential of the Electrostatic energy. The
%differential will need to be done numerically. 
i=1;
while (i<length(x)),
    %F(i) = (overlap_length*(1e-6))*0.5*V*V*((E_smooth(i+1) - E_smooth(i))/(1e-6*delta_x))*425*((3+(overlap_length/2))*(1e-3))*(1e6); % Force in uN/m
    F(i) = 0.5*V*V*((E(i+1) - E(i))/(1e-6*delta_x)); % delta_x is in microns and hence multiplied by 1e-6
    i = i+1;
end

Torque = F*(l*1e-6)*((W/2)*1e-3)*num_of_gaps;
Torque_smooth = smooth(Torque);
x1 = x_start+(delta_x/2):delta_x:(x_end-(delta_x/2));
theta = (asin(x1/3000))*(180/pi);
theta1 = (asin(x/3000))*(180/pi);
TorqueN = Torque/(V*V);

% Plot the torque against deflection angle
plot(theta, Torque_smooth*1e12/(V*V));
title('Normalized Torque against rotational angle (24w, 200h, 16g, 200d)');
xlabel('Rotation angle (degrees)');
ylabel('Torque (pNm)');
grid on;
