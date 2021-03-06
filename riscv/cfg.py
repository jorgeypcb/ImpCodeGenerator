from il_utils import *

def make_cfg(insFile,varsFile,printIns=False):    
    cfg={}
    #load all instructions
    instructions,varmap=load_il(insFile,varsFile)

    gen_ins=('Plus','Minus','Times','Greater','And','Or','GreaterEq','Equal','Not','Move')
    
    check_gen=lambda i,f: True if (f(i)!='null' and op(i) in gen_ins and f(i) in list(varmap)) else False
    check_kill=lambda i: True if (output(i)!='null' and output(i) in list(varmap)) else False
    gen=lambda b: set([varmap[i0(i)] for i in instructions_in_block(b,instructions) if check_gen(i,i0)]+ 
                           [varmap[i1(i)] for i in instructions_in_block(b,instructions) if check_gen(i,i1)])
    kill=lambda b: set([varmap[output(i)] for i in instructions_in_block(b,instructions) if check_kill(i)])
    
    def connect(i,x,edge_annotation=''): 
        assert i!=x,'self-connection detected'
        if block(i) not in cfg: cfg[block(i)]={'succ':[],'succ_labels':[]}
#         if x not in [z[0] for z in cfg[block(i)]]:
        cfg[block(i)]['succ'].append(x)
        cfg[block(i)]['succ_labels'].append(edge_annotation)
        
    if printIns: print_ins(instructions)

    #main loop
    for ni,i in enumerate(instructions[:-1]):
        next_i=instructions[ni+1]
        if ni>0: prev_i=instructions[ni-1]
        if op(i) == 'Jump': connect(i,jumpto_uncond(i,instructions),edge_annotation=' j ')
        if op(i) in ('JumpIfNonzero','JumpIfZero'): 
            connect(i,jumpto_cond(i,instructions),edge_annotation=' if ')
            connect(i,block(next_i),edge_annotation=' else ')
        if (block(i) != block(next_i)) and (op(i)[:4]!='Jump'): connect(i,block(next_i))
    
    for i in set([block(j) for j in instructions]):
        if i not in cfg:cfg[i]={'succ':[],'succ_labels':[]}
        cfg[i]['gen']=gen(i)
        cfg[i]['kill']=kill(i)

    return cfg
