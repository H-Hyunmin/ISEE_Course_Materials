syms x1 x2 x3
syms X
X=[x1;x2;x3];
f1(X)=3*x1-cos(x2*x3)-1/2;
f2(X)=4*x1*x1-625*x2*x2+2*x2-1;
f3(X)=exp(-x1*x2)+20*x3+10*pi/3-1;
F(X)=[f1;f2;f3];
j(X)=jacobian(F,X);
format long 
X0=[0;0;0];
X1=X0-inv(j(0,0,0))*F(0,0,0);
X2=X1-inv(j(X1(1),X1(2),X1(3)))*F(X1(1),X1(2),X1(3));
X2=vpa(X2,12)
%X3=X2-inv(j(X2(1),X2(2),X2(3)))*F(X2(1),X2(2),X2(3));
%X3=vpa(X3,18);
%X4=X3-inv(j(X3(1),X3(2),X3(3)))*F(X3(1),X3(2),X3(3));
%X4=vpa(X3,18);
