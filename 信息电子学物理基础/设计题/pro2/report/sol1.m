% filepath: /c:/Users/Huang Xianmin/Desktop/信电物/设计题/pro2/report/sol.m
% 定义变量
syms N_D positive
N_A = 1.677 * N_D;
V_R = 5;
epsilon_r = 11.7;
epsilon_0 = 8.85e-14;
n_i = 1.5e10;
k_B = 1.38e-23;
T = 300;
e = 1.6e-19;

% 计算内建电势 V_D
V_D = (k_B * T / e) * log((N_A * N_D) / (n_i^2));

% 定义不等式
C_B = sqrt((e*epsilon_r * epsilon_0 * N_A * N_D) / (2 * (V_D + V_R) * (N_A + N_D)));
inequality = C_B - 1.60e-9;

% 使用数值方法求解不等式
solution = vpasolve(inequality == 0, N_D, 1e10);

% 显示解
disp(solution)