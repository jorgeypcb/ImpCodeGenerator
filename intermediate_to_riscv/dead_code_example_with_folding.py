from cfg import*
from il_utils import*
from reaching_definitions import*
from fixed_point_iterator import*
from dead_code_elimination import*
from constant_folding import*
import copy

name='../programs/dead-code-test-2'
cfg=make_cfg(name)
instructions,varmap=load_il(name)  
fold_instructions=True

if fold_instructions:
    allvars=list(varmap)
    iteration_function=iterate_reaching_definitions
    rd_iterations= fixed_point_iteration(iteration_function,cfg=cfg,allvars=allvars)
    rd_final=rd_iterations[list(rd_iterations)[-1]]  
    instructions=fixed_point_iteration(fold,cfg=cfg,rd=rd_final,program_name=name)
    instructions=instructions[list(instructions)[-1]]

cleanup_iter=fixed_point_iteration(cleanup,init_instr=instructions,varmap=varmap)
for i in cleanup_iter:
    print('\n',i,'\n')
    print_ins(cleanup_iter[i])
    print('\n----- len',len(cleanup_iter[i]),'-----\n')
final=cleanup_iter[list(cleanup_iter)[-1]]
print('\nfinal len',len(final))
print('\n',varmap)