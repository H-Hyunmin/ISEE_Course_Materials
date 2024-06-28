a=[1 -5/6 1/6]
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
