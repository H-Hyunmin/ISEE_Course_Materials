% 定义信号
n = 1:100; % 取一个足够大的范围
x = 3.^(-n+1);

% 计算 DTFT
X = fft(x);

% 计算频率轴
w = linspace(-pi, pi, length(X));

% 绘制结果
figure;
subplot(2,1,1);
plot(w, abs(fftshift(X)));
title('Magnitude');
subplot(2,1,2);
plot(w, angle(fftshift(X)));
title('Phase');

% 定义信号
n = -10:10; % 取一个足够大的范围
x = zeros(size(n));
x(n == 3) = 1; % δ(6-2n)

% 计算 DTFT
X = fft(x, 1024);

% 计算频率轴
w = linspace(-pi, pi, length(X));

% 绘制结果
figure;
subplot(2,1,1);
plot(w, abs(fftshift(X)));
title('Magnitude');
subplot(2,1,2);
plot(w, angle(fftshift(X)));
title('Phase');

n=0:100;
x=zeros(size(n));
x(1:5)=1;

X=fft(x);
w=linspace(-pi,pi,length(X));
figure;
subplot(2,1,1);
plot(w,abs(fftshift(X)));
title('Magnitude');
subplot(2,1,2);
plot(w,angle(fftshift(X)));
title('Phase');