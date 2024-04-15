/* File chapter8-6-3.ox
   Stochastic level and trigonometric seasonal model
   with explanatory variable log(PETROL PRICE)
   for UK drivers KSI: forecasting.
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

static decl s_mY, s_cT;		// dependent variable (1 x n) and n
static decl s_mX;			// independent variable (1 x n)
static decl s_mStsm, s_vVarCmp;	// matrices for state space model
static decl s_dVar;			// scale factor
static decl s_vPar;			// parameter vector of model

static decl s_iLvlVar = 0;	// 0 = stochastic level; -1 = fixed level
static decl s_iSeaVar = -1;	// 0 = stochastic seasonal; -1 = fixed seasonal
static decl s_asCmps;		// string array of component names

SetStsmModel(const vP)
{
	// map to stochastic level and seasonal model 
	s_mStsm = <	CMP_LEVEL,     0.5,  0,  0;
                CMP_SEAS_TRIG, 0.4, 12,  0;
                CMP_IRREG,       1,  0,  0>;
	// change BFGS parameters into error variances
	decl vr = exp(2.0 * vP);
	// s_vVarCmp is used to update diagonal(Omega)
	if ( (s_iLvlVar != -1) && (s_iSeaVar != -1))
		//          expl.var.  level      trigo seasonal     irregular
		s_vVarCmp =    0     | vr[0] | constant(vr[1],11,1) |  vr[2];
	else if ((s_iLvlVar != -1) && (s_iSeaVar == -1))
		s_vVarCmp =    0     | vr[0] | constant(0,11,1)     |  vr[1];
	else if ((s_iLvlVar == -1) && (s_iSeaVar != -1))
		s_vVarCmp =    0     |   0   | constant(vr[0],11,1) |  vr[1];
	else
		s_vVarCmp =    0     |   0   | constant(0,11,1)     |  vr[0];
}
LogLikStsm(const vY, const pdLik, const pdVar)
{
	decl mphi, momega, msigma, mjphi, ret_val;

	// set up system matrices for state space model
	GetSsfStsm(s_mStsm, &mphi, &momega, &msigma);
	// add explanatory variable log(PETROL PRICE) to state space model
	mjphi = <>;
	AddSsfReg(s_mX, &mphi, &momega, &msigma, &mjphi);
	momega = diag(s_vVarCmp);	// create Omega from s_vVarCmp
	// compute loglikelihood and scale factor
	ret_val = SsfLikEx(pdLik, pdVar, vY, mphi, momega, msigma,
						<>, mjphi, <>, <>, s_mX);
	return ret_val;     		// 1 indicates success, 0 failure
}
LogLikScoStsm(const vY, const pdLik, const pvSco)
{
    decl mphi, momega, msigma, mjphi, msco, ret_val, dvar, vs;

	// set up system matrices for state space model
	GetSsfStsm(s_mStsm, &mphi, &momega, &msigma);
	// add explanatory variable log(PETROL PRICE) to state space model
	mjphi = <>;
	AddSsfReg(s_mX, &mphi, &momega, &msigma, &mjphi);
	momega = diag(s_vVarCmp);

	// compute loglikelihood and score vector
    ret_val = SsfLikScoEx(pdLik, &dvar, &msco, vY, mphi, momega, msigma,
							<>, mjphi, <>, <>, s_mX);
	vs = (diagonal(msco)' .*  s_vVarCmp);
	if ( (s_iLvlVar != -1) && (s_iSeaVar != -1))
	{
		pvSco[0][0] = vs[1];
		pvSco[0][1] = sumc(vs[2:12]);
		pvSco[0][2] = vs[13];
		pvSco[0][] /= s_cT;			// scale score vector by sample size
	}
	else if ((s_iLvlVar != -1) && (s_iSeaVar == -1))
	{
		pvSco[0][0] = vs[1];
		pvSco[0][1] = vs[13];
		pvSco[0][] /= s_cT;			// scale score vector by sample size
	}
	else if ((s_iLvlVar == -1) && (s_iSeaVar != -1))
	{
		pvSco[0][0] = sumc(vs[2:12]);
		pvSco[0][1] = vs[13];
    	pvSco[0][] /= s_cT;			// scale score vector by sample size
	}
	else
	{
		pvSco[0] = vs[13] /s_cT;	// scale score vector by sample size
	}
	pdLik[0] /= s_cT;		// scale loglikelihood by sample size
	
    return ret_val;     	// 1 indicates success, 0 failure
}
InitialPar()
{
	decl dlik, dvar, vp;

	if ( (s_iLvlVar != -1) && (s_iSeaVar != -1))
	{
		s_asCmps = {"level     ", "seasonal  ", "irregular "};
		vp = log(<3; 2; 1>);
	}
	else if ((s_iLvlVar != -1) && (s_iSeaVar == -1))
	{
		s_asCmps = {"level     ", "irregular "};
		vp = log(<0.5; 1>);
	}
	else if ((s_iLvlVar == -1) && (s_iSeaVar != -1))
	{
		s_asCmps = {"seasonal  ", "irregular "};
		vp = log(<0.4; 1>);
	}
	else
	{
		s_asCmps = {"irregular "};
		vp = log(<1>);
	}
	SetStsmModel(vp);	// map vP to stochastic level and seasonal model
	LogLikStsm(s_mY, &dlik, &dvar);
	// scale initial estimates by scale factor
	return vp + 0.5 * log(dvar);
}
Likelihood(const vP, const pdLik, const pvSco, const pmHes)
{                       		// arguments dictated by MaxBFGS()
    decl ret_val;

	SetStsmModel(vP);	// map vP to stochastic level and seasonal model
	ret_val = pvSco ? LogLikScoStsm(s_mY, pdLik, pvSco)
				: LogLikStsm(s_mY, pdLik, &s_dVar);
	return ret_val;		// 1 indicates success, 0 failure
}
MaxLik()
{
	decl vp, dlik, ir;

	vp = InitialPar();	// initialise unconstrained BFGS parameters
	print("\ninitial values BFGS parameters",vp);
	print("\n");
	MaxControl(50, 1, 1);	// start iterations BFGS algorithm
    ir = MaxBFGS(Likelihood, &vp, &dlik, 0, FALSE);

    println("\n", MaxConvergenceMsg(ir),
          " using analytical derivatives",
          "\n(1/n) Log-likelihood = ", "%.8g", dlik,
		  "; n = ", s_cT, ";");

	// set up system matrices and compute AIC
	decl mphi, momega, msigma, mjphi, daic, i;
	GetSsfStsm(s_mStsm, &mphi, &momega, &msigma);
	// add explanatory variable to state space model
	mjphi = <>;
	AddSsfReg(s_mX, &mphi, &momega, &msigma, &mjphi);
	
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
    decl cst, mphi, momega, msigma, mjphi, mks, mstate, mtrend, mseas;

	SetStsmModel(s_vPar); // map vP to stochastic level and seasonal model
	// set up system matrices for state space model
	GetSsfStsm(s_mStsm, &mphi, &momega, &msigma);
	// add explanatory variable log(PETROL PRICE) to state space model
	mjphi = <>;
	AddSsfReg(s_mX, &mphi, &momega, &msigma, &mjphi);
	momega = diag(s_vVarCmp);

	// compute smoothed state vector
	cst = columns(mphi);
	mstate = SsfMomentEstEx(ST_SMO, &mks, mY, mphi, momega, msigma,
						<>, mjphi, <>, <>, s_mX);

	print("\nmks[][0:4]", mks[][0:4]);
	mtrend = mks[1][] + (mks[0][] .* s_mX[0][]);
	mseas = mks[13][] - mtrend;

	SetDraw(SET_COLORMODEL, 3);
	DrawTMatrix(0, mY, {"log UK drivers KSI"}, 1969, 1, 12, 0, 1);
	if ( (s_iLvlVar != -1) && (s_iSeaVar != -1))
	{
		DrawTMatrix(0, mtrend,
		{"stochastic level + beta*log(PETROL PRICE)"},
								1969, 1, 12, 0, 2);
		DrawTMatrix(1, mseas, {"stochastic seasonal"},
								1969, 1, 12, 0, 3);
	}
	else if ((s_iLvlVar != -1) && (s_iSeaVar == -1))
	{
		DrawTMatrix(0, mtrend,
		{"stochastic level + beta*log(PETROL PRICE)"},
								1969, 1, 12, 0, 2);
		DrawTMatrix(1, mseas, {"deterministic seasonal"},
								1969, 1, 12, 0, 3);
	}
	else if ((s_iLvlVar == -1) && (s_iSeaVar != -1))
	{
		DrawTMatrix(0, mtrend,
		{"deterministic level + beta*log(PETROL PRICE)"},
								1969, 1, 12, 0, 2);
		DrawTMatrix(1, mseas, {"stochastic seasonal"},
								1969, 1, 12, 0, 3);
	}
	else
	{
		DrawTMatrix(0, mtrend,
		{"deterministic level + beta*log(PETROL PRICE)"},
								1969, 1, 12, 0, 2);
		DrawTMatrix(1, mseas, {"deterministic seasonal"},
								1969, 1, 12, 0, 3);
	}
	DrawTMatrix(2, mY - mks[cst][], {"irregular"}, 1969, 1, 12, 0, 6);
	SaveDrawWindow("chapter8-6-3-1.eps");
	CloseDrawWindow();

	// compute forecasts for next 23 months
	decl mf, cf, index, csy, mp, i, mx;
	cf = 23;
	csy = rows(mphi);
	index = range(1, cf, 1)';
	mp = SsfMomentEstEx(ST_PRED, 0, s_mY, mphi, momega, msigma,
						<>, mjphi, <>, <>, s_mX);
	mx = loadmat("logUKpetrolprice.txt")';
	mf = SsfForecast(zeros(1, cf), mphi, momega, mp,
						<>, mjphi, <>, <>, mx[][169:191]);
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

	// plot forecasts plus confidence intervals
	DrawTMatrix(0, mf[cst][], {"forecasts +/- 1.64SE"}, 1983, 2, 12, 0, 2);
	DrawZ(sqrt(mf[cst+csy][]), "", ZMODE_BAND, 1.64, 5);

	// load signal complete model analysed in chapter 7.3
	decl mC = loadmat("signalchapter7-3.txt")';
	// reload complete drivers KSI
	decl my = log(loadmat("UKdriversKSI.txt")');
	DrawTMatrix(1, my[][155:191], {"log UK drivers KSI"},
								1982, 1, 12, 0, 1);
	DrawTMatrix(1, mks[cst][155:168]~mf[cst][], {"signal plus forecasts"},
								1982, 1, 12, 0, 2);
	DrawTMatrix(1, mC[][155:191], {"signal complete model"},
								1982, 1, 12, 0, 3);

	SaveDrawWindow("chapter8-6-3-2.eps");

	// compute t-tests for regression coefficients
	decl pdLik, dvar, msta, ck, vse, mls;
	csy = cst + 1;
	ck = rows(s_mX);
	SsfLikEx(&pdLik, &dvar, mY, mphi, momega, msigma,
							<>, mjphi, <>, <>, s_mX);
	momega *= dvar;
	msta = SsfMomentEstEx(ST_PRED, <>, mY, mphi, momega, msigma,
							<>, mjphi, <>, <>, s_mX);
	vse = sqrt(diagonal(msta[0:ck-1][0:ck-1]));
	mls = msta[cst][0:ck-1] | vse | msta[cst][0:ck-1]./vse;
	print("\nRegression results");
	println("%c",{"coefficient","standard error","t-value"},"%20.10g",mls');
	println("\n(1/n) Log-likelihood ","%18.10g", pdLik/s_cT,
	"\n\nsigma ","%20.10g", sqrt(dvar), " Residual SS ","%20.10g", dvar*(s_cT-ck));
}
main()
{
	s_mY = log(loadmat("UKdriversKSI.txt")');	// load data, transpose, log()
	s_cT = columns(s_mY);				// no of observations

	s_mY = s_mY[][0:168];		// drop last 23 observations
	s_cT = columns(s_mY);		// no of observations

	// load explanatory variable log(PETROL PRICE)
	s_mX = loadmat("logUKpetrolprice.txt")';
	s_mX = s_mX[][0:168];		// drop last 23 observations


	MaxLik();
	DrawComponents(s_mY);
}
