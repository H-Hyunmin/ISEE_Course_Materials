%  QR_Algorithm.m
% 用 QR 算法求解矩阵的特征值
% 输入：矩阵 A，要求的特征值个数 k，最大迭代次数 max_iter
% 输出：矩阵 A 的前 k 个特征值
function eigenvalues = QR_Algorithm(A, k, max_iter)
    for m = 1:max_iter
        n = size(A, 1);         % 获取矩阵 A 的阶数
        Q = zeros(n);           % 初始化 Q
        R = zeros(n);           % 初始化 R
        % 用 Gram-Schmidt 过程进行 QR 分解
        for i = 1:n
        v = A(:, i);    
            for j = 1:i-1   
                R(j, i) = Q(:, j)' * v;
                v = v - R(j, i) * Q(:, j);
            end
        R(i, i) = norm(v);      
        Q(:, i) = v / R(i, i);
        end
            A = R * Q;          % 更新 A
    end
eigenvalues_temp = diag(A);% 提取对角线上的特征值
eigenvalues_temp = sort((eigenvalues_temp), 'descend');% 降序排列特征值
eigenvalues = eigenvalues_temp(1:k);% 取前k个特征值
end

