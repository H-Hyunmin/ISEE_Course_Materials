%  Arnoldi_Algorithm.m
% 用Arnoldi迭代算法求解矩阵A的k个最大特征值
% 输入：矩阵A，迭代次数k，子空间维度dimension
% 输出：k个最大特征值，特征向量矩阵Q

function [eigenvalues, Q] = Arnoldi_Algorithm(A, k, dimension)
    n = size(A, 1);                         
    Q = zeros(n, dimension);                % 存储Arnoldi迭代过程中的正交阵
    H = zeros(dimension, dimension);        % 存储Hessenberg矩阵
    Q(:, 1) = randn(n, 1);                  % 随机生成初始向量
    Q(:, 1) = Q(:, 1) / norm(Q(:, 1));      % 归一化

    for j = 1:dimension                     % Arnoldi迭代
        v = A * Q(:, j);                    
        for i = 1:j
            H(i, j) = Q(:, i)' * v;         
            v = v - H(i, j) * Q(:, i);      % 正交化
        end
        if j < dimension                    
            H(j+1, j) = norm(v);            
            if H(j+1, j) == 0               % 正交化后向量为0，说明已经找到A的所有特征值
                break;
            end
            Q(:, j+1) = v / H(j+1, j);      
        end
    end
    eigenvalues = QR_Algorithm(H, k,10000); %调用QR算法求解特征值
end