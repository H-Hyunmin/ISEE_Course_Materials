syms s
F1 = 1/(s^2+4);
F2 = 1/(s+1);
F3 = s/(s^2+25);
F4 = (s+1)/(s^2+5*s+6);
f1 = ilaplace(F1)
f2 = ilaplace(F2)
f3 = ilaplace(F3)
f4 = ilaplace(F4)