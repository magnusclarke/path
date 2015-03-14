source('a2p.R')
source('path.R')

get_path = function(m) 
{
	dat = sim(m, path)

	return(dat[,1])
}

# Use random example
# apet 	= rUMT(5)
# t 		= ape2peth(apet)

# Use written example
ntip=5
t = rUMT(5)
t$edge = matrix(c(6,9,9,1,9,2,6,7,7,3,7,8,8,4,8,5), ncol=2, byrow=TRUE)
t$edge.length = c(3.5,0.1,0.1,1.2,2.4,0.8,1.6,1.6)
t$tip.label = c('t3','t1','t5','t2','t4')
m = (ape2peth(t))


# Get VCV representation of tree using ape
ape_cor = cov2cor(vcv(t))

# Get VCV matrix of many reps of bm simulations on tree
d = replicate(1e2, sim(m, bm)[,1])

# Get vcv matrix of path simulations on tree
p = replicate(1e2, get_path(m))

peth_cor = cor(t(d))
path_cor = cor(t(p))

diff = abs(ape_cor - peth_cor)
pdiff = abs(ape_cor - path_cor)

# Print summed absolute differences between correct vcv, simulated vcv and randomised vcv
# First should be small, second should be large!
print(sum(diff))
print(sum(pdiff))

