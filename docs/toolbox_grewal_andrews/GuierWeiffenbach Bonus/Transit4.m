%  Transit4.m
% 
%  Performs HDOP analysis of Doppler-based satellite navigation for receivers
%  located on a half-degree grid from -60 deg. lattitude to +60 deg latitude,
%  and from -15 degrees longitude to +105 degrees longitude, during a single 
%  pass of a U.S. Navy Transit satellite in 1000 km polar orbit from the
%  South Pole to the North Pole.
%
%%%%%%        WARNING: THIS MAY RUN FOR SEVERAL HOURS
%
close all;
clear all;
%
% PRINCIPAL SUBROUTINES CALLED
%
%   SatLoc      Computes satellite location in ECI coordinates as a function of time
%   AntLoc      Computes receiver antenna location in ECI coordinates as a function of time
%   dRRdLAT     Computes partial derivative of transmitter-to-receiver range rate with respect to receiver latitude
%   dRRdLON     Computes partial derivative of transmitter-to-receiver range rate with respect to receiver longitude
%    
% KEY PARAMETERS AND VARIABLES
%
alphaPM0    = 30*pi/180;        % right ascension of prime meridian at time t=0 [rad]
alphaSAT    = 75*pi/180;        % right ascension of polar satellite orbit [rad]
c           = 299792458;        % speed of light [m/s]
delta0      = 0;                % declination of satellite at time t=0 [rad]
fSAT        = 1.5e8;            % satellite signal frequency [Hz]
hSAT        = 1e6;              % altitude of circular satellite orbit [m]
LatAnt      = 45*pi/180;        % latitude of receiver antenna [rad]
LonAnt      = 30*pi/180;        % longitude of receiver antenna [rad]
OmegaEarth  = 7.292115e-5;      % Earth rotation rate [rad/s]
OmegaSAT    = 2*pi/(105*60);    % satellite rotation rate around Earth [rad/s]
Rearth      = 6371000;          % radius of Earth [m]
t           = 0;                % time [s]
%
% Derived parameters
%
CosConeRad  = Rearth/(Rearth+hSAT); % cosine of earth-centric angle from satellite sub-point to horizon
%
% Cumulative summed variables (for unscaled information matrix)
%
gamma11     = zeros(241,281);
gamma12     = zeros(241,281);
gamma22     = zeros(241,281);
HDOP        = zeros(241,281);
%
%
%
for t=-1575:1575, % sampling every second
    disp(['t = ',num2str(t),' [sec]']);
    SATECI  = SatLoc(alphaPM0,alphaSAT,delta0,hSAT,LatAnt,LonAnt,OmegaEarth,OmegaSAT,Rearth,t);
    uSAT    = SATECI/norm(SATECI);
    for klat=1:241,
        ilat        = klat/2;
        LatAnt      = (ilat - 60.5)*pi/180;
        cLat(klat)  = cos(LatAnt);
        for klon=1:281,
            ilon    = klon/2;
            LonAnt  = (ilon - 35.5)*pi/180;
            ANTECI  = AntLoc(alphaPM0,alphaSAT,delta0,hSAT,LatAnt,LonAnt,OmegaEarth,OmegaSAT,Rearth,t);
            uANT    = ANTECI/norm(ANTECI);
            if (uANT'*uSAT) > CosConeRad
                dRangeRatedLat      = dRRdLAT(alphaPM0,alphaSAT,delta0,hSAT,LatAnt,LonAnt,OmegaEarth,OmegaSAT,Rearth,t);
                dRangeRatedLon      = dRRdLON(alphaPM0,alphaSAT,delta0,hSAT,LatAnt,LonAnt,OmegaEarth,OmegaSAT,Rearth,t);
                H                   = [dRangeRatedLat;dRangeRatedLon];
                dGamma              = H*H';
                gamma11(klat,klon)  = gamma11(klat,klon) + dGamma(1,1);
                gamma12(klat,klon)  = gamma12(klat,klon) + dGamma(1,2);
                gamma22(klat,klon)  = gamma22(klat,klon) + dGamma(2,2);
            end;
       end;
    end;
end;
save 'Gamma', gamma11,gamma12,gamma22;
%
% Analyze information matrices used to get HDOPs
%
disp('ANALYZE CONDITION NUMBERS OF INFORMATION MATRICES');
disp('--- AT ALL ANTENNA LOCATIONS.');
CNlimit             =  1e-2/eps;
format long;
disp(['Machine precision limit = eps']);
disp(['                        = ',num2str(eps)]);
disp(['Condition number limit  = 1e-6/eps']);
disp(['Condition number limit  = ',num2str(CNlimit)]);
CNvec               = [];
DetGammaVec         = [];
HDOPvec             = [];
NoInfCNs            = 0;   % number of infinite condition numbers
for klat=1:241,
    ilat                = klat/2;
    LatAntDeg(klat)     = ilat - 60.5;
    CosLatSq(klat)      = (cos(LatAntDeg(klat)*pi/180))^2;
    for klon=1:281,
        ilon            = klon/2;
        LonAntDeg(klon) = ilon - 35.5;
        Gamma           = [gamma11(klat,klon),gamma12(klat,klon);gamma12(klat,klon),gamma22(klat,klon)];
        CNS             = cond(Gamma);
        if CNS == Inf,
            NoInfCNs    = NoInfCNs + 1;
        end;
        if CNS < CNlimit
            condGamma(klat,klon)    = CNS;
            CNvec                   = [CNvec,condGamma(klat,klon)];
            detGamma                = gamma11(klat,klon)*gamma22(klat,klon) - gamma12(klat,klon)^2;
            DetGammaVec             = [DetGammaVec,detGamma];
            HDOP(klat,klon)         = c*Rearth/fSAT*sqrt((CosLatSq(klat)*gamma22(klat,klon) + gamma11(klat,klon))/detGamma);
            HDOPvec                 = [HDOPvec,HDOP(klat,klon)];
        else
            condGamma(klat,klon)    = ceil(CNlimit);
            HDOP(klat,klon)         = 1e5;
        end;
    end;
end;
maxCN       =   max(CNvec);
minCN       =   min(CNvec);
maxDetGamma =   max(DetGammaVec);
minDetGamma =   min(DetGammaVec);
maxHDOP     =   max(HDOPvec);
minHDOP     =   min(HDOPvec);
format long;
disp(['Number of infinite condition numbers      = ',num2str(NoInfCNs)]);
disp(['Maximum condition number of Gamma < 1e10  = ',num2str(maxCN)]);
disp(['Minimum condition number of Gamma < 1e10  = ',num2str(minCN)]);
disp(['Maximum determinant of Gamma in this case = ',num2str(maxDetGamma)]);
disp(['Minimum determinant of Gamma in this case = ',num2str(minDetGamma)]);
disp(['Maximum HDOP in this case                 = ',num2str(maxHDOP)]);
disp(['Minimum HDOP in this case                 = ',num2str(minHDOP)]);
figure;
hist(log10(CNvec),100);
title('HISTOGRAM OF CONDITION NUMBERS');
xlabel('log10 OF CONDITION NUMBER');
ylabel('NUMBER OF OCCURRENCES');
figure;
hist(log10(DetGammaVec),100);
title('HISTOGRAM OF INFORMATION MATRIX DETERMINANTS');
xlabel('log10 OF INFORMATION MATRIX DETERMINANT');
ylabel('NUMBER OF OCCURRENCES');
figure;
hist(log10(HDOPvec),100);
title('HISTOGRAM OF HDOP VALUES');
xlabel('log10 HDOP VALUE');
ylabel('NUMBER OF OCCURRENCES');
%
figure;
[C,h] = contour(LonAntDeg,LatAntDeg,condGamma,[100,1000,50000],'k-','LineWidth',1.5);
clabel(C,h,'Color','k','FontSize',11);
set(gca,'FontSize',12);
xlabel('LONGITUDE [DEG]');
ylabel('LATITUDE [DEG]');
title('SIGMA CONDITION NUMBER CONTOURS AFTER SINGLE TRANSIT PASS');
%
figure;
[C,h] = contour(LonAntDeg,LatAntDeg,HDOP,[100,1000,50000],'k-','LineWidth',1.5);
clabel(C,h,'Color','k','FontSize',11);
set(gca,'FontSize',16);
xlabel('LONGITUDE [DEG]');
ylabel('LATITUDE [DEG]');
title('HDOP CONTOURS AFTER SINGLE TRANSIT PASS');
% print -dbmp -r600 'HDOPContours.bmp';
figure;
semilogy(LonAntDeg,HDOP(120,:),'k-','LineWidth',1.5);
set(gca,'FontSize',16);
xlabel('LONGITUDE [DEG]');
ylabel('HDOP AT EQUATOR');
title('EQUATORIAL CROSS-SECTION OF HDOP');
