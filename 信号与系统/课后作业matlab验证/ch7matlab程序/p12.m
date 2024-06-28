% 差分方程的系数
b = [0 1]; % x[n] 的系数
a = [1 -1 0.5]; % y[n] 的系数

% 计算零点和极点
zeros = roots(b);
poles = roots(a);

% 画出零极点图
zplane(zeros, poles);
title('零极点图');

% 求单位样值响应
[h, n] = impz(b, a);


stem(n, h);
xlabel('n');
ylabel('h[n]');
title('单位样值响应');