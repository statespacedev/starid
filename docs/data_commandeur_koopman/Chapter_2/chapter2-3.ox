/* File chapter2-3.ox
   Local level model
   for log Norwegian fatalities.
   Ox/SsfPack code for Chapter 2, Section 3, of:
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

static decl s_iLvlVar = 0;  // 0 = stochastic level; -1 = deterministic level
static decl s_asCmps;		// string array of component names

SetStsmModel(const vP)
{
	// map to local level model 
	s_mStsm = <	CMP_LEVEL,	0.5,	0,	0;
				CMP_IRREG,	1,		0,	0>;
	// change BFGS parameters into error variances
	decl vr = exp(2.0 * vP);
	// s_vVarCmp is used to update diagonal(Omega)
	if (s_iLvlVar != -1)
		//          level irregular
		s_vVarCmp = vr[0]| vr[1];
	else
		s_vVarCmp =	  0  | vr[0];
}

LogLikStsm(const vY, const pdLik, const pdVar)
{
	decl mphi, momega, msigma, ret_val;
 
	// set up system matrices for state space model
	GetSsfStsm(s_mStsm, &mphi, &momega, &msigma);
	momega = diag(s_vVarCmp);		// update Omega from s_vVarCmp
	// compute loglikelihood and scale factor
	ret_val = SsfLikEx(pdLik, pdVar, vY, mphi, momega, msigma);
	return ret_val;     			// 1 indicates success, 0 failure
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
	if (s_iLvlVar != -1)
	{
		pvSco[0][0] = vs[0];
		pvSco[0][1] = vs[1];
		pvSco[0][] /= s_cT;	// scale score vector by sample size
	}
	else
	{
		pvSco[0] = vs[1];
    	pvSco[0] /= s_cT;	// scale score vector by sample size
	}
    pdLik[0] /= s_cT;		// scale loglikelihood by sample size
	
    return ret_val;     	// 1 indicates success, 0 failure
}
InitialPar()
{
	decl dlik, dvar, vp;

	if (s_iLvlVar != -1)
	{
		s_asCmps = {"level     ", "irregular "};
		vp = log(<0.5; 1>);
	}
	else
	{
		s_asCmps = {"irregular "};
		vp = log(<1>);
	}

	SetStsmModel(vp);		// map vP to local level model
	LogLikStsm(s_mY, &dlik, &dvar);
	// scale initial estimates by scale factor
	return vp + 0.5 * log(dvar);
}
Likelihood(const vP, const pdLik, const pvSco, const pmHes)
{                       		// arguments dictated by MaxBFGS()
    decl ret_val;

	SetStsmModel(vP);		// map vP to local level model
	ret_val = pvSco ? LogLikScoStsm(s_mY, pdLik, pvSco)
				: LogLikStsm(s_mY, pdLik, &s_dVar);
	return ret_val;		// 1 indicates success, 0 failure
}
MaxLik()
{
	decl vp, dlik, ir;

	vp = InitialPar();			// initialise unconstrained BFGS parameters
	print("\ninitial values BFGS parameters",vp);
	print("\n");
	MaxControl(50, 1, 1);		// start iterations BFGS algorithm
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
    decl minf, mKF, mstres;

	SetStsmModel(s_vPar);	// map vP to local level model
	GetSsfStsm(s_mStsm, &mphi, &momega, &msigma);
	momega = diag(s_vVarCmp);

	//compute prediction errors and their variances
	cst = columns(mphi);
	minf = KalmanInit(mY, mphi, momega, msigma);
	mKF = KalmanFilEx(minf, mY, mphi, momega, msigma);
	print("\nmKF[][0:4]", mKF[][0:4]);

	//compute standardised prediction errors
	mstres = mKF[0][].*sqrt(mKF[2][]);

	// determine serial correlation of residuals
	decl macf, ddenom, dQ10;
	macf = acf(mstres[][1:s_cT-1]', 10);
	print("\n         lag   autocorrelation", (range(1, 10)')~macf[1:10][]);
	print("\n95%-confidence limit = ", 2/sqrt(s_cT));
	ddenom = (s_cT-1) - <1;2;3;4;5;6;7;8;9;10>;
	dQ10 = (s_cT-1)*(s_cT-1+2)*sumc((macf[1:10][].^2)./ddenom);
	print("\n\nAutocorrelation: Box-Ljung statistic");
	print("\n------------------------------------");
	print("\nQ(10) = ", dQ10);
	print("\nChi-2(9;0.05) = 16.92\n");

	// determine homoscedasticity of residuals
	cst = columns(mphi);
	decl ch, dH;
	ch = round((s_cT-cst)/3);
//	print("\nch = ",ch);
	dH = (sumr(mstres[][s_cT-ch:s_cT-1].^2))/(sumr(mstres[][cst:cst+ch-1].^2));
	print("\nHomoscedasticity");
	print("\n-----------");
	print("\nH(",ch,") = ", dH);
	print("\n1/H(",ch,") = ", 1/dH);
	print("\nF(12,12;0.025) = 3.28\n");

	// determine normality of residuals
	decl mmoments, dS, dK, dN;
	mmoments = moments(mstres[][1:s_cT-1]', 4, 0);
	dS = mmoments[3][]/sqrt(mmoments[2][]^3);
	dK = mmoments[4][]/(mmoments[2][]^2);
	dN = (s_cT-1)*(((dS^2)/6)+(((dK-3)^2)/24));
	print("\nNormality");
	print("\n---------");
	print("\nN = ", dN); 
	print("\nChi-2(2;0.05) = 5.99\n");

	//compute smoothed state vector
	mstate = SsfMomentEstEx(ST_SMO, &mks, mY, mphi, momega, msigma);
	print("\nmks[][0:4]", mks[][0:4]);
	//compute filtered state vector
	decl msta, mkf;
	msta = SsfMomentEstEx(ST_PRED, &mkf, mY, mphi, momega, msigma);
	print("\nmkf[][0:4]", mkf[][0:4]);
	
	SetDraw(SET_COLORMODEL, 3);
	DrawTMatrix(0, mY, {"log fatalities in Norway"}, 1970, 1, 1, 0, 1);
	if (s_iLvlVar != -1)
	{
		DrawTMatrix(0, mks[0][], {"stochastic level"}, 1970, 1, 1, 0, 2);
	}
	else
		DrawTMatrix(0, mks[0][], {"deterministic level"}, 1970, 1, 1, 0, 2);
	DrawTMatrix(1, mY - mks[cst][], {"irregular"}, 1970, 1, 1, 0, 6);
	SaveDrawWindow("chapter2-3.eps");

}

main()
{
	decl data;
	data = loadmat("NorwayFinland.txt")'; // load data, transpose

	print("\nNorway 1970-2003");
	print("\n----------------\n");
	s_mY = log(data[1][]);		 // log 1970-2003
	s_cT = columns(s_mY);		// no of observations

	MaxLik();
	DrawComponents(s_mY);
}
