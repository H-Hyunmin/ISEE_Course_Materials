%项目主体文件
%author:ZJUHuangXM
%date:2023-11-18

%--------------------------------------------------------------------------
%1. ⽣成随机矩阵
% (1)生成一个 10x10 的随机矩阵
matrix_10x10 = rand(10)+1i*rand(10);

% (2)生成一个 100x100 维度且密度为 0.1 的随机稀疏矩阵
%生成实数部分
sparse_matrix = sprand(100, 100, 0.001);

% 找到矩阵所有非零元素的位置
[row, col, val] = find(sparse_matrix);

% 为所有非零元素加上一个随机的纯虚数，生成虚部
val = val + 1i * rand(size(val));
% 重新构造矩阵生成复数矩阵
sparse_matrix = sparse(row, col, val, size(sparse_matrix, 1), size(sparse_matrix, 2));

% 统计稀疏矩阵中非零元素的数量
non_zero_elements = nnz(sparse_matrix);

% (3) 计算 10x10 随机矩阵的特征值
eigenvalues_10x10 = eig(matrix_10x10);
% 计算 100x100 稀疏矩阵的特征值
eigenvalues_sparse = eigs(sparse_matrix,100);

%--------------------------------------------------------------------------
%2. 给出 Power Method 的伪代码并⽤代码实现，能够输出绝对值最⼤的特征值。
max_iter = 1000; % 设置最大迭代次数
TOL = 1e-9;      % 设置收敛阈值
%(1)利⽤ Power Method 计算题⽬ 1(1)中矩阵的绝对值最⼤的特征
Power_Method_find_max_eigenvalue_10x10 = Power_Method(matrix_10x10, max_iter,TOL)
%(2)利⽤ Power Method 计算题⽬ 1(2)中稀疏矩阵的绝对值最⼤的特征值。
Power_Method_find_max_eigenvalue_sparse = Power_Method(sparse_matrix, max_iter,TOL)

%--------------------------------------------------------------------------
%3. 给出 QR 算法的伪代码并⽤代码实现，并能够实现输出前 k 个绝对值最⼤的特征值，其中k 为⾃定义参数。
% (1)利⽤ QR 算法计算题⽬ 1(1)中矩阵的前 4 个绝对值最⼤的特征值。 
% (2)利⽤ QR 算法计算题⽬ 1(2)中稀疏矩阵的前 5 个绝对值最⼤的特征值。
QR_Algorithm_find_kth_max_eigenvalue_10x10 = QR_Algorithm(matrix_10x10,4,10000)
QR_Algorithm_find_kth_max_eigenvalue_sparse = QR_Algorithm(sparse_matrix,5,7000)

%--------------------------------------------------------------------------
% 4. ⽤代码实现 Arnoldi 迭代算法，并能够实现输出前 k 个绝对值最⼤的特征值，其中 k 为⾃定义参数。 
% (1)利⽤ Arnoldi 迭代算法计算题⽬ 1(2)中矩阵的前 6 个绝对值最⼤的特征值。 
Arnoldi_Algorithm_find_kth_max_eigenvalue_10x10 = Arnoldi_Algorithm(matrix_10x10,6,10)
% (2)利⽤ Arnoldi 迭代算法计算题⽬ 1(2)中稀疏矩阵的前 7 个绝对值最⼤的特征值。
Arnoldi_Algorithm_find_kth_max_eigenvalue_sparse = Arnoldi_Algorithm(sparse_matrix,7,100)

%--------------------------------------------------------------------------
% 5.（bonus，⾮必做）给出 ERAM 或 IRAM 算法的伪代码并⽤代码实现，并能够实现输出前k个绝对值最⼤的特征值，其中 k为⾃定义参数。 
% (1)利⽤ ERAM 或 IRAM 算法计算题⽬ 1(2)中稀疏矩阵的前 20 个绝对值最⼤的特征值。
v1=rand(100,1);
IRAM_Algorithm_find_kth_max_eigenvalue_sparse = IRAM_Algorithm(sparse_matrix,100,100,v1);
IRAM_Algorithm_find_kth_max_eigenvalue_sparse = sort((IRAM_Algorithm_find_kth_max_eigenvalue_sparse), 'descend');
IRAM_Algorithm_find_kth_max_eigenvalue_sparse = IRAM_Algorithm_find_kth_max_eigenvalue_sparse(1:20)
