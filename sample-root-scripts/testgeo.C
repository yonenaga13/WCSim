#include <iostream>
#include <stdio.h>     
#include <stdlib.h>    

#include "TH1F.h"
#include "TFile.h"
#include "TTree.h"
#include "TSystem.h"
#include "TROOT.h"

#include "WCSimRootGeom.hh"

// Simple example of reading a generated Root file
int testgeo(const char *filename="../wcsim.root")
{

  // Clear global scope
  gROOT->Reset();

  // Open the file
  TFile *file = new TFile(filename,"read");
  if (!file->IsOpen()){
    cout << "Error, could not open input file: " << filename << endl;
    return -1;
  }
  
  // Get the a pointer to the tree from the file
  TTree *gtree = (TTree*)file->Get("wcsimGeoT");
  
  // Get the number of events
  int nevent = gtree->GetEntries();
  printf("geo nevent %d\n",nevent);
  
  // Create a WCSimRootGeom to put stuff from the tree in

  WCSimRootGeom* wcsimrootgeom = new WCSimRootGeom();

  // Set the branch address for reading from the tree
  TBranch *branch = gtree->GetBranch("wcsimrootgeom");
  branch->SetAddress(&wcsimrootgeom);

  // Now loop over "events"  (should be only one for geo tree)
  int ev;
  for (ev=0;ev<nevent; ev++)  {
      // Read the event from the tree into the WCSimRootGeom instance
      gtree->GetEntry(ev);
      printf("Cyl radius %f\n", wcsimrootgeom->GetWCCylRadius());
      printf("Cyl length %f\n", wcsimrootgeom->GetWCCylLength());
      printf("PMT radius %f\n", wcsimrootgeom->GetWCPMTRadius());
      printf("Offset x y z %f %f %f\n", wcsimrootgeom->GetWCOffset(0),
	     wcsimrootgeom->GetWCOffset(1),wcsimrootgeom->GetWCOffset(2));
      int numpmt = wcsimrootgeom->GetWCNumPMT();
      printf("Num PMTs %d\n", numpmt);

      int i;
      for (i=0;i<((numpmt<20)?numpmt:20);i++){
	WCSimRootPMT pmt;
	pmt = wcsimrootgeom->GetPMT(i);
	printf ("pmt %d %d %d\n",i,pmt.GetTubeNo(), pmt.GetCylLoc());
	printf ("position: %f %f %f\n", pmt.GetPosition(0),
		pmt.GetPosition(1),pmt.GetPosition(2));
	printf ("orientation: %f %f %f\n", pmt.GetOrientation(0),
		pmt.GetOrientation(1),pmt.GetOrientation(2));
      }
            
    } // End of loop over events

  return 0;
}
