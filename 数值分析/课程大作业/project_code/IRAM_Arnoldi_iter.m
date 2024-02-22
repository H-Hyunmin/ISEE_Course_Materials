function [ H, V, f, flag ] = IRAM_Arnoldi_iter( A, dimension, v1)
% 该函数进行Arnoldi迭代，用于求解特征值问题
% A:输入的矩阵
% dimension:是Hessenberg矩阵的维数，也是迭代的次数
% v1:初始向量
% H: 是迭代后的Hessenberg矩阵
% V: 是由krylov子空间的正交基组成的矩阵
% f: 是最后一次迭代后的残差向量
% flag: 用于标记是否提前找到了一个特征值

        % 初始化flag
        flag = 0;

        % 检测输入矩阵是否为方阵
        [m,n] = size(A);
        if m~=n
            error('A is not square')
        end

        % 如果没有输入初始向量，则随机生成一个
        if nargin == 2
            v1 = rand(n,1);
        end

        % 归一化初始向量
        v1 = v1/norm(v1);

        % 初始化Hessenberg矩阵和Krylov子空间的正交基
        V = zeros(n,dimension);
        V(:,1) = v1;
        H = zeros(dimension,dimension);

        % 进行Arnoldi迭代
        for j = 1:dimension
            for i = 1:j
                H(i,j) = V(:,i)'*(A*V(:,j));
            end
            f = A*V(:,j) - V(:,1:j)*H(1:j,j);

            if j == dimension
                return
            else
                H(j+1,j) = norm(f);
            end

            if H(j+1,j) ~= 0            
                V(:,j+1) = f/H(j+1,j);  
            else
                % 如果H(j+1,j)为0，则说明已经找到了一个特征值
                H(j+1,j)    
             flag = 1;
                H = H(1:dimension+j-1,1:dimension+j-1);
                V = V(:,1:dimension+j-1);
                return
            end
        end