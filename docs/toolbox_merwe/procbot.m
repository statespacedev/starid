addpath('./core')
addpath('./examples/gssm');         % add relative search path to example GSSM files to MATLABPATH
addpath('./examples/data');         % add relative search path to example data files to MATLABPATH
model = bot('init');
Arg.model = model;                                   % embed GSSM
Arg.type = 'state';                                  % estimation type
Arg.tag = 'State estimation for bearings-only tracking problem';  % info tag (not required)
InfDS = geninfds(Arg);                               % call generate function
ftype = input('Inference algorithm  [ srcdkf / pf / sppf / gmsppf ] : ','s');  %  set type of inference algorithm (estimator) to use :
N = 25;                                              % max. time k=1..N
V = model.pNoise.sample( model.pNoise, N);     % generate process noise
W = model.oNoise.sample( model.oNoise, N);     % generate observation noise
X = zeros(InfDS.statedim, N);                        % system state buffer
y = zeros(InfDS.obsdim,N);                           % system observations buffer
bearing_0      = -pi+rand(1)*2*pi;
bearing_rate_0 = 0.1*randn(1);
range_0        = 0.1*randn(1)+1;
range_rate_0   = 0.01*randn(1)-0.1;
X(:,1) = [range_0*cos(bearing_0);                       % initial target location in 2D-cartesian space
         (range_0 + range_rate_0)*cos(addangle(bearing_0,bearing_rate_0)) - range_0*cos(bearing_0);
         range_0*sin(bearing_0);
         (range_0 + range_rate_0)*sin(addangle(bearing_0,bearing_rate_0)) - range_0*sin(bearing_0)];
y(:,1) = model.hfun( model, X(:,1), W(:,1), []);  % initial observation
for k=2:N,
    X(:,k) = model.ffun( model, X(:,k-1), V(:,k-1), []);
    y(:,k) = model.hfun( model, X(:,k), W(:,k), []);
end
true_range   = sqrt(X(1,:).^2 + X(3,:).^2);             % calculate range ground truth trajectory
true_bearing = atan2(X(3,:), X(1,:));                   % calculate bearing ground truth trajectory

Xh = zeros(InfDS.statedim, N);
Sx = eye(InfDS.statedim);
range_error   = zeros(1,N);
bearing_error = zeros(1,N);
pos_error     = zeros(1,N);
Nstat = 10000;
Wstat = model.oNoise.sample( model.oNoise, Nstat);
bearing_stat      = bearing_0 + sqrt(model.oNoise.cov(1,1))*randn(1,Nstat);
bearing_rate_stat = 0.1*randn(1,Nstat);
range_stat        = 0.1*randn(1,Nstat)+1;
range_rate_stat   = 0.01*randn(1,Nstat)-0.1;
Xstat = [range_stat.*cos(bearing_stat);
    (range_stat + range_rate_stat).*cos(addangle(bearing_stat,bearing_rate_stat)) - range_stat.*cos(bearing_stat);
    range_stat.*sin(bearing_stat);
    (range_stat + range_rate_stat).*sin(addangle(bearing_stat,bearing_rate_stat)) - range_stat.*sin(bearing_stat)];
