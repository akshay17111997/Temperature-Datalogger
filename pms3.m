tspan = [0 50];   %time limit
y0 = 80;           %initial concentration of A
[t,y] = ode45(@(t,y) (70-y)/5, tspan, y0);  %differential equation solver 
plot(t,y);        %graph plot
xlabel('time in minutes');    
ylabel('Temperature');
title('Temperature v/s time');
