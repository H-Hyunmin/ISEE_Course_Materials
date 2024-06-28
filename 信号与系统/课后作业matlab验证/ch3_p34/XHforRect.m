% 程序 XHforRect: 产生用于计算傅里叶反变换的被积函数： y=(XH(w)/ (2 * pi)). * exp(j * w * t) 
function y = XHforRect(w, t, T, W)
    % w 为频域变量，t 为信号时间变量，W 为理想低通滤波器的截止频率
    % T 为对称方波的时宽

    % ［— T/2,T/2] 方波频谱与理想低通滤波器频率响应相乘： X*H
    % 方波频谱为 T* sinc(w* T/(2.0 * pi)) 
    XH = (abs(w) <= W) .* (T * sinc(w * T / (2.0 * pi)));

    % 计算被积函数 y
    y = (XH / (2.0 * pi)) .* exp(j * w * t);
end