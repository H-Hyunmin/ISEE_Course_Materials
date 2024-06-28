syms t
f1 = 4*sin(pi*(t-3))*heaviside(t-3);
F1 = laplace(f1)
f2 = cos(10*pi*t)*heaviside(t);
f2t= diff(f2,t);
F2 = laplace(f2t)
