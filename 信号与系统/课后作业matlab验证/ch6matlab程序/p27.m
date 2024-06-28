syms t s
x1=exp(-2*t)*heaviside(t+1);
X1 = laplace(x1)

x2=dirac(t)+dirac(t+1)+exp(-2*(t+3))*heaviside(t+1);
X2 = laplace(x2)