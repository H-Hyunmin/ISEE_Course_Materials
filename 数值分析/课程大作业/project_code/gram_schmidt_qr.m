%  gram_schmidt_qr.m
% 用 Gram-Schmidt 方法计算 QR 分解
% 输入：矩阵 A
% 输出：矩阵 Q 和上三角矩阵 R
% 由于matlab调用函数相比于直接计算要慢很多，所以QR算法中直接实现了Gram-Schmidt方法没有调用该函数

function [Q, R] = gram_schmidt_qr(A)
    [m, n] = size(A);
    Q = zeros(m, n);% Q 是正交矩阵
    R = zeros(n, n);% R 是上三角矩阵

    for j = 1:n
        v = A(:, j);                

        for i = 1:j-1         
            R(i, j) = Q(:, i)' * A(:, j);
            v = v - R(i, j) * Q(:, i);
        end

        R(j, j) = norm(v); 
        Q(:, j) = v / R(j, j);
    end
end