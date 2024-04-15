/* File chapter1.ox
   Classical linear regression
   of log UK drivers KSI on time.
   Ox/SsfPack code for Chapter 1 of:
   ****************************************************************
   *  An Introduction to State Space Time Series Analysis (2007). *
   *  Jacques J.F. Commandeur and Siem Jan Koopman.			      *
   *  Oxford: Oxford University Press.							  *
   ****************************************************************/

#include <oxstd.h>
#include <oxdraw.h>
#import  <maximize>
#include <packages/ssfpack/ssfpack.h>

main()
{
    decl mY, mN, mX, mB, mYpred, mNormal;

	mY = log(loadmat("UKdriversKSI.txt"));	// load data and take logarithm
	mN = rows(mY);						// no of observations
	
	mX = constant(1, mN, 1) ~ (range(1, mN, 1)');
	mB = invertsym((mX')*mX)*(mX')*mY;
	print("\nregression weights = ", mB);
	mYpred = mX*mB;
	
	SetDraw(SET_COLORMODEL, 3);
	DrawXMatrix(0, mY', {"log UK drivers KSI against time (in months)"}, mX[][1]', {"TIME"}, 1, 1);   
	DrawXMatrix(0, mYpred', {"regression line"}, mX[][1]', {"TIME"}, 0, 2);   
	DrawXMatrix(1, mY', {"log UK drivers KSI"}, mX[][1]', {"TIME"}, 0, 1);   
	DrawXMatrix(2, (mY - mYpred)', {"residuals"}, mX[][1]', {"TIME"}, 0, 6);
	mNormal = rann(1, 192);
	DrawAcf(3, mNormal, {"random residuals"}, 14, TRUE, FALSE, TRUE,6);
	DrawAcf(4, (mY - mYpred)', {"regression residuals"}, 14, TRUE, FALSE, TRUE, 6);
	SaveDrawWindow("chapter1.eps");
}
