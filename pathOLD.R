library(gplots)

bits = 100
limit = bits/2
offset = bits / 2 				# Put zero in centre of plot

rate = bits/75


gen_species = function(n)
{
	lspecies = list()
	for(i in 1:n)
	{
		species = list(c(0,0)) 		#list(sample(1:5, 2))  
		new_lspecies = append(lspecies, species)
		lspecies <- new_lspecies
	}
	return(lspecies)
}

x = matrix(0, nrow=bits, ncol=bits)

# Evolve a species randomly (BM)
step_species = function(lspecies)
{
	# for(i in 1:length(lspecies[,1]))
	# {
		# lspecies[i,] = lspecies[i,] + as.integer(rate*rnorm(2))
	# }
	lspecies = lspecies + rnorm(length(lspecies))
	return(lspecies)
}

step_map = function(x, lspecies)
{
	# x = matrix(0, nrow=bits, ncol=bits)
	# Only accept evolution if it is within grid bounds
	for(species in which(abs(lspecies[,1])<limit & abs(lspecies[,2]<limit)))		#1:length(lspecies[,1]))
	{
		x[lspecies[species,1]+offset, lspecies[species,2]+offset] = 1
	}
	return(x)
}

vis = function(x) 
{
	dev.hold() 	# wait until plot is computed before plotting (avoid flicker)
	par(bg = "grey28")
	heatmap.2(x, dendrogram='none', Rowv=F, Colv=F, col=c('grey28', 'grey'), 
		trace='none', key=F, lwid = c(1,10), lhei = c(1,10),
		labRow=rep('', bits), labCol=rep('', bits), margins=c(3, 3))
	dev.flush()
	Sys.sleep(0.1) # wait 
}

# Take a set of species and evolve them for one step
evolve = function(lspecies, visualise=TRUE)
{
	lspecies = step_species(lspecies)
	x = step_map(x, lspecies)
	if(visualise==TRUE)	vis(x)
	return(lspecies)
}

#for(i in 1:10) evolve()

# Simulate the random evolution of a bunch of randomly positioned species
sim_run = function(nspecies, ntime)
{
	lspecies = gen_species(nspecies)
	for(i in 1:ntime)		lspecies = evolve(lspecies)
	return(lspecies)
}

path = function(lspecies, time, visualise=FALSE) 
{
	dt = 0.01
	time_steps = as.integer(time / dt)
	for (i in 1:time_steps) 
	{
		lspecies = evolve(lspecies, visualise=visualise)
	}
	return(lspecies)
}

