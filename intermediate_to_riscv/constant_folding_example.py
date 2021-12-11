from il_utils import*
from cfg import*
from fixed_point_iterator import*
from reaching_definitions import*
from constant_folding import*
import copy

name='../programs/dead-code-test-2'
cfg=make_cfg(name)
_,varmap=load_il(name)
allvars=list(varmap)
iteration_function=iterate_reaching_definitions
rd_iterations= fixed_point_iteration(iteration_function,cfg=cfg,allvars=allvars)
rd_final=rd_iterations[list(rd_iterations)[-1]]    

instructions=fixed_point_iteration(fold,cfg=cfg,rd=rd_final,program_name=name)
for ni,i in enumerate(instructions):
    if ni==0: print('init len',len(instructions[i]),'\n')
    print('\n',i,'\n')
    print_ins(instructions[i])
    print('\n----------\n')
folded_instructions=instructions[list(instructions)[-1]]