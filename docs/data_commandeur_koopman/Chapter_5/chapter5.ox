/* File chapter5.ox
   Level model with
   explanatory variable TIME or log(PETROL PRICE)
   for log UK drivers KSI.
   Ox/SsfPack code for Chapter 5 of:
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
static decl s_mX;			// independent variable (1 x n) and n
static decl s_mStsm, s_vVarCmp;	// matrices for state space model
static decl s_dVar;			// scale factor
static decl s_vPar;			// parameter vector of model

static decl s_iLvlVar = -1;  // 0 = stochastic level; -1 = deterministic level
static decl s_asCmps;		// string array of component names

SetStsmModel(const vP)
{
	// map to local level model 
	s_mStsm = <	CMP_LEVEL, 0.5, 0, 0;
				CMP_IRREG,   1, 0, 0>;
	// change BFGS parameters into error variances
	decl vr = exp(2.0 * vP);
	// s_vVarCmp is used to update diagonal(Omega)
	if (s_iLvlVar != -1)
		//          expl.var.   level    irregular
		s_vVarCmp =    0     |  vr[0]  |  vr[1];
	else
		s_vVarCmp =    0     |     0   |  vr[0];

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
	if (s_iLvlVar != -1)
	{
		pvSco[0][0] = vs[1];
		pvSco[0][1] = vs[2];
		pvSco[0][] /= s_cT;		// scale score vector by sample size
	}
	else
	{
		pvSco[0] = vs[2] /s_cT;	// scale score vector by sample size
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

	vp = InitialPar();		// initialise BFGS parameters
	print("\ninitial values BFGS parameters",vp);
	print("\n");
	MaxControl(10, 1, 1);	// start iterations BFGS algorithm
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
    decl cst, mphi, momega, msigma, mjphi, mks, mstate;

	SetStsmModel(s_vPar);	// map vP to local level model
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
	
	SetDraw(SET_COLORMODEL, 3);
	DrawTMatrix(0, mY, {"log UK drivers KSI"}, 1969, 1, 12, 0, 1);
	if (s_iLvlVar != -1)
	{
		DrawTMatrix(0, mks[cst][], {"stochastic level + beta*log(PETROL PRICE)"}, 1969, 1, 12, 0, 2);
		DrawTMatrix(1, mY - mks[cst][], {"irregular"}, 1969, 1, 12, 0, 6);
	}
	else
	{
		DrawTMatrix(0, mks[cst][], {"deterministic level + beta*log(PETROL PRICE)"}, 1969, 1, 12, 0, 2);
		DrawXMatrix(1, mY, {"log UK drivers KSI against log PETROL PRICE"}, s_mX, {""}, 1, 1);   
		DrawXMatrix(1, mks[cst][], {"deterministic level + beta*log(PETROL PRICE)"}, s_mX, {""}, 0, 2);   
		DrawTMatrix(2, mY - mks[cst][], {"irregular"}, 1969, 1, 12, 0, 6);
	}
	SaveDrawWindow("chapter5.eps");

	// compute t-test for regression coefficient
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
	s_mY = log(loadmat("UKdriversKSI.txt")');	// load data, transpose, log()
	s_cT = columns(s_mY);					// no of observations

	// load explanatory variable log(PETROL PRICE) and transpose
	s_mX = loadmat("logUKpetrolprice.txt")';

	// construct explanatory variable TIME = 1, 2, ..., n
	// for using TIME instead of log(PETROL PRICE) as explanatory
	// variable, uncomment next line
//	s_mX = range(1, s_cT, 1);

	MaxLik();
	DrawComponents(s_mY);
}
