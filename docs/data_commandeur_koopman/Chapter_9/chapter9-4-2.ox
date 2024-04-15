/* File chap9-4-2.ox
   Bivariate analysis of log of monthly UK front and rear seat
   passengers KSI.
   Stochastic level and deterministic seasonal.
   Explanatory variables log(kms travelled) and log(petrol price)
   on both series, and intervention variable seatbelt law
   Februari 1983 on first series only.
   Rank one restriction on level error variance matrix.
   Full observation disturbance variance matrix: first method,
   store observation disturbances in the state space vector.
   (see Durbin and Koopman, 2001, p.169).
   Ox/SsfPack code for Chapter 9, Section 4, of:
   ****************************************************************
   *  An Introduction to State Space Time Series Analysis (2007). *
   *  Jacques J.F. Commandeur and Siem Jan Koopman.			      *
   *  Oxford: Oxford University Press.							  *
   ****************************************************************/

#include <oxstd.h>
#include <oxfloat.h>
#include <oxdraw.h>
#import  <maximize>
#include <packages/ssfpack/ssfpack.h>

static decl s_mY, s_cN, s_cT;			// data (N x T), N and T
static decl s_mX;						// explanatory variables
static decl s_mStsm;					// basic unobserved component model

static decl s_iLvlVar = 0; // 0 = stochastic level; -1 = fixed level
static decl s_iSeaVar = -1;// 0 = stochastic seasonal; -1 = fixed seasonal
static decl s_iIrrVar = 0;

static decl s_iSeasLength = 12, s_igr = 0;

// state space
static decl s_mPhi= <>, s_mOmega = <>, s_mSigma = <>, s_mjPhi = <>, s_q;

IniPar()
{
	decl cp = 0;
	if (s_iLvlVar != -1)
	{
		s_iLvlVar = cp;
		cp += s_cN;
	}
	if (s_iSeaVar != -1)
	{
		s_iSeaVar = cp;
		cp += s_cN * (s_cN + 1) / 2;
	}
	if (s_iIrrVar != -1)
	{
		s_iIrrVar = cp;
		cp += s_cN * (s_cN + 1) / 2;
	}
	return zeros(cp, 1);
}

SetSsfModel(const vP)
{
	s_mStsm = <	CMP_LEVEL,       0.5,  0, 0;
				CMP_SEAS_DUMMY,  0.6,  1, 0;
				CMP_IRREG,       0.7,  0, 0>;
	s_mStsm[1][2] = s_iSeasLength;

	decl cst, rst, mphi1, momega1, msigma1, mphi, momega, msigma;
	decl mjphi1, mjphi;
	// get state space model for univariate level plus seasonal model
	GetSsfStsm(s_mStsm, &mphi1, &momega1, &msigma1);
	// add explanatory variables log(PETROL PRICE)
	// and intervention variable SEATBELT LAW to state space model
	mjphi1 = <>;
	AddSsfReg(s_mX, &mphi1, &momega1, &msigma1, &mjphi1);

	cst = columns(mphi1);

	//adapt mphi
	mphi = zeros(cst+1, cst+1);
	mphi[0:cst-1][0:cst-1] = mphi1[0:cst-1][0:cst-1];
	mphi = mphi | (mphi1[cst][]~1);
	
	//adapt momega
	momega = zeros(cst+2, cst+2);
	momega[0:cst][0:cst] = momega1[0:cst][0:cst];
	
	//adapt msigma
	msigma = zeros(cst+2, cst+1);
	msigma[0:cst-1][0:cst-1] = msigma1[0:cst-1][0:cst-1];
	msigma[cst][cst] = -1;
	
	// make it multivariate using kronecker product
	s_mPhi = mphi ** unit(s_cN);
	s_mOmega = momega ** unit(s_cN);
	s_mSigma = msigma ** unit(s_cN);
	//drop row and column corresponding to seatbelt law(rear)
	s_mPhi = dropr(s_mPhi,5);
	s_mPhi = dropc(s_mPhi,5);
	s_mOmega = dropr(s_mOmega,5);
	s_mOmega = dropc(s_mOmega,5);
	s_mSigma = dropr(s_mSigma,5);
	s_mSigma = dropc(s_mSigma,5);

	cst = columns(s_mPhi);
	rst = rows(s_mPhi);
	// set up s_mjPhi to include all explanatory variables in first
	// observation equation, but only log kms travelled and petrol
	// price in second observation equation
	s_mjPhi = (-1) * ones(rst, cst);
	s_mjPhi[rst-2][0] = 0;
	s_mjPhi[rst-2][2] = 1;
	s_mjPhi[rst-2][4] = 2;
	s_mjPhi[rst-1][1] = 0;	
	s_mjPhi[rst-1][3] = 1;
	
	s_mSigma = s_mSigma[0:cst][];

	decl A, D, ip, ist, i, j;
    // rank one level error variance matrix
	ip = 0;
	s_q = vP[1] | 1;
	ist = (s_cN * rows(s_mX))-1; // skip first regression error variance matrices
	s_mOmega[ist:ist+s_cN-1][ist:ist+s_cN-1] = exp(vP[0]) * s_q * s_q';

	// seasonal error variance matrix
	ip = s_cN;
	ist += s_cN; 
	A = unit(s_cN); D = zeros(s_cN, s_cN);
	if (s_iSeaVar > -1)
	{
		for (i=0; i<s_cN; i++)
			for (D[i][i] = exp(vP[ip++]), j=0; j<i; j++)
				A[i][j] = vP[ip++];
	}
	s_mOmega[ist:ist+s_cN-1][ist:ist+s_cN-1] = A * D * A';

	// irregular variance matrix
	ist += (s_iSeasLength - 1) * s_cN; 
	A = unit(s_cN); D = zeros(s_cN, s_cN);
	if (s_iIrrVar > -1)
	{
		for (i=0; i<s_cN; i++)
			for (D[i][i] = exp(vP[ip++]), j=0; j<i; j++)
				A[i][j] = vP[ip++];
	}
	s_mOmega[ist:ist+s_cN-1][ist:ist+s_cN-1] = A * D * A';

}

