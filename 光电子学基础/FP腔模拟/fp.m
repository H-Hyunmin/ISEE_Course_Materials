% filepath: c:\Users\Huang Xianmin\Desktop\光电子学基础\FP\fp_test.m
% 绘制FP谐振器的matlab测试代码
% FP公式：T=(1-A/(1-r))^2* 1/(1+4r/(1-r)^2*sin^2(kL))

% 参数设置
A = 0.015;          % 吸收损耗
r = 0.85;           % 反射率
n = 1.49;           % 介质折射率
d = 300;            % 谐振腔厚度（单位：纳米）
theta = 0;          % 入射角（单位：度）
phi = 0;            % 相位偏移（单位：弧度）

lambda_min = 50;   % 波长最小值
lambda_max = 1200;  % 波长最大值

% 将角度转换为弧度
theta_rad = deg2rad(theta);

% 波长范围
lambda_range = lambda_min:0.02:lambda_max; % 波长从 lambda_min 到 lambda_max，步长为 1
T_values = zeros(size(lambda_range));  % 初始化透射率数组

% 计算每个波长的透射率
for i = 1:length(lambda_range)
    lambda = lambda_range(i);
    % 公式中的光程部分
    optical_path = n * d * cos(theta_rad);
    % 计算相位差
    phase_difference = (2 * pi / lambda) * optical_path - phi;
    % 计算透射率 T
    denominator = 1 + (4 * r / (1 - r)^2) * sin(phase_difference)^2;
    T_values(i) = (1 - (A / (1 - r)))^2 * (1 / denominator);
end

% 绘制透射率随波长变化的图像
figure;
plot(lambda_range, T_values, 'b-', 'LineWidth', 1.5);
xlabel('波长 (nm)');
ylabel('透射率 T');
title('FP谐振器透射率随波长的变化');
grid on;