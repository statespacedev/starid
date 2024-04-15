/*  File chapter11-5.ox
	Prediction, filtering, smoothing and score vector evaluation
	of the local linear trend model.
  	Ox/SsfPack code for Chapter 11, Section 5, of:
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
	
    // prediction, filtering and smoothing
    decl mPrd, mFil, mSmo;
    SsfMomentEstEx(ST_PRED, &mPrd, mYt, mPhi, mOmega, mSigma);
    SsfMomentEstEx(ST_FIL,  &mFil, mYt, mPhi, mOmega, mSigma);
    SsfMomentEstEx(ST_SMO,  &mSmo, mYt, mPhi, mOmega, mSigma);
    print("\nPrediction, filtering and smoothing of LEVEL");
    print("\nwith corresponding s.e.'s (last 3 columns)\n");
    print((mPrd[0][] | mFil[0][] | mSmo[0][] | sqrt(mPrd[3][] | mFil[3][] | mSmo[3][]))');
    print("\nPrediction, filtering and smoothing of SLOPE");
    print("\nwith corresponding s.e.'s (last 3 columns)\n");
    print((mPrd[1][] | mFil[1][] | mSmo[1][] | sqrt(mPrd[4][] | mFil[4][] | mSmo[4][]))');

}
