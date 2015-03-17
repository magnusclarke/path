#ifndef __SIM_H__
#define __SIM_H__

#include <vector>
#include "tree.h"	

using std::vector;

class Sim
{
	// Tree tree;
	int num_tips;
	// int num_segment;
	// vector<int> segment_steps; 		// number time steps in each segment
	vector<int> speciators;
	double dt;	
	double rate; 						// BM sigma^2
	int total_time_steps;				// total time / dt for clade	
	int fitness_size;					// size of 1 dim of fitness matrix
	vector<vector<double> > fitness;	// fitness matrix ; bm : all=1

	/* evolve_segment: matches path in path.R.
	Modifies tvals as per single segment. 
	Needs number of time steps for that segment*/
	void evolve_segment(int&);
	/* step_segment: matches evolve fn in path.R
	Updates tval, AND fitness_map */
	void step_segment();
	/* step_species: matches step_traits function in path.R
	do evolutionary step on one species, accepting based on 
	fitness matrix. */
	void step_species();
	/* Update fitness map. For BM the fitness map has every 
	element equal to 1, and doesn't change. */
	void step_map();
	
public:

	vector<vector<double> > tval;		// trait values, intially 1x1


	/* Take dt, rate, fitness matrix 1dim length, and intial fitness matrix as one
	linear array. Then need to split that array into square matrix */
	void set_values(double&, double&, int&, double[], Tree&); 
	/* path: use class vars, change tval; effect is whole clade
	Corresponds to sim function in a2p.R */
	void path();		

};

#endif
