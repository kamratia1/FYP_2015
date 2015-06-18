% This script outputs a field plot between the gap of two electrodes. The
% corresponding field plot is also shown in the rectangular domain. 
% The capacitance between the gap is also calculated 

w = 34;  % width of each electrode in um
h = 200;  % height of each electrode in um
g = 16;   % gap between the electrode in um
h1 = 180;    % height of 2nd electrode in um
l = 250;    % overlap length in um

W = 6;   % width of mirror in mm
L = 17;  % length of mirror in mm

disp = 50; % displacement between electrodes in um

num_of_gaps = (L*1e-3)/((w+g)*1e-6);

% Describes how many field lines and equipotential lines to draw in the
% field plot
lines = 30;
equipotential = 10;

% Geometry of the electrode
vertices = [1i*h, (w/2)+(1i*h), (w/2), 0, Inf, (w+g)+(1i*disp), ((w/2)+g)+ (1i)*disp, ((w/2)+g)+ (1i*(disp+h1)), ((w)+g)+(1i*(disp+h1)), Inf];
ang = [1/2, 3/2, 3/2, 1/2, 0, 1/2, 3/2, 3/2, 1/2, 0];

%Create the polygon in the physical domain (z-plane)
p = polygon(vertices, ang);
f = rectmap(p, [1 4 6 9]);
k = polygon(prevertex(f));

figure;
subplot(1,2,1)
plot(f, equipotential, lines);  % plot the polygon
grid on;
str = sprintf('Field plot in physical domain of electrodes at displacement %d', disp);
title(str);
ylabel('z-axis (\mum)')
xlabel('x-axis (\mum)')
hold on;
plot(p, 1, 10, 'r');
subplot(1,2,2);
plot(k);
grid on;
ylabel('z-axis (\mum)');
xlabel('x-axis (\mum)');
title ('SC Map of the field into the rectangular domain');
hold on;

% draw the field in the rectangular domain
A = evalinv(f, vertices(1));
B = evalinv(f, vertices(6));

e0 = 8.85418782e-12;
Cap = e0 *abs((imag(B)-imag(A)))/(abs(real(A)- real(B)));

y = (imag(B)-imag(A))/(lines+1);
i = imag(A)+y;

while(i < imag(B)-0.01)
   hold on;
   plot([real(A) real(B)],[i i], 'k')
   i = i + y;
end

x = abs(real(A) - real(B))/(equipotential+1);
i = real(B)+x;

while(i < real(A)-0.01)
   hold on;
   plot([i i], [imag(A) imag(B)], 'k')
   i = i + x;
end

str = sprintf('Capacitance per unit length per gap: %.2fpF/m', Cap*1e12);
str2 = sprintf('Capacitance per gap: %.2fpF', Cap*1e12*l*1e-6);
str3 = sprintf('Capacitance of array: %.2fpF', Cap*1e12*l*1e-6*num_of_gaps);
legend(str, str2, str3);