N=8;
for k=0:1:N-1
    ak(k+1)=sin(k*pi/4);
end
xn = zeros(1, N);
for n = 0:N-1
    for k = 0:N-1
        xn(n+1) = xn(n+1) + ak(k+1)*exp(1j*2*pi*k*n/N);
    end
end

disp(xn);