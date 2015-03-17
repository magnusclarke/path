#include "sim.h"
#include "tree.h"
#include <vector>
#include <cmath>

using std::vector;

/* evolve_segment: matches path in path.R.
Modifies tvals as per single segment. 
Needs number of time steps for that segment */
void Sim::evolve_segment(int &nsteps)
{
}

/* step_segment: matches evolve fn in path.R
Updates tval, AND fitness_map */
void Sim::step_segment()
{
}

/* step_species: matches step_traits function in path.R
do evolutionary step on one species, accepting based on 
fitness matrix. */
void Sim::step_species()
{
}

/* Update fitness map. For BM the fitness map has every 
element equal to 1, and doesn't change. */
void Sim::step_map()
{
}

/* path: use class vars, change tval; effect is whole clade
Corresponds to sim function in a2p.R */
void Sim::path()
{
	// append to tval: one vector of two elements, per speciation event
	// for (int i = 0; i < num_tips+1; ++i)
	// {
	// 	vector<double> new_species;
	// 	new_species.assign(2, 5);		// two traits, value=5 in example
	// 	tval.push_back(new_species);
	// }

	int num_internal_nodes = num_tips - 1;

	for (int i = 0; i < num_internal_nodes-2; ++i)
	{
		int splitting_node = speciators[i];
		// tval.push_back(tval[splitting_node]);	//segfaults

		tval.push_back(tval[0]);				// works
	}




	for (int i = 0; i < num_internal_nodes; ++i)
	{
		// std::vector<int> test;
		// test.assign(num_internal_nodes, 0);
		// int splitting_node = test[i];
		// tval.push_back(tval[splitting_node]);	


		// tval.push_back(tval[0]);				// works

		// evolve_segment(segment_steps[i]);
	}



}

void Sim::set_values(double &r_dt, double &r_rate, int &fsize, double fmatrix[], Tree &tre)
{
	num_tips = tre.num_tips;
	// num_segment = num_tips-1;

	speciators = tre.speciators;

	// tree = tre;

	fitness_size = fsize;
	dt = r_dt;
	rate = r_rate;
	total_time_steps = tre.total_time / dt;

	// Compute number of time steps for each segment
	// segment_steps.assign(num_segment, 0);
	// for (int i = 0; i < num_segment; ++i)
	// {
	// 	segment_steps[i] = tre.intervals[i] / dt;
	// }

	// Root trait value is in middle of fitness surface; set all to this
	double root = fitness_size / 2;
	vector<double> root_species;
	root_species.assign(2, root);		// two traits
	tval.assign(1, root_species);	// initially one species

	// Get fitness matrix correct size
	vector<double> fitness_row;
	fitness_row.assign(fsize, 1);
	fitness.assign(fsize, fitness_row);

	// Make square matrix with std::vector from linear array from R
	for(int i=0; i<fitness_size; ++i)
	{
		for(int j=0; j<fitness_size; ++j)
		{
			fitness[i][j] = fmatrix[(fitness_size*i)+j];
		}
	}	
}
