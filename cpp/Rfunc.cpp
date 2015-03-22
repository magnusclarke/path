#include <math.h>
#include <vector>
#include <random>
#include "tree.h"
#include "sim.h"

extern "C" void pathsim(int &ntip, double &dt, int &fsize, double fmatrix[], double &rate, double r_intervals[], int splitters[], double tval[])
{
	//--------- INITIALISE TREE --------------------//
	Tree tree;
	tree.num_tips = ntip;
	tree.total_time = 0;
	int n_interval = ntip - 1;
	tree.speciators.assign(n_interval, 0);
	for(int i=0; i<n_interval; ++i)
	{
		tree.total_time += r_intervals[i];
		tree.speciators[i] = splitters[i];	
	}
	//----------------------------------------------//


	//--------- INITIALISE SIM ---------------------//
	Sim sim;
	sim.set_values(dt, rate, fsize, fmatrix, r_intervals, tree);		
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
