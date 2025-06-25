% %绘制FP谐振器的matlab测试代码
% %FP公式：T=(1-A/(1-r))^2* 1/(1+4r/(1-r)^2*sin^2(kL))

% % 参数设置
% A = 0.015;          % 吸收损耗
% r = 0.85;          % 反射率
% lambda = 450;     % 光的波长（单位：纳米）
% n = 1.49;          % 介质折射率
% d = 300;           % 谐振腔厚度（单位：纳米）
% theta = 0;       % 入射角（单位：度）
% phi = 0;        % 相位偏移（单位：弧度）

% lambda_min = 200;  % 波长最小值
% lambda_max = 1200; % 波长最大值


% % 将角度转换为弧度
% theta_rad = deg2rad(theta);

% % 公式中的光程部分
% optical_path = n * d * cos(theta_rad);

% % 计算相位差
% phase_difference = (2 * pi / lambda) * optical_path - phi;

% % 计算透射率 T
% denominator = 1 + (4 * r / (1 - r)^2) * sin(phase_difference)^2;
% T = (1 - (A / (1 - r)))^2 * (1 / denominator);

% % 输出结果
% fprintf('透射率 T = %.4f\n', T);


