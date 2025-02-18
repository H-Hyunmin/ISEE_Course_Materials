% 三阶线性方程组 ax + by + cz = d, ex + fy + gz = h, ix + jy + kz = l
a = 10;
b = -1;
c = 0;
d = 9;
e = -1;
f = 10;
g = -2;
h = 7;
i = 0;
j = -2;
k = 10;
l = 6;

% 构造系数矩阵和常数向量
A = [a b c; e f g; i j k];
B = [d; h; l];

% Jacobi 迭代法求解线性方程组
n = size(A, 1);
x = zeros(n, 1);
x_new = zeros(n, 1);
iter = 0;
TOL = 0.001;
while max(abs(A * x_new - B)) > TOL
    iter = iter + 1;
    for i = 1:n
        x_new(i) = (B(i) - A(i, 1:i-1) * x(1:i-1) - A(i, i+1:n) * x(i+1:n)) / A(i, i);
    end
    x = x_new;
end
fprintf('Jacobi method converged after %d iterations: x1 = %f, x2 = %f, x3 = %f\n', iter, x(1), x(2), x(3));

% Gauss-Seidel 迭代法求解线性方程组
x = zeros(n, 1);
x_new = zeros(n, 1);
iter = 0;
TOL = 0.001;
while max(abs(A * x_new - B)) > TOL
    iter = iter + 1;
    for i = 1:n
        x_new(i) = (B(i) - A(i, 1:i-1) * x_new(1:i-1) - A(i, i+1:n) * x(i+1:n)) / A(i, i);
    end
    x = x_new;
end
fprintf('Gauss-Seidel method converged after %d iterations: x1 = %f, x2 = %f, x3 = %f\n', iter, x(1), x(2), x(3));