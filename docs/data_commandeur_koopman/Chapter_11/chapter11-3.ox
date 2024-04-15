/*  File chapter11-3.ox
	Adding explanatory and intervention variables
  	to the local linear trend model.
  	Ox/SsfPack code for Chapter 11, Section 3, of:
  	****************************************************************
	*  An Introduction to State Space Time Series Analysis (2007). *
	*  Jacques J.F. Commandeur and Siem Jan Koopman.			   *
	*  Oxford: Oxford University Press.							   *
	****************************************************************/

#include <oxstd.h>
#include <packages/ssfpack/ssfpack.h>

main()
{
    decl mX, mStsm, mPhi, mOmega, mSigma, mJPhi = <>;

    // set up data matrix with explanatory and intervention variables
    mX = loadmat("logpetrol.txt")' | (constant(0, 1, 169) ~ constant(1, 1, 23));

    // set up state space definition matrix local linear trend model
    mStsm = <   CMP_LEVEL, 0.5, 0, 0;
                CMP_SLOPE, 0.3, 0, 0;
                CMP_IRREG, 0.4, 0, 0>;

    // set up system matrices local linear trend model
    GetSsfStsm(mStsm, &mPhi, &mOmega, &mSigma);
	// add explanatory and intervention variables to system matrices
    AddSsfReg(mX, &mPhi, &mOmega, &mSigma, &mJPhi);

    // print state space definition matrix and system matrices
    print("mStsm", mStsm, "mPhi", mPhi, "mOmega", mOmega);
    print("mSigma", mSigma, "mJPhi", mJPhi);
}
