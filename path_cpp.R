source('a2p.R')
if(.Platform$pkgType == "mac.binary")	dyn.load("cpp/Rfunc_mac.so")
if(.Platform$pkgType == "source")		dyn.load("cpp/Rfunc.so")

tre = rUMT(5)
t = ape2peth(tre)

# Use consistent tree
# t$times = c(1.686388710, 0.175231621, 0.535529443, 0.008047728)
# t$splitting_nodes = c(1,1,1,2)

# The main speed constraint is in 
# the time to copy the fitness matrix for each simulation.
# This needs to be stored in C++ and kept, not copied, 
# between simulations.
run = function(tree, fmatrix=rep(1, 20^2), dt=0.1, rate=1)
{
	num_tips = length(tree$data_order)
	splitting_nodes = tree$splitting_nodes - 1 		# c counts from 0!!
	times = tree$times
	tval = rep(c(5, 5), num_tips)					# 1D vector
	fsize = sqrt(length(fmatrix))


	result = .C ("pathsim", ntip=as.integer(num_tips), dt=as.double(dt), 
				fsize = as.integer(fsize), fmatrix=as.double(fmatrix),
				rate = as.double(rate), r_intervals=as.double(times), 
				splitters=as.integer(splitting_nodes), tval = as.double(tval)
				)

	result$tval = matrix(result$tval, ncol=2, byrow=TRUE)
	result$tval = result$tval[tree$data_order,]

	return(result)
}

# system.time(replicate(1e3, run(t, dt=0.01)))

# print(run(t))

reps=1e4
x	= t( replicate(reps, run(t)$tval[,1]))
print( cor(x) )
print( cov2cor(vcv.phylo(tre)) )
