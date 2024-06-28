i = 0;
dw = 0.05;
for w=0:dw:2*pi
    i=i+1;
    X(i) = 1-exp(-1j*w)+2*exp(-2j*w)-exp(-3j*w)+4*exp(-4j*w);
end

% 创建频率点数组
w = 0:dw:2*pi;

for i=0:length(w)-1
    for k=0:length(w)-1
        x(i+1)= x(i+1)+X(k+1)*exp(j*w(k+1)*i)*dw;
    end
    x(i+1)=x(i+1)/2/pi;
end
disp(x)

for w=0:dw:2*pi
    i=i+1;
    X(i) = 1-exp(-1j*w)/(1-(5/6)*exp(-1j*w)+(1/6)*exp(-2j*w))
end

for i=0:length(w)-1
    for k=0:length(w)-1
        x(i+1)= x(i+1)+X(k+1)*exp(j*w(k+1)*i)*dw;
    end
    x(i+1)=x(i+1)/2/pi;
end
disp(x)

for w=0:dw:2*pi
    i=i+1;
    X(i) = 1-exp(-1j*w)/(1-(5/6)*exp(-1j*w)+(1/6)*exp(-2j*w))
end