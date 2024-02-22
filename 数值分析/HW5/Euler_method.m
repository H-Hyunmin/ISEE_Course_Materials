% 定义微分方程
f = @(t, y) 1+ y/t + (y/t)^2;

% 初始化初始条件
t = 1;
y = 0;
h = 0.2;

% 输出初始条件
fprintf('t=%f, y=%.f\n', t, y);

% 使用欧拉方法进行迭代
while t < 3
    y = y + h * f(t, y);
    t = t + h;
    fprintf('t=%f, y=%f\n', t, y);
end