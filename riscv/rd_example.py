from il_utils import*
from cfg import*
from fixed_point_iterator import*
from reaching_definitions import*
import copy
import sys

args=sys.argv[1:]

insFile,varsFile=args
cfg=make_cfg(insFile,varsFile)
instructions,varmap=load_il(insFile,varsFile)
allvars=list(varmap)
iteration_function=iterate_reaching_definitions
rd_iterations= fixed_point_iteration(iteration_function,cfg=cfg,allvars=allvars)
for i in rd_iterations:
    print('\n-----',i,'-----')
    print('\nentry\n')
    for j in rd_iterations[i]['rd_entry']:
        print(j,'\t',str([k for k in rd_iterations[i]['rd_entry'][j]]).strip('[]'))
    print('\nexit\n')
    for j in rd_iterations[i]['rd_exit']:
        print(j,'\t',str([k for k in rd_iterations[i]['rd_exit'][j]]).strip('[]'))