PrintPar(const vPar)
{
	SetSsfModel(vPar);					// map vP to ssf model

	decl ist;
	ist = (s_cN * rows(s_mX))-1; // skip first regression error variance matrices
	if (s_iLvlVar >= 0)
		println("level variances are estimated (pos = ", s_iLvlVar, ")");
	else
		println("level variances are not estimated");
	println(" level variance is ", s_mOmega[ist:ist + s_cN -1][ist:ist + s_cN -1], "\n\n");

	ist += s_cN;
	if (s_iSeaVar >= 0)
		println("seasonal variances are estimated (pos = ", s_iSeaVar, ")");
	else
		println("seasonal variances are not estimated");
	println(" seasonal variance is ", s_mOmega[ist:ist + s_cN -1][ist:ist + s_cN -1], "\n\n");

	ist += (s_iSeasLength - 1) * s_cN;
	if (s_iIrrVar >= 0)
		println("irregular variances are estimated (pos = ", s_iIrrVar, ")");
	else
		println("irregular variances are not estimated");
	println(" irregular variance is ", s_mOmega[ist:ist + s_cN -1][ist:ist + s_cN -1], "\n\n");
}

Likelihood(const vP, const pdLik, const pvSco, const pmHes)
{										// arguments dictated by MaxBFGS()
	decl ret_val, d;
	SetSsfModel(vP);					// map vP to ssf model
	ret_val = SsfLikEx(pdLik, &d, s_mY, s_mPhi, s_mOmega, s_mSigma,
										<>, s_mjPhi, <>, <>, s_mX);
	pdLik[0] /= s_cT * s_cN;			// log-likelihood scaled by sample size
	return ret_val;						// 1 indicates success, 0 failure
}
MaxLik()
{
	decl vp, dlik, ir, aic;
	decl nvp, initvp, seed, bestlik = -100000, bestvp;
	decl alldliks, trial, maxtrial = 10;

	vp = IniPar();						// get starting values
	nvp = rows(vp);
	alldliks = constant(0, 1, maxtrial);

	// use maxtrial random starts for parameter vector vp to avoid
	// convergence to local optima
	for(trial=0; trial<maxtrial; trial++)
	{
		print("\ntrial ", trial,"\n");
		seed = ranseed(0);
		vp = rann(nvp,1);
		initvp = vp;
		Likelihood(vp, &dlik, <>, <>);

		MaxControl(-1, 100, 1);				// get some output from MaxBFGS
		// maximize loglikelihood using numerical first derivatives
    	ir = MaxBFGS(Likelihood, &vp, &dlik, 0, TRUE);

    	print("dlik = ", dlik,"\n");
		alldliks[][trial] = dlik;
		if((dlik >= bestlik) && (dlik < 2))
		{
			   bestlik = dlik;
			   bestvp = initvp;
		}
		
	}
    print("\nalldliks  = ", alldliks);
    print("\nbestlik  = ", bestlik);
	print("\nbestvp", bestvp');

	vp = bestvp;
	println("\nFinal best initial parameter vector vp in MaxLik", vp');

	Likelihood(vp, &dlik, <>, <>);

	MaxControl(-1, 5, 1);				// get some output from MaxBFGS
	
	// maximize loglikelihood using numerical first derivatives
	ir = MaxBFGS(Likelihood, &vp, &dlik, 0, TRUE);

	println("\n", MaxConvergenceMsg(ir),
			" using numerical derivatives",
			"\nLog-likelihood = ", "%.20g", dlik * s_cT * s_cN,
			"; T = ", s_cT, "; N = ", s_cN);
	aic = (-2*dlik*s_cT*s_cN) + (2*(rows(vp)+columns(s_mPhi)));
	println("\nAIC = ", aic/(s_cT*s_cN));	  
	return vp;
}

StderrPar(const vP)
{
	decl covar, invcov, result;

	result = Num2Derivative(Likelihood, vP, &covar);

	if (!result)
	{	print("Covar() failed in numerical second derivatives\n");
		return zeros(vP);
	}
	invcov = invertgen(-covar, 30);
	return sqrt(diagonal(invcov) / (s_cT * s_cN))';
}

DrawComponents(const vPar, const iDrawType)
{
	SetSsfModel(vPar);					// map vP to ssf model

	// compute smoothed state vector
	decl mcmp, cst = columns(s_mPhi);
	decl md = SsfCondDensEx(ST_SMO, s_mY, s_mPhi, s_mOmega, s_mSigma,
											<>, s_mjPhi, <>, <>, s_mX);
	/* md =
	| kms travelled(front)		|
	| kms travelled(rear)		|
	| petrol price(front)		|
	| petrol price(rear)		|
	| seatbelt law(front)		|
	| level(front)				|
	| level(rear)				|
	| seasonal.1(front)			|
	| seasonal.1(rear)			|
	| seasonal.2(front)			|
	| seasonal.2(rear)			|
	|			:				|
	| seasonal.(s-1)(front)		|
	| seasonal.(s-1)(rear)		|
	| irregular(front)			|
	| irregular(rear)			|
	| signal(front)				|
	| signal(rear)				|
	NB. For dummy seasonal, 'seasonal.1' is the smoothed seasonal. */

	if(iDrawType == 0)
	{
		decl mds, mcorr, my, mx, db, mypred;
		decl ist;
		ist = (s_cN * rows(s_mX))-1;
		// compute smoothed disturbances
		mds = SsfCondDensEx(DS_SMO, s_mY, s_mPhi, s_mOmega, s_mSigma,
											<>, s_mjPhi, <>, <>, s_mX);
		// compute correlation level disturbances front and rear and 
		// regress level disturbances front seat on rear seat
		mcorr = correlation(mds[ist:ist+1][]');
		print("\ncorrelation between level disturbances = ", mcorr);
		my = mds[ist][]';
		mx = mds[ist+1][]';
		db = invertsym((mx')*mx)*(mx')*my;
		print("\nregression weight = ", db);
		mypred = mx*db;
		DrawXMatrix(s_igr, my', {"level disturbances rear against front seat KSI"}, mx',
							{""}, 1, 1);   
		DrawXMatrix(s_igr, mypred', {"regression line"}, mx',
							{""}, 0, 2);   
		s_igr++;
		
		// compute correlation level components front and rear and 
		// regress level component front seat on rear seat
		mcorr = correlation(md[ist:ist+1][]');
		print("\ncorrelation between levels = ", mcorr);
		my = md[ist][]';
		mx = constant(1, s_cT, 1) ~ (md[ist+1][]');
		db = invertsym((mx')*mx)*(mx')*my;
		print("\nregression weights = ", db);
		mypred = mx*db;
		DrawXMatrix(s_igr, my', {"level rear against level front seat KSI"}, mx[][1]',
						{""}, 1, 1);   
		DrawXMatrix(s_igr, mypred', {"regression line"}, mx[][1]',
						{""}, 0, 2);
		s_igr++;

		// compute t-tests for regression coefficients
		decl pdLik, dvar, msta, csy, ck, vse, mls;
		csy = cst + 1;
		ck = (s_cN * rows(s_mX))-1;
		SsfLikEx(&pdLik, &dvar, s_mY, s_mPhi, s_mOmega, s_mSigma,
							<>, s_mjPhi, <>, <>, s_mX);
		s_mOmega *= dvar;
		msta = SsfMomentEstEx(ST_PRED, <>, s_mY, s_mPhi, s_mOmega, s_mSigma,
							<>, s_mjPhi, <>, <>, s_mX);
		vse = sqrt(diagonal(msta[0:ck-1][0:ck-1]));
		mls = msta[cst][0:ck-1] | vse | msta[cst][0:ck-1]./vse;
		mls = mls | 2*tailt(fabs(mls[2][]),s_cT-3-1);
		print("Regression results");
		println("%c",{"coefficient","standard error","t-value","p-value"},"%20.10g",mls');
		println("log-likelihood ","%20.10g", pdLik,
		"\nsigma ","%20.10g", sqrt(dvar), " Residual SS ","%20.10g", dvar*(s_cT-ck));
	}
	
	// get bivariate level
	decl i = iDrawType, j= (s_cN * rows(s_mX))-1; // skip regression coefficients
	decl mweights, mtrend, mlevel;

	// compute trend and level
	mcmp = md[j:j+s_cN -1][];
	if(iDrawType == 0)
	{
			mweights = md[i][0]~md[2+i][0]~md[4+i][0];
			mtrend = mcmp[i][] + (mweights*s_mX);
			mlevel = mcmp[i][] + (mweights[][2]*s_mX[2][]);
	}
	else
	{
			mweights = md[i][0]~md[2+i][0];
			mtrend = mcmp[i][] + (mweights*s_mX[0:1][]);
			mlevel = mcmp[i][];
	}
		

	SetDraw(SET_COLORMODEL, 3);
	if (iDrawType < 0)
		DrawTMatrix(s_igr++, s_mY | mcmp, {"Data", "trend"}, 1969, 1, s_iSeasLength, 0, 2);
	else
		DrawTMatrix(s_igr++, s_mY[i][] | mtrend, {"Data", "trend"}, 1969, 1, s_iSeasLength, 0, 1);

	if (iDrawType < 0)
		DrawTMatrix(s_igr++, s_mY | mcmp, {"Data", "trend"}, 1969, 1, s_iSeasLength, 0, 2);
	else
		DrawTMatrix(s_igr++, mlevel, {"level"}, 1969, 1, s_iSeasLength, 0, 2);

	// get bivariate seasonal
	j += s_cN;
	mcmp = md[j:j+s_cN -1][];
	if (iDrawType < 0)
		DrawTMatrix(s_igr++, mcmp, {"seasonal"}, 1969, 1, s_iSeasLength, 0, 2);
	else
		DrawTMatrix(s_igr++, mcmp[i][], {"seasonal"}, 1969, 1, s_iSeasLength, 0, 3);

	// get bivariate irregular
	j += (s_iSeasLength - 1) * s_cN;
	mcmp  = md[j:j+s_cN -1][];
	if (iDrawType < 0)
		DrawTMatrix(s_igr++, mcmp, {"irregular"}, 1969, 1, s_iSeasLength, 0, 2);
	else
		DrawTMatrix(s_igr++, mcmp[i][], {"irregular"}, 1969, 1, s_iSeasLength, 0, 6);

	// get bivariate signal
	j += s_cN;
	mcmp  = md[j:j+s_cN -1][];
	print("signal: md[j:j+s_cN -1][0:5] ",md[j:j+s_cN -1][0:5]);
	if (iDrawType < 0)
		DrawTMatrix(s_igr++, mcmp, {"signal"}, 1969, 1, s_iSeasLength, 0, 2);
	else
		DrawTMatrix(s_igr++, mcmp[i][], {"signal"}, 1969, 1, s_iSeasLength, 0, 2);

}

main()
{
	decl data, cp0 = 169, cp1 = 23;
	data = loadmat("UKfrontrearseatKSI.txt")';// load data, transpose
	s_mY = data[1:2][];				// log front and rear KSI
	s_cT = columns(s_mY);			// no of observations
	s_cN = rows(s_mY);				// no of time series

	// log kms travelled and petrol price, and seatbelt law
	s_mX = data[3:4][] |
		(constant(0,1,cp0) ~ constant(1,1,cp1));

	decl vP = IniPar();
	println("initial parameter vector vP ", vP);
	SetSsfModel(vP);

	vP = MaxLik();

	println("Par estimates plus std.err", vP ~ StderrPar(vP));
	PrintPar(vP);
	
	DrawComponents(vP, 0); // -1 is all series; 0,1,2,.. is specific series
	DrawComponents(vP, 1); // -1 is all series; 0,1,2,.. is specific series
	
	SaveDrawWindow("chapter9-4-2.eps");
}
