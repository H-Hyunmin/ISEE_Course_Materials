%用0填充的快速傅里叶变换

function X = FastFT(x)
    N = length(x);

    if log2(N) ~= round(log2(N))

        N_pad = 2^nextpow2(N);

        x = [x, zeros(1, N_pad - N)];

        N = N_pad;
    end

    if N <= 1
        X = x;
    else
        X_even = FastFT(x(1:2:end));
        X_odd = FastFT(x(2:2:end));
        factor = exp(-2i * pi * (0:N/2-1) / N);
        X = [X_even + factor .* X_odd, X_even - factor .* X_odd];
    end
end