/*  File chap11-4-5-1.ox
	Maximisation of likelihood with the BFGS algorithm using
    analytical score vector.
    Local level model for log Norwegian fatalities.
  	Ox/SsfPack code for Chapter 11, Section 4.5, of:
  	****************************************************************
	*  An Introduction to State Space Time Series Analysis (2007). *
	*  Jacques J.F. Commandeur and Siem Jan Koopman.			   *
	*  Oxford: Oxford University Press.							   *
	****************************************************************/

#include <oxstd.h>
#import  <maximize>
#include <packages/ssfpack/ssfpack.h>

static decl s_mY, s_cT;			// data (1 x n) and n
static decl s_mStsm, s_vVarCmp;	// matrices for state space model

SetStsmModel(const vP)
{
	s_mStsm = <	CMP_LEVEL,     0.5,  0, 0;
				CMP_IRREG,     1,    0, 0>;
	decl vr = exp(2.0 * vP);
	s_vVarCmp =	vr[0]  |  vr[1];	
}

LogLikStsm(const vY, const pdLik, const pdVar)
{
	decl mphi, momega, msigma, ret_val;
	GetSsfStsm(s_mStsm, &mphi, &momega, &msigma);
	momega = diag(s_vVarCmp);	// create Omega from s_vVarCmp
	ret_val = SsfLikEx(pdLik, pdVar, vY, mphi, momega, msigma);
	return ret_val;
}
LogLikScoStsm(const vY, const pdLik, const pvSco)
{
    decl mphi, momega, msigma, msco, ret_val, dvar, vs;
	GetSsfStsm(s_mStsm, &mphi, &momega, &msigma);
	momega = diag(s_vVarCmp);
    ret_val = SsfLikScoEx(pdLik, &dvar, &msco, vY, mphi, momega, msigma);
	vs = (diagonal(msco)' .*  s_vVarCmp);
	pvSco[0][0:1] = vs[0:1] / s_cT;
	pdLik[0] /= s_cT;
	return ret_val;
}
Likelihood(const vP, const pdLik, const pvSco, const pmHes)
{                       	
    decl ret_val, dvar;
	SetStsmModel(vP);
	ret_val = pvSco ? LogLikScoStsm(s_mY, pdLik, pvSco) 
				: LogLikStsm(s_mY, pdLik, &dvar);
	return ret_val;		// 1 indicates success, 0 failure
}
InitialPar()
{
	decl dlik, dvar, vp = log(<0.5; 1>);
	SetStsmModel(vp);
	LogLikStsm(s_mY, &dlik, &dvar);
	return vp + 0.5 * log(dvar);
}
MaxLik()
{
	decl vp, dlik, ir;

	vp = InitialPar();
	MaxControl(10, 1, 1);
    ir = MaxBFGS(Likelihood, &vp, &dlik, 0, FALSE);

    println("\n", MaxConvergenceMsg(ir),
          " using analytical derivatives",
          "\n(1/n) Log-likelihood = ", "%.8g", dlik,
		  "; n = ", s_cT, ";");

	decl i;
	print("\nparameter estimates (unconstrained)");
	for(i=0;i<=rows(vp)-1; i++) 
		print("\n    ", vp[i]);
	print("\n\ndisturbance variance estimates");
	for(i=0;i<=rows(vp)-1; i++) 
		print("\n    ", exp(2.0 * vp[i]));
	print("\n");
}
main()
{
	s_mY = log(loadmat("norway.txt")');	// load data, transpose, log()
	s_cT = columns(s_mY);				// no of observations
	
	MaxLik();
}
