//____________________________________________________________________________
/*!

\program gtestKPhaseSpace

\brief   Program used for testing / debugging the kinematic phase space calc

\author  Costas Andreopoulos <C.V.Andreopoulos@rl.ac.uk>
         STFC, Rutherford Appleton Laboratory

\created June 20, 2004

\cpright Copyright (c) 2003-2008, GENIE Neutrino MC Generator Collaboration
         For the full text of the license visit http://copyright.genie-mc.org
         or see $GENIE/LICENSE
*/
//____________________________________________________________________________

#include <TFile.h>
#include <TTree.h>

#include "Interaction/Interaction.h"
#include "Messenger/Messenger.h"
#include "PDG/PDGCodes.h"

using namespace genie;

void PrintLimits(const Interaction * interaction);

//__________________________________________________________________________
int main(int /*argc*/, char ** /*argv*/)
{
  // -- get a DIS interaction object & access its kinematics

  int tgt         = kPdgTgtFe56;
  int hit_nucleon = kPdgProton;
  int neutrino    = kPdgNuMu;
  double Ev       = 3;

  Interaction * qelcc = Interaction::QELCC(tgt,hit_nucleon,neutrino,Ev);
  Interaction * rescc = Interaction::RESCC(tgt,hit_nucleon,neutrino,Ev);
  Interaction * discc = Interaction::DISCC(tgt,hit_nucleon,neutrino,Ev);

  PrintLimits(qelcc);
  PrintLimits(rescc);
  PrintLimits(discc);

  return 0;
}
//__________________________________________________________________________
void PrintLimits(const Interaction * interaction)
{
  LOG("Main", pNOTICE) << *interaction;

  const KPhaseSpace & phase_space = interaction->PhaseSpace();

  Range1D_t xl  = phase_space.Limits(kKVx);
  Range1D_t yl  = phase_space.Limits(kKVy);
  Range1D_t Q2l = phase_space.Limits(kKVQ2);
  Range1D_t Wl  = phase_space.Limits(kKVW);

  LOG("Main", pNOTICE) << "x  e [" << xl.min  << ", " << xl.max  << "]";
  LOG("Main", pNOTICE) << "y  e [" << yl.min  << ", " << yl.max  << "]";
  LOG("Main", pNOTICE) << "Q2 e [" << Q2l.min << ", " << Q2l.max << "]";
  LOG("Main", pNOTICE) << "W  e [" << Wl.min  << ", " << Wl.max  << "]";
}
//__________________________________________________________________________
