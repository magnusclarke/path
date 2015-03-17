#include <math.h>
#include <vector>

#include "tree.h"
#include "sim.h"

extern "C" void pathsim(int &ntip, double &dt, double fmatrix[], double &rate, double r_intervals[], int splitters[], double tval[])
{
	//--------- INITIALISE TREE --------------------//
	Tree tree;
	tree.num_traits = 2;		// two traits for now
	tree.total_time = 0;
	int n_interval = ntip - 1;
	tree.speciators.assign(n_interval, 0);
	// tree.intervals.assign(n_interval, 0.0);
	for(int i=0; i<n_interval; ++i)
	{
		tree.total_time += r_intervals[i];
		tree.speciators[i] = splitters[i];	//int
		// tree.intervals[i] = r_intervals[i];	//double
	}
	//----------------------------------------------//


	//--------- INITIALISE SIM ---------------------//
	Sim sim;
	int fsize = sqrt(sizeof(fmatrix));
	sim.set_values(dt, rate, fsize, fmatrix, tree);		
	sim.path();
	//----------------------------------------------//
	
	//---------- RETURN VALS TO R ------------------//
	for (int i = 0; i < ntip; ++i)
	{
		tval[(2*i)] 	= sim.tval[i][0];
		tval[(2*i)+1] 	= sim.tval[i][1];

	}
	//----------------------------------------------//

}
