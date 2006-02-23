//
// Exports conrete XSecAlgorithmI class as a GSFunc (GENIE scalar function)
// for interfacing with the numerical integration code
//
#ifndef _GENIE_XSEC_FUNCTIONS_H_
#define _GENIE_XSEC_FUNCTIONS_H_

#include "Numerical/GSFunc.h"

namespace genie {

class XSecAlgorithmI;
class Interaction;

//____________________________________________________________________________
/*!
\class    genie::GXSecFunc
\brief    ABC for a cross section function. Exports a conrete XSecAlgorithmI
          class as a GSFunc (GENIE scalar function). Note that this is merely
          a structural trick for interfacing with the numerical integration
          code. The implementation of xsec models is in concrete XSecAlgorithmIs
\author   Costas Andreopoulos <C.V.Andreopoulos@rl.ac.uk>
          CCLRC, Rutherford Appleton Laboratory
\created  February 20, 2006
*/
//____________________________________________________________________________

class GXSecFunc : public GSFunc
{
public:
  GXSecFunc(const XSecAlgorithmI * m, const Interaction * i, unsigned int n);
  ~GXSecFunc();

protected:
  const XSecAlgorithmI * fModel;
  const Interaction *    fInteraction;
};

//____________________________________________________________________________
/*!
\class    genie::Integrand_D2XSec_DxDy_E
\brief    A 2-D cross section function: d2xsec/dxdy = f(x,y)|(fixed E)
\author   Costas Andreopoulos <C.V.Andreopoulos@rl.ac.uk>
          CCLRC, Rutherford Appleton Laboratory
\created  February 20, 2006
*/
//____________________________________________________________________________

class Integrand_D2XSec_DxDy_E : public GXSecFunc
{
public:
  Integrand_D2XSec_DxDy_E(const XSecAlgorithmI * m, const Interaction * i);
  ~Integrand_D2XSec_DxDy_E();

  double operator () (const vector<double> & x);
};

//____________________________________________________________________________
/*!
\class    genie::Integrand_D2XSec_DxDy_E_WQ2Cuts
\brief    A 2-D cross section function: d2xsec/dxdy = f(x,y)|(fixed E)
          with cuts on W and Q2
\author   Costas Andreopoulos <C.V.Andreopoulos@rl.ac.uk>
          CCLRC, Rutherford Appleton Laboratory
\created  February 20, 2006
*/
//____________________________________________________________________________

class Integrand_D2XSec_DxDy_E_WQ2Cuts : public GXSecFunc
{
public:
  Integrand_D2XSec_DxDy_E_WQ2Cuts(
            const XSecAlgorithmI * m, const Interaction * i,
                                          Range1D_t WCuts, Range1D_t Q2Cuts);
  ~Integrand_D2XSec_DxDy_E_WQ2Cuts();

  double operator () (const vector<double> & x);

private:
  Range1D_t fWCuts;
  Range1D_t fQ2Cuts;
};

//____________________________________________________________________________
/*!
\class    genie::Integrand_DXSec_DQ2_E
\brief    A 1-D cross section function: dxsec/dQ2 = f(Q2)|(fixed E)
\author   Costas Andreopoulos <C.V.Andreopoulos@rl.ac.uk>
          CCLRC, Rutherford Appleton Laboratory
\created  February 20, 2006
*/
//____________________________________________________________________________

class Integrand_DXSec_DQ2_E : public GXSecFunc
{
public:
  Integrand_DXSec_DQ2_E(const XSecAlgorithmI * m, const Interaction * i);
  ~Integrand_DXSec_DQ2_E();

  double operator () (const vector<double> & x);
};

//____________________________________________________________________________
/*!
\class    genie::Integrand_D2XSec_DWDQ2_E
\brief    A 2-D cross section function: d2xsec/dWdQ2 = f(W,Q2)|(fixed E)
\author   Costas Andreopoulos <C.V.Andreopoulos@rl.ac.uk>
          CCLRC, Rutherford Appleton Laboratory
\created  February 20, 2006
*/
//____________________________________________________________________________

class Integrand_D2XSec_DWDQ2_E : public GXSecFunc
{
public:
  Integrand_D2XSec_DWDQ2_E(const XSecAlgorithmI * m, const Interaction * i);
  ~Integrand_D2XSec_DWDQ2_E();

