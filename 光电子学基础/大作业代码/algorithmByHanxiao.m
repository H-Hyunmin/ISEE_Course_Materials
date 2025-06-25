function outputData = algorithmByHanxiao( wavelength,gunits,c )
%FuncParameterEst_BGM_1 Summary of this function goes here
% INPUTS:
% the input variables wavelength,units,c should be imported from the
% dataset: measurement.mat, where:
%   wavelength: 29*1    double (29 is the number of measured wavelenth)
%   gunits:     29*18   double 
%   c:          1*18    double (18 is the number of units, i.e. there are
%   18 sensors that were used for the measurement)
%
% OUTPUTS
%   ts:     (N*1 double)  x??????????????????N=100??????????????????????????????
%                       ?? (????????????case??????490nm-630nm?? ????????100????
%   fs:     (N*1 double)  f(ts)??????ts????????????????
%   outputParams (struct)
%       outputParams.Q - ???????? - ??????????????????????????Q??????????????
%                        ????????????????????
%       outputParams.r - ???????? - ????????????????????
%       outputParams,varphi - ?????? - f(x)??????????????????????????????????       

method_id = 2;
parameter_choice_id = 3;

% the number of subdivision ti is essential to the solution, you can
% imporve the accuracy by further increasing numOfti. 


% test setting
params.test.flag = false;
if params.test.flag % if true, then configurate test setting
    params.test.spectrum = 'complex'; % 'complex'
    params.test.noise = 0.005;
    if nargin==4
        if ~isempty(seed)
            params.test.seed = seed; % current seed
        end
    end
end


  
if method_id ==1 % BGM
    params.method = 'BGM'; % Solution method
    params.criterion = 'Q-Criterion';
    
    params.numOfti = 25; % subdivision of ti
    params.numOfts = 100; % number of ts
    params.regularisation.lambda = 0; % the regularisation parameter
    
    % optimisation settings
    params.optimisation = optimoptions('quadprog',...
        'Display','off','Diagnostics','off',...
        'ConstraintTolerance',1e-8,'OptimalityTolerance',1e-8); 
end

if method_id ==2 % Gaussian Basis
    params.method = 'Expansion';
    params.basis.name = 'Guassian';
    params.basis.coeff = struct('fwhmN',0.01); % coefficient
    
    params.numOfti = 50; % subdivision of ti
    params.numOfts = 100; % number of ts / for visualisation purpose only
    
    % settings
    params.regularisation.method = 'Tikhonov';
    params.regularisation.order = 0;
    params.regularisation.lambda = 1e-6; % the variance of c
    
    % parameter choice
    if parameter_choice_id == 1 % no automatic choice
        params.parameterChoice.flag = false;
    elseif parameter_choice_id == 2 % L-curve
        params.parameterChoice.flag = true;
        if params.parameterChoice.flag && ~isempty(params.regularisation.method)
            assert(strcmp(params.regularisation.method,'Tikhonov'));
            params.parameterChoice.method = 'L-curve';
        end
    elseif parameter_choice_id == 3 % GCV
        params.parameterChoice.flag = true;
        params.parameterChoice.method = 'GCV';
    else
        error('unknown method');
    end
    
    % visualisation
    params.show.Lcurve = false; % show Lcurve
    
    
    % optimisation 
    params.optimisation = optimoptions('lsqlin',...
        'Display','off'); 
end
    
checkParams(params); % check input params

% Test 1
if params.test.flag % if yes, run test
    outputData = testFunction_case(wavelength,gunits,params);
else
    assert(~isempty(c));
    outputData = calculateSpectrum(wavelength,gunits,c,params);
end


assert(isscalar(outputData));
end

%% Test Scripts
% Test 1: reconstruct a complex wave with noise and L-curve method
function outputData = testFunction_case(wavelength,gunits,params)

    if isfield(params.test,'noise')
        noise = params.test.noise;
    else
        noise = 0.01; % default
    end
    
    if isfield(params.test,'seed')
        rng(params.test.seed); % set seed;
    end

    % generate input ts, fs and c;
    if strcmp(params.test.spectrum,'impluse')
        fwhm = 20; %nm
        wImpluse_id = randi([1,length(wavelength)]); % randomly pick a peak
        
        [tsExact,fsExact] = generateImpluseFunctions(wavelength,wImpluse_id,fwhm);
    elseif strcmp(params.test.spectrum,'complex')
        [tsExact,fsExact] = generateComplexFunctions(wavelength);
    else
        error('undefined spectrum function');
    end
    
    % get the noise term
    noiseTerm = (rand(size(fsExact))-0.5)*noise;
    
    fsNoise = fsExact+noiseTerm; % introduce noise
    cExact = computeMeasurement(wavelength,gunits,tsExact,fsExact);
    cNoise = computeMeasurement(wavelength,gunits,tsExact,fsNoise);

    % ------------------------------------------------------------------------

    % Compute Spectrum
    [tsCal,fsCal,coeff,const] = computeSpecturm(wavelength,gunits,cNoise,params);

    % Data visualisation
    % 1. show Spectrum: compare the original spectrum with the calculated one;
    figure(1);hold off;
    plot(tsExact,fsExact,'k','LineWidth',1.5);hold on;
    plot(tsExact,fsNoise,'b','LineWidth',0.5);
    plot(tsCal,fsCal,'r','LineWidth',1.5);
    grid on
    xlim([min(wavelength),max(wavelength)]);
    set(gcf,'color','w');
    xlabel('wavelength/nm');ylabel('response intensity');

    % 2. show c: compare the original c with the calculated one;
    cCal = computeMeasurement(wavelength,gunits,tsCal,fsCal);
    showMeasurements([cExact;cNoise;cCal],1,gunits,{'cNoise','cCalculated'})
    
    
    % 3. show basis
    showBasis(wavelength,coeff,const,params);
    
    % 4. show L-curve
    % figure(5);hold on;
    % x1 = norm(coeffCal.A*coeffCal.alphaN-cOriginal'/max(cOriginal));
    % x2 = norm(coeffCal.alphaN);
    % scatter(x1,x2); grid on;
    % txt = num2str(params.regularisation.lambda);
    % text(x1,x2,txt)
    
    if params.show.Lcurve % if true
        numOfSampling = 20;
        s = svd(coeff.A);
        paramsL = params;
        
        % initialisation
        lambdai = logspace(log10(1e-3*min(s)),log10(1e-3*max(s)),numOfSampling);
        residualNorm = zeros(numOfSampling,1);
        solutionNorm = zeros(numOfSampling,1);
        
        for i = 1:numOfSampling
            paramsL.lambda = lambdai(i);
            [~,~,coeff,~] = computeSpecturm(wavelength,gunits,cNoise,paramsL);
            
            residualNorm(i) = ...
                norm(coeff.A*coeff.alphaN-cNoise'/max(cNoise));
            solutionNorm(i) = norm(coeff.alphaN);
        end
        figure(5);hold off;
        plot(log10(residualNorm),log10(solutionNorm),'k','LineWidth',1.5);hold on;
        plot(log10(residualNorm),log10(solutionNorm),'.','markers',25);
        
        grid on;
        xlabel('residual norm (log10)');
        ylabel('solution norm (log10)');
    end
    
    figure(1);
    
    % create output data
    Exact = struct('tsExact',tsExact,...
        'fsExact',fsExact,'cExact',cExact);
    Calculated= struct('tsCal',tsCal,'fsCal',fsCal,'cCal',cCal,...
        'coeff',coeff,'const',const);
    
    outputData = struct('Calculated',Calculated,'Exact',Exact);
    
end

%% functions
function outputData = calculateSpectrum(wavelength,gunits,c,params)

    [ts,fs,coeff,const] = computeSpecturm(wavelength,gunits,c,params);
    showSpectrum(ts,fs,wavelength,'off');
    showBasis(wavelength,coeff,const,params);
    
    figure(1);
    outputData = struct('ts',ts,...
        'fs',fs,'coeff',coeff,'const',const);
end

%% Test Functions
function [tsTest,fsTest] = generateImpluseFunctions(wavelength,wImpluse_id,fwhm)
% fwhm: Full width at half maximum of the Gaussian curve

    numOftsTest = 300;
    
    dW = unique(diff(wavelength)); assert(isscalar(dW));
    minW = min(wavelength);
    maxW = max(wavelength);
    w0 = minW + dW*(wImpluse_id-1);
    
    sigma = fwhm/(2*sqrt(2*log(2)));
    
    tsTest = linspace(minW,maxW,numOftsTest);
    fsTest = 1/(sigma*sqrt(2*pi))*exp(-0.5*((tsTest- w0 )/sigma).^2);
end

function [tsTest,fsTest] = generateComplexFunctions(wavelength)

    n = 300;
    tsTest = linspace(min(wavelength),max(wavelength),n);
    ffsTest = @(x)(cos(8*pi*x.^2)+1).*exp(sin(10*pi*x));
    
    % normalise spectrum
    IfsTest = quadgk(@(s) ffsTest(s),0,1)*(max(wavelength)-min(wavelength));
    fsTest = ffsTest(linspace(0,1,n))/IfsTest;
    

end


%% Compute Spectrum from Measurements
function [ts,fs,coeff,const] = computeSpecturm(wavelength,gunits,c,params)
    % pre-procesing preparation
    [data,const] = preProcessing(gunits,wavelength,c);

    % compute basis function varphi
    coeff = computeBasisFunctions(data,const,params);
        % check output coeff
        assert(all(isfield(coeff,{'varphiN','alphaN','tiN','tsN'})),...
            'The variable coeff does not have varphi or tiN');

    % ------------------------------------------------------------------------
    % obtain the normalised basis function f(x)
    fsN = getfsN(coeff,params);

    % denormalise data
    [ts,fs] = getDenomralisVariables(coeff.tsN,fsN,const);

end


function k = getKernel(w,wavelengthN,gunitsN,unit_id)
% SPECIFICATION
%   get the value of response (of the unit_id sensor) at the wavelength of
%   "w"
%
% INPUTS:
%   w: the interpolated wavelength
%   wavelength: your measured wavelength
%   gunits: your measured unit responses (a matrix)
%   unit_id: the unit you want to interpolate
%
% OUTPUT:
%   k: the value of response of the unit with id = unit_id at the
%   wavelength of "w"

    k = interp1(wavelengthN,gunitsN(:,unit_id),w,'spline',0);
    % this function will assign any interpolation beyond the input range of
    % wavelength (490nm to 630nm) to zero.
end


% compute fsN at spectied points, tsN
function fsN = getfsN(coeff,params)

    % check input 
    assert(length(coeff.tsN)==params.numOfts); 

    fsN = zeros(params.numOfts,1);
    
    if strcmpi(params.method,'BGM')
        for j = 1:params.numOfts
             for k = 1:length(coeff.alphaN)
                %fsN(j) = fsN(j) + cN(i)*getPhi(tsN(j),i,tiN,varphi);
                fsN(j) = fsN(j) + coeff.alphaN(k)*...
                    interp1(coeff.tiN,coeff.varphiN(k,:),coeff.tsN(j),'spline');
             end
        end
    elseif strcmpi(params.method,'Expansion')
        fsN = coeff.varphiN*coeff.alphaN;
    else
        error('unknown method!');
    end

end

%% Compute Basis Function
function coeff = computeBasisFunctions(data,const,params)

if strcmpi(params.method,'BGM')
    switch params.criterion
        case 'Q-Criterion'
            % compute coefficients
            coeff = computeCoefficient_QMethod(data,const,params);

            % compute the basis function varphiN
            coeff.varphiN = zeros(const.numOfUnits,params.numOfti);
            for i = 1:params.numOfti
                coeff.varphiN(:,i) = quadprog(coeff.Q{1}{i},...
                    [],[],[],coeff.r,1,[],[],[],...
                    params.optimisation);
            end
        case 's-Criterion'
            % compute coefficients
            coeff = computeCoefficient_sMethod(data,const,params);
        otherwise
            error('unknown method');
    end
end

if strcmpi(params.method,'Expansion')
    switch params.basis.name
        case 'Guassian'
            coeff = computeCoefficient_Gaussian(data,const,params);
            
            if params.parameterChoice.flag
                params.regularisation.lambda = ...
                    computeRegularisationParameter(data,coeff,params);
                disp(params.regularisation.lambda);
            end
            
            % compute the basis function varphiN
            coeff.alphaN = zeros(params.numOfti,1);
            
            if isempty(coeff.L) % no regularisation
                coeff.alphaN = lsqlin(coeff.A,data.cN,...
                    [],[],[],[],zeros(params.numOfti,1),[],[],params.optimisation);
            else % use the augmented matrix
                coeff.alphaN = lsqlin([coeff.A;params.regularisation.lambda*coeff.L],...
                    [data.cN';zeros(size(coeff.L,1),1)],...
                    [],[],[],[],zeros(params.numOfti,1),[],[],params.optimisation);
            end
        otherwise
            error('unknown basis');
    end
    
end
    
end

% BGM
% compute coefficient Q and r; get tiN
function coeff = computeCoefficient_QMethod(data,const,params)

    % input check
    assert(all(isfield(data,{'cN','gunitsN','wavelengthN'})),...
        'the input variable: data, does have all required fields');

    % Initialisation
    tiN = linspace(const.minWavelengthN,const.maxWavelengthN,...
        params.numOfti);
    tsN = linspace(const.minWavelengthN,const.maxWavelengthN,...
        params.numOfts);

    Q = cell(params.numOfti,1);
    Qn = getQn(const,params); % compute the noise term

    % compute coefficient matrix Q0
    for k = 1:params.numOfti
        Q{k} = getQt(tiN(k),data,const);
        Q{k} = (Q{k}+Q{k}')/2 + Qn; % force symmetricity and add noise term
    end

    r = getR(data,const); % compute r

    % assembly output coefficients into a struct
    coeff.Q{1} = Q;
    coeff.r = r;
    coeff.tiN = tiN;
    coeff.tsN = tsN;
    coeff.alphaN = data.cN;

    % output check
    assert(isscalar(coeff)); % ensure that coeff is a scalar struct    

    function Q = getQt(tiNi,data,const)

        % intialisation
        Q = zeros(const.numOfUnits);

        for p = 1:const.numOfUnits
            for j = 1:const.numOfUnits
                Q(p,j) = quadgk(@(s) (s-tiNi).^2 ...
                .* getKernel(s,data.wavelengthN,data.gunitsN,p) ...
                .* getKernel(s,data.wavelengthN,data.gunitsN,j)    ,...
                    const.minWavelengthN,const.maxWavelengthN);
            end
        end

    end

    % add the noise term
    function Qn = getQn(const,params)

        % check input datatype
        assert(isvector(params.regularisation.lambda),'the variance of c must be a vector or scalar');

        if isscalar(params.regularisation.lambda)
            Qn = eye(const.numOfUnits)*params.regularisation.lambda;
        elseif length(params.regularisation.lambda)==const.numOfUnits
            Qn = diag(params.regularisation.lambda);
        else
            error('the length of varOfc is not correct');
        end
    end

    function r = getR(data,const)
        r = zeros(1,const.numOfUnits);

        for p = 1:const.numOfUnits
            r(p) = quadgk(@(s) getKernel(s,data.wavelengthN,data.gunitsN,p),...
                const.minWavelengthN,const.maxWavelengthN);
        end
    end

end

% function computeCoefficient_sMethod(data,const,params)
% 
%     % preprocessing
%     gunitsh = fft(gunits); % get the fourier transformation
%     
%     % compute matrix B
%     
%     
%     % compute vector b 
% end

% L2-Gaussian
function coeff = computeCoefficient_Gaussian(data,const,params)
    % input check
    assert(all(isfield(data,{'cN','gunitsN','wavelengthN'})),...
        'the input variable: data, does have all required fields');
    
    tiN = linspace(const.minWavelengthN,const.maxWavelengthN,...
        params.numOfti);
    tsN = linspace(const.minWavelengthN,const.maxWavelengthN,...
        params.numOfts);
    
    % get the basis function
    sigma = params.basis.coeff.fwhmN/(2*sqrt(2*log(2)));
    fvarphi = @(x,x0) 1/(sigma*sqrt(2*pi))*exp(-0.5*((x - x0 )/sigma).^2);
    
    % compute coefficient A
    coeff.A = getA(fvarphi,tiN,data,const,params);
        %coeff.H = coeff.A'*coeff.A;
        %coeff.f = -coeff.A'*data.cN';
    
    % compute the regularisation coefficeint
    coeff.L = getL(params);
    
    coeff.varphiN = zeros(params.numOfts,params.numOfti);
    coeff.tiN = tiN;
    coeff.tsN = tsN;
    
    for i = 1:params.numOfts
        coeff.varphiN(i,:) = fvarphi(tiN,tsN(i));
    end
    
    % check output
    assert(isscalar(coeff)); % ensure that coeff is a scalar struct   
    
    function A = getA(fPhi,tiN,data,const,params)
        % intialisation
        A = zeros(const.numOfUnits,params.numOfti);

        for p = 1:const.numOfUnits
            for j = 1:params.numOfti
                A(p,j) = quadgk(@(s) ...
                getKernel(s,data.wavelengthN,data.gunitsN,p) ...
                .* fPhi(s,tiN(j)),...
                    const.minWavelengthN,const.maxWavelengthN);
            end
        end
    end

    function L = getL(params)
        switch params.regularisation.order
            case -1 || params.regularisation.lambda==0
                L = [];
            case 0
                L = diag(ones(params.numOfti,1));
            case 1
                L = diag(ones(params.numOfti,1))+...
                    diag(-ones(params.numOfti-1,1),1);
            case 2
                L = 2*diag(ones(params.numOfti,1))+...
                    diag(-ones(params.numOfti-1,1),1)+...
                    diag(-ones(params.numOfti-1,1),-1);
            otherwise
                error('undefined');
        end
    end

end


%% Parameter Choice - L-curve method
function lambda = computeRegularisationParameter(data,coeff,params)

    % if use L-curve method to find the lambda
    if strcmp(params.parameterChoice.method,'L-curve')
        lambda = computeParameter_Lcurve(data,coeff,params);
    elseif strcmp(params.parameterChoice.method,'GCV')     
        lambda = computeParameter_GCV(data,coeff,params);
    end
end

% L-curve method
function lambda = computeParameter_Lcurve(data,coeff,params)
    
    [U,S,~] = svd(coeff.A);
    sigma = diag(S);
    
    % if L is an identity matrix;
    assert(params.regularisation.order == 0); % current model only works for 0-th order
    
    if params.regularisation.order == 0
        options = optimset('Display','iter');
        
        %lambda = fminsearch(@(x) -computeCurvature(x,U,sigma,data.cN'),...
        %    params.regularisation.lambda,options);
        
        lambda = fmincon(@(x) -computeCurvature(x,U,sigma,data.cN'),...
            params.regularisation.lambda,[],[],[],[],0,[],[],options);
    end
    
    assert(isscalar(lambda)); % check output
    
    function curvature = computeCurvature(lambda,U,sigma,b)

        numOfcol = length(sigma);

        % initialisation
        [eta, rho, deta] = deal(0);

        % compute fi
        fi = sigma.^2./(sigma.^2+lambda^2);

        for i = 1:numOfcol 
            beta = U(:,i)'*b;

            % compute solution norm
            eta = eta + (fi(i)/sigma(i)*beta).^2;

            % compute the residual norm
            rho = rho + ((1-fi(i))*beta).^2;

            % compute the derivative of solution norm
            deta = deta - 4/lambda*(1-fi(i))*(fi(i)*beta/sigma(i))^2;
        end

        curvature = 2*eta*rho/deta*...
            (lambda^2*deta*rho + 2*lambda*eta*rho+lambda^4*eta*deta)/...
            (lambda^2*eta^2+rho^2)^(3/2);

    end

end

% GCV method
% see: Discrete inverse problems insight and algorithms, pp96.
function lambda = computeParameter_GCV(data,coeff,params)
    [U,S,~] = svd(coeff.A);
    sigma = diag(S);
    
    m = size(coeff.A,1);
    
    options = optimset('Display','iter');

    lambda = fmincon(@(x) computeGCV(x,U,sigma,data.cN',m),...
        params.regularisation.lambda,[],[],[],[],0,[],[],options);
    
    assert(isscalar(lambda)); % check output
    
    function G = computeGCV(lambda,U,sigma,b,m)
        
        numOfcol = length(sigma);
        % compute filter factor
        fi = sigma.^2./(sigma.^2+lambda^2);
        
        rho = 0;
        
        for i = 1:numOfcol 
            beta = U(:,i)'*b;
            
            % compute the residual norm
            rho = rho + ((1-fi(i))*beta).^2;
        end
        
        
        G = rho/(m-sum(fi))^2;
    end
end


%% Compute Measurement from Spectrum
function cCal = computeMeasurement(wavelength,gunits,ts,fs)
    
    numOfUnits = size(gunits,2); % number of units
    
    cCal = zeros(1,numOfUnits); % initialise cCal
    for i = 1:numOfUnits
        cCal(i) = quadgk(@(s) ...
            getKernel(s,wavelength,gunits,i).*...
            interp1(ts,fs,s,'spline'),...
            min(wavelength),max(wavelength));
    end
end


%% Utility
% Pre-processing
function [data,const] = preProcessing(gunits,wavelength,c)

    [data.cN,const.Mc] = getNormalisedVariable(c); % normalise c
    [data.gunitsN,const.Mgunits] = getNormalisedVariable(gunits); % normalise gunits
    [data.wavelengthN,const.Mwavelength] = getNormalisedVariable(wavelength); % normalise wavelength

    const.numOfUnits = size(gunits,2); % total number of units
    const.maxWavelength = max(wavelength); % 
    const.minWavelength = min(wavelength); % 
    const.maxWavelengthN = max(data.wavelengthN);
    const.minWavelengthN = min(data.wavelengthN);

    % Check output data type
    assert(iscolumn(data.wavelengthN),'wavelength must be a column vector');
    assert(isrow(data.cN),'the response c must be a row vector');

    assert(size(data.gunitsN,1)==length(data.wavelengthN), ...
        'the number of rows in gunits must match the length of wavelength');
    assert(size(data.gunitsN,2)==length(data.cN), ...
        'the number of column in gunits must match the length of c');
end

% check input params
function checkParams(params)
    % check params
    assert(all(isfield(params,...
        {'method','numOfti','numOfts','regularisation','optimisation'})),...
        'params does not have all required fields');
    
    if strcmp(params.method,'BGM')
        assert(isfield(params,'criterion'));
        
        if strcmp(params.criterion,'Q-Criterion')
            assert(isfield(params.options,'optimisation'),...
                'params does not have the required OptimisationOptions parameter');
        end
    end
    
    if strcmp(params.method,'Expansion')
        if isfield(params,'regularisation')
            assert(isfield(params.regularisation,'order'));
            assert(isfield(params.regularisation,'lambda'));
        end
    end
end

% normalisation
function [yn,yM] = getNormalisedVariable(y)
    if isvector(y)
        yM = max(y);
        yn = y./yM;
    elseif ismatrix(y)
        yM = max(max(y));
        yn = y./yM;
    else
        error('unknown data type');
    end
end

% de-normalisation
function [ts,fs] = getDenomralisVariables(tsN,fsN,const)
    ts = tsN*const.Mwavelength;
    fs = fsN*(const.Mc/const.Mwavelength/const.Mgunits);
end

% visualisation
function showSpectrum(ts,fs,wavelength,HoldOff)
    figure(1);
    if strcmpi(HoldOff,'off')
        hold on;
    elseif strcmpi(HoldOff,'on')
        hold on;
    end
    
    plot(ts,fs)
    grid on
    xlim([min(wavelength),max(wavelength)]);
    set(gcf,'color','w');
    xlabel('wavelength/nm');
    
end

function showMeasurements(c,c0Index,gunits,labels)
    numOfUnits = size(gunits,2); % number of units

    assert(size(c,2)==numOfUnits)
    cShow = c';
    c0 = cShow(:,c0Index);
    cShow(:,c0Index) = []; % delete the c0 column
    
    figure(2); hold on;
    plot((cShow-c0)./c0*100,'.','markers',25);
    
    if exist('labels','var')==1
        if iscell(labels) && isvector(labels) && length(labels)==size(cShow,2)
            legend(labels);
        end
    end
    set(gcf,'color','w');
    grid on;
    xlabel('measurement units');
    ylabel('relative error (%)');
end

function showBasis(wavelength,coeff,const,params)

    switch params.method
        case 'Expansion'           
            [xs,ys] = getDenomralisVariables(coeff.tsN,coeff.varphiN,const);
        case 'BGM'
            [xs,ys] = getDenomralisVariables(coeff.tiN,coeff.varphiN',const);
        otherwise
            error('unknown method!')
    end
    
        
    
end