/* File chapter8-6-2.ox
   Local linear trend model
   for log Finnish fatalities: forecasting.
   Ox/SsfPack code for Chapter 8, Section 6, of:
   ****************************************************************
   *  An Introduction to State Space Time Series Analysis (2007). *
   *  Jacques J.F. Commandeur and Siem Jan Koopman.			      *
   *  Oxford: Oxford University Press.							  *
   ****************************************************************/

#include <oxstd.h>
#include <oxdraw.h>
#import  <maximize>
#include <packages/ssfpack/ssfpack.h>

static decl s_mY, s_cT;			// data (1 x n) and n
static decl s_mStsm, s_vVarCmp;	// matrices for state space model
static decl s_dVar;				// scale factor
static decl s_vPar;				// parameter vector of model

static decl s_iLvlVar = -1;  // 0 = stochastic level; -1 = fixed level
static decl s_iSloVar = 0;  // 0 = stochastic slope; -1 = fixed slope
static decl s_asCmps;		// string array of component names

SetStsmModel(const vP)
{
	// map to local linear trend model
	s_mStsm = <	CMP_LEVEL,     0.5,  0, 0;
				CMP_SLOPE,     0.4,  0, 0;
				CMP_IRREG,     1,    0, 0>;
	// change BFGS parameters into error variances
	decl vr = exp(2.0 * vP);
	// s_vVarCmp is used to update diagonal(Omega)
	if ( (s_iLvlVar != -1) && (s_iSloVar != -1))
		//          level     slope   irregular
		s_vVarCmp =	vr[0]  |  vr[1]  |  vr[2];	
	else if ((s_iLvlVar != -1) && (s_iSloVar == -1))
		s_vVarCmp = vr[0]  |     0   |  vr[1];
	else if ((s_iLvlVar == -1) && (s_iSloVar != -1))
		s_vVarCmp =    0   |  vr[0]  |  vr[1];
	else
		s_vVarCmp =    0   |     0   |  vr[0];
}

