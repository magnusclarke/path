dir=getwd()
source('../a2p.R', chdir=TRUE)
setwd(dir)
dyn.load("Rfunc.so")

t = rUMT(5)
t = ape2peth(t)

run = function(tree, fmatrix=rep(1, 100^2), dt=0.1, rate=1)
{
	splitting_nodes = tree$splitting_nodes
	times = tree$times


	result = .C ("pathsim", ntip=as.integer(ntip), dt=as.double(dt), fmatrix=as.double(fmatrix),
				rate = as.double(rate), r_intervals=as.double(times), 
				splitters=as.integer(splitting_nodes), tval = as.double(c(1,2,1,2,1,2))
				)

	result = result$tval
	result = result[data_order]

	return(result)

}
