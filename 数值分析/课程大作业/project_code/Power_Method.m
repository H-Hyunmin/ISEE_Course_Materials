% 用幂法求矩阵 A 的最大特征值 lambda
% 输入：矩阵 A，最大迭代次数 max_iter，容差 TOL
% 输出：最大特征值 lambda
function lambda = Power_Method(A, max_iter,TOL)
    % 初始化一个非零向量 v
    v = rand(size(A, 1), 1);
    for i = 1:max_iter
        w = A * v;                      % 计算 w 的 2-范数
        new_v = w / norm(w);            % sum = 0;
                                        %     for i = 1:length(w)
                                        % sum = sum + w(i)^2;
                                        %     end
                                        % norm_w = sqrt(sum);

        if norm(new_v - v) < TOL        %计算new_v与v的差的2-范数
        break;                          % d = new_v - v;
        end                             % sum = 0;
       v=new_v;                         % for i = 1:length(d)
       lambda = v' * A * v;             %     sum = sum + d(i)^2;
    end                                 % end
end                                     % norm_d = sqrt(sum);

            
        
                            
        
    
