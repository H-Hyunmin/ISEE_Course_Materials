% 三阶线性方程组 ax + by + cz = d, ex + fy + gz = h, ix + jy + kz = l
a = -2;
b = 1;
c = 1/2;
d = 4;
e = -1;
f = -2;
g = -1/2;
h = -4;
i = 0;
j = 1;
k = 2;
l = 0;

% 构造系数矩阵和常数向量
A = [a b c; e f g; i j k];
B = [d; h; l];

% Jacobi 迭代法求解线性方程组
n = size(A, 1);
x = zeros(n, 1);
x_new = zeros(n, 1);
for iter = 1:3
    for i = 1:n
        x_new(i) = (B(i) - A(i, 1:i-1) * x(1:i-1) - A(i, i+1:n) * x(i+1:n)) / A(i, i);
    end
    x = x_new;
    fprintf('Iteration %d: x1 = %f, x2 = %f, x3 = %f\n', iter, x(1), x(2), x(3));
end