  double operator () (const vector<double> & x);
};

//____________________________________________________________________________
/*!
\class    genie::Integrand_DXSec_Dy_E
\brief    A 1-D cross section function: dxsec/dy = f(y)|(fixed E)
\author   Costas Andreopoulos <C.V.Andreopoulos@rl.ac.uk>
          CCLRC, Rutherford Appleton Laboratory
\created  February 20, 2006
*/
//____________________________________________________________________________

class Integrand_DXSec_Dy_E : public GXSecFunc
{
public:
  Integrand_DXSec_Dy_E(const XSecAlgorithmI * m, const Interaction * i);
  ~Integrand_DXSec_Dy_E();

  double operator () (const vector<double> & x);
};

//____________________________________________________________________________
/*!
\class    genie::Integrand_D2XSec_DxDy_Ex
\brief    A 1-D cross section function: d2xsec/dxdy = f(y)|(fixed:E,x)
\author   Costas Andreopoulos <C.V.Andreopoulos@rl.ac.uk>
          CCLRC, Rutherford Appleton Laboratory
\created  February 20, 2006
*/
//____________________________________________________________________________

class Integrand_D2XSec_DxDy_Ex : public GXSecFunc
{
public:
  Integrand_D2XSec_DxDy_Ex(
            const XSecAlgorithmI * m, const Interaction * i, double x);
  ~Integrand_D2XSec_DxDy_Ex();

  double operator () (const vector<double> & x);

private:
  double fx;
};

//____________________________________________________________________________
/*!
\class    genie::Integrand_D2XSec_DxDy_Ey
\brief    A 1-D cross section function: d2xsec/dxdy = f(x)|(fixed:E,y)
\author   Costas Andreopoulos <C.V.Andreopoulos@rl.ac.uk>
          CCLRC, Rutherford Appleton Laboratory
\created  February 20, 2006
*/
//____________________________________________________________________________

class Integrand_D2XSec_DxDy_Ey : public GXSecFunc
{
public:
  Integrand_D2XSec_DxDy_Ey(
          const XSecAlgorithmI * m, const Interaction * i, double x);
  ~Integrand_D2XSec_DxDy_Ey();

  double operator () (const vector<double> & x);

private:
  double fy;
};

//____________________________________________________________________________
/*!
\class    genie::Integrand_D2XSec_DWDQ2_EW
\brief    A 1-D cross section function: d2xsec/dWdQ2= f(Q2)|(fixed:E,W)
\author   Costas Andreopoulos <C.V.Andreopoulos@rl.ac.uk>
          CCLRC, Rutherford Appleton Laboratory
\created  February 20, 2006
*/
//____________________________________________________________________________

class Integrand_D2XSec_DWDQ2_EW : public GXSecFunc
{
public:
  Integrand_D2XSec_DWDQ2_EW(
             const XSecAlgorithmI * m, const Interaction * i, double W);
  ~Integrand_D2XSec_DWDQ2_EW();

  double operator () (const vector<double> & x);

private:
  double fW;
};

//____________________________________________________________________________
/*!
\class    genie::Integrand_D2XSec_DWDQ2_EQ2
\brief    A 1-D cross section function: d2xsec/dWdQ2= f(W)|(fixed:E,Q2)
\author   Costas Andreopoulos <C.V.Andreopoulos@rl.ac.uk>
          CCLRC, Rutherford Appleton Laboratory
\created  February 20, 2006
*/
//____________________________________________________________________________

class Integrand_D2XSec_DWDQ2_EQ2 : public GXSecFunc
{
public:
  Integrand_D2XSec_DWDQ2_EQ2(
            const XSecAlgorithmI * m, const Interaction * i, double Q2);
  ~Integrand_D2XSec_DWDQ2_EQ2();

  double operator () (const vector<double> & x);

private:
  double fQ2;
};

}        // genie namespace
#endif   // _GENIE_XSEC_FUNCTIONS_H_

