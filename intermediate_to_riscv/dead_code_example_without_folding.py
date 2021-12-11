from cfg import*
from il_utils import*
from fixed_point_iterator import*
from dead_code_elimination import*
import copy

name='../programs/dead-code-test-2'
cfg=make_cfg(name)
instructions,varmap=load_il(name)  

cleanup_iter=fixed_point_iteration(cleanup,init_instr=instructions,varmap=varmap)
for i in cleanup_iter:
    print('\n',i,'\n')
    print_ins(cleanup_iter[i])
    print('\n----- len',len(cleanup_iter[i]),'-----\n')
final=cleanup_iter[list(cleanup_iter)[-1]]
print('\nfinal len',len(final))
print('\n',varmap)