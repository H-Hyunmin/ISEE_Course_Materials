x=0:1/500/pi:1;
x=[x,flip(x)];
x=[a,x,a];

x=[x,x,x,x,x,x,x,x,x,x,x];


% n=-10*pi:0.001:10*pi;
% n=[0,0,0,0,n,0,0,0,0];
n = linspace(-21*pi, 21*pi, length(x));

% plot(n, x);
% axis([-pi pi 0 1]);
x_=ifft(x);


% k= -length(x)/2:length(x)/2-1;
k=0:1:length(x)-1;
p1=cos(pi*k);
p2=cos(pi*k/2);
% 生成长度与k相同，0和1交错的序列
p3 = repmat([1 0], 1, ceil(length(k)/2));

% 如果k的长度是奇数，那么需要删除最后一个元素以使p3的长度与k相同
if mod(length(k), 2) == 1
    p3 = p3(1:end-1);
end

p4 = repmat([1 0 0 0 ], 1, ceil(length(k)/4));

% 如果k的长度是奇数，那么需要删除最后一个元素以使p3的长度与k相同
if mod(length(k), 2) == 1
    p4 = p3(1:end-1);
end

w1=x_ .*p1;
w2=x_ .*p2;
w3=x_ .*p3;
w4=x_ .*p4;

Y1=fft(w1);
Y2=fft(w2);
Y3=fft(w3);
Y4=fft(w4);


figure
subplot(4,1,1);
plot(n, real(Y1));
axis([-pi pi 0 1]);
title('Output Signal y1[n]');
xlabel('n');
ylabel('y1[n]');

subplot(4,1,2);
plot(n, real(Y2));
axis([-pi pi 0 1]);
title('Output Signal y2[n]');
xlabel('n');
ylabel('y2[n]');


subplot(4,1,3);
plot(n, real(Y3));
axis([-pi pi 0 1]);
title('Output Signal y3[n]');
xlabel('n');
ylabel('y3[n]');

subplot(4,1,4);
plot(n, real(Y4));
axis([-pi pi 0 1]);
title('Output Signal y4[n]');
xlabel('n');
ylabel('y4[n]');


%低通滤波
H=ones(size(a));
H=[a,H,H,a];
% 计算需要在前后补零的数量
pad_size = (length(n) - length(H)) / 2;

% 在H的前后补零
H_padded = padarray(H, [0 pad_size], 'both');

% 如果n的长度是奇数，那么需要在H_padded的末尾再补一个零
if mod(length(n), 2) == 1
    H_padded = [H_padded, 0];
end
H=H_padded;

% 计算逆傅立叶变换
% y1 = ifft(ifftshift(Y1_filtered));
% y2 = ifft(ifftshift(Y2_filtered));
% y3 = ifft(ifftshift(Y3_filtered));
% y4 = ifft(ifftshift(Y4_filtered));
l=length(Y1);
start_index = round(l*10.5/22);
end_index = round(l*11.5/22);

% 取出Y1的这部分向量
y1 = Y1(start_index:end_index);
y2 = Y2(start_index:end_index);
y3 = Y3(start_index:end_index);
y4 = Y4(start_index:end_index);
y1=[a,y1,a];
y2=[a,y2,a];
y3=[a,y3,a];
y4=[a,y4,a];



y1_=ifft(y1);
y2_=ifft(y2);
y3_=ifft(y3);
y4_=ifft(y4);




n=linspace(-pi/2,pi/2,length(y1));
% 绘制结果
figure;
subplot(4,1,1);
plot(n, real(y1));
title('Output Signal y1[n] after Low Pass Filter');
xlabel('n');
ylabel('y1[n]');

subplot(4,1,2);
plot(n, real(y2));
title('Output Signal y2[n] after Low Pass Filter');
xlabel('n');
ylabel('y2[n]');

subplot(4,1,3);
plot(n, real(y3));
title('Output Signal y3[n] after Low Pass Filter');
xlabel('n');
ylabel('y3[n]');

subplot(4,1,4);
plot(n, real(y4));
title('Output Signal y4[n] after Low Pass Filter');
xlabel('n');
ylabel('y4[n]');

figure;
subplot(4,1,1);
plot(n, y1_);
title('Output Signal y1_[n] after Low Pass Filter');
xlabel('n');
ylabel('y1_[n]');

subplot(4,1,2);
plot(n, y2_);
title('Output Signal y2_[n] after Low Pass Filter');
xlabel('n');
ylabel('y2_[n]');

subplot(4,1,3);
plot(n, y3_);
title('Output Signal y3_[n] after Low Pass Filter');
xlabel('n');
ylabel('y3_[n]');

subplot(4,1,4);
plot(n, y4_);
title('Output Signal y4_[n] after Low Pass Filter');
xlabel('n');
ylabel('y4_[n]');