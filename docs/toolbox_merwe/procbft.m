addpath('./core')
addpath('./examples/gssm');         % add relative search path to example GSSM files to MATLABPATH
addpath('./examples/data');         % add relative search path to example data files to MATLABPATH
model = bft('init');
Arg.model = model;                                   % embed GSSM
Arg.type = 'state';                                  % estimation type
Arg.tag = 'State estimation for bearings and frequency tracking problem';  % info tag (not required)
InfDS = geninfds(Arg);                               % call generate function
ftype = input('Inference algorithm  [ srcdkf / pf / sppf / gspf / gmsppf ] : ','s');  %  set type of inference algorithm (estimator) to use :
T = 100;                                              % max. time k=1..T
V = model.pNoise.sample( model.pNoise, T);     % generate process noise
W = model.oNoise.sample( model.oNoise, T);     % generate observation noise
sub=zeros(InfDS.statedim,T);
sub(1,1)=0;
sub(2,1)=10;
sub(3,1)=0;
sub(4,1)=0;
sub(5,1)=350;
for k=2:(T/2-1);
    sub(:,k)=model.ffun(model,sub(:,k-1),V(:,k-1),[]);
end
sub_speedX=sub(2,T/2-1);
sub_speedY=sub(4,T/2-1);
sub(2,T/2-1)=sub_speedY;
sub(4,T/2-1)=sub_speedX;
for k=T/2:T;
    sub(:,k)=model.ffun(model,sub(:,k-1),V(:,k-1),[]);
end
X = zeros(InfDS.statedim, T);                        % system state buffer
y = zeros(InfDS.obsdim,T);                           % system observations buffer
range_0        = 2000 + 100*randn(1);
bearing_0      = -pi + 2*pi*rand(1);
frequency_0    = 300;
course_0       = -pi + 2*pi*rand(1);
speed_0        = 12+1*randn(1);
X(:,1)=[range_0.*cos(bearing_0) + sub(1,1);
    speed_0.*cos(course_0);
    range_0.*sin(bearing_0) + sub(1,1);
    speed_0.*sin(course_0);
    frequency_0];
y(:,1) = model.hfun( model, X(:,1), W(:,1), sub(:,1));  % initial observation
for k=2:T,
    X(:,k) = model.ffun( model, X(:,k-1), V(:,k-1), []);
    y(:,k) = model.hfun( model, X(:,k), W(:,k), sub(:,k));
end
true_range   = sqrt((X(1,:)-sub(1,:)).^2 + (X(3,:)-sub(3,:)).^2); % calculate range ground truth trajectory
true_bearing = atan2(X(3,:)-sub(3,:),X(1,:)-sub(1,:));   % calculate bearing ground truth trajectory
for m=1:T
true_frequency(1,m) = X(5,m)*(1+1/1500*((sub(2,m)-X(2,m))*cos(true_bearing(1,m))+(sub(4,m)-X(4,m))*sin(true_bearing(1,m))));
end

Xh = zeros(InfDS.statedim, T);
Sx = eye(InfDS.statedim);
range_error   = zeros(1,T);
bearing_error = zeros(1,T);
pos_error     = zeros(1,T);
Nstat = 10000;
bearing_stat      = true_bearing(1)+sqrt(model.oNoise.cov(1,1))*randn(1,Nstat);          % observed (measured) bearing (mean) + observation noise
range_stat        = 2000+500*randn(1,Nstat);
course_stat       = course_0 + 2*rand(1,Nstat);
speed_stat        = 12+1*randn(1,Nstat);
frequency_stat    = 300+sqrt(model.oNoise.cov(2,2))*randn(1,Nstat);
Xstat=[range_stat.*cos(bearing_stat) + sub(1,1);
    speed_stat.*cos(course_stat);
    range_stat.*sin(bearing_stat) + sub(1,1);
    speed_stat.*sin(course_stat);
    frequency_stat];
