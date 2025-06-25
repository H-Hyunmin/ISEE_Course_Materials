    close all
    clear
    load widelight.mat
    current = current;
    %load data1.mat
    %outputData = algorithmByHanxiao( wavelength,gunits,c )
    outputData = algorithmByHanxiao( wavelength,responses,current )
    hold on
