source('a2p.R')
if(.Platform$pkgType == "mac.binary")	dyn.load("cpp/Rfunc_mac.so")
if(.Platform$pkgType == "source")		dyn.load("cpp/Rfunc.so")

t = rUMT(5)
t = ape2peth(t)

run = function(tree, fmatrix=rep(1, 2^2), dt=0.1, rate=1, root=50)
{
	num_tips = length(tree$data_order)
	splitting_nodes = tree$splitting_nodes - 1 		# c counts from 0!!
	times = tree$times
	tval = rep(c(50, 50), num_tips)		# 1D vector


	result = .C ("pathsim", ntip=as.integer(num_tips), dt=as.double(dt), fmatrix=as.double(fmatrix),
				rate = as.double(rate), r_intervals=as.double(times), 
				splitters=as.integer(splitting_nodes), tval = as.double(tval)
				)


	# result = result$tval
	# result = result[data_order]

	return(result)

}

run(t)
