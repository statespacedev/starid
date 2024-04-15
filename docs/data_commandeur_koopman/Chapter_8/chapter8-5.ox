/* File chapter8-5.ox
   Stochastic level and deterministic seasonal model
   with explanatory variable log(PETROL PRICE)
   and intervention variable SEATBELT LAW
   for log UK drivers KSI: diagnostic tests.
   Ox/SsfPack code for Chapter 8, Section 5, of:
   ****************************************************************
   *  An Introduction to State Space Time Series Analysis (2007). *
   *  Jacques J.F. Commandeur and Siem Jan Koopman.			      *
   *  Oxford: Oxford University Press.							  *
   ****************************************************************/

#include <oxstd.h>
#include <oxdraw.h>
#import  <maximize>
#include <packages/ssfpack/ssfpack.h>

static decl s_mY, s_cT;			// dependent variable (1 x n) and n
static decl s_mX;				// independent variables (2 x n)
static decl s_mStsm, s_vVarCmp;	// matrices for state space model
static decl s_dVar;				// scale factor
static decl s_vPar;				// parameter vector of model

SetStsmModel(const vP)
{
	// map to stochastic level and deterministic seasonal model 
	s_mStsm = <	CMP_LEVEL,     0.5,  0,  0;
				CMP_SEAS_TRIG,   0, 12,  0;
				CMP_IRREG,       1,  0,  0>;
	// change BFGS parameters into error variances
	decl vr = exp(2.0 * vP);
	// s_vVarCmp is used to update diagonal(Omega)
	//          expl.var. int.var. level    trigo seasonal   irregular
	s_vVarCmp =    0     |   0   | vr[0] | constant(0,11,1) |  vr[1];
}
LogLikStsm(const vY, const pdLik, const pdVar)
{
	decl mphi, momega, msigma, mjphi, ret_val;

	// set up system matrices for state space model
	GetSsfStsm(s_mStsm, &mphi, &momega, &msigma);
	// add explanatory variable log(PETROL PRICE)
	// and intervention variable SEATBELT LAW to state space model
	mjphi = <>;
	AddSsfReg(s_mX, &mphi, &momega, &msigma, &mjphi);
	momega = diag(s_vVarCmp);	// create Omega from s_vVarCmp
	// compute loglikelihood and scale factor
	ret_val = SsfLikEx(pdLik, pdVar, vY, mphi, momega, msigma,
						<>, mjphi, <>, <>, s_mX);
	return ret_val;     		// 1 indicates success, 0 failure
}
LogLikScoStsm(const vY, const pdLik, const pvSco)
{
    decl mphi, momega, msigma, mjphi, msco, ret_val, dvar, vs;

	// set up system matrices for state space model
	GetSsfStsm(s_mStsm, &mphi, &momega, &msigma);
	// add explanatory variable log(PETROL PRICE)
	// and intervention variable SEATBELT LAW to state space model
	mjphi = <>;
	AddSsfReg(s_mX, &mphi, &momega, &msigma, &mjphi);
	momega = diag(s_vVarCmp);

	// compute loglikelihood and score vector
    ret_val = SsfLikScoEx(pdLik, &dvar, &msco, vY, mphi, momega, msigma,
							<>, mjphi, <>, <>, s_mX);
	vs = (diagonal(msco)' .*  s_vVarCmp);
	pvSco[0][0] = vs[2];
	pvSco[0][1] = vs[14];
	pvSco[0][] /= s_cT;	// scale score vector by sample size
    pdLik[0] /= s_cT;		// scale loglikelihood by sample size
	
    return ret_val;     	// 1 indicates success, 0 failure
}
InitialPar()
{
	decl dlik, dvar, vp;

	vp = log(<0.5; 1>);
	SetStsmModel(vp);	// map vP to stochastic level and deterministic seasonal model
	LogLikStsm(s_mY, &dlik, &dvar);
	// scale initial estimates by scale factor
	return vp + 0.5 * log(dvar);
}
Likelihood(const vP, const pdLik, const pvSco, const pmHes)
{                       		// arguments dictated by MaxBFGS()
    decl ret_val;

	SetStsmModel(vP);	// map vP to stochastic level and deterministic seasonal model
	ret_val = pvSco ? LogLikScoStsm(s_mY, pdLik, pvSco)
				: LogLikStsm(s_mY, pdLik, &s_dVar);
	return ret_val;		// 1 indicates success, 0 failure
}
MaxLik()
{
	decl vp, dlik, ir;

	vp = InitialPar();	// initialise unconstrained BFGS parameters
	print("\ninitial values BFGS parameters",vp);
	print("\n");
	MaxControl(50, 5, 1);	// start iterations BFGS algorithm
    ir = MaxBFGS(Likelihood, &vp, &dlik, 0, FALSE);

    println("\n", MaxConvergenceMsg(ir),
          " using analytical derivatives",
          "\n(1/n) Log-likelihood = ", "%.8g", dlik,
		  "; n = ", s_cT, ";");
	s_vPar = vp;
	print("\nparameter estimates (unconstrained)", vp);
	print("\nerror variance estimates", exp(2.0 * vp));
}
DrawComponents(const mY)
{
    decl mphi, momega, msigma, mjphi, minf, mKfilter, cst, cres;
	decl msofe, macf, mdenom, dQ10, mmoments, dS, dK, dN;
	decl mdisturb, mauxres1, mauxres2;

	SetStsmModel(s_vPar); // map vP to stochastic level and deterministic seasonal model
	// set up system matrices for state space model
	GetSsfStsm(s_mStsm, &mphi, &momega, &msigma);
	// add explanatory variable log(PETROL PRICE)
	// and intervention variable SEATBELT LAW to state space model
	mjphi = <>;
	AddSsfReg(s_mX, &mphi, &momega, &msigma, &mjphi);
	momega = diag(s_vVarCmp);

	// compute one step forecast errors and prediction error variances
	minf = KalmanInit(mY, mphi, momega, msigma,
						<>, mjphi, <>, <>, s_mX);
	mKfilter = KalmanFilEx(minf, mY, mphi, momega, msigma,
						<>, mjphi, <>, <>, s_mX);

	print("\nmKfilter[][0:4]", mKfilter[][0:4]);
	
	// compute smoothed disturbances
	SsfMomentEstEx(DS_SMO, &mdisturb, mY, mphi, momega, msigma,
						<>, mjphi, <>, <>, s_mX);

	// compute auxiliary residuals
	mauxres1 = mdisturb[2][] ./ sqrt(mdisturb[17][]);
	mauxres2 = mdisturb[14][] ./ sqrt(mdisturb[29][]);

	// test independence of residuals
	cst = columns(mphi);
	cres = s_cT - cst;
	msofe = mKfilter[0][] .* sqrt(mKfilter[15][]);
	macf = acf(msofe[][cst:s_cT-1]', 10);
	print("\nmacf", macf);
	mdenom = cres - <1;2;3;4;5;6;7;8;9;10>;
	dQ10 = cres*(cres+2)*sumc((macf[1:10][].^2)./mdenom);
	print("\nAutocorrelation: Box-Ljung statistic");
	print("\n------------------------------------");
	print("\nQ(10) = ", dQ10);

	// test homoscedasticity of residuals
	decl ch, dH;
	ch = round((s_cT-cst)/3);
	dH = (sumr(msofe[][s_cT-ch:s_cT-1].^2))/(sumr(msofe[][cst:cst+ch-1].^2));
	print("\nHomoscedasticity");
	print("\n----------------");
	print("\nH(",ch,") = ", dH);
	print("\n1/H(",ch,") = ", 1/dH);
	print("\nF(10,10;0.025) = 3.72\n");
	
	// test normality of residuals
	mmoments = moments(msofe[][cst:s_cT-1]', 4, 0);
	dS = mmoments[3][]/sqrt(mmoments[2][]^3);
	dK = mmoments[4][]/(mmoments[2][]^2);
	dN = cres*(((dS^2)/6)+(((dK-3)^2)/24));
	print("\nNormality");
	print("\n---------");
	print("\nN = ", dN); 
	print("\nChi-2(2;0.05) = 5.99\n");

	SetDraw(SET_COLORMODEL, 3);
	DrawTMatrix(0, msofe[][cst:s_cT-1], {"standardised one-step prediction errors"}, 1970, 3, 12, 0, 6);
	DrawAcf(1, msofe[][cst:s_cT-1], {"standardised one-step prediction errors"}, 10, TRUE, FALSE, TRUE,6);
	DrawDensity(2, msofe[][cst:s_cT-1], {"Histogram of standardised one-step prediction errors"},
		0, 1, 1, 0, 0, 0, 6);
	SaveDrawWindow("chapter8-5-1.eps");
	CloseDrawWindow();
	
	// plot auxiliary residuals
	// replace infinite value of mauxres1 in January 1983 by zero
	mauxres1[][168] = 0;
	DrawTMatrix(0, mauxres1, {"Structural level break $t$-tests"}, 1969, 1, 12, 0, 6);
	DrawLine(0, 1969, 2, 1985, 2, 4);
	DrawLine(0, 1969, -2, 1985, -2, 4);
	DrawTMatrix(1, mauxres2, {"Outlier $t$-tests"}, 1969, 1, 12, 0, 6);
	DrawLine(1, 1969, 2, 1985, 2, 4);
	DrawLine(1, 1969, -2, 1985, -2, 4);
	SaveDrawWindow("chapter8-5-2.eps");
}
main()
{
	decl cp0 = 169, cp1 = 23;

	s_mY = log(loadmat("UKdriversKSI.txt")');	// load data, transpose, log()
	s_cT = columns(s_mY);					// no of observations

	// set up predictor matrix with explanatory variable log(PETROL PRICE)
	// and intervention variable SEATBELT LAW
	s_mX = loadmat("logUKpetrolprice.txt")' |
		(constant(0,1,cp0) ~ constant(1,1,cp1));

	MaxLik();
	DrawComponents(s_mY);
}
