//____________________________________________________________________________
/*!

\class    genie::RESXSec

\brief    Computes the RES Cross Section.\n
          Is a concrete implementation of the XSecIntegratorI interface.\n

\author   Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
          STFC, Rutherford Appleton Laboratory

\created  May 04, 2004

\cpright  Copyright (c) 2003-2015, GENIE Neutrino MC Generator Collaboration
          For the full text of the license visit http://copyright.genie-mc.org
          or see $GENIE/LICENSE
*/
//____________________________________________________________________________

#ifndef _RES_XSEC_H_
#define _RES_XSEC_H_

#include "Base/XSecIntegratorI.h"

namespace genie {

class RESXSec : public XSecIntegratorI {

public:
  RESXSec();
  RESXSec(string param_set);
  virtual ~RESXSec();

  //! XSecIntegratorI interface implementation
  double Integrate(const XSecAlgorithmI * model, const Interaction * i) const;

  //! overload the Algorithm::Configure() methods to load private data
  //! members from configuration options
  void Configure(const Registry & config);
  void Configure(string config);

private:
  void LoadConfig (void);
};

}       // genie namespace
#endif  // _RES_XSEC_H_
