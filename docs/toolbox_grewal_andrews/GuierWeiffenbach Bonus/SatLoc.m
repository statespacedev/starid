function SATECI = SatLoc(alphaPM0,alphaSAT,delta0,hSAT,LatAnt,LonAnt,OmegaEarth,OmegaSAT,Rearth,t)
%
% Compute satellite location in ECI coordinates
%
% INPUTS
%
%   alphaPM0    right ascension of prime meridian at time t=0 [rad]
%   alphaSAT,   right ascension of polar satellite orbit [rad]
%   delta0,     declination of satellite at time t=0 [rad]
%   hSAT,       altitude of circular satellite orbit [m]
%   LatAnt,     latitude of receiver antenna [rad]
%   LonAnt,     longitude of receiver antenna [rad]
%   OmegaEarth, Earth rotation rate [rad/s]
%   OmegaSAT,   satellite rotation rate around Earth [rad/s]
%   Rearth,     radius of Earth [m]
%   t           time at which Doppler-based range rate is measured [s]
%
% OUTPUT
%
%   SATECI      Cartesian coordinates of satellite at time t [m]
%
t1 = Rearth + hSAT;
t3 = t * OmegaSAT + delta0;
t4 = cos(t3);
t5 = t4 * t1;
t6 = cos(alphaSAT);
t8 = sin(alphaSAT);
t10 = sin(t3);
SATECI  = [t6 * t5;t8 * t5;t10 * t1];

