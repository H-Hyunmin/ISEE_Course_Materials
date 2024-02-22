function [ H, V, f, flag ] = Arnoldi_Implicitly_Restart(A, Hk, Vk, fk, p)
    % ARNOLDI_V2 performs the Implicitly Restarted Arnoldi Method
    % A:输入矩阵
    % Hk:上一次迭代的Hessenberg矩阵
    % Vk:上一次迭代的正交矩阵
    % fk:上一次迭代的残差向量
    % p:重启次数
    % H:本次迭代的Hessenberg矩阵
    % V:本次迭代的正交矩阵
    % f:本次迭代的残差向量
    % flag:用于标记是否提前找到了一个特征值

            % 初始化flag
            flag = 0;
            % 获取A的大小
            [m,n] = size(A);
            % 获取Hk的大小
            k = length(Hk);
            % 初始化V和H
            V = zeros(n, k+p);
            V(:,1:k) = Vk;
            H = zeros(k+p, k+p);
            H(1:k,1:k) = Hk;
            % 初始化残差向量
            f = fk;
            % 进行arnoldi迭代
            for j = 1:p
                beta = norm(f);
                if beta == 0            % 如果残差向量为0，说明已经找到了一个特征值，直接返回
                    beta
                    flag = 1;
                    H = H(1:k+j-1,1:k+j-1);
                    V = V(:,1:k+j-1);
                    return;
                end
                V(:,k+j) = f/beta;      % 正交化
                H(k+j,k+j-1) = beta;    % 更新Hessenberg矩阵

                w = A*V(:,k+j);

                H(1:k+j, k+j) = V(:,1:k+j)'*w;      % 更新Hessenberg矩阵

                f = w - V(:,1:k+j)*H(1:k+j, k+j);   % 更新残差向量
            end
        end