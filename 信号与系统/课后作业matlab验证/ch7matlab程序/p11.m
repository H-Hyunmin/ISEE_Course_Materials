% 差分方程的系数
b = [0 1]; 
a = [1 -1 -1]; 

% 求单位样值响应
[h, n] = impz(b, a);


stem(n, h);
xlabel('n');
ylabel('h[n]');
title('单位样值响应');