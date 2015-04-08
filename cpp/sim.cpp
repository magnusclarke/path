#include <vector>
#include <cmath>
#include <random>
#include "sim.h"
#include "tree.h"

using std::vector;

/* 	RNG. Declaring this inside the looped function would cost the earth in time. */
std::random_device rd;
std::mt19937 gen(rd());
std::normal_distribution<> d(0,1);	// concievably faster to gen all rnums at outset, then use.
std::uniform_real_distribution<> ud(0, 1);	// random number from uniform dist between 0 and 1.

/* 	Modify trait values and fitness mapfor segment between speciation
	events. Needs number of time steps within that segment. 	
	This is where all the runtime is spent!	 */
void Sim::evolve_segment(int &nsteps)
{
	for (int step = 0; step < nsteps; ++step)
	{
		step_segment();
	}
}

/* 	Update trait values and fitness map for one time step. 	*/
void Sim::step_segment()
{
	int num_species = tval.size();
	for (int species = 0; species < num_species; ++species)
	{
		step_species(species);
	}
	step_map();		// Update fitness map
}

/* 	Do one evolutionary step on one species  */
void Sim::step_species(int &species)
{

	/* Create a trial evolutionary step, and accept with likelihood
	proportional to fitness of new traits. */
	bool accept = false;

	while(accept==false)
	{
		accept = false;
		vector<double> change;		// change is a vector of two doubles; n doubles for n traits.

		change.assign(2, 1);	
		change[0] = d(gen);		// random number drawn from normal distribution
		change[1] = d(gen);
		vector<double> new_state;
		new_state.assign(2, 1);	
		new_state[0] = tval[species][0] + change[0];
		new_state[1] = tval[species][1] + change[1];

		// Accept only if new state is within fitness matrix boundries
		if (new_state[0] > -1 && new_state[1] > -1 && new_state[0] < fitness_size && new_state[1] < fitness_size)
		{
			int ns0 = new_state[0];
			int ns1 = new_state[1];
			double new_fitness = fitness[ns0][ns1];		// fn to change fitness. Currently no change.

			// Accept new state with likelihood = fitness
			if(new_fitness > ud(gen))
			{
				accept = true;
				tval[species] = new_state;
			}
		}
	}	
}

/*	Update fitness map. For BM the fitness map has every 
element equal to 1, and doesn't change. */
void Sim::step_map()
{
}

/*	Perform simulation on whole tree, modifying object variable tvals, 
	and lengthening tvals to length = number tree tips. A vector of 
	two doubles is appended to tval per speciation event  	*/
void Sim::path()
{
	for (int i = 0; i < num_segment; ++i)
	{
		int x = tree.speciators[i];
		tval.push_back(tval[x]);
		evolve_segment(segment_steps[i]);
	}
}

// Doesn't use up runtime!!
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

	// Make square 'fitness' matrix with std::vector from linear array from R
	for(int i=0; i<fitness_size; ++i)
	{
		for(int j=0; j<fitness_size; ++j)
		{
			fitness[i][j] = fmatrix[(fitness_size*i)+j];
		}
	}	
}
