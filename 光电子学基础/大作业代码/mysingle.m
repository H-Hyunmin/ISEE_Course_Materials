%生成单峰光谱光电流
clear all
clc
path_MODIS='.\';  %数据存放的路径
data_MODIS = dir(strcat(path_MODIS,'*.xlsx'));
na=data_MODIS(1).name;
pl=strcat(path_MODIS,na);
A=xlsread(pl,1);
wavelength=A(:,1);
response=A(:,2);

delwavelength=wavelength(2)-wavelength(1);%波长间隔
swavelength=wavelength(1):delwavelength/5:wavelength(length(wavelength));%波长范围精细化
light1=normpdf(swavelength,swavelength(2001),delwavelength*1.25);%生成待测单峰光1
light=light1/max(max(light1))+0.05;%合成双峰光
noise=0.75*(2*rand(size(light))-1).*light;%生成噪声
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
save('singlelight.mat','wavelength','current','responses');

% %生成单峰光谱光电流
% clear all
% clc
% path_MODIS = '.\';  % 数据存放的路径
% data_MODIS = dir(strcat(path_MODIS, '*.xlsx'));
% na = data_MODIS(1).name;
% pl = strcat(path_MODIS, na);
% A = xlsread(pl, 1);
% wavelength = A(:, 1);
% response = A(:, 2);

% %delwavelength = wavelength(2) - wavelength(1); % 波长间隔

% delwavelength = 1; % 波长间隔
% swavelength = wavelength(1):delwavelength/5:wavelength(length(wavelength)); % 波长范围精细化

% light1 = normpdf(swavelength, swavelength(1700), delwavelength *10); % 生成待测单峰光1
% light = light1 / max(max(light1)) + 0.05; % 合成双峰光
% noise = 0.01 * (2 * rand(size(light)) - 1) .* light; % 生成噪声
% light = light + noise;

% plot(swavelength, light);
% hold on
% i = 1:length(wavelength);
% i = (i - 1) * 5 + 1;
% lightx = light(i); % 得到原本波长分布下的光谱
% plot(wavelength, lightx);
% current = [];
% responses = [];
% % 循环，生成光电流
% for k = 1:25
%     A = xlsread(pl, k);
%     wavelength = A(:, 1);
%     response = A(:, 2);
%     responses = [responses response];
    
%     % 逐点积分，考虑不等距波长间隔
%     current_k = 0; % 初始化当前工作表的光电流
%     for j = 1:length(wavelength) - 1
%         % 使用梯形积分法
%         delta_lambda = wavelength(j + 1) - wavelength(j); % 当前波长间隔
%         current_k = current_k + 0.5 * (lightx(j) * response(j) + lightx(j + 1) * response(j + 1)) * delta_lambda;
%     end
%     current = [current current_k]; % 累积光电流
% end
% save('singlelight.mat', 'wavelength', 'current', 'responses');

