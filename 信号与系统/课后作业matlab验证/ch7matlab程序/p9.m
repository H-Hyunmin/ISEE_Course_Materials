syms z
F1 = (1-z^-1)/(1-z^-1-2*z^-2);
F2 = (z+1)/(z-1)^2; 
% 转换为传递函数
[num1, den1] = numden(F1);
[num2, den2] = numden(F2);

% 转换为多项式系数
num1 = sym2poly(num1);
den1 = sym2poly(den1);
num2 = sym2poly(num2);
den2 = sym2poly(den2);

% 计算零点和极点
zeros1 = roots(num1);
poles1 = roots(den1);
zeros2 = roots(num2);
poles2 = roots(den2);
% 画出零极图
figure;
% zplane(num1, den1);
zplane(num2, den2);

% 对Z变换进行部分分式展开

F1 = partfrac(F1)
f1 = iztrans(F1)
F2 = partfrac(F2)
f2 = iztrans(F2)
