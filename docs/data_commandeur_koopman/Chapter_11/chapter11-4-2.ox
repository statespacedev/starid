/*  File chapter11-4-2.ox
	Likelihood and score vector evaluation for the local linear
	trend model.
  	Ox/SsfPack code for Chapter 11, Section 4.2, of:
  	****************************************************************
	*  An Introduction to State Space Time Series Analysis (2007). *
	*  Jacques J.F. Commandeur and Siem Jan Koopman.			   *
	*  Oxford: Oxford University Press.							   *
	****************************************************************/


#include <oxstd.h>
#include <packages/ssfpack/ssfpack.h>

main()
{
    decl mStsm, mPhi, mOmega, mSigma, mYt, dLogLik, dVar, mSco;

    // load Norwegian fatalities, transpose and log()
    mYt = log(loadmat("norway.txt")');

    // set up state space definition matrix local linear trend model
    mStsm = <   CMP_LEVEL, 0.5, 0, 0;
                CMP_SLOPE, 0.3, 0, 0;
                CMP_IRREG, 0.4, 0, 0>;

    // set up system matrices local linear trend model
    GetSsfStsm(mStsm, &mPhi, &mOmega, &mSigma);

    // print state space definition matrix and system matrices
    print("mStsm", mStsm, "mPhi", mPhi, "mOmega", mOmega, "mSigma", mSigma);

    //evaluate log-likelihood and matrix M
    SsfLikScoEx(&dLogLik, &dVar, &mSco, mYt, mPhi, mOmega, mSigma);
    print("\ndLogLik = ", dLogLik);
    print("\ndVar    = ", dVar);
    print("\nmSco    = ", mSco);
}
