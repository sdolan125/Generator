//____________________________________________________________________________
/*!

\class   genie::NucBindEnergyAggregator

\brief   A nuclear binding energy 'collector' which visits the event record,
         finds nucleons originating from within a nuclei and subtracts the
         binding energy they had in the nucleus.

         To record this action in the event record a hypothetical BINDINO is
         added to the event record.

         Is a concerete implementation of the EventRecordVisitorI interface.

\author  Costas Andreopoulos <C.V.Andreopoulos@rl.ac.uk>
         CCLRC, Rutherford Appleton Laboratory

\created November 19, 2004

*/
//____________________________________________________________________________

#include <TLorentzVector.h>

#include "Conventions/Constants.h"
#include "EVGModules/NucBindEnergyAggregator.h"
#include "GHEP/GHepRecord.h"
#include "GHEP/GHepParticle.h"
#include "GHEP/GHepStatus.h"
#include "Interaction/Interaction.h"
#include "Messenger/Messenger.h"
#include "PDG/PDGCodes.h"
#include "PDG/PDGUtils.h"
#include "Utils/MathUtils.h"
#include "Utils/NuclearUtils.h"

using namespace genie;
using namespace genie::constants;

//___________________________________________________________________________
NucBindEnergyAggregator::NucBindEnergyAggregator() :
EventRecordVisitorI("genie::NucBindEnergyAggregator")
{

}
//___________________________________________________________________________
NucBindEnergyAggregator::NucBindEnergyAggregator(string config) :
EventRecordVisitorI("genie::NucBindEnergyAggregator", config)
{

}
//___________________________________________________________________________
NucBindEnergyAggregator::~NucBindEnergyAggregator()
{

}
//___________________________________________________________________________
void NucBindEnergyAggregator::ProcessEventRecord(GHepRecord * event_rec) const
{
  Interaction * interaction = event_rec->GetInteraction();
  const InitialState & init_state = interaction->GetInitialState();

  TIter stdhep_iter(event_rec);
  GHepParticle * p = 0;

  int ipos = 0;

  while( (p = (GHepParticle * ) stdhep_iter.Next()) ) {

     bool is_nucleon        =  pdg::IsNeutronOrProton(p->PdgCode());
     bool is_in_final_state =  p->Status() == kIStStableFinalState;

     if( is_nucleon && is_in_final_state ) {

        // check if it is coming from a nucleus and find it in the record

        GHepParticle * nucleus = this->FindMotherNucleus(ipos, event_rec);
        if(nucleus) {
           //-- ask for the binding energy of the most loose nucleon
           //  (separation energy)

           const Target & target = init_state.GetTarget();
           double bindE = utils::nuclear::BindEnergyLastNucleon(target);

           LOG("Nuclear", pINFO) << "Binding energy = " << bindE;

           //-- subtract this energy from the final state nucleon
           LOG("Nuclear", pINFO)
              << "Subtracting the binding energy from the escaped nucleon";

           double M  = p->Mass();
           double En = p->Energy() - bindE;

           double pmag_old = p->P4()->P();
           double pmag_new = TMath::Sqrt(utils::math::NonNegative(En*En-M*M));

           double scale = pmag_new / pmag_old;

           double pxn = scale * p->Px();
           double pyn = scale * p->Py();
           double pzn = scale * p->Pz();

           double pxb = (1-scale) * p->Px();
           double pyb = (1-scale) * p->Py();
           double pzb = (1-scale) * p->Pz();

           p->SetEnergy ( En  );
           p->SetPx     ( pxn );
           p->SetPy     ( pyn );
           p->SetPz     ( pzn );

           //-- and add a BINDINO to record this in the event record and
           //   conserve energy/momentum

           LOG("Nuclear", pINFO)
               << "Adding a BINDINO to account for nuclear binding energy";

           TLorentzVector v4(0,0,0,0);             // dummy position 4-vector
           TLorentzVector p4(pxb,pyb,pzb,bindE);   // momentum 4-vector

           int          bpdgc = kPdgBindino;
           GHepStatus_t bist  = kIStStableFinalState;

           event_rec->AddParticle(bpdgc, bist, -1,-1,-1,-1, p4, v4);

        } // nucleus != 0
     }  // if is a final state p,n

     ipos++;
  }
}
//___________________________________________________________________________
GHepParticle * NucBindEnergyAggregator::FindMotherNucleus(
                                    int ipos, GHepRecord * event_rec) const
{
  GHepParticle * p = event_rec->Particle(ipos);

  //-- get its mothet
  int mother_pos = p->FirstMother();

  //-- if mother is set
  if(mother_pos != -1) {
     GHepParticle * mother = event_rec->Particle(mother_pos);

     //-- check its status
     if( mother->Status() == kIstNucleonTarget ) {

        //-- get the mother's mother
        int grandmother_pos = mother->FirstMother();

        //-- if grandmother is set get its PDG code a check if it is an ion
        if(grandmother_pos != -1) {
             GHepParticle * grandmother =
                                   event_rec->Particle(grandmother_pos);

             int grandmother_pdgc = grandmother->PdgCode();
             if( pdg::IsIon(grandmother_pdgc) ) return grandmother;

        } // gmpos != -1
     } // mother-status
  }  //mpos != -1

  return 0;
}
//___________________________________________________________________________
