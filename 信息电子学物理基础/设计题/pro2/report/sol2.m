% filepath: /c:/Users/Huang Xianmin/Desktop/信电物/设计题/pro2/report/sol2.m
% 使用符号计算
syms ND positive

% 定义常数
ni = 1.5e10; % 本征载流子浓度，单位 cm^-3
e = 1.6e-19; % 电子电荷，单位 C
Dn = 34.93; % 电子扩散系数，单位 cm^2/s
Dp = 12.42; % 空穴扩散系数，单位 cm^2/s
tau_n0 = 10e-6; % 电子寿命，单位 s
tau_p0 = 10e-6; % 空穴寿命，单位 s
NA = 1.677 * ND; % 受主浓度

% 定义电流密度 J_s
Js = (e * ni^2 / (NA * ND)) * (ND * sqrt(Dn/tau_n0) + NA * sqrt(Dp/tau_p0));

% 设置不等式
inequality = Js - 1.30e-8; % 电流密度小于 1.30e-8 A/cm^2

% 求解不等式
solution = vpasolve(inequality==0, ND, 1e10);

% 显示解
disp(solution)