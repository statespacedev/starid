/* File chapter8-3.ox
   Stochastic level and deterministic
   trigonometric seasonal model
   for UK drivers KSI: confidence intervals.
   Ox/SsfPack code for Chapter 8, Section 3, of:
   ****************************************************************
   *  An Introduction to State Space Time Series Analysis (2007). *
   *  Jacques J.F. Commandeur and Siem Jan Koopman.			      *
   *  Oxford: Oxford University Press.							  *
   ****************************************************************/

#include <oxstd.h>
#include <oxdraw.h>
#import  <maximize>
#include <packages/ssfpack/ssfpack.h>

static decl s_mY, s_cT;		// data (1 x n) and n
static decl s_mStsm, s_vVarCmp;	// matrices for unobserved components model
static decl s_dVar;			// and scale factor
static decl s_vPar;			// parameter vector of model

SetStsmModel(const vP)
{
	// map to stochastic level and deterministic seasonal model
	s_mStsm = <	CMP_LEVEL,     0.5,  0,  0;
    		   	CMP_SEAS_TRIG,   0, 12,  0;		// 12 for monthly data
				CMP_IRREG,       1,  0,  0>;
	// change BFGS parameters into error variances
	decl vr = exp(2.0 * vP);
	// s_vVarCmp is used to update diagonal(Omega)
	//             level    trigonometric seasonal   irregular
	s_vVarCmp =	 vr[0]  |  (constant( 0, 11, 1)) |   vr[1];
}
LogLikStsm(const vY, const pdLik, const pdVar)
{
	decl mphi, momega, msigma, ret_val;

	// set up system matrices for state space model
	GetSsfStsm(s_mStsm, &mphi, &momega, &msigma);
	momega = diag(s_vVarCmp);		// create Omega from s_vVarCmp
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
	pvSco[0][0] = vs[0];
	pvSco[0][1] = vs[12];
    pvSco[0][] /= s_cT;		// scale score vector by sample size
    pdLik[0] /= s_cT;		// scale loglikelihood by sample size
	
    return ret_val;     		// 1 indicates success, 0 failure
}
InitialPar()
{
	decl dlik, dvar, vp;

	vp = log(<0.5; 1>);
	SetStsmModel(vp);		// map vP to state space model
	LogLikStsm(s_mY, &dlik, &dvar);
	// scale initial estimates by scale factor
	return vp + 0.5 * log(dvar);
}
Likelihood(const vP, const pdLik, const pvSco, const pmHes)
{                       		// arguments dictated by MaxBFGS()
    decl ret_val;

	SetStsmModel(vP);		// map vP to state space model
	ret_val = pvSco ? LogLikScoStsm(s_mY, pdLik, pvSco)
				: LogLikStsm(s_mY, pdLik, &s_dVar);
	return ret_val;		// 1 indicates success, 0 failure
}
MaxLik()
{
	decl vp, dlik, ir;

	vp = InitialPar();		// initialise unconstrained BFGS parameters
	print("\ninitial values BFGS parameters",vp);
	print("\n");
	MaxControl(50, 1, 1);	// start iterations BFGS algorithm
    ir = MaxBFGS(Likelihood, &vp, &dlik, 0, FALSE);

    println("\n", MaxConvergenceMsg(ir),
          " using analytical derivatives",
          "\n(1/n) Log-likelihood = ", "%.8g", dlik,
		  "; n = ", s_cT, ";");
	s_vPar = vp;
	print("\nparameter estimates (unconstrained)", vp);
	print("\nerror variance estimates", exp(2.0 * vp));
}
DrawComponents(const mY)
{
    decl cst, mphi, momega, msigma, mks, mstate, mseas;

	SetStsmModel(s_vPar);	// map vP to state space model
	// set up system matrices for state space model
	GetSsfStsm(s_mStsm, &mphi, &momega, &msigma);
	momega = diag(s_vVarCmp);

	//compute smoothed state vector
	cst = columns(mphi);
	mstate = SsfMomentEstEx(ST_SMO, &mks, mY, mphi, momega, msigma);
	mseas = mks[1][]+mks[3][]+mks[5][]+mks[7][]+mks[9][]+mks[11][];  

	print("\nmks[][0:4]", mks[][0:4]);
	SetDraw(SET_COLORMODEL, 3);
	DrawTMatrix(0, mks[13][], {"level estimation error variance"}, 1969, 1, 12, 0, 2);
	DrawTMatrix(1, mY, {"log UK drivers KSI"}, 1969, 1, 12, 0, 1);
	DrawTMatrix(1, mks[0][], {"stochastic level +/- 1.64SE"}, 1969, 1, 12, 0, 2);
	DrawZ(sqrt(mks[13][]), "", ZMODE_BAND, 1.64, 2);
	DrawTMatrix(2, mseas[][143:191], {"deterministic seasonal +/- 1.64SE"}, 1981, 1, 12, 0, 3);
	DrawZ(sqrt(mks[25][143:191]-mks[13][143:191]), "", ZMODE_BAND, 1.64, 3);
	DrawTMatrix(3, mks[12][143:191], {"signal +/- 1.64SE"}, 1981, 1, 12, 0, 2);
	DrawZ(sqrt(mks[25][143:191]), "", ZMODE_BAND, 1.64, 2);
	SaveDrawWindow("chapter8-3.eps");
}

main()
{
	s_mY = log(loadmat("UKdriversKSI.txt")');	// load data, transpose, log()
	s_cT = columns(s_mY);			// no of observations

	MaxLik();
	DrawComponents(s_mY);
}
