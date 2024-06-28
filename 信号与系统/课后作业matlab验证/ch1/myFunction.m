function y = myFunction(t)
    y = arrayfun(@(t) (0).*(t<-1) + (-t).*(-1<t & t<0) + (1).*(0<t & t<1) + (2).*(1<t & t<2) + (0).*(t>2), t);
end