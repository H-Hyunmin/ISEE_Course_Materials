t = linspace(-2, 3, 1000); % 创建一个从-1到2的向量
x1 = 2*t - 1; % 计算2t-1
y1 = myFunction(x); % 调用myFunction函数

subplot(3, 1, 1); % 创建一个2行1列的图形区域，并在第一个区域绘制图形
plot(t, y1);
xlabel('t');
ylabel('y1');

t = linspace(-5, 5, 1000); 
x2= -t/2+1; % 计算-1/2t+1
y2 = myFunction(x2); % 调用myFunction函数

subplot(3, 1, 2); % 创建一个2行1列的图形区域，并在第二个区域绘制图形
plot(t, y2);
xlabel('t');
ylabel('y2');

% ... 其他代码 ...
t = linspace(-2, 3, 1000); % 创建一个从-2到2的向量

y_delta = y .* (dirac(t + 1) + dirac(t - 2)); % 计算y(t)*[δ(t+1)+δ(t-2)]

subplot(3, 1, 3); % 创建一个3行1列的图形区域，并在第三个区域绘制图形
plot(t, y); % 绘制冲激函数的图像
ylim([-1.5, 2.5]); % 设置y轴的范围


% 在t=0处添加一个向上的箭头
hold on
quiver(-1, 0, 0, 1, 'r', 'MaxHeadSize', 0.05, 'AutoScale', 'off');
quiver(2, 0, 0, 2, 'r', 'MaxHeadSize', 0.05, 'AutoScale', 'off');
axis equal; % 保持x轴和y轴的比例
xlabel('t'); % 添加x轴标签
ylabel('δ(t)'); % 添加y轴标签
title('Impulse Function'); % 添加标题