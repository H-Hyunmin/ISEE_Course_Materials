syms s t
H=(2*s+3)/(s^2+2*s-3)
X1=laplace(heaviside(t))
X2=laplace(exp(-t)*heaviside(t))
Y1=H*X1
Y2=H*X2
y1=ilaplace(Y1)
y2=ilaplace(Y2)
