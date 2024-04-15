/*  File chapter11-4-1.ox
	Likelihood evaluation for the local linear trend model.
  	Ox/SsfPack code for Chapter 11, Section 4.1, of:
  	****************************************************************
	*  An Introduction to State Space Time Series Analysis (2007). *
	*  Jacques J.F. Commandeur and Siem Jan Koopman.			   *
	*  Oxford: Oxford University Press.							   *
	****************************************************************/

#include <oxstd.h>
#include <packages/ssfpack/ssfpack.h>

main()
{
    decl mStsm, mPhi, mOmega, mSigma, mYt, dLogLik, dVar;

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

    //evaluate log-likelihood
    SsfLikEx(&dLogLik, &dVar, mYt, mPhi, mOmega, mSigma);
    print("\ndLogLik = ", dLogLik);
    print("\ndVar    = ", dVar);
}
