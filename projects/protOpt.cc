//*******************************************************************************************************
//*******************************************************************************************************
//*************************************                      ********************************************
//*************************************       protOpt        ********************************************
//*************************************                      ********************************************
//*******************************************************************************************************
//******** -sidechain and backbone optimization with a burial-based scaling of electrostatics- **********
//*******************************************************************************************************

/////// Just specify a infile and outfile, it will optimize to a generally effective minimum.

#include "ensemble.h"
#include "PDBInterface.h"
#include <sstream>
#include <time.h>
int main (int argc, char* argv[])
{
	if (argc !=3)
	{   cout << "protOpt <inFile.pdb> <outFile.pdb>" << endl;
		exit(1); }

	string infile = argv[1];
	string outFile = argv[2];
	PDBInterface* thePDB = new PDBInterface(infile);
	ensemble* theEnsemble = thePDB->getEnsemblePointer();
	molecule* pMol = theEnsemble->getMoleculePointer(0);
	protein* _prot = static_cast<protein*>(pMol);
    bool homosymmetric = false;
    bool backbone = false;
    clock_t start, end;
	double cpu_time_used;
    
    residue::setTemperature(300);
    residue::setElectroSolvationScaleFactor(0.0);
    residue::setHydroSolvationScaleFactor(0.0);
	amberVDW::setScaleFactor(1.0);
	amberElec::setScaleFactor(1.0);
	residue::setPolarizableElec(true);

    UInt _frozenResidues[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34};
    UInt _activeChains[] = {0,2};
	UInt activeChainsSize = sizeof(_activeChains)/sizeof(_activeChains[0]), frozenResiduesSize = sizeof(_frozenResidues)/sizeof(_frozenResidues[0]);
	UIntVec activeChains, frozenResidues;
	for (UInt i = 0; i < activeChainsSize; i++)
	{
		activeChains.push_back(_activeChains[i]);
	}
	for (UInt i = 0; i < frozenResiduesSize; i++)
	{
		frozenResidues.push_back(_frozenResidues[i]);
    }
    if (homosymmetric)
    {
        for (UInt i = 0; i < _prot->getNumChains(); i++)
        {_prot->symmetryLinkChainAtoB(activeChains[0],i);}
    }
    cout << "start Energy: " << _prot->protEnergy() << endl;
    start = clock();
    //_prot->protOpt(backbone,0,15);
    //_prot->protOpt(backbone);
    _prot->protOpt(backbone, frozenResidues, activeChains);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    cout << "end Energy: "  << _prot->protEnergy() << " time: " << cpu_time_used << endl;
	pdbWriter(_prot, outFile);

	return 0;
}

