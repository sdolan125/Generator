//____________________________________________________________________________
/*!

\class    genie::BardinIMDRadCorPXSec

\brief    Computes the Inverse Muon Decay (IMD) diff. cross section, dxsec/dy,
          where y is the interaction inelasticity, using the Bardin -
          Dokuchaeva model which includes all 1-loop radiative corrections. \n

          This is a 'trully' inclusive IMD cross section, i.e. the brem. cross
          section (dxsec_brem/dy)|w>w0 [see Bardin paper, cited below] is not
          subtracted from the IMD cross section and therefore it is not suitable
          for experimental situations where a photon energy trigger threshold
          is applied.

          BardinIMDRadCorPXSec is a concrete implementation of the
          XSecAlgorithmI interface. \n

\ref      D.Yu.Bardin and V.A.Dokuchaeva, Nucl.Phys.B287:839 (1987)

\author   Costas Andreopoulos <C.V.Andreopoulos@rl.ac.uk>
          CCLRC, Rutherford Appleton Laboratory

\created  Fabruary 14, 2005

*/
//____________________________________________________________________________

#include <iostream>

#include <TMath.h>

#include "Conventions/Constants.h"
#include "Conventions/RefFrame.h"
#include "InverseMuonDecay/BardinIMDRadCorPXSec.h"
#include "Messenger/Messenger.h"
#include "Numerical/IntegratorI.h"

using namespace genie;
using namespace genie::constants;

