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
	// Do something
	// append to tval: one vector of two elements, per speciation event
	for (int i = 0; i < num_tips+1; ++i)
	{
		vector<double> new_species;
		new_species.assign(2, 5);		// two traits, value=5 in example
		tval.push_back(new_species);
	}


	// dt = 5 * dt;
}

void Sim::set_values(double &r_dt, double &r_rate, int &fsize, double fmatrix[], Tree &tre)
{
	num_tips = tre.num_tips;

	fitness_size = fsize;
	dt = r_dt;
	rate = r_rate;
	total_time_steps = tre.total_time / dt;

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
