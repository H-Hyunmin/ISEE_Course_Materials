a=[1 1/6 -1/6]
b=[1 -1]
% 计算频率响应
[H, w] = freqz(b, a, 1024);

% 绘制频率响应的幅度
plot(w, abs(H));
title('Frequency Response');
xlabel('Frequency (rad/sample)');
ylabel('Magnitude');

% 绘制频率响应的相位
figure;
plot(w, angle(H));
title('Frequency Response');
xlabel('Frequency (rad/sample)');
ylabel('Phase (radians)');

% 计算单位脉冲响应
h = impz(b, a);

% 绘制单位脉冲响应
figure;
stem(h);
title('Unit Impulse Response');
xlabel('n');
ylabel('h[n]');

% 定义输入信号
n = 0:100; % 定义时间范围
x = 4.^(-n); % 计算输入信号

% 计算差分方程的响应
y = filter(b, a, x);

% 绘制响应
figure;
stem(n, y);
title('Response to 4^{-n}u[n]');
xlabel('n');
ylabel('y[n]');