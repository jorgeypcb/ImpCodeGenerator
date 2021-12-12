from cfg import*
from il_utils import*
from fixed_point_iterator import*
from dead_code_elimination import*
import copy
import sys

args=sys.argv[1:]
insname,varsname=args
cfg=make_cfg(insname,varsname)
instructions,varmap=load_il(insname,varsname)  

cleanup_iter=fixed_point_iteration(cleanup,init_instr=instructions,varmap=varmap)
for i in cleanup_iter:
    print('\n',i,'\n')
    print_ins(cleanup_iter[i])
    print('\n----- len',len(cleanup_iter[i]),'-----\n')
final=cleanup_iter[list(cleanup_iter)[-1]]
print('\nfinal len',len(final))
print('\n',varmap)