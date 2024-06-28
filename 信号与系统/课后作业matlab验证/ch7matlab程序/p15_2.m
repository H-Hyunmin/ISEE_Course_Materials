% 差分方程的系数
a = [1 -1 -2]; % y[n] 的系数
b = [0 0 1]; % x[n] 的系数


% 输入信号u[n]
n = 0:100;
x = 1.0*(n>=0);

% 初始条件
initialCondition = [0 1/2];

% 求解差分方程
y = filter(b, a, x, initialCondition);

% 画出输出信号
stem(n, y);
xlabel('n');
ylabel('y[n]');
title('差分方程的输出信号');

