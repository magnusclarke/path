#ifndef __SIM_H__
#define __SIM_H__

#include <vector>
#include "tree.h"	

using std::vector;

class Sim
{
	Tree tree;
	int num_segment;
	int total_time_steps;				// total time / dt for clade	
	int fitness_size;					// size of 1 dim of fitness matrix
	double dt;	
	double rate; 						// BM sigma^2
	vector<int> segment_steps; 			// number time steps in each segment
	vector<vector<double> > fitness;	// fitness matrix ; bm has all=1

	/* 	Modify trait values and fitness mapfor segment between speciation
		events. Needs number of time steps within that segment 	*/
	void evolve_segment(int&);

	/* 	Update trait values and fitness map for one time step. 	*/
	void step_segment();

	/* 	Do one evolutionary step on one species  
		This is where all the runtime is spent!	 */
	void step_species(int&);

	/*	Update fitness map. For BM the fitness map has every 
	element equal to 1, and doesn't change. */
	void step_map();
	
public:

	vector<vector<double> > tval;		// trait values, two per species

	/*	Take dt, rate, fitness matrix 1dim length, and intial fitness matrix as one
		linear array. Then need to split that array into square matrix	*/
	void set_values(double&, double&, int&, double[], double[], Tree&); 

	/*	Perform simulation on whole tree, modifying object variable tvals, 
		and lengthening tvals to length = number tree tips. A vector of 
		two doubles is appended to tval per speciation event  	*/
	void path();		
};

#endif
