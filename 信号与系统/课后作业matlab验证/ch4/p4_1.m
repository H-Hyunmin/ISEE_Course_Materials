N=6; 
xn1 = [ 1, 1, 1, 1, 0, 0];
n=0:1:N-1;

for k1=0:1:N-1
    for k2=0:1:N-1
    w(k1+1,k2+1)=exp(-i*k2*k1*2*pi/N);
    end
end

an1=xn1*w/N;

N=4;
xn2 = [ 2, 4, 0, 0];
n=0:1:N-1;

for k1=0:1:N-1
    for k2=0:1:N-1
    t(k1+1,k2+1)=exp(-i*k2*k1*2*pi/N);
    end
end

an2=xn2*t/N;

disp(an1);
disp(an2);
