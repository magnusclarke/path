library(ape)
library(TESS)
source('path.R')

# random ultrametric tree
rUMT	= function(nt, lambda=1, mu=0)
{
	tree 	= sim.globalBiDe.taxa(n=1, nTaxa=nt, lambda=lambda, mu=mu)	
	return( tree[[1]] )
}

# Constructor for peth tree class
pethtree = function(map, dord, nts, tts) 
{
	out = list(map, dord, nts, tts)
	class(out) = 'pethtree'
	names(out) = c('map', 'data_order', 'splitting_nodes', 'times')
	invisible(out)
}

# Convert a tree in ape format to one in peth format (return a pethtree object)
# Bug: only works for ntip > 4
ape2peth = function(tree)
{
	edge = tree$edge
	edge.length = tree$edge.length

	nedge = length(edge[,1])
	ntip = (nedge+2)/2

	# Set up ape-to-peth map and vector to keep track of running branches.
	running = rep(FALSE, 2*ntip-2)
	map = matrix(ncol=2, nrow=nedge+1)

	# Assign root
	root = edge[1,1]
	map[1,1] = root
	map[1,2] = 1

	# splitting gives the NODE, as labelled in edge, which is speciating
	splitting = root
	# splitting_row = 1		# row of map that is splitting
	# running[ which(edge[,1]==root) ] = TRUE

	edge_counter = 1

	# Keep track of how many nodes we've already assigned.
	node_counter = 1

	# Produce vector of nodes to split for future simulations (peth numbering)
	nts = c()

	# Produce vector of times between speciation events
	tts = c()

	for(i in seq(2,2*(ntip-1), by=2))
	{
		# 'splitting' is the splitting node in ape format
		# 'peth_split' is the splitting node in peth format
		# There SHOULD only be one match in map[,1] (each element is unique)
		peth_split = map[ which(map[,1] == splitting), 2 ][1]
		nts = c(nts, peth_split)

		# The branches coming from this node are now running
		running[ which(edge[,1]==splitting) ] = TRUE
		running[ which(edge[,2]==splitting) ] = FALSE

		# We're at a speciation event. Find out which nodes come from it.
		new_edges = which(edge[,1] == splitting)
		new_nodes = edge[new_edges, 2]

		# Get which of the new nodes comes from the shortest branch
		# We want this one to inherit the label-number of its parent.
		new_node1 = new_nodes[order(edge.length[new_edges])[1]]
		new_node2 = new_nodes[order(edge.length[new_edges])[2]]

		# First new node keeps ancestor number (splitting node), 
		# other new node takes node_counter+1
		map[i,1] = new_node1
		map[i,2] = peth_split
		map[i+1,1] = new_node2
		map[i+1,2] = node_counter + 1

		node_counter = node_counter + 1

		# time_to_split = time to next split = shortest branch running.
		time_to_split = min(edge.length[running])
		tts = c(tts, time_to_split)

		# Subtract time to next split from running branches.
		edge.length[running] = edge.length[running] - time_to_split

		# Which node splits next? 
		# Of the running branches, take the shortest one and then the splitting node 
		# is the second elemtn of edge for that branch (i.e. the child)
		el_temp = edge.length
		el_temp[!running] = 9e99
		splitting = edge[ which.min(el_temp),2 ]
	}
	
	# Need to know what order simulated data will be in, relative to ape's expectatons
	ordered_map = order(map[,1])
	want = ordered_map[1:ntip]
	dord=rev(map[want,2])

	# Need to know what order simulated data will be in, relative to ape's expectatons
	# This is given by working from bottom up through map
	dord = 1:ntip
	for(i in 1:ntip)
	{
		want = which(map[,1]==i)
		dord[i] = map[want , 2]
	}

	ptree = pethtree(map, dord, nts, tts)
	return(ptree)
}

# BM simulation on a peth tree; returns data with ape ordering
# Deprecated. Use sim(tree, bm)
sim_bm = function(tree, sigma=1) 
{
	dat = 0				# root trait value
	tts = tree$times 	# Time intervals beween speciation events
	nperiod = length(tts)
	dord = tree$data_order

	for(i in 1:nperiod)
	{
		dat = c(dat, dat[tree$splitting_nodes[i]])
		# Brownian evolution. We have to use sqrt(time) because the time-integral of
		# a Wiener process is a Gaussian with variance = time. But variance has units
		# squared, so a draw from the distribution should be scaled by sqrt(time)
		dat = unlist(lapply(dat, function(x) x = x + (sqrt(tts[i])*sigma*rnorm(1))) )
	}
	return(dat[dord])
}

bm = function(dat, sigma=1, time)
{
	return( (lapply(dat, function(x) x = x + (sqrt(time)*sigma*rnorm(1))) ) )
}

# Simulate data, given tree in peth format, according to arbitrary evolution function
# Evolution function must take a vector of length n and 
# the time period to simulate, and return a vector of length n
# It will be more efficient in time to make the fitness matrix only once for the whole
# tree evolution. That'll mean integrating this fn with the actual path fn.
sim = function(tree, FUN, visualise=FALSE, root=50)
{
	if(identical(FUN, bm))
	{
		dat = root					# root trait value
		nTraits=1				
	} else if(identical(FUN, path)){
		dat = list(c(root,root))		# root trait value
		nTraits=2
	}
	# if(identical(FUN, path))	dat = matrix(c(0,0), ncol=2)	# root trait values


	tts = tree$times 		# Time intervals beween speciation events
	nperiod = length(tts)
	dord = tree$data_order

	for(i in 1:nperiod)
	{
		dat = c(dat, dat[tree$splitting_nodes[i]])
		dat = FUN(dat, time=tts[i])#, visualise=visualise)
	}
	dat = dat[dord]
	dat = matrix( unlist(dat), ncol=nTraits, byrow=T )
	return(dat)
}








