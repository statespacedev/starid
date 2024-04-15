function dRangeRatedLon = dRRdLON(alphaPM0,alphaSAT,delta0,hSAT,LatAnt,LonAnt,OmegaEarth,OmegaSAT,Rearth,t)
%
% Compute partial derivative of satellite-to receiver range rate with respect
% to receiver antenna longitude [m/s/rad]
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
%   dRangeRatedLon  partial derivative of satellite-to receiver range rate with respect
% to receiver antenna longitude [m/s/rad]
%
t1   = cos(alphaSAT);
t2   = Rearth*t1;
t3   = t*OmegaSAT;
t4   = cos(t3);
t5   = OmegaSAT*t4;
t6   = t5*t2;
t7   = sin(delta0);
t8   = cos(LatAnt);
t9   = t8*t7;
t10  = t*OmegaEarth;
t11  = sin(t10);
t12  = cos(LonAnt);
t13  = t12*t11;
t14  = cos(alphaPM0);
t15  = t14*t13;
t16  = t15*t9;
t18  = cos(delta0);
t19  = t18*t4;
t20  = t19*t2;
t21  = cos(t10);
t22  = t21*t8;
t23  = sin(LonAnt);
t24  = t23*OmegaEarth;
t25  = sin(alphaPM0);
t26  = t25*t24;
t27  = t26*t22;
t29  = sin(alphaSAT);
t30  = hSAT*t29;
t31  = sin(t3);
t32  = OmegaSAT*t31;
t33  = t32*t30;
t34  = t8*t18;
t35  = t23*t21;
t36  = t25*t35;
t37  = t36*t34;
t39  = hSAT*t1;
t40  = t7*t31;
t41  = t40*t39;
t43  = t23*t11;
t44  = t14*t43;
t45  = t44*t34;
t47  = t19*t30;
t48  = t14*t24;
t49  = t48*t22;
t51  = t12*OmegaEarth;
t52  = t14*t51;
t53  = t52*t22;
t55  = t5*t39;
t56  = t25*t43;
t57  = t56*t9;
t59  = t40*t30;
t60  = t11*t8;
t61  = t52*t60;
t63  = t5*t30;
t64  = t36*t9;
t66  = t26*t60;
t68  = t14*t35;
t69  = t68*t9;
t71  = Rearth*t29;
t72  = t40*t71;
t73  = t25*t51;
t74  = t73*t22;
t76  = t5*t71;
t77  = t12*t21;
t78  = t14*t77;
t79  = t78*t9;
t83  = -t16*t6 - t27*t20 + t27*t41 - t37*t33 - t45*t33 - t53*t41 + t49*t47 + t57*t55 - t69*t55 - t61*t59 + t66*t59 - t61*t72 - t64*t63 - t64*t76 - t74*t72 + t79*t76;
t86  = t40*t2;
t87  = t48*t60;
t89  = t32*t2;
t90  = t68*t34;
t93  = t25*t77;
t94  = t93*t34;
t96  = t73*t60;
t98  = t93*t9;
t101 = t32*t71;
t103 = t19*t71;
t105 = t25*t13;
t106 = t105*t9;
t110 = t44*t9;
t112 = t105*t34;
t114 = -t112*t101 - t45*t101 + t49*t103 - t106*t63 - t110*t63 - t87*t20 - t96*t20 - t66*t47 - t49*t59 - t69*t6 - t98*t6 + t66*t72 + t87*t86 + t96*t86 - t90*t89 - t94*t89;
t119 = t15*t34;
t121 = t32*t39;
t123 = t56*t34;
t125 = t19*t39;
t132 = t78*t34;
t137 = t74*t103 - t112*t33 - t119*t121 - t119*t89 + t123*t121 - t90*t121 - t94*t121 - t27*t125 + t53*t125 - t87*t125 - t96*t125 + t132*t33 + t27*t86 + t61*t47 + t74*t47 + t57*t6;
t154 = t132*t101 - t37*t101 + t61*t103 - t66*t103 - t106*t76 - t110*t76 + t123*t89 - t16*t55 + t53*t20 + t87*t41 + t96*t41 - t49*t72 - t53*t86 - t98*t55 - t74*t59 + t79*t63;
t158 = t8*Rearth;
t159 = t78*t158;
t161 = t36*t158;
t163 = t44*t158;
t165 = t105*t158;
t167 = t15*t158;
t169 = t56*t158;
t171 = t68*t158;
t173 = t93*t158;
t184 = Rearth ^ 2;
t185 = t31*hSAT;
t187 = sin(LatAnt);
t190 = -t187*Rearth*t18*t185 + Rearth*hSAT - t159*t125 + t161*t125 + t163*t125 + t165*t125 + t159*t41 - t161*t41 - t163*t41 - t165*t41 - t167*t47 + t167*t59 + t169*t47 - t169*t59 - t171*t47 + t171*t59 - t173*t47 + t173*t59 + t184;
t191 = t4*hSAT;
t204 = hSAT ^ 2;
t205 = t184*t29;
t206 = t40*t205;
t207 = t25*t12;
t208 = t207*t22;
t211 = t14*t23;
t212 = t211*t22;
t215 = t25*t23;
t216 = t215*t60;
t219 = t184*t1;
t220 = t40*t219;
t221 = t215*t22;
t224 = t19*t205;
t225 = t14*t12;
t226 = t225*t60;
t237 = t19*t219;
t238 = t207*t60;
t241 = t225*t22;
t250 = t211*t60;
t255 = -0.2e1*t187*Rearth*t7*t191 - 0.2e1*t187*t18*t31*t184 - 0.2e1*t187*t7*t4*t184 + t204 + 0.2e1*t208*t206 + 0.2e1*t212*t206 - 0.2e1*t216*t206 - 0.2e1*t221*t220 - 0.2e1*t226*t224 + 0.2e1*t216*t224 - 0.2e1*t212*t224 - 0.2e1*t208*t224 + 0.2e1*t226*t206 + 0.2e1*t238*t237 + 0.2e1*t241*t220 - 0.2e1*t241*t237 + 0.2e1*t221*t237 - 0.2e1*t238*t220 - 0.2e1*t250*t220 + 0.2e1*t250*t237;
t256 = 0.2e1*t190 + t255;
t257 = sqrt(t256);
t277 = -t112*t121 - t112*t89 + t132*t121 - t37*t121 - t45*t121 + t49*t125 + t61*t125 - t66*t125 + t74*t125 + t16*t63 + t49*t20 + t74*t20 + t90*t33 + t94*t33 + t87*t47 + t96*t47 + t53*t59;
t295 = -t106*t55 - t110*t55 + t119*t33 - t123*t33 - t27*t59 - t49*t41 - t61*t41 + t66*t41 - t74*t41 - t53*t47 - t64*t55 + t79*t55 - t57*t63 - t87*t59 - t96*t59 + t69*t63 + t98*t63;
t314 = t132*t89 + t16*t76 + t61*t20 - t66*t20 + t27*t47 - t27*t72 - t37*t89 + t53*t72 - t57*t76 - t64*t6 + t79*t6 - t61*t86 + t66*t86 + t69*t76 - t87*t72 - t96*t72 + t98*t76;
t329 = t187*t18*OmegaSAT;
t332 = t187*t7*OmegaSAT;
t338 = t332*t31*Rearth - t329*t4*Rearth + t119*t101 - t123*t101 + t90*t101 + t94*t101 + t27*t103 - t53*t103 + t87*t103 + t96*t103 - t106*t6 - t110*t6 + t332*t185 - t329*t191 - t45*t89 - t49*t86 - t74*t86;
t360 = t167*t125 - t169*t125 + t171*t125 + t173*t125 - t159*t47 + t159*t59 + t161*t47 - t161*t59 + t163*t47 - t163*t59 + t165*t47 - t165*t59 - t167*t41 + t169*t41 - t171*t41 - t173*t41;
t377 = -t221*t206 - t238*t206 + t241*t206 - t250*t206 - t208*t220 + t208*t237 - t212*t220 + t212*t237 + t216*t220 - t216*t237 - t226*t220 + t221*t224 + t238*t224 - t241*t224 + t250*t224 + t226*t237;
t382 = 0.1e1 / t257*(t83 + t114 + t137 + t154)*Rearth - (0.2e1*t360 + 0.2e1*t377) / t257 / t256*(t277 + t295 + t314 + t338)*Rearth / 0.2e1;
dRangeRatedLon = t382;