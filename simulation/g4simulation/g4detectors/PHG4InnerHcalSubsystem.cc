#include "PHG4InnerHcalSubsystem.h"
#include "PHG4InnerHcalDetector.h"
#include "PHG4EventActionClearZeroEdep.h"
#include "PHG4FlushStepTrackingAction.h"
#include "PHG4InnerHcalSteppingAction.h"
#include "PHG4Parameters.h"

#include <g4main/PHG4HitContainer.h>

#include <pdbcalbase/PdbParameterMap.h>

#include <phool/getClass.h>

#include <Geant4/globals.hh>

#include <boost/foreach.hpp>

#include <set>
#include <sstream>

using namespace std;

//_______________________________________________________________________
PHG4InnerHcalSubsystem::PHG4InnerHcalSubsystem( const std::string &name, const int lyr ):
  PHG4DetectorSubsystem( name, lyr ),
  detector_(NULL),
  steppingAction_( NULL ),
  trackingAction_(NULL),
  eventAction_(NULL)
{
  InitializeParameters();
}

//_______________________________________________________________________
int 
PHG4InnerHcalSubsystem::InitRunSubsystem( PHCompositeNode* topNode )
{
  PHNodeIterator iter( topNode );
  PHCompositeNode *dstNode = dynamic_cast<PHCompositeNode*>(iter.findFirst("PHCompositeNode", "DST" ));

  // create detector
  detector_ = new PHG4InnerHcalDetector(topNode, GetParams(), Name());
  detector_->SuperDetector(SuperDetector());
  detector_->OverlapCheck(CheckOverlap());
  set<string> nodes;
  if (GetParams()->get_int_param("active"))
    {
      PHCompositeNode *DetNode = dynamic_cast<PHCompositeNode*>(iter.findFirst("PHCompositeNode",SuperDetector()));
      if (! DetNode)
	{
          DetNode = new PHCompositeNode(SuperDetector());
          dstNode->addNode(DetNode);
        }

      ostringstream nodename;
      if (SuperDetector() != "NONE")
	{
	  nodename <<  "G4HIT_" << SuperDetector();
	}
      else
	{
	  nodename <<  "G4HIT_" << Name();
	}
      nodes.insert(nodename.str());
      if (GetParams()->get_int_param("absorberactive"))
	{
	  nodename.str("");
	  if (SuperDetector() != "NONE")
	    {
	      nodename <<  "G4HIT_ABSORBER_" << SuperDetector();
	    }
	  else
	    {
	      nodename <<  "G4HIT_ABSORBER_" << Name();
	    }
          nodes.insert(nodename.str());
	}
      BOOST_FOREACH(string node, nodes)
	{
	  PHG4HitContainer* g4_hits =  findNode::getClass<PHG4HitContainer>( topNode , node.c_str());
	  if ( !g4_hits )
	    {
	      g4_hits = new PHG4HitContainer(node);
              DetNode->addNode( new PHIODataNode<PHObject>( g4_hits, node.c_str(), "PHObject" ));
	    }
	  if (! eventAction_)
	    {
	      eventAction_ = new PHG4EventActionClearZeroEdep(topNode, node);
	    }
	  else
	    {
	      PHG4EventActionClearZeroEdep *evtact = dynamic_cast<PHG4EventActionClearZeroEdep *>(eventAction_);

	      evtact->AddNode(node);
	    }
	}

      // create stepping action
      steppingAction_ = new PHG4InnerHcalSteppingAction(detector_, GetParams());
    }
  else
    {
      // if this is a black hole it does not have to be active
      if (GetParams()->get_int_param("blackhole"))
	{
	  steppingAction_ = new PHG4InnerHcalSteppingAction(detector_, GetParams());
	}
    }
  if (steppingAction_)
    {
      trackingAction_ = new PHG4FlushStepTrackingAction(steppingAction_);
    }
  return 0;

}

//_______________________________________________________________________
int
PHG4InnerHcalSubsystem::process_event( PHCompositeNode * topNode )
{
  // pass top node to stepping action so that it gets
  // relevant nodes needed internally
  if (steppingAction_)
    {
      steppingAction_->SetInterfacePointers( topNode );
    }
  return 0;
}


void
PHG4InnerHcalSubsystem::Print(const string &what) const
{
  cout << Name() << " Parameters: " << endl;
  GetParams()->Print();
  if (detector_)
    {
      detector_->Print(what);
    }
  return;
}

//_______________________________________________________________________
PHG4Detector* PHG4InnerHcalSubsystem::GetDetector( void ) const
{
  return detector_;
}

void
PHG4InnerHcalSubsystem::SetDefaultParameters()
{
  set_default_double_param("inner_radius",116.);
  set_default_double_param("light_balance_inner_corr", NAN);
  set_default_double_param("light_balance_inner_radius", NAN);
  set_default_double_param("light_balance_outer_corr", NAN);
  set_default_double_param("light_balance_outer_radius", NAN);
  set_default_double_param("outer_radius", 136.);
  set_default_double_param("place_x", 0.);
  set_default_double_param("place_y", 0.);
  set_default_double_param("place_z", 0.);
  set_default_double_param("rot_x", 0.);
  set_default_double_param("rot_y", 0.);
  set_default_double_param("rot_z", 0.);
  set_default_double_param("scinti_eta_coverage", 1.1);
  set_default_double_param("scinti_inner_gap", 0.85);
  set_default_double_param("scinti_outer_gap", 1.22);
  set_default_double_param("scinti_gap_neighbor", 0.1);
  set_default_double_param("scinti_tile_thickness", 0.7);
  set_default_double_param("size_z", 175.94 * 2);
  set_default_double_param("steplimits", NAN);
  set_default_double_param("tilt_angle", NAN); // default is 4 crossinge, angle is calculated from this

  set_default_int_param("light_scint_model", 1);
  set_default_int_param("ncross", 4);
  set_default_int_param("n_scinti_plates", 5*64);
  set_default_int_param("n_scinti_tiles", 12);

  set_default_string_param("material", "SS310");
}


void
PHG4InnerHcalSubsystem::SetLightCorrection(const double inner_radius, const double inner_corr,const double outer_radius, const double outer_corr)
{
  set_double_param("light_balance_inner_corr", inner_corr);
  set_double_param("light_balance_inner_radius", inner_radius);
  set_double_param("light_balance_outer_corr", outer_corr);
  set_double_param("light_balance_outer_radius", outer_radius);
  return;
}