//____________________________________________________________________________
BardinIMDRadCorPXSec::BardinIMDRadCorPXSec() :
XSecAlgorithmI("genie::BardinIMDRadCorPXSec")
{

}
//____________________________________________________________________________
BardinIMDRadCorPXSec::BardinIMDRadCorPXSec(string config) :
XSecAlgorithmI("genie::BardinIMDRadCorPXSec", config)
{

}
//____________________________________________________________________________
BardinIMDRadCorPXSec::~BardinIMDRadCorPXSec()
{

}
//____________________________________________________________________________
double BardinIMDRadCorPXSec::XSec(const Interaction * interaction) const
{
  if(! this -> ValidProcess    (interaction) ) return 0.;
  if(! this -> ValidKinematics (interaction) ) return 0.;

  const InitialState & init_state = interaction -> GetInitialState();

  double E    = init_state.GetProbeE(kRfLab);
  double sig0 = kGF_2 * kElectronMass * E / kPi;
  double re   = 0.5 * kElectronMass / E;
  double r    = (kMuonMass_2 / kElectronMass_2) * re;
  double y    = interaction->GetKinematics().y();

  y = 1-y;  //Note: y = (Ev-El)/Ev but in Bardin's paper y=El/Ev.

  double ymin = r + re;
  double ymax = 1 + re + r*re / (1+re);

  double e = 1E-5;
  ymax = TMath::Min(ymax,1-e); // avoid ymax=1, due to a log(1-y)

  LOG("BardinIMD", pDEBUG)
           << "sig0 = " << sig0 << ", r = " << r << ", re = " << re;
  LOG("BardinIMD", pDEBUG)
                   << "allowed y: [" << ymin << ", " << ymax << "]";

  if(y<ymin || y>ymax) return 0;

  double dsig_dy = 2 * sig0 * ( 1 - r + (kAem/kPi) * Fa(re,r,y) );

  LOG("BardinIMD", pINFO)
     << "dxsec[1-loop]/dy (Ev = " << E << ", y = " << y << ") = " << dsig_dy;


  if( interaction->TestBit(kIAssumeFreeNucleon) ) return dsig_dy;

  int NNucl = init_state.GetTarget().Z(); // num of scattering centers
  dsig_dy *= NNucl; 

  return dsig_dy;
}
//____________________________________________________________________________
bool BardinIMDRadCorPXSec::ValidProcess(const Interaction * interaction) const
{
  if(interaction->TestBit(kISkipProcessChk)) return true;

  return true;
}
//____________________________________________________________________________
bool BardinIMDRadCorPXSec::ValidKinematics(
                                        const Interaction * interaction) const
{
  if(interaction->TestBit(kISkipKinematicChk)) return true;

  const InitialState & init_state = interaction -> GetInitialState();
  double E = init_state.GetProbeE(kRfLab);
  double s = kElectronMass_2 + 2*kElectronMass*E;

  //-- check if it is kinematically allowed
  if(s < kMuonMass_2) {
     LOG("BardinIMD", pINFO)
        << "Ev = " << E << " (s = " << s << ") is below threshold (s-min = "
        << kMuonMass_2 << ") for IMD";
     return false;
  }
  return true;
}
//____________________________________________________________________________
double BardinIMDRadCorPXSec::Fa(double re, double r, double y) const
{
  double y2  = y * y;
  double rre = r * re;
  double r_y = r/y;
  double y_r = y/r;

  double fa = 0;

  fa = (1-r) *       ( TMath::Log(y2/rre) * TMath::Log(1-r_y) +
                       TMath::Log(y_r) * TMath::Log(1-y) -
                       this->Li2( r ) +
                       this->Li2( y ) +
                       this->Li2( (r-y) / (1-y) ) +
                       1.5 * (1-r) * TMath::Log(1-r)
                     )
                     +

       0.5*(1+3*r) * ( this->Li2( (1-r_y) / (1-r) ) -
                       this->Li2( (y-r)   / (1-r) ) -
                       TMath::Log(y_r) * TMath::Log( (y-r) / (1-r) )
                     )
                     +

       this->P(1,r,y)                   -
       this->P(2,r,y) * TMath::Log(r)   -
       this->P(3,r,y) * TMath::Log(re)  +
       this->P(4,r,y) * TMath::Log(y)   +
       this->P(5,r,y) * TMath::Log(1-y) +
       this->P(6,r,y) * (1 - r_y) * TMath::Log(1-r_y);

  return fa;
}
//____________________________________________________________________________
double BardinIMDRadCorPXSec::P(int i, double r, double y) const
{
  int kmin = -3;
  int kmax =  2;

  double p = 0;

  for(int k = kmin; k <= kmax; k++) {

     double c  = this->C(i,k,r);
     double yk = TMath::Power(y,k);

     p += (c*yk);
  }
  return p;
}
//____________________________________________________________________________
double BardinIMDRadCorPXSec::Li2(double z) const
{
  double epsilon = 1e-2;
  Range1D_t t(epsilon, 1.0 - epsilon);

  LOG("BardinIMD", pDEBUG)
    << "Summing BardinIMDRadCorIntegrand in [" << t.min<< ", " << t.max<< "]";

  BardinIMDRadCorIntegrand * func = new BardinIMDRadCorIntegrand(z);
  func->SetParam(0,"t",t);

  double li2 = fIntegrator->Integrate(*func);

  LOG("BardinIMD", pDEBUG) << "Li2(z = " << z << ")" << li2;

  delete func;
  return li2;
}
//____________________________________________________________________________
double BardinIMDRadCorPXSec::C(int i, int k, double r) const
{
  if        ( i == 1 ) {

      if      (k == -3) return -0.19444444*TMath::Power(r,3.);
      else if (k == -2) return (0.083333333+0.29166667*r)*TMath::Power(r,2.);
      else if (k == -1) return -0.58333333*r - 0.5*TMath::Power(r,2.) - TMath::Power(r,3.)/6.;
      else if (k ==  0) return -1.30555560 + 3.125*r + 0.375*TMath::Power(r,2.);
      else if (k ==  1) return -0.91666667 - 0.25*r;
      else if (k ==  2) return 0.041666667;
      else              return 0.;

  } else if ( i == 2 ) {

      if      (k == -3) return 0.;
      else if (k == -2) return 0.5*TMath::Power(r,2.);
      else if (k == -1) return 0.5*r - 2*TMath::Power(r,2.);
      else if (k ==  0) return 0.25 - 0.75*r + 1.5*TMath::Power(r,2);
      else if (k ==  1) return 0.5;
      else if (k ==  2) return 0.;
      else              return 0.;

  } else if ( i == 3 ) {

      if      (k == -3) return 0.16666667*TMath::Power(r,3.);
      else if (k == -2) return 0.25*TMath::Power(r,2.)*(1-r);
      else if (k == -1) return r-0.5*TMath::Power(r,2.);
      else if (k ==  0) return 0.66666667;
      else if (k ==  1) return 0.;
      else if (k ==  2) return 0.;
      else              return 0.;

  } else if ( i == 4 ) {

      if      (k == -3) return 0.;
      else if (k == -2) return TMath::Power(r,2.);
      else if (k == -1) return r*(1-4.*r);
      else if (k ==  0) return 1.5*TMath::Power(r,2.);
      else if (k ==  1) return 1.;
      else if (k ==  2) return 0.;
      else              return 0.;

  } else if ( i == 5 ) {

      if      (k == -3) return 0.16666667*TMath::Power(r,3.);
      else if (k == -2) return -0.25*TMath::Power(r,2.)*(1+r);
      else if (k == -1) return 0.5*r*(1+3*r);
      else if (k ==  0) return -1.9166667+2.25*r-1.5*TMath::Power(r,2);
      else if (k ==  1) return -0.5;
      else if (k ==  2) return 0.;
      else              return 0.;

  } else if ( i == 6 ) {

      if      (k == -3) return 0.;
      else if (k == -2) return 0.16666667*TMath::Power(r,2.);
      else if (k == -1) return -0.25*r*(r+0.33333333);
      else if (k ==  0) return 1.25*(r+0.33333333);
      else if (k ==  1) return 0.5;
      else if (k ==  2) return 0.;
      else              return 0.;

  } else return 0.;
}
//____________________________________________________________________________
void BardinIMDRadCorPXSec::Configure(const Registry & config)
{
  Algorithm::Configure(config);
  this->LoadSubAlg();
}
//____________________________________________________________________________
void BardinIMDRadCorPXSec::Configure(string param_set)
{
  Algorithm::Configure(param_set);
  this->LoadSubAlg();
}
//____________________________________________________________________________
void BardinIMDRadCorPXSec::LoadSubAlg(void)
{
  fIntegrator = 0;

  //-- get specified integration algorithm
  fIntegrator = dynamic_cast<const IntegratorI *> (
                 this->SubAlg("integrator-alg-name", "integrator-param-set"));
  assert(fIntegrator);
}
//____________________________________________________________________________
// Auxiliary scalar function for internal integration
//____________________________________________________________________________
BardinIMDRadCorIntegrand::BardinIMDRadCorIntegrand(double z) :
GSFunc(1)
{
  fZ = z;
}
//____________________________________________________________________________
BardinIMDRadCorIntegrand::~BardinIMDRadCorIntegrand()
{

}
//____________________________________________________________________________
double BardinIMDRadCorIntegrand::operator() (const vector<double> & param)
{
  double t = param[0];
  if(t==0) return 0.;

  if(t*fZ>=1.) return 0.;
  double f = TMath::Log(1.-fZ*t)/t;
  return f;
}
//____________________________________________________________________________

