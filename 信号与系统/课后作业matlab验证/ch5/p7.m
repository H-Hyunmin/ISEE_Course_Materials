% 定义参数
A = 5;  
B = 10;  
T = 0.33;  
theta = pi/7;  

% 创建信号函数
x = @(t) A + B * cos((2*pi/T)*t + theta);

t = -10000:0.01:10000;  
y = x(t);  
plot(t, y); 
axis equal;  


delta=0.70710678118655;

Tc=T+delta;


start = ceil(-10000 / Tc) * Tc;
t_sampled = start:Tc:10000;
% 采样
y_sampled = x(t_sampled);

% 绘制采样后的信号图
stem(t_sampled, y_sampled, 'Marker', 'o');
title('采样后的信号');
xlabel('时间 (s)');
ylabel('幅度');
axis equal;

% 计算FFT
N = length(y_sampled);  
Y = fft(y_sampled, N);  
f = (-N/2:N/2-1)*(1/(Tc*N));  % 频率轴

% 计算双边频谱
Y = fftshift(Y);  

figure;
plot(f, abs(Y));
title('双边频谱');
xlabel('频率 (Hz)');
ylabel('幅度');