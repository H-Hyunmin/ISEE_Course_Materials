% 定义时间 t
t = -100:0.01:100;

h = zeros(size(t));

v1 = [-0.2, 0];
v2 = [0, 1];
v3 = [0.2, 0];

h(t >= v1(1) & t < v2(1)) = ((v2(2)-v1(2))/(v2(1)-v1(1))) * (t(t >= v1(1) & t < v2(1)) - v1(1));

h(t >= v2(1) & t <= v3(1)) = ((v3(2)-v2(2))/(v3(1)-v2(1))) * (t(t >= v2(1) & t <= v3(1)) - v2(1)) + v2(2);

t = -100:0.01:100;

% 生成信号 x(t) = cos(2*pi*t)
x = cos(2*pi*t);


T = 0.2;

% 初始化 Xr
Xr = zeros(size(t));

n = round(length(t) / 2);

for i = -n:n
    % 计算 x(nT) 和 h(t - nT)
    x_nt = cos(2*pi*i*T);
    h_t_nt = interp1(t, h, t - i*T, 'linear', 'extrap');
    
    Xr = Xr + x_nt * h_t_nt;
end

plot(t, Xr);
title('Xr(t)');
xlabel('t');
ylabel('Xr(t)');
axis equal;