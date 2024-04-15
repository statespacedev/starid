/* File chapter7-1&7-2&7-3.ox
   Level and trigonometric seasonal model
   with explanatory variable log(PETROL PRICE)
   and intervention variable SEATBELT LAW
   for log UK drivers KSI.
   Ox/SsfPack code for Chapter 7, Sections 1-3, of:
   ****************************************************************
   *  An Introduction to State Space Time Series Analysis (2007). *
   *  Jacques J.F. Commandeur and Siem Jan Koopman.			      *
   *  Oxford: Oxford University Press.							  *
   ****************************************************************/

#include <oxstd.h>
#include <oxdraw.h>
#import  <maximize>
#include <packages/ssfpack/ssfpack.h>

static decl s_mY, s_cT;			// dependent variable (1 x n) and n
static decl s_mX;				// independent variables (2 x n)
static decl s_mStsm, s_vVarCmp;	// matrices for state space model
static decl s_dVar;				// scale factor
static decl s_vPar;				// parameter vector of model

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
		//          expl.var. int.var. level      trigo seasonal     irregular
		s_vVarCmp =    0     |   0   | vr[0] | constant(vr[1],11,1) |  vr[2];
	else if ((s_iLvlVar != -1) && (s_iSeaVar == -1))
		s_vVarCmp =    0     |   0   | vr[0] | constant(0,11,1)     |  vr[1];
	else if ((s_iLvlVar == -1) && (s_iSeaVar != -1))
		s_vVarCmp =    0     |   0   |   0   | constant(vr[0],11,1) |  vr[1];
	else
		s_vVarCmp =    0     |   0   |   0   | constant(0,11,1)     |  vr[0];
}
LogLikStsm(const vY, const pdLik, const pdVar)
{
	decl mphi, momega, msigma, mjphi, ret_val;

	// set up system matrices for state space model
	GetSsfStsm(s_mStsm, &mphi, &momega, &msigma);
	// add explanatory variable log(PETROL PRICE)
	// and intervention variable SEATBELT LAW to state space model
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
	// add explanatory variable log(PETROL PRICE)
	// and intervention variable SEATBELT LAW to state space model
	mjphi = <>;
	AddSsfReg(s_mX, &mphi, &momega, &msigma, &mjphi);
	momega = diag(s_vVarCmp);

	// compute loglikelihood and score vector
    ret_val = SsfLikScoEx(pdLik, &dvar, &msco, vY, mphi, momega, msigma,
							<>, mjphi, <>, <>, s_mX);
	vs = (diagonal(msco)' .*  s_vVarCmp);
	if ( (s_iLvlVar != -1) && (s_iSeaVar != -1))
	{
		pvSco[0][0] = vs[2];
		pvSco[0][1] = sumc(vs[3:13]);
		pvSco[0][2] = vs[14];
		pvSco[0][] /= s_cT;			// scale score vector by sample size
	}
	else if ((s_iLvlVar != -1) && (s_iSeaVar == -1))
	{
		pvSco[0][0] = vs[2];
		pvSco[0][1] = vs[14];
		pvSco[0][] /= s_cT;			// scale score vector by sample size
	}
	else if ((s_iLvlVar == -1) && (s_iSeaVar != -1))
	{
		pvSco[0][0] = sumc(vs[3:13]);
		pvSco[0][1] = vs[14];
    	pvSco[0][] /= s_cT;			// scale score vector by sample size
	}
	else
	{
		pvSco[0] = vs[14] /s_cT;	// scale score vector by sample size
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
		vp = log(<0.5; 0.4; 1>);
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
	decl minf, mKfilter, cres, msofe;
	decl macf, mdenom, dQ, ch, dH, mmoments, dS, dK, dN;
	decl mdisturb, mauxres1, mauxres2;

	SetStsmModel(s_vPar); // map vP to stochastic level and seasonal model
	// set up system matrices for state space model
	GetSsfStsm(s_mStsm, &mphi, &momega, &msigma);
	// add explanatory variable log(PETROL PRICE)
	// and intervention variable SEATBELT LAW to state space model
	mjphi = <>;
	AddSsfReg(s_mX, &mphi, &momega, &msigma, &mjphi);
	momega = diag(s_vVarCmp);
	
	// compute one step forecast errors and prediction error variances
	minf = KalmanInit(mY, mphi, momega, msigma,
						<>, mjphi, <>, <>, s_mX);
	mKfilter = KalmanFilEx(minf, mY, mphi, momega, msigma,
						<>, mjphi, <>, <>, s_mX);

	print("\nmKfilter[][0:4]", mKfilter[][0:4]);

	// test independence of residuals
	cst = columns(mphi);
	cres = s_cT - cst;
	msofe = mKfilter[0][] .* sqrt(mKfilter[15][]);
	macf = acf(msofe[][cst:s_cT-1]', 15);
	print("\n         lag   autocorrelation", (range(1, 15)')~macf[1:15][]);
	print("\n95%-confidence limit = ", 2/sqrt(s_cT));
	mdenom = cres - range(1, 15)';
	dQ = cres*(cres+2)*sumc((macf[1:15][].^2)./mdenom);
	print("\n\nAutocorrelation: Box-Ljung statistic");
	print("\n------------------------------------");
	print("\nQ(15) = ", dQ);

	// test homoscedasticity of residuals
	ch = round((s_cT-cst)/3);
	dH = (sumr(msofe[][s_cT-ch:s_cT-1].^2))/(sumr(msofe[][cst:cst+ch-1].^2));
	print("\nHomoscedasticity");
	print("\n----------------");
	print("\nH(",ch,") = ", dH);
	print("\n1/H(",ch,") = ", 1/dH);
	
	// test normality of residuals
	mmoments = moments(msofe[][cst:s_cT-1]', 4, 0);
	dS = mmoments[3][]/sqrt(mmoments[2][]^3);
	dK = mmoments[4][]/(mmoments[2][]^2);
	dN = cres*(((dS^2)/6)+(((dK-3)^2)/24));
	print("\nNormality");
	print("\n---------");
	print("\nN = ", dN); 
	print("\nChi-2(2;0.05) = 5.99\n");

	// compute smoothed state vector
	cst = columns(mphi);
	mstate = SsfMomentEstEx(ST_SMO, &mks, mY, mphi, momega, msigma,
						<>, mjphi, <>, <>, s_mX);

	print("\nmks[][0:4]", mks[][0:4]);
	mtrend = mks[2][] + (mks[0][] .* s_mX[0][]) + (mks[1][] .* s_mX[1][]);
	mseas = mks[14][] - mtrend;

	// compute smoothed disturbances
	SsfMomentEstEx(DS_SMO, &mdisturb, mY, mphi, momega, msigma,
						<>, mjphi, <>, <>, s_mX);

	// compute auxiliary residuals
	mauxres1 = mdisturb[2][] ./ sqrt(mdisturb[17][]);
	mauxres2 = mdisturb[14][] ./ sqrt(mdisturb[29][]);

	SetDraw(SET_COLORMODEL, 3);
	DrawTMatrix(0, mY, {"log UK drivers KSI"}, 1969, 1, 12, 0, 1);
	if ( (s_iLvlVar != -1) && (s_iSeaVar != -1))
	{
		DrawTMatrix(0, mtrend,
		{"stochastic level + beta*log(PETROL PRICE) + lambda*(SEATBELT LAW)"},
								1969, 1, 12, 0, 2);
		DrawTMatrix(1, mseas, {"stochastic seasonal"},
								1969, 1, 12, 0, 3);
	}
	else if ((s_iLvlVar != -1) && (s_iSeaVar == -1))
	{
		DrawTMatrix(0, mtrend,
		{"stochastic level + beta*log(PETROL PRICE) + lambda*(SEATBELT LAW)"},
								1969, 1, 12, 0, 2);
		DrawTMatrix(1, mseas, {"deterministic seasonal"},
								1969, 1, 12, 0, 3);
	}
	else if ((s_iLvlVar == -1) && (s_iSeaVar != -1))
	{
		DrawTMatrix(0, mtrend,
		{"deterministic level + beta*log(PETROL PRICE) + lambda*(SEATBELT LAW)"},
								1969, 1, 12, 0, 2);
		DrawTMatrix(1, mseas, {"stochastic seasonal"},
								1969, 1, 12, 0, 3);
	}
	else
	{
		DrawTMatrix(0, mtrend,
		{"deterministic level + beta*log(PETROL PRICE) + lambda*(SEATBELT LAW)"},
								1969, 1, 12, 0, 2);
		DrawTMatrix(1, mseas, {"deterministic seasonal"},
								1969, 1, 12, 0, 3);
	}
	DrawTMatrix(2, mY - mks[cst][], {"irregular"}, 1969, 1, 12, 0, 6);
	DrawAcf(3, mY - mks[cst][], {"residuals"}, 14, TRUE, FALSE, TRUE, 6);
	SaveDrawWindow("chapter7-1&7-2&7-3.eps");
	CloseDrawWindow();
	
	// plot auxiliary residuals
	// replace infinite value of mauxres1 in January 1983 by zero
	mauxres1[][168] = 0;
	DrawTMatrix(0, mauxres1, {"Structural level break $t$-tests"}, 1969, 1, 12, 0, 6);
	DrawLine(0, 1969, 2, 1985, 2, 4);
	DrawLine(0, 1969, -2, 1985, -2, 4);
	DrawTMatrix(1, mauxres2, {"Outlier $t$-tests"}, 1969, 1, 12, 0, 6);
	DrawLine(1, 1969, 2, 1985, 2, 4);
	DrawLine(1, 1969, -2, 1985, -2, 4);
	SaveDrawWindow("chapter7-1&7-2&7-3auxiliary.eps");

	// compute t-tests for regression coefficients
	decl pdLik, dvar, msta, csy, ck, vse, mls;
	csy = cst + 1;
	ck = rows(s_mX);
	SsfLikEx(&pdLik, &dvar, mY, mphi, momega, msigma,
							<>, mjphi, <>, <>, s_mX);
	momega *= dvar;
	msta = SsfMomentEstEx(ST_PRED, <>, mY, mphi, momega, msigma,
							<>, mjphi, <>, <>, s_mX);
	vse = sqrt(diagonal(msta[0:ck-1][0:ck-1]));
	mls = msta[cst][0:ck-1] | vse | msta[cst][0:ck-1]./vse;
	mls = mls | 2*tailt(fabs(mls[2][]),s_cT-rows(s_mX)-1);
	print("\nRegression results");
	println("%c",{"coefficient","standard error","t-value","p-value"},"%20.10g",mls');
	println("\n(1/n) Log-likelihood ","%18.10g", pdLik/s_cT,
	"\n\nsigma ","%20.10g", sqrt(dvar), " Residual SS ","%20.10g", dvar*(s_cT-ck));
}
main()
{
	decl cp0 = 169, cp1 = 23;

	s_mY = log(loadmat("UKdriversKSI.txt")');	// load data, transpose, log()
	s_cT = columns(s_mY);					// no of observations

	// set up predictor matrix with explanatory variable log(PETROL PRICE)
	// and intervention variable SEATBELT LAW
	s_mX = loadmat("logUKpetrolprice.txt")' |
					(constant(0,1,cp0) ~ constant(1,1,cp1));

	MaxLik();
	DrawComponents(s_mY);
}
