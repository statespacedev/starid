function ANTECI = AntLoc(alphaPM0,alphaSAT,delta0,hSAT,LatAnt,LonAnt,OmegaEarth,OmegaSAT,Rearth,t)
%
% Compute receiver antenna location in ECI coordinates
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
%   ANTECI      receiver antenna location vector in ECI coordinates [m]
%
t1 = cos(LatAnt);
t2 = Rearth * t1;
t4 = t * OmegaEarth + LonAnt + alphaPM0;
t5 = cos(t4);
t7 = sin(t4);
t9 = sin(LatAnt);
ANTECI = [t5 * t2;t7 * t2;t9 * Rearth];
