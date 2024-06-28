syms s t
vi=exp(-3*t)*heaviside(t);
Vi=laplace(vi);
Vc=(Vi+5/2+1/2*s)/(1/2*s^2+3/2*s+1);
vc=ilaplace(Vc)

