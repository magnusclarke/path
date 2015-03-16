#ifndef __TREE_H__
#define __TREE_H__

#include <vector>
using std::vector;


class Tree
{

public:
        int num_tips;                   // = length(speciators)+1 I think

	vector<double> intervals;		// Time periods between speciation events
	double total_time;
	vector<int> speciators;			// Order of splitting lines
	int num_traits;					// Number of traits = 2 for now.
};

#endif
