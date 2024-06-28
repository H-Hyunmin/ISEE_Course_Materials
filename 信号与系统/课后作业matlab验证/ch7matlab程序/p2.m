% syms n z
% f = (1/3)^-n;
% F = ztrans(f, n, z)

syms z a;

F3=(1-1/2*z^-1)/(1+3/4*z^-1+1/8*z^-2);
f3=iztrans(F3,z)
F4=(z-a)/(1-a*z);
f4=iztrans(F4)
F7=(z^2-a*z)/(z-a)^3;
f7=iztrans(F7)