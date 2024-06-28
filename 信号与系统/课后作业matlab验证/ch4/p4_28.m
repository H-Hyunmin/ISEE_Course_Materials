% 定义信号
n = -200:0.001:200;
x = (-1).^n;
x2=1+sin(3*pi*n/8+pi/4)+1/2*cos(pi*n/2+pi/6)+1/4*sin(2*pi*n/3+pi/4);


% 计算傅立叶变换
X = fftshift(fft(x));
X2=fftshift(fft(x2));

% 定义频率范围
w = linspace(-pi, pi, length(n));

% 创建带通滤波器
H = zeros(size(w));
H(w >= -5*pi/12 & w <= -pi/3) = 1;
H(w >= pi/3 & w <= 5*pi/12) = 1;

% 应用滤波器
Y = X .* H;
Y2=X2.*H;
% 计算逆傅立叶变换
y = ifft(ifftshift(Y));
y2=ifft(ifftshift(Y2));

% 绘制结果
figure;
subplot(2, 1, 1);
plot(n, x);
title('Input Signal x[n]');
xlabel('n');
ylabel('x[n]');

subplot(2, 1, 2);
plot(n, real(y));
title('Output Signal y[n]');
xlabel('n');
ylabel('y[n]');

figure;
subplot(2, 1, 1);
plot(n, x2);
title('Input Signal x2[n]');
xlabel('n');
ylabel('x2[n]');

subplot(2, 1, 2);
plot(n, real(y2));
title('Output Signal y2[n]');
xlabel('n');
ylabel('y2[n]');