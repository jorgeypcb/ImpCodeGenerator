#Generic fixed point iterator
#The iteration function must return its own initial data 
#structure if given 'init_data=None', otherwise it will 
#take the initial data and return the next iteration on 
#that data until no changes are made. All iterations are 
#saved in a dictionary labeled by the iteration number.

import copy
def fixed_point_iteration(iteration_function,**kwargs):
    init_data=iteration_function(init_data=None,**kwargs)
    iteration_data={0:init_data}
    iteration=1
    iteration_data[iteration]=iteration_function(init_data,**kwargs)
    while iteration_data[iteration]!=iteration_data[iteration-1]:
        iteration+=1
        iteration_data[iteration]=iteration_function(iteration_data[iteration-1],**kwargs)
    return iteration_data