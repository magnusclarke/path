#include "sim.h"
#include "tree.h"
#include <vector>
#include <cmath>
#include <random>

using std::vector;

/* evolve_segment: matches path in path.R.
Modifies tvals as per single segment. 
Needs number of time steps for that segment */
void Sim::evolve_segment(int &nsteps)
{
	for (int step = 0; step < nsteps; ++step)
	{
		step_segment();
	}
}

/* step_segment: matches evolve fn in path.R
Updates tval, AND fitness_map */
void Sim::step_segment()
{
	int x = tval.size();




	// for (int species = 0; species < x-25; ++species)
	for (int species = 0; species < x; ++species)
	{
		step_species(species);
	}
	step_map();		// Update fitness map
}

/* step_species: matches step_traits function in path.R
do evolutionary step on one species, accepting based on 
fitness matrix. */
void Sim::step_species(int &species)
{
	/* Create a trial evolutionary step, and accept with likelihood
	proportional to fitness of new traits. */
	bool accept = false;
	while(accept==false)
	{
		accept = false;
		// change is a vector of two doubles, each 1 for now. 
		// They should be drawn from a normal dist!!
		vector<double> change;

		change.assign(2, 1);						// should be = rate * rnorm(2);
		change[0] = rand() / 500000000;		// random uniform-ish between 0 and 1 ish
		change[1] = rand() / 500000000;		// should change: rand() is considered harmful
		vector<double> new_state;
		new_state.assign(2, 1);	
		new_state[0] = tval[species][0] + change[0];
		new_state[1] = tval[species][1] + change[1];

		// tval[species] = new_state;
		// accept = true;

		// Accept only if new state is within fitness matrix boundries
		if (new_state[0] > -1 && new_state[1] > -1 && new_state[0] < fitness_size && new_state[1] < fitness_size)
		// if (new_state[0] > -1 && new_state[1] > -1 && new_state[0] < 100 && new_state[1] < 100)
		{
			// double new_fitness = fitness[new_state[0]][new_state[1]];
			double new_fitness = 1;

			// Accept with likelihood = fitness
			// Temporarily likelihood = 1. Should be if(new_fitness > runif(1)).
			if(new_fitness > 0)
			{
				accept = true;
			// }
			// if(accept==true)
			// {
				tval[species] = new_state;
			}
		}
	}	
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
	// tval[0][0] = tval.size();

	// append to tval: one vector of two elements, per speciation event
	for (int i = 0; i < num_segment; ++i)
	{
		int x = tree.speciators[i];

		tval.push_back(tval[x]);	// add to tvals a copy of the currently splitting species
		evolve_segment(segment_steps[i]);
	}


}

void Sim::set_values(double &r_dt, double &r_rate, int &fsize, double fmatrix[], double r_intervals[], Tree &t)
{
	tree = t;
	num_segment = tree.num_tips-1;

	fitness_size = fsize;
	dt = r_dt;
	rate = r_rate;
	total_time_steps = tree.total_time / dt;

	// Compute number of time steps for each segment
	segment_steps.assign(num_segment, 0);
	for (int i = 0; i < num_segment; ++i)
	{
		segment_steps[i] = r_intervals[i] / dt;
	}

	// Root trait value is in middle of fitness surface; set all to this
	double root = fitness_size / 2;
	vector<double> root_species;
	root_species.assign(2, root);		// two traits
	tval.assign(1, root_species);		// initially one species

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
