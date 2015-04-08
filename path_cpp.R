source('a2p.R')
if(.Platform$pkgType == "mac.binary")	dyn.load("cpp/Rfunc_mac.so")
if(.Platform$pkgType == "source")		dyn.load("cpp/Rfunc.so")

tre = rUMT(5)
t = ape2peth(tre)

# The main speed constraint is in 
# the time to copy the fitness matrix for each simulation.
# This needs to be stored in C++ and kept, not copied, 
# between simulations.
run = function(tree, fmatrix=rep(1, 100^2), dt=0.01, rate=1)
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

# reps=1e4
# x	= t( replicate(reps, run(t)$tval[,1]))
# print( cor(x) )
# print( cov2cor(vcv.phylo(tre)) )


# Show fitness landscape on average ---------------------------------#
fm = matrix(1, nrow=100, ncol=100)
fm[,45:50] = 0.4
fm = as.numeric(fm)


ntip = 5
tre = rUMT(ntip)
t = ape2peth(tre)

reps = 1e3
landscape = matrix(nrow=0, ncol=2)
for (i in 1:reps) 
{
	landscape = rbind(landscape, run(t, fmatrix=fm)$tval)
}

library(MASS)
den3d <- kde2d(landscape[,1], landscape[,2])
persp(den3d, box=FALSE)
#----------------------------------------------------------------------#





# Function to generate a posterior of trait fitness landscapes
get_landscape = function(tree, tdata, reps)
{
	# Cutoff is smallest of 100 runs
	cutoff = c()
	for (i in 1:250) 
	{
		fm = matrix(runif(100^2), nrow=100, ncol=100)
		new_data = run(t, fmatrix=fm)$tval
		dist = sum(abs(new_data - tdata))
		cutoff = c(cutoff, dist)
	}
	cutoff = min(cutoff)

	post=list()		# posterior
	post_index = 1
	for (i in 1:reps) 
	{
		fm = matrix(runif(100^2), nrow=100, ncol=100)
		new_data = run(t, fmatrix=fm)$tval
		dist = sum(abs(new_data - tdata))
		if(dist < cutoff)
		{
			post[[post_index]] = fm
			post_index = post_index + 1
		}
	}

	# Get element-wise mean of posterior landscape
	consensus = apply(simplify2array(post), 1:2, mean) 		# 1:2 means apply to every element!!

	return(consensus)
}

# Generate 'true' data
ntip = 20
tre = rUMT(ntip)
t = ape2peth(tre)
fm = matrix(runif(100^2), nrow=100, ncol=100)
tdata = run(t, fmatrix=fm)$tval
# Get posterior estimate
l_posterior = get_landscape(t, tdata, reps=1e5)


library(MASS)
par(mfrow=c(1,2))
	# Plot 'true' fitness landscape 
	den3d <- kde2d(fm[,1], fm[,2])
	persp(den3d, box=FALSE)

	# Plot posterior fitness landscape 
	den3d <- kde2d(l_posterior[,1], l_posterior[,2])
	persp(den3d, box=FALSE)

