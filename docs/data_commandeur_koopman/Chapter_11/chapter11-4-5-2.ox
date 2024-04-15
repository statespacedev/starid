/*  File chapter11-4-5-2.ox
	Maximisation of likelihood with the EM algorithm using
    analytical score vector.
    Local level model for log Norwegian fatalities.
  	Ox/SsfPack code for Chapter 11, Section 4.5, of:
  	****************************************************************
	*  An Introduction to State Space Time Series Analysis (2007). *
	*  Jacques J.F. Commandeur and Siem Jan Koopman.			   *
	*  Oxford: Oxford University Press.							   *
	****************************************************************/

#include <oxstd.h>
#include <packages/ssfpack/ssfpack.h>

static decl s_mY, s_cT;		// data (1 x n) and n
static decl s_mStsm, s_vVarCmp;	// matrices for state space model
static decl s_dVar;			// scale factor
static decl s_vPar;			// parameter vector of model

EM()
{
	decl vp, dLikold, dLik, dVar, iter, maxiter = 100;
    decl mphi, momega, msigma, msco, vs;

	s_mStsm = <	CMP_LEVEL,     0.5,  0, 0;
				CMP_IRREG,     1,    0, 0>;
	GetSsfStsm(s_mStsm, &mphi, &momega, &msigma);
	s_vPar = diagonal(momega)';
    SsfLikScoEx(&dLikold, &dVar, &msco, s_mY, mphi, momega, msigma);   // E-step
	s_vPar *= dVar;	// initial parameter values

	for(iter=0; iter <= maxiter; ++iter)
	{
		vs = (diagonal(msco)' .*  s_vPar); // score
		s_vPar = ((s_vPar .* vs) ./ (s_cT-1 | s_cT)) + s_vPar;         // M-step
		momega = diag(s_vPar); // new variance matrix

    	SsfLikScoEx(&dLik, &dVar, &msco, s_mY, mphi, momega, msigma);  // E-step
		dLik /= s_cT;
		if(fabs((dLik - dLikold)/dLikold) < 10^(-12))
		{
			print("\nConvergence in iteration ", iter);
			print("\nloglikelihood = ", dLik);
			print("\nparameter vector", s_vPar);
			return TRUE;
			break;
		}
		dLikold = dLik;
	}
	return FALSE;
}
main()
{
	s_mY = log(loadmat("norway.txt")');	// load data, transpose, log()
	s_cT = columns(s_mY);			// no of observations

	EM();
}
