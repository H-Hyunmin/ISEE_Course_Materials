    x = rand(1, 1024);
    X_custom = FastFT(x);
    X_matlab = fft(x);

    diff = X_custom - X_matlab;

    abs_diff = abs(diff);

    max_diff = max(abs_diff);

    if max_diff < 1e-6
        disp('The two functions are consistent.');
    else
        disp('The two functions are inconsistent.');
    end