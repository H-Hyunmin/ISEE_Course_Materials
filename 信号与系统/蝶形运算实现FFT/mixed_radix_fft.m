function X = mixed_radix_fft(x)
    N = length(x);
    if N == 1
        X = x;
    else
        factors = factor(N);
        p = factors(1);
        X = zeros(1, N);
        for k = 0:N-1
            X_temp = zeros(1, p);
            for n = 0:p-1
                for m = 0:N/p-1
                    X_temp(n+1) = X_temp(n+1) + x(m*p+n+1) * exp(-2i*pi*k*m/N);
                end
            end
            X(k+1) = sum(X_temp);
        end
    end
end