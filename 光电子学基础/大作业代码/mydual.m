%生成双峰光谱光电流
clear all
clc
path_MODIS='.\';  %数据存放的路径
data_MODIS = dir(strcat(path_MODIS,'*.xlsx'));
na=data_MODIS(1).name;
pl=strcat(path_MODIS,na);
A=xlsread(pl,1);
wavelength=A(:,1);
response=A(:,2);
delwavelength=wavelength(2)-wavelength(1);
swavelength=wavelength(1):delwavelength/5:wavelength(length(wavelength));%波长范围精细化
light1=normpdf(swavelength,swavelength(1601),delwavelength*1.25);%生成待测单峰光1
light2=normpdf(swavelength,swavelength(1901),delwavelength*1.25);%生成待测单峰光2
light=light1/max(max(light1))+light2/max(max(light2))+0.05;%合成双峰光
noise=1.4*(2*rand(size(light))-1).*light;%生成噪声
light=light+noise;
%plot(swavelength,light);
%hold on
i=1:length(wavelength);
i=(i-1)*5+1;
lightx=light(i);%得到原本波长分布下的光谱
plot(wavelength,lightx);
current=[];
responses=[];
%循环，生成光电流
for k=1:25
    A=xlsread(pl,k);
    wavelength=A(:,1);
    response=A(:,2);
    responses=[responses response];
    current=[current lightx*response*delwavelength];%生成光电流
end
save('duallight.mat','wavelength','current','responses');
