import copy
from il_utils import*

def cleanup(instructions_=None,**kwargs):
    if instructions_ is None:return kwargs['init_instr']
    instructions=copy.deepcopy(instructions_)
    varmap=kwargs['varmap']
    instructions=compress_constant_assignments(instructions,varmap)
    instructions=remove_unused_temporary_assignments(instructions,varmap)
    return instructions
  
def compress_constant_assignments(instructions,varmap):
    delete=[]
    for ni,i in enumerate(instructions[:-1]):
        next_i=instructions[ni+1]
        if (op(i),op(next_i))==('LoadConstant','Move'):
            if output(i)==i0(next_i):
                instructions[ni+1]=['LoadConstant',i0(i),'null',output(next_i),block(next_i)]
                if output(i) not in list(varmap): delete.append(ni)
    return [instructions[i] for i in range(len(instructions)) if i not in delete]

def remove_unused_temporary_assignments(instructions,varmap):
    def keep(idx):
        ins=instructions[:idx]+instructions[idx+1:]
        return output(instructions[idx]) in [i0(i) for i in ins if op(i)!='LoadConstant']+list(varmap)
    delete=[]
    for ni,i in enumerate(instructions):
        if op(i)=='LoadConstant':
            if not keep(ni): delete.append(ni)
    return [instructions[i] for i in range(len(instructions)) if i not in delete]