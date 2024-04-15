function dRangeRatedLat = dRRdLAT(alphaPM0,alphaSAT,delta0,hSAT,LatAnt,LonAnt,OmegaEarth,OmegaSAT,Rearth,t)
%
% Compute partial derivative of satellite-to receiver range rate with respect
% to receiver antenna latitude [m/s/rad]
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
% to receiver antenna latitude [m/s/rad]
%
t1 = cos(alphaSAT);
t2 = Rearth * t1;
t3 = t * OmegaSAT;
t4 = sin(t3);
t5 = OmegaSAT * t4;
t6 = t5 * t2;
t7 = cos(delta0);
t8 = sin(LatAnt);
t9 = t8 * t7;
t10 = t * OmegaEarth;
t11 = sin(t10);
t12 = cos(LonAnt);
t13 = t12 * t11;
t14 = sin(alphaPM0);
t15 = t14 * t13;
t16 = t15 * t9;
t18 = sin(delta0);
t19 = t18 * t4;
t20 = t19 * t2;
t21 = cos(t10);
t22 = t21 * t8;
t23 = t12 * OmegaEarth;
t24 = t14 * t23;
t25 = t24 * t22;
t27 = cos(t3);
t28 = OmegaSAT * t27;
t29 = t28 * t2;
t30 = t8 * t18;
t31 = sin(LonAnt);
t32 = t31 * t11;
t33 = cos(alphaPM0);
t34 = t33 * t32;
t35 = t34 * t30;
t37 = t31 * OmegaEarth;
t38 = t33 * t37;
t39 = t38 * t22;
t41 = t34 * t9;
t43 = t7 * t27;
t44 = t43 * t2;
t46 = sin(alphaSAT);
t47 = Rearth * t46;
t48 = t5 * t47;
t49 = t31 * t21;
t50 = t33 * t49;
t51 = t50 * t9;
t53 = t43 * t47;
t54 = t11 * t8;
t55 = t38 * t54;
t57 = t12 * t21;
t58 = t14 * t57;
t59 = t58 * t9;
t61 = t24 * t54;
t64 = hSAT * t46;
t65 = t5 * t64;
t67 = hSAT * t1;
t68 = t5 * t67;
t69 = t33 * t57;
t70 = t69 * t9;
t72 = t43 * t67;
t76 = t14 * t37;
t77 = t76 * t54;
t79 = t16 * t6 + t16 * t68 + t25 * t20 + t39 * t20 - t25 * t44 - t25 * t72 + t35 * t29 - t39 * t44 - t39 * t72 + t41 * t6 - t51 * t48 - t59 * t48 - t55 * t53 - t61 * t53 - t59 * t65 - t70 * t68 + t77 * t72;
t81 = t43 * t64;
t83 = t33 * t23;
t84 = t83 * t54;
t86 = t14 * t49;
t87 = t86 * t9;
t89 = t76 * t22;
t92 = t83 * t22;
t94 = t14 * t32;
t95 = t94 * t9;
t97 = t28 * t67;
t98 = t15 * t30;
t100 = t19 * t67;
t102 = t33 * t13;
t103 = t102 * t9;
t111 = t25 * t100 + t39 * t100 - t103 * t65 + t98 * t29 + t35 * t97 + t41 * t68 + t95 * t48 - t51 * t65 - t89 * t53 + t92 * t53 - t55 * t81 + t95 * t65 + t87 * t68 - t84 * t72 - t89 * t81 + t92 * t81 + t98 * t97;
t113 = t86 * t30;
t117 = t27 * hSAT;
t118 = t7 * OmegaSAT;
t119 = cos(LatAnt);
t120 = t119 * t118;
t122 = t4 * hSAT;
t123 = t18 * OmegaSAT;
t124 = t119 * t123;
t126 = t27 * Rearth;
t128 = t4 * Rearth;
t135 = t19 * t47;
t137 = t69 * t30;
t141 = t28 * t47;
t142 = t58 * t30;
t144 = -t77 * t100 - t103 * t48 + t113 * t29 - t120 * t117 - t120 * t126 + t124 * t122 + t124 * t128 + t55 * t135 + t61 * t135 - t137 * t29 - t142 * t141 - t77 * t20 + t84 * t20 + t77 * t44 - t84 * t44 - t70 * t6 + t87 * t6;
t146 = t28 * t64;
t147 = t102 * t30;
t149 = t19 * t64;
t155 = t50 * t30;
t158 = t94 * t30;
t167 = t84 * t100 + t113 * t97 + t89 * t135 - t92 * t135 - t137 * t97 - t147 * t141 - t155 * t141 + t158 * t141 - t142 * t146 - t147 * t146 - t155 * t146 + t158 * t146 + t55 * t149 + t61 * t149 + t89 * t149 - t92 * t149 - t61 * t81;
t171 = Rearth ^ 2;
t172 = t4 * t171;
t175 = t27 * t171;
t178 = Rearth * t7;
t182 = Rearth * t18;
t187 = hSAT ^ 2;
t190 = t119 * Rearth;
t191 = t94 * t190;
t194 = t50 * t190;
t197 = t58 * t190;
t200 = t102 * t190;
t209 = t171 * t1;
t210 = t19 * t209;
t211 = t11 * t119;
t212 = t33 * t31;
t213 = t212 * t211;
t216 = t43 * t209;
t219 = t14 * t12;
t220 = t219 * t211;
t223 = t21 * t119;
t224 = t33 * t12;
t225 = t224 * t223;
t230 = -0.2e1 * t8 * t182 * t117 - 0.2e1 * t8 * t178 * t122 + 0.2e1 * Rearth * hSAT - 0.2e1 * t191 * t149 + 0.2e1 * t194 * t149 + 0.2e1 * t197 * t149 + 0.2e1 * t200 * t149 - 0.2e1 * t9 * t172 - 0.2e1 * t30 * t175 + 0.2e1 * t191 * t81 - 0.2e1 * t194 * t81 - 0.2e1 * t197 * t81 - 0.2e1 * t213 * t210 + 0.2e1 * t225 * t210 + 0.2e1 * t213 * t216 + 0.2e1 * t220 * t216 - 0.2e1 * t225 * t216 + 0.2e1 * t171 + t187;
t231 = t14 * t31;
t232 = t231 * t223;
t235 = t171 * t46;
t236 = t43 * t235;
t237 = t224 * t211;
t239 = t231 * t211;
t241 = t212 * t223;
t243 = t219 * t223;
t245 = t19 * t235;
t251 = t69 * t190;
t253 = t86 * t190;
t255 = t34 * t190;
t257 = t15 * t190;
t264 = t251 * t100 - t253 * t100 - t255 * t100 - t257 * t100 - t200 * t81 - t220 * t210 - t232 * t210 + t232 * t216 - t237 * t236 + t239 * t236 - t241 * t236 - t243 * t236 + t237 * t245 - t239 * t245 + t241 * t245 + t243 * t245 - t251 * t72 + t253 * t72 + t255 * t72 + t257 * t72;
t265 = t230 + 0.2e1 * t264;
t266 = sqrt(t265);
t269 = t8 * t118;
t271 = t8 * t123;
t275 = t119 * t7;
t276 = t50 * t275;
t278 = t38 * t211;
t280 = t58 * t275;
t282 = t24 * t211;
t284 = t119 * t18;
t285 = t102 * t284;
t287 = t83 * t223;
t289 = t94 * t284;
t291 = t76 * t223;
t293 = t50 * t284;
t296 = t58 * t284;
t299 = t69 * t284;
t301 = -t269 * t117 + t271 * t122 - t269 * t126 + t271 * t128 - t278 * t135 - t282 * t135 + t287 * t135 - t291 * t135 + t285 * t141 - t289 * t141 + t293 * t141 + t296 * t141 + t276 * t48 + t278 * t53 + t280 * t48 + t282 * t53 + t299 * t29;
t302 = t83 * t211;
t304 = t69 * t275;
t307 = t86 * t275;
t309 = t76 * t211;
t311 = t86 * t284;
t314 = t102 * t275;
t317 = t94 * t275;
t320 = t15 * t284;
t322 = t24 * t223;
t324 = t34 * t284;
t326 = t38 * t223;
t328 = t34 * t275;
t331 = -t302 * t20 + t309 * t20 - t322 * t20 - t326 * t20 - t287 * t53 - t311 * t29 - t320 * t29 - t324 * t29 + t291 * t53 + t302 * t44 + t304 * t6 - t307 * t6 - t309 * t44 + t314 * t48 - t317 * t48 + t326 * t44 - t328 * t6;
t333 = t15 * t275;
t351 = -t302 * t100 + t309 * t100 + t285 * t146 - t289 * t146 + t293 * t146 + t296 * t146 - t278 * t149 - t282 * t149 + t287 * t149 - t291 * t149 + t280 * t65 + t282 * t81 + t299 * t97 - t311 * t97 + t322 * t44 - t324 * t97 - t333 * t6;
t369 = -t322 * t100 - t326 * t100 + t276 * t65 + t278 * t81 - t287 * t81 + t291 * t81 + t302 * t72 + t304 * t68 - t307 * t68 - t309 * t72 + t314 * t65 - t317 * t65 - t320 * t97 + t322 * t72 + t326 * t72 - t328 * t68 - t333 * t68;
t381 = t8 * Rearth;
t382 = t58 * t381;
t384 = t102 * t381;
t386 = t94 * t381;
t388 = t50 * t381;
t392 = t15 * t381;
t396 = t86 * t381;
t398 = t34 * t381;
t400 = t69 * t381;
t404 = -t119 * t182 * t117 - t119 * t178 * t122 + t392 * t100 + t396 * t100 + t398 * t100 - t400 * t100 - t382 * t149 - t384 * t149 + t386 * t149 - t388 * t149 - t275 * t172 - t284 * t175 + t382 * t81 + t384 * t81 - t386 * t81 + t388 * t81 - t392 * t72 - t396 * t72;
t407 = t231 * t54;
t409 = t212 * t22;
t411 = t219 * t22;
t413 = t224 * t54;
t418 = t219 * t54;
t420 = t212 * t54;
t424 = t224 * t22;
t427 = t231 * t22;
t431 = t418 * t210 + t420 * t210 - t424 * t210 + t427 * t210 - t418 * t216 - t420 * t216 + t424 * t216 - t427 * t216 - t407 * t236 + t409 * t236 + t411 * t236 + t413 * t236 + t407 * t245 - t409 * t245 - t411 * t245 - t413 * t245 - t398 * t72 + t400 * t72;
t436 = 0.1e1 / t266 * (t79 + t111 + t144 + t167) * Rearth - (0.2e1 * t404 + 0.2e1 * t431) / t266 / t265 * (t301 + t331 + t351 + t369) * Rearth / 0.2e1;
dRangeRatedLat = t436;
%
