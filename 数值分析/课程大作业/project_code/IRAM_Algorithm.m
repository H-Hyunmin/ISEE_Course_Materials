function [ theta, x ] = IRAM_Algorithm( A, k, p, v1)
%A：输入矩阵，这个函数将计算其特征值和特征向量
%k：Arnoldi步骤的数量。这是一个整数，表示在开始隐式重启之前，Arnoldi过程应该执行多少步，也代表想要计算的特征值的数量。
%p：隐式重启的步骤数。这是一个整数，表示在Arnoldi过程中执行多少步之后，应该执行隐式重启。
%v1：Arnoldi过程的初始向量。
%theta：特征值的向量，包含k个特征值。
%x：特征向量的矩阵，每一列都是一个特征向，对应于theta中的特征值。

    %进行k步Arnoldi迭代
    [ Hk, Vk, fk, flag ] = IRAM_Arnoldi_iter( A, k, v1);

    if flag                     %如果提前找到所有的特征值
        [y, theta] = eig(Hk);   %计算HK的特征值矩阵和特征向量矩阵
        theta = diag(theta);    %将特征值矩阵转换为向量
        x = Vk * y;             %计算A的特征向量矩阵
        return
    end

    m = k + p;                  %计算Arnoldi迭代的总步数

    for iter = 1:1000           %进行隐式重启的迭代

        %进行p步Arnoldi迭代
        [ Hm, Vm, fm, flag ] = Arnoldi_Implicitly_Restart(A, Hk, Vk, fk, p);
        
        if flag                 
            [y, theta] = eig(Hm);
            theta = diag(theta);
            x = Vm * y;
            return
        end

        % 检测是否发生特征值deflation
        subdiag = diag(Hm, -1);                             %提取Hm的次对角线元素
        [subdiag_minval, subdiag_min] = min(abs(subdiag));  %找到次对角线元素的最小值和最小值的索引
    %    rtol = 0.1;

        if subdiag_minval <= 0.001                          %如果最小值小于0.001，则认为发生了特征值deflation
    %        disp('deflation')
            V1 = Vm(:, 1:subdiag_min);                      %提取Vm的前subdiag_min列
    %        V2 = Vm(:, subdiag_min:end);                    %提取Vm的后subdiag_min列
            H1 = Hm(1:subdiag_min, 1:subdiag_min);          %提取Hm的左上角subdiag_min*subdiag_min子矩阵
    %        H2 = Hm(subdiag_min+1:m, subdiag_min+1:m);      %提取Hm的右下角subdiag_min*subdiag_min子矩阵

            if norm(H1) < 0.01                              %如果H1的范数小于0.01，则认为H1是奇异的，无法计算特征值
                theta = [];
                x = [];
                return
            end

            [y, theta] = eig(H1);                           %如果H1非奇异，则计算H1的特征值和特征向量
            theta = diag(theta);
            x = V1 * y;


            A_rec  = (eye(length(A)) - V1 * V1') * A;       %计算修正后的A矩阵
            %对修正后的A矩阵重新进行IRAM迭代
            [ theta_rec, x_rec ] = IRAM_Algorithm( A_rec, k, p, Vm(:, subdiag_min + 1));  


            theta = [theta; theta_rec];                     %合并特征值
            x = [x, x_rec];                                 %合并特征向量
            return
        end

        %计算Hm的特征值和特征向量
        [y, theta] = eig(Hm);
        theta = diag(theta);
    %    arnoldires = norm(fm) * abs(y(m, :));               %计算残差

        [~, ind] = sort(theta, 'descend');                  %对特征值进行排序
        ind = ind(k + 1:m);                                 %提取剩余的特征值

        et = zeros(m, 1);                                 
        et(m) = 1;                                          %et向量用于跟踪最后一个Arnoldi向量在隐式QR算法中的变换。      
        
        %
        %用QR shitf更新矩阵Hm和Vm，使得Hm更接近于一个上Hessenberg矩阵
        % 而Vm的列向量仍然是A的Krylov子空间的基。
        for i = 1:numel(ind)                                %计算残差向量
            [Q, R] = qr(Hm - theta(ind(i)) * eye(m));       %计算QR分解
            Hm = R * Q + theta(ind(i)) * eye(m);            %更新Hm
            Vm = Vm * Q;                                    %更新Vm
            et = Q' * et;
        end

        beta = Hm(k + 1, k);                                
        sigma = et(k);
        fk = beta * Vm(:, k + 1) + sigma * fm;              %计算新的残差fk
        Hk = Hm(1:k, 1:k);                                  %更新Hk
        Vk = Vm(:, 1:k);                                    %更新Vk
    end
end