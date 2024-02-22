% 三元线性方程组 ax + by + cz = d, ex + fy + gz = h, ix + jy + kz = l
a = 3.03;
b = -12.1;
c = 14;
d = -119;
e = -3.03;
f = 12.1;
g = -7;
h = 120;
i = 6.11;
j = -14.2;
k = 21;
l = -139;

% 构造系数矩阵和常数向量
A = [a b c; e f g; i j k];
B = [d; h; l];

% 缩放部分主元法求解线性方程组
n = size(A, 1);
s = max(abs(A), [], 2);
for k = 1:n-1
    [~, i] = max(abs(A(k:n, k))./s(k:n));
    ipr = i + k - 1;
    if A(ipr, k) == 0
        error('Matrix is singular');
    end
    if ipr ~= k
        A([k, ipr], :) = A([ipr, k], :);
        B([k, ipr]) = B([ipr, k]);
        s([k, ipr]) = s([ipr, k]);
    end
    for i = k+1:n
        m = A(i, k) / A(k, k);
        A(i, k+1:n) = A(i, k+1:n) - m * A(k, k+1:n);
        B(i) = B(i) - m * B(k);
    end
end

% 回代求解线性方程组
x = zeros(n, 1);
x(n) = B(n) / A(n, n);
for i = n-1:-1:1
    x(i) = (B(i) - A(i, i+1:n) * x(i+1:n)) / A(i, i);
end

% 输出解
x1 = x(1);
x2 = x(2);
x3 = x(3);
fprintf('x1 = %f, x2 = %f, x3 = %f\n', x1, x2, x3);