Mu0 = mean(Xstat,2);
P0  = cov(Xstat');
Xh(:,1) = Mu0;                  % initial state distribution : mean
Sx = chol(P0)';                 % initial state distribution : covariance Cholesky factor
switch ftype
case {'pf','gspf','gmsppf'}
  numParticles = 1000;                        % number of particles
otherwise
  numParticles = 200;
end
bearing_stat      = bearing_0+sqrt(model.oNoise.cov(1,1))*randn(1,numParticles);
bearing_rate_stat = 0.1*randn(1,numParticles);
range_stat        = 0.1*randn(1,numParticles)+1;
range_rate_stat   = 0.01*randn(1,numParticles)-0.1;
initialParticles = [range_stat.*cos(bearing_stat);
                    (range_stat + range_rate_stat).*cos(addangle(bearing_stat,bearing_rate_stat)) - range_stat.*cos(bearing_stat);
                    range_stat.*sin(bearing_stat);
                    (range_stat + range_rate_stat).*sin(addangle(bearing_stat,bearing_rate_stat)) - range_stat.*sin(bearing_stat)];
initialParticles = Sx*randn(InfDS.statedim,numParticles) + cvecrep(Mu0,numParticles);
initialParticlesCov = repmat(Sx,[1 1 numParticles]);  % particle covariances

disp([ftype ' : Estimating trajectory...']);
  switch ftype
  case 'pf'
      [pNoise, oNoise, InfDS] = gensysnoiseds(InfDS, ftype);     % call system noise sources generation function
      ParticleFiltDS.N = numParticles;
      ParticleFiltDS.particles = initialParticles;
      ParticleFiltDS.weights = (1/numParticles)*ones(1,numParticles);
      InfDS.resampleThreshold = 1;    % set resample threshold
      InfDS.estimateType = 'mean';    % estimate type for Xh
      [Xh, ParticleFiltDS] = pf(ParticleFiltDS, pNoise, oNoise, y, [], [], InfDS);
  case 'gmsppf'
      [pNoise, oNoise, InfDS] = gensysnoiseds(InfDS, ftype);     % call system noise sources generation function
      ParticleFiltDS.N = numParticles;      % number of particles
      ParticleFiltDS.stateGMM = gmmfit(initialParticles, 5, [0.001 10], 'sqrt');  % fit a 5 component GMM to initial state distribution
      InfDS.estimateType = 'mean';    % estimate type for Xh
      InfDS.spkfType = 'srcdkf';      % Type of SPKF to use inside SPPF (note that ParticleFiltDS.particlesCov should comply)
      InfDS.spkfParams  = sqrt(3);    % scale factor (CDKF parameter h)
      [Xh, ParticleFiltDS] = gmsppf(ParticleFiltDS, pNoise, oNoise, y, [], [], InfDS);
  case 'sppf'
      [pNoise, oNoise, InfDS] = gensysnoiseds(InfDS, ftype);     % call system noise sources generation function
      InfDS.spkfType = 'srcdkf';      % Type of SPKF to use inside SPPF (note that ParticleFiltDS.particlesCov should comply)
      InfDS.spkfParams  = sqrt(3);    % scale factor (CDKF parameter h)
      InfDS.resampleThreshold = 1;    % set resample threshold
      InfDS.estimateType = 'mean';    % estimate type for Xh
      [pNoiseGAUS, oNoiseGAUS, foo] = gensysnoiseds(InfDS, InfDS.spkfType); % generate Gaussian system noise sources for internal SPKFs
      ParticleFiltDS.N = numParticles;              % number of particles
      ParticleFiltDS.particles = initialParticles;  % initialize particle means
      ParticleFiltDS.particlesCov = initialParticlesCov;  % initialize article covariances
      ParticleFiltDS.pNoise = pNoiseGAUS;      % embed SPKF noise sources
      ParticleFiltDS.oNoise = oNoiseGAUS;      %   "   "       "    "
      ParticleFiltDS.weights = cvecrep(1/numParticles,numParticles); % initialize particle weights
      [Xh, ParticleFiltDS] = sppf(ParticleFiltDS, pNoise, oNoise, y, [], [], InfDS);
  case 'srcdkf'
      [pNoise, oNoise, InfDS] = gensysnoiseds(InfDS, ftype);     % call system noise sources generation function
      InfDS.spkfParams  = sqrt(3);    % scale factor (CDKF parameter h)
      [Xh, Sx] = srcdkf(Xh(:,1), Sx, pNoise, oNoise, y, [], [], InfDS);
  case 'srukf'
      [pNoise, oNoise, InfDS] = gensysnoiseds(InfDS, ftype);     % call system noise sources generation function
      InfDS.spkfParams  = [1 2 0];    % scale factor (CDKF parameter h)
      [Xh, Sx] = srukf(Xh(:,1), Sx, pNoise, oNoise, y, [], [], InfDS);
  otherwise
      error([' Unknown inference algorithm type ''' ftype '''']);
end

range_estimate = sqrt(Xh(1,:).^2 + Xh(3,:).^2);
bearing_estimate = atan2(Xh(3,:), Xh(1,:));
range_error   =  range_estimate - true_range;
bearing_error =  bearing_estimate - true_bearing;
pos_error     =  sqrt((Xh([1;3],:)-X([1;3],:)).^2);

