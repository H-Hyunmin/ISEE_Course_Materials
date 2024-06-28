% % 程序 RectResponse: 计算理想低通对矩形脉冲的响应
% W = 2.0 * pi; % 设置理想低通滤波器截止频率
% T = 5.0 * (2 * pi / W); % 设置信号时宽
% w1 = linspace(-1.5 * W, 1.5 * W, 512); % 设置频域变量取值范围

% % 理想低通滤波器频率响应，仅用于显示
% H = 0.5 * ((sign(w1 + eps + W) + 1.0) - (sign(w1 + eps - W) + 1.0));

% t = linspace(-10 * T / 2, 10 * T / 2, 512); % 设置时域变量取值范围

% % [-T/2, T/2] 矩形脉冲，仅用于显示
% x = 0.5 * ((sign(t + eps) + 1.0) - (sign(t + eps - T) + 1.0));

% N = length(t); 
% y = zeros(1, N); 

% % 计算傅里叶反变换
% for k = 1 : N 
%     y(k) = quadl(@XHforRect, -W, W, [], [], t(k), T, W); 
% end 

% % 绘图
% subplot(3, 1, 1); 
% plot(w1, H); 
% xlabel('\omega'); 
% ylabel('H(j \omega)'); 
% axis([-1.5 * W 1.5 * W -0.1 1.1]);
% title('截止频率 = 2\pi');

% subplot(3, 1, 2); 
% plot(t, x); 
% ylabel('x(t)'); 
% axis([-5 5 -0.25 1.25]);
% title('T = 1.0');

% subplot(3, 1, 3); 
% plot(t, y); 
% ylabel('y(t)'); 
% axis([-5 5 -0.25 1.25]); 
% xlabel('t');

% W和T
W = 10 * pi;
T= 10*2 *pi/ W;

% 定义输入信号 x(t)
t = linspace(0, 4*T, 1000);
x1 = 2*heaviside(t) - 4*heaviside(t-T)+4*heaviside(t-2*T)-2*heaviside(t-3*T); % 通过两个阶跃函数相减得到矩形信号

T= 2*2 *pi/ W;
x2 = 2*heaviside(t) - 4*heaviside(t-T)+4*heaviside(t-2*T)-2*heaviside(t-3*T); % 通过两个阶跃函数相减得到矩形信号

T= 0.1*2 *pi/ W;
x3 = 2*heaviside(t) - 4*heaviside(t-T)+4*heaviside(t-2*T)-2*heaviside(t-3*T); % 通过两个阶跃函数相减得到矩形信号


% 计算傅里叶变换
X1 = fft(x1);
X2 = fft(x2);
X3 = fft(x3);

% 定义时移
t_ = 0.2; % 例如，时移为0.2

% 创建频率向量
f = linspace(-500, 499, 1000);

% 创建理想低通滤波器，并加入相位移动
H = double(abs(ifftshift(f)) <= W) ;%.* exp(-1i * 2 * pi * f * t_);

% 应用滤波器
Y1 = X1 .* H;
Y2 = X2 .* H;
Y3 = X3 .* H;
% 计算傅里叶反变换
y1 = ifft(Y1);
y2 = ifft(Y2);
y3 = ifft(Y3);

% 绘制 x(t) 和 y(t)
subplot(6, 1, 1);
plot(t, x1);
title('x(t)');
xlabel('t');
ylabel('x1');

subplot(6, 1, 2);
plot(t, x2);
title('x(t)');
xlabel('t');
ylabel('x2');

subplot(6, 1, 3);
plot(t, x3);
title('x(t)');
xlabel('t');
ylabel('x3');


subplot(6, 1, 4);
plot(t, real(y1));
title('y(t)');
xlabel('t');
ylabel('y1');

subplot(6, 1, 5);
plot(t, real(y2));
title('y(t)');
xlabel('t');
ylabel('y2');


subplot(6, 1, 6);
plot(t, real(y3));
title('y(t)');
xlabel('t');
ylabel('y3');