Mu0 = mean(Xstat,2);
P0  = cov(Xstat');
switch ftype
  case 'sppf'
      numParticles = 200;                        % number of particles
  otherwise
      numParticles = 1000;
end
bearing_stat      = true_bearing(1)+sqrt(model.oNoise.cov(1,1))*randn(1,numParticles);
range_stat        = 2000+500*randn(1,numParticles);
course_stat       = course_0 + 2*rand(1,numParticles);
speed_stat        = 12+1*randn(1,numParticles);
frequency_stat    = 300+sqrt(model.oNoise.cov(2,2))*randn(1,numParticles);
initialParticles = [range_stat.*cos(bearing_stat) + sub(1,1);
                    speed_stat.*cos(course_stat);
                    range_stat.*sin(bearing_stat) + sub(1,1);
                    speed_stat.*sin(course_stat);
                    frequency_stat];

fprintf('Estimating trajectory...');
switch ftype
  case 'pf'
      [pNoise, oNoise, InfDS] = gensysnoiseds(InfDS, ftype);     % call system noise sources generation function
      ParticleFiltDS.N = numParticles;
      ParticleFiltDS.particles = initialParticles;
      ParticleFiltDS.weights = (1/numParticles)*ones(1,numParticles);
      InfDS.resampleThreshold = 1;    % set resample threshold
      InfDS.estimateType = 'mean';    % estimate type for Xh
      [Xh, ParticleFiltDS] = pf(ParticleFiltDS, pNoise, oNoise, y, [], sub, InfDS);
  case 'gspf'
      [pNoise, oNoise, InfDS] = gensysnoiseds(InfDS, ftype);     % call system noise sources generation function
      ParticleFiltDS.N = numParticles;      % number of particles
      ParticleFiltDS.stateGMM = gmmfit(initialParticles, 3, [0.001 10], 'sqrt', 1);  % fit a 5 component GMM to initial state distribution
      InfDS.estimateType = 'mean';    % estimate type for Xh
      [Xh, ParticleFiltDS] = gspf(ParticleFiltDS, pNoise, oNoise, y, [], sub, InfDS);
  case 'gmsppf'
      [pNoise, oNoise, InfDS] = gensysnoiseds(InfDS, ftype);     % call system noise sources generation function
      ParticleFiltDS.N = numParticles;      % number of particles
      ParticleFiltDS.stateGMM = gmmfit(initialParticles, 3, [0.001 10], 'sqrt', 1);  % fit a 5 component GMM to initial state distribution
      InfDS.estimateType = 'mean';    % estimate type for Xh
      InfDS.spkfType = 'srcdkf';      % Type of SPKF to use inside SPPF (note that ParticleFiltDS.particlesCov should comply)
      InfDS.spkfParams  = sqrt(3);    % scale factor (CDKF parameter h)
      [Xh, ParticleFiltDS] = gmsppf(ParticleFiltDS, pNoise, oNoise, y, [], sub, InfDS);
  case 'sppf'
      [pNoise, oNoise, InfDS] = gensysnoiseds(InfDS, ftype);     % call system noise sources generation function
      InfDS.spkfType = 'srcdkf';      % Type of SPKF to use inside SPPF (note that ParticleFiltDS.particlesCov should comply)
      InfDS.spkfParams  = sqrt(3);    % scale factor (CDKF parameter h)
      InfDS.resampleThreshold = 1;    % set resample threshold
      InfDS.estimateType = 'mean';    % estimate type for Xh
      [pNoiseGAUS, oNoiseGAUS, foo] = gensysnoiseds(InfDS, InfDS.spkfType); % generate Gaussian system noise sources for internal SPKFs
      ParticleFiltDS.N = numParticles;              % number of particles
      ParticleFiltDS.particles = initialParticles;  % initialize particle means
      ParticleFiltDS.particlesCov = repmat(chol(P0)',[1 1 numParticles]);  % initialize article covariances
      ParticleFiltDS.pNoise = pNoiseGAUS;      % embed SPKF noise sources
      ParticleFiltDS.oNoise = oNoiseGAUS;      %   "   "       "    "
      ParticleFiltDS.weights = cvecrep(1/numParticles,numParticles); % initialize particle weights
      [Xh, ParticleFiltDS] = sppf(ParticleFiltDS, pNoise, oNoise, y, [], sub, InfDS);
  case 'srcdkf'
      [pNoise, oNoise, InfDS] = gensysnoiseds(InfDS, ftype);     % call system noise sources generation function
      InfDS.spkfParams  = sqrt(3);    % scale factor (CDKF parameter h)
      Sx = chol(P0)';
      Xh(:,1) = Mu0;
      [Xh, Sx] = srcdkf(Xh(:,1), Sx, pNoise, oNoise, y, [], sub, InfDS);
  case 'srukf'
      [pNoise, oNoise, InfDS] = gensysnoiseds(InfDS, ftype);     % call system noise sources generation function
      InfDS.spkfParams  = [1 2 0];    % scale factor (CDKF parameter h)
      Sx = chol(P0)';
      Xh(:,1) = Mu0;
      [Xh, Sx] = srcdkf(Xh(:,1), Sx, pNoise, oNoise, y, [], sub, InfDS);
  otherwise
      error([' Unknown inference algorithm type ''' ftype '''']);
end

range_estimate = sqrt((Xh(1,:)-sub(1,:)).^2 + (Xh(3,:)-sub(3,:)).^2);
bearing_estimate = atan2(Xh(3,:)-sub(3,:),Xh(1,:)-sub(1,:));
for m=1:T
frequency_estimate(1,m) = Xh(5,m)*(1+1/1500*((sub(2,m)-Xh(2,m))*cos(bearing_estimate(1,m))+(sub(4,m)-Xh(4,m))*sin(bearing_estimate(1,m))));
end
range_error     =  range_estimate - true_range;
bearing_error   =  bearing_estimate - true_bearing;
pos_error       =  sqrt((Xh([1;3],:)-X([1;3],:)).^2);