LogLikStsm(const vY, const pdLik, const pdVar)
{
	decl mphi, momega, msigma, ret_val;

	// set up system matrices for state space model
	GetSsfStsm(s_mStsm, &mphi, &momega, &msigma);
	momega = diag(s_vVarCmp);	// create Omega from s_vVarCmp
	ret_val = SsfLikEx(pdLik, pdVar, vY, mphi, momega, msigma);
	return ret_val;     		// 1 indicates success, 0 failure
}
LogLikScoStsm(const vY, const pdLik, const pvSco)
{
    decl mphi, momega, msigma, msco, ret_val, dvar, vs;

	// set up system matrices for state space model
	GetSsfStsm(s_mStsm, &mphi, &momega, &msigma);
	momega = diag(s_vVarCmp);

	// compute loglikelihood and score vector
    ret_val = SsfLikScoEx(pdLik, &dvar, &msco, vY, mphi, momega, msigma);
	vs = (diagonal(msco)' .*  s_vVarCmp);
	if ( (s_iLvlVar != -1) && (s_iSloVar != -1))
	{
		pvSco[0][0] = vs[0];
		pvSco[0][1] = vs[1];
		pvSco[0][2] = vs[2];
 		pvSco[0][] /= s_cT;		// scale score vector by sample size
	}
	else if ((s_iLvlVar != -1) && (s_iSloVar == -1))
	{
		pvSco[0][0] = vs[0];
		pvSco[0][1] = vs[2];
    	pvSco[0][] /= s_cT;	// scale score vector by sample size
	}
	else if ((s_iLvlVar == -1) && (s_iSloVar != -1))
	{
		pvSco[0][0] = vs[1];
		pvSco[0][1] = vs[2];
    	pvSco[0][] /= s_cT;	// scale score vector by sample size
	}
	else
	{
		pvSco[0] = vs[2];
    	pvSco[0] /= s_cT;	// scale score vector by sample size
	}
	pdLik[0] /= s_cT;		// scale loglikelihood by sample size
	
    return ret_val;     		// 1 indicates success, 0 failure
}
InitialPar()
{
	decl dlik, dvar, vp;

	if ( (s_iLvlVar != -1) && (s_iSloVar != -1))
	{
		s_asCmps = {"level     ", "slope     ", "irregular "};
		vp = log(<0.5; 0.4; 1>);
	}
	else if ((s_iLvlVar != -1) && (s_iSloVar == -1))
	{
		s_asCmps = {"level     ", "irregular "};
		vp = log(<0.5; 1>);
	}
	else if ((s_iLvlVar == -1) && (s_iSloVar != -1))
	{
		s_asCmps = {"slope     ", "irregular "};
		vp = log(<0.4; 1>);
	}
	else
	{
		s_asCmps = {"irregular "};
		vp = log(<1>);
	}
	SetStsmModel(vp);		// map vp to local linear model
	
	LogLikStsm(s_mY, &dlik, &dvar);
	// scale initial estimates by scale factor
	return vp + 0.5 * log(dvar);
}
Likelihood(const vP, const pdLik, const pvSco, const pmHes)
{                       	// arguments dictated by MaxBFGS()
    decl ret_val;

	SetStsmModel(vP);		// map vP to local linear model
	ret_val = pvSco ? LogLikScoStsm(s_mY, pdLik, pvSco) 
				: LogLikStsm(s_mY, pdLik, &s_dVar);
	return ret_val;		// 1 indicates success, 0 failure
}
MaxLik()
{
	decl vp, dlik, ir;

	vp = InitialPar();		// initialise unconstrained parameters
	print("\ninitial values BFGS parameters",vp);
	print("\n");
	MaxControl(200, 1, 1);	// start iterations BFGS algorithm
    ir = MaxBFGS(Likelihood, &vp, &dlik, 0, FALSE);

    println("\n", MaxConvergenceMsg(ir),
          " using analytical derivatives",
          "\n(1/n) Log-likelihood = ", "%.8g", dlik,
		  "; n = ", s_cT, ";");

	// set up system matrices and compute AIC
	decl mphi, momega, msigma, daic, i;
	GetSsfStsm(s_mStsm, &mphi, &momega, &msigma);
	daic = (-2*dlik*s_cT) + (2*(rows(vp)+columns(mphi)));	
	println("\nAkaike Information Criterion = ", daic/s_cT);

	s_vPar = vp;
	print("\nparameter estimates (unconstrained)");
	for(i=0;i<=rows(vp)-1; i++) 
		print("\n    ", s_asCmps[i], vp[i]);
	print("\n\nerror variance estimates");
	for(i=0;i<=rows(vp)-1; i++) 
		print("\n    ", s_asCmps[i], exp(2.0 * vp[i]));
	print("\n");
}
DrawComponents(const mY)
{
    decl cst, mphi, momega, msigma, mks, mstate;
	decl mKF, mstres, minf;

	SetStsmModel(s_vPar);	// map vP to local linear model
	GetSsfStsm(s_mStsm, &mphi, &momega, &msigma);
	momega = diag(s_vVarCmp);

	//compute prediction errors and their variances
	cst = columns(mphi);
	minf = KalmanInit(mY, mphi, momega, msigma);
	mKF = KalmanFilEx(minf, mY, mphi, momega, msigma);
	print("\nmKF[][0:4]", mKF[][0:4]);

	//compute standardised prediction errors
	mstres = mKF[0][].*sqrt(mKF[3][]);

	// determine serial correlation of residuals
	decl macf, ddenom, dQ10;
	macf = acf(mstres[][2:s_cT-1]', 10);
	print("\n         lag   autocorrelation", (range(1, 10)')~macf[1:10][]);
	print("\n95%-confidence limit = ", 2/sqrt(s_cT));
	ddenom = (s_cT-2) - <1;2;3;4;5;6;7;8;9;10>;
	dQ10 = (s_cT-2)*(s_cT-2+2)*sumc((macf[1:10][].^2)./ddenom);
	print("\n\nAutocorrelation: Box-Ljung statistic");
	print("\n------------------------------------");
	print("\nQ(10) = ", dQ10);
	print("\nChi-2(9;0.05) = 16.92\n");

	// determine homoscedasticity of residuals
	cst = columns(mphi);
	decl ch, dH;
	ch = round((s_cT-cst)/3);
	dH = (sumr(mstres[][s_cT-ch:s_cT-1].^2))/(sumr(mstres[][cst:cst+ch-1].^2));
	print("\nHomoscedasticity");
	print("\n----------------");
	print("\nH(",ch,") = ", dH);
	print("\n1/H(",ch,") = ", 1/dH);
	print("\nF(12,12;0.025) = 3.28\n");

	// determine normality of residuals
	decl mmoments, dS, dK, dN;
	mmoments = moments(mstres[][2:s_cT-1]', 4, 0);
	dS = mmoments[3][]/sqrt(mmoments[2][]^3);
	dK = mmoments[4][]/(mmoments[2][]^2);
	dN = (s_cT-2)*(((dS^2)/6)+(((dK-3)^2)/24));
	print("\nNormality");
	print("\n---------");
	print("\nN = ", dN); 
	print("\nChi-2(2;0.05) = 5.99\n");
	
	// compute smoothed state vector
	mstate = SsfMomentEstEx(ST_SMO, &mks, mY, mphi, momega, msigma);
	
	// compute forecasts for next 5 years
	decl mf, cf, index, csy, mp, i;
	cf = 5;
	csy = rows(mphi);
	index = range(1, cf, 1)';
	mp = SsfMomentEstEx(ST_PRED, 0, s_mY, mphi, momega, msigma);
	mf = SsfForecast(zeros(1, cf), mphi, momega, mp);
	println("State and signal forecasts (cf=","%3d", cf, ") with SEs");
	for (i=0; i<cst; i++)
		println("State ", "%2d", i+1, index ~ mf[i][]' ~ sqrt(mf[csy+i][])');
	for (; i<csy; i++)
		println("Signal ", "%2d", i-cst+1, index ~ mf[i][]' ~ sqrt(mf[csy+i][])');

	mf[csy+cst:][] += diagonal(momega[cst:][cst:])'; 
	println("Observation forecasts (cf=","%3d", cf, ") with SEs");
	for (i=cst; i<csy; i++)
		println("Obs ", "%2d", i-cst+1, index ~ mf[i][]' ~ sqrt(mf[csy+i][])');

	print("\nmf[][0:4]",mf[][0:4]);

	//compute filtered trend and filtered estimation error variance
	decl mme;
	mp = SsfMomentEstEx(ST_PRED, &mme, s_mY, mphi, momega, msigma);
	print("\nmme[][0:4]",mme[][0:4]);

	// plot trend plus forecasts plus confidence intervals
	SetDraw(SET_COLORMODEL, 3);
	DrawTMatrix(0, mY, {"log fatalities in Finland"}, 1970, 1, 1, 0, 1);
	DrawTMatrix(0, mme[0][1:s_cT-1]~mf[cst][], {"filtered trend and forecasts"},
								1971, 1, 1, 0, 2);
	DrawZ(sqrt(mme[3][1:s_cT-1]~mf[cst+csy][]), "", ZMODE_BAND, 1.64, 5);
	SaveDrawWindow("chapter8-6-2.eps");
}

main()
{
	decl data;
	data = loadmat("NorwayFinland.txt")'; // load data, transpose

	print("\nFinland 1970-2003");
	print("\n-----------------\n");
	s_mY = log(data[2][]);		 // log 1970-2003
	s_cT = columns(s_mY);		// no of observations

	MaxLik();
	DrawComponents(s_mY);
}
