from il_utils import*
from cfg import*
from fixed_point_iterator import*
from liveness_analysis import*
import copy

cfg=make_cfg('../programs/arith')
iteration_function=iterate_liveness
liveness_iterations= fixed_point_iteration(iteration_function,cfg=cfg)

for j in liveness_iterations:
    print('\n--------',j,'--------\n')
    print('in\n')
    for i in liveness_iterations[j]['in']:print(i,liveness_iterations[j]['in'][i])
    print('\nout\n')
    for i in liveness_iterations[j]['out']:print(i,liveness_iterations[j]['out'][i])