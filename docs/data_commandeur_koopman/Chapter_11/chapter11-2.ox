/*  File chapter11-2.ox
	Setting up system matrices
  	for the local linear trend model.
  	Ox/SsfPack code for Chapter 11, Section 2, of:
  	****************************************************************
	*  An Introduction to State Space Time Series Analysis (2007). *
	*  Jacques J.F. Commandeur and Siem Jan Koopman.			   *
	*  Oxford: Oxford University Press.							   *
	****************************************************************/


#include <oxstd.h>
#include <packages/ssfpack/ssfpack.h>

main()
{
    decl mStsm, mPhi, mOmega, mSigma;

    // set up state space definition matrix for the local linear
	// trend model
    mStsm = <   CMP_LEVEL, 0.5, 0, 0;
                CMP_SLOPE, 0.3, 0, 0;
                CMP_IRREG, 0.4, 0, 0>;

    // set up system matrices for the local linear trend model
    GetSsfStsm(mStsm, &mPhi, &mOmega, &mSigma);

    // print state space definition matrix and system matrices
	// for the local linear trend model
    print("mStsm", mStsm, "mPhi", mPhi, "mOmega", mOmega, "mSigma", mSigma);
}
