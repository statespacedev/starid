% GSSM_BRT  General state space model for Bearing-and-Range Tracking of a randomly maneuvering
%           target relative to a stationary observer.
%
%   The following state space model is used :
%
%     X(k) = |1 1 0 0| X(k-1) + |0.5  0 | V(k-1)
%            |0 1 0 0|          | 1   0 |
%            |0 0 1 1|          | 0  0.5|
%            |0 0 0 1|          | 0   1 |
%
%     O(k) = |  arctan(x3(k)/x1(k))| + N(k)
%            |sqrt(x1(k)^2+x2(k)^2)|
%
%   Where the state vector is defined as the 2D position and velocity vector of the target,
%   relative to a fixed external reference frame, i.e.
%
%     X(k) = |x1(k)| = |x-position at time k|
%            |x2(k)|   |x-velocity at time k|
%            |x3(k)|   |y-position at time k|
%            |x4(k)|   |y-velocity at time k|
%
%   and the observation at time k, O(k) is the bearing angle (in radians) and range from the fixed
%   observer towards the target.
%   The state dynamics are driven by a 2 dimensional white Gaussian noise source and the
%   observations are corrupted by additive white Gaussian noise.
%   See :  Gordon, Salmond & Ewing, "Bayesian State Estimation for Tracking and Guidance Using
%   the Bootstrap Filter", Journal of Guidance, Control and Dynamics, 1995.

function [varargout] = model_interface(func, varargin)
  switch func
    case 'init'
      model = init(varargin);
        error(consistent(model,'gssm'));               % check consistentency of initialized model
      varargout{1} = model;
    otherwise
      error(['Function ''' func ''' not supported.']);
  end

function model = init(init_args)
  model.type = 'gssm';                         % object type = generalized state space model
  model.tag  = 'GSSM_Bearings_Only_Tracking';  % ID tag
  model.statedim   = 4;                      %   state dimension
  model.obsdim     = 2;                      %   observation dimension
  model.paramdim   = 10;                     %   parameter dimension
  model.U1dim      = 0;                      %   exogenous control input 1 dimension
  model.U2dim      = 0;                      %   exogenous control input 2 dimension
  model.Vdim       = 2;                      %   process noise dimension
  model.Ndim       = 2;                      %   observation noise dimension
  model.ffun      = @ffun;                   % file handle to FFUN
  model.hfun      = @hfun;                   % file handle to HFUN
  model.prior     = @prior;
  model.likelihood = @likelihood;            % file handle to LIKELIHOOD
  model.innovation = @innovation;            % file handle to INNOVATION
  model.setparams  = @setparams;             % file handle to SETPARAMS
  model.obsAngleCompIdxVec = [1];            % indicate that the first (and only component) of the observation
  Arg.type = 'gaussian';
  Arg.cov_type = 'full';
  Arg.dim = model.Vdim;
  Arg.mu  = zeros(Arg.dim,1);
  Arg.cov   = (0.01^2)*eye(Arg.dim);
  model.pNoise = gennoiseds(Arg);            % process noise : zero mean white Gaussian noise, cov = 0.001^2
  Arg.type = 'gaussian';
  Arg.cov_type = 'full';
  Arg.dim = model.Ndim;
  Arg.mu = zeros(Arg.dim,1);
  Arg.cov  = [0.1^2 0; 0 0.1^2];
  model.oNoise = gennoiseds(Arg);            % observation noise : zero mean white Gaussian noise, cov=0.01^2
  model.params = zeros(model.paramdim,1);
  model.A = zeros(model.statedim, model.statedim);
  model.G = zeros(model.statedim, model.Vdim);
  model = setparams(model,[1 1 1 1 1 1 0.5 1 0.5 1]');

function model = setparams(model, params, index_vector)
  if (nargin==2)
    model.params = params(:);
  elseif (nargin==3)
    model.params(index_vector) = params(:);
  else
    error('[ setparams ] Incorrect number of input arguments.');
  end
  model.A([1 5 6 11 15 16]) = params(1:6);
  model.G([1 2 7 8]) = params(7:10);
  G = model.G;
  model.convFact1 = (G'*G)\G';    % conversion matrix needed to calculate state transition prior

function new_state = ffun(model, state, V, U1)
  if isempty(V)
      new_state = model.A*state;
  else
      new_state = model.A*state + model.G*V;
  end

function observ = hfun(model, state, N, U2)
  observ_ = [atan2(state(3,:),state(1,:));
            sqrt(state(1,:).^2 + state(3,:).^2)];
  if isempty(N),
    observ = observ_;
  else
    observ = observ_ + N;
    observ(1,:) = addangle(observ_(1,:), N(1,:));
  end

function tranprior = prior(model, nextstate, state, U1, pNoiseDS)
  V = model.convFact1 * (nextstate - model.A*state);
  tranprior = pNoiseDS.likelihood( pNoiseDS, V);

function llh = likelihood(model, obs, state, U2, oNoiseDS)
  observ =  hfun(model, state, [], U2);
  N = obs - observ;
  N(1,:) = subangle(obs(1,:), observ(1,:));
  llh = oNoiseDS.likelihood( oNoiseDS, N);

function innov = innovation(model, obs, observ)
  innov = obs - observ;
  innov(1,:) = subangle(obs(1,:),observ(1,:));
