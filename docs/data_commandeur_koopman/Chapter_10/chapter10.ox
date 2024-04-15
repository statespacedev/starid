/*  File chapter10.ox
    ARIMA processes.
  	Ox/SsfPack code for Chapter 10 of:
  	****************************************************************
	*  An Introduction to State Space Time Series Analysis (2007). *
	*  Jacques J.F. Commandeur and Siem Jan Koopman.			   *
	*  Oxford: Oxford University Press.							   *
	****************************************************************/

#include <oxstd.h>
#include <oxdraw.h>

main()
{
    decl mRandom, mN = 200, t, mRWalk, mMA1, mAR1, mARMA11;

	//generate realisation of random process
	mRandom = rann(1, mN);
	//draw realisation of random process
	SetDraw(SET_COLORMODEL, 3);
	DrawTMatrix(0, mRandom, {"random process"}, 1, 1, 1, 0, 2);   
	DrawAcf(1, mRandom, {"random process"}, 12, TRUE, FALSE, TRUE);

	//generate realisation of random walk
	mRWalk = constant(0, 1, mN);
	for(t = 0; t < (mN-1); ++t)
	{
		mRWalk[][t+1] = mRWalk[][t] + mRandom[][t];
	}
	//draw realisation of random walk
	DrawTMatrix(2, mRWalk, {"random walk"}, 1, 1, 1, 0, 2);   
	DrawAcf(3, mRWalk, {"random walk"}, 12, TRUE, FALSE, TRUE);

	//generate realisation of moving average (1) process
	mMA1 = constant(0, 1, mN);
	for(t = 1; t < (mN-1); ++t)
	{
		mMA1[][t] = mRandom[][t] + (0.5 * mRandom[][t-1]);
	}
	//draw realisation of moving average (1) process
	DrawTMatrix(4, mMA1[][1:199], {"MA(1) process"}, 2, 1, 1, 0, 2);   
	DrawAcf(5, mMA1[][1:199], {"MA(1) process"}, 12, TRUE, FALSE, TRUE);

	//generate realisation of auto-regressive (1) process
	mAR1 = constant(0, 1, mN);
	for(t = 1; t < (mN-1); ++t)
	{
		mAR1[][t] = (0.5 * mAR1[][t-1]) + mRandom[][t];
	}
	//draw realisation of auto-regressive (1) process
	DrawTMatrix(6, mAR1, {"AR(1) process"}, 1, 1, 1, 0, 2);   
	DrawAcf(7, mAR1, {"AR(1) process"}, 12, TRUE, FALSE, TRUE);

	//generate realisation of ARMA(1,1) process
	mARMA11 = constant(0, 1, mN);
	for(t = 1; t < (mN-1); ++t)
	{
		mARMA11[][t] = (0.5 * mARMA11[][t-1]) + mRandom[][t] + (0.5 * mRandom[][t-1]);
	}
	//draw realisation of auto-regressive (1) process
	DrawTMatrix(8, mARMA11[][1:199], {"ARMA(1,1) process"}, 2, 1, 1, 0, 2);   
	DrawAcf(9, mARMA11[][1:199], {"ARMA(1,1) process"}, 12, TRUE, FALSE, TRUE);
	SaveDrawWindow("chapter10.eps");
}
