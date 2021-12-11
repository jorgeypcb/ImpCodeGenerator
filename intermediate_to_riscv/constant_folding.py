plus=lambda a,b: int(a)+int(b)
minus=lambda a,b: int(a)-int(b)
times=lambda a,b: int(a)*int(b)
greater=lambda a,b: 1 if int(a)>int(b) else 0
greater_eq=lambda a,b: 1 if int(a)>=int(b) else 0
equal=lambda a,b: 1 if int(a)==int(b) else 0
calculate={'Plus':plus,'Minus':minus,'Times':times,'Greater':greater,'GreaterEq':greater_eq,'Equal':equal}

binary_arithmetic_functions=('Plus','Minus','Times','Greater','GreaterEq','Equal')
def fold(instructions_=None,**kwargs):
    instructions=copy.deepcopy(instructions_)
    cfg=kwargs['cfg']
    rd=kwargs['rd']
    program_name=kwargs['program_name']
    _,varmap=load_il(program_name)
    if instructions is None: return _
    stackmap={varmap[i]:i for i in varmap}
    labels=list(cfg)
    e=rd['rd_entry']
    e_dict= lambda l: {i[0]:i[1] for i in rd['rd_entry'][l]}
    undefined_in_l=lambda v,l: True if (v,'?') in e[l] else False
    v_in_e_l=lambda v, l: e_dict(l)[v] if v in [i[0] for i in e[l]] else False
    for l in labels:
        foldval={v:const(stackmap[v],v_in_e_l(v,l),instructions) for v in list(stackmap)}
#         print('foldval',foldval,'\n---\n')
        foldable= lambda v, l: ((not undefined_in_l(v,l)) and foldval[v]) if v else False
        blockstart=instruction_number_of_label(instructions,l)
        ins=instructions_in_block(l,instructions)
        for ni,i in enumerate(ins):
            op_instruction=blockstart+ni
#             print('i0(i)',i0(i),'\nstackmap',stackmap,'\nv0_in_stackmap',i0(i)in list(varmap))
            v0= varmap[i0(i)] if (i0(i) in list(varmap)) else False
            v1= varmap[i1(i)] if (i1(i) in list(varmap)) else False
            t0= temp_const_val(i0(i),l,instructions,op_instruction) if (not v0) and i0(i)!='null' else False
            t1= temp_const_val(i1(i),l,instructions,op_instruction) if (not v1) and i1(i)!='null' else False
            v0_foldable = foldable(v0,l)
            v1_foldable = foldable(v1,l)
#             print(i,'\nv0',v0,'\nv1',v1,'\nt0',t0,'\nt1',t1,'\ntempConstVal(t1)',temp_const_val(i1(i),l,instructions,op_instruction),'\ni1(i)',i1(i),'\nv0_foldable',v0_foldable,'\nv1_foldable',v1_foldable,'\nv0_not_undefined_in_l',(not undefined_in_l(v0,l)),'\n----\n')
            if op(i)=='Move' and v0_foldable:
                return fold_var(instructions,op_instruction,foldval[v0][0])
            if op(i) in binary_arithmetic_functions: 
                if t0 and t1:
#                     print('\nconstconst\n')
                    return fold_const_const(instructions,[t0[1],t1[1],op_instruction],delete=True)
                if (t0 and v1_foldable):
                    return fold_var_const(instructions,[t0[1],op_instruction],foldval[v1][0],delete=True)
                if (v0_foldable and t1):
                    return fold_var_const(instructions,[t1[1],op_instruction],foldval[v0][0],delete=True)
                if (v0_foldable and v1_foldable):
                    return fold_var_var(instructions,op_instruction,stackmap[v0],foldval[v0][0],stackmap[v1],foldval[v1][0])
    return instructions
    
def fold_var(instructions,line,varval):
#     print('\n-----\n','FOLDVAR\n','line',line,'\nvarval',varval)
    ins=instructions[line]
    new_ins=['LoadConstant',str(varval),'null',output(ins),block(ins)]
    instructions[line]=new_ins
    return instructions
                                
def fold_var_const(instructions,lines,varval,delete=False):
    const_def,operation=[instructions[i] for i in lines]
    assert op(const_def)=='LoadConstant'
    assert op(operation) in binary_arithmetic_functions
    constval=i0(const_def)
    if output(const_def)==i0(operation):
        operands=constval,varval
    elif output(const_def)==i1(operation):
        operands=varval,constval
#     print('\n----FOLD_VAR_CONST----\n')
#     print('op(operation)',op(operation),'operands[0]',operands[0],'operands[1]',operands[1],'\n')    
    folded_val = calculate[op(operation)](operands[0],operands[1])
    new_ins = ['LoadConstant',str(folded_val),'null',output(operation),block(operation)] #replace operation with loadconstant
    instructions[lines[-1]]=new_ins
#     print(new_ins)
#     print(instructions[lines[-1]])
    if delete: return [instructions[i] for i in range(len(instructions)) if i not in (lines[:-1])]
    return instructions

def fold_var_var(instructions,line,var1_stack_location,var1_val,var2_stack_location,var2_val):
    ins = instructions[line]
    assert var1_stack_location in [i0(ins),i1(ins)]
    assert var2_stack_location in [i0(ins),i1(ins)]
    if var1_stack_location == i0(ins):
        operands = var1_val,var2_val
    elif var1_stack_location == i1(ins):
        operands = var2_val,var1_val
    folded_val=calculate[op(ins)](operands[0],operands[1])
    new_ins = ['LoadConstant',str(folded_val),'null',output(ins),block(ins)]
    instructions[line]=new_ins
#     print('\n----FOLD_VAR_VAR----\n')
#     print(new_ins)
#     print(instructions[line])
#     print(instructions,'*****\n')
    return instructions

def fold_const_const(instructions,lines,delete=False):
    const1_def,const2_def,operation=[instructions[i] for i in lines]
    assert all([op(i)=='LoadConstant' for i in (const1_def,const2_def)])
    assert op(operation) in binary_arithmetic_functions
    if output(const1_def)==i0(operation):
        operands=i0(const1_def),i0(const2_def)
    elif output(const1_def)==i1(operation):
        operands=i0(const2_def),i0(const1_def)
#     print(lines)
#     print(const1_def)
#     print(const2_def)
    folded_val=calculate[op(operation)](operands[0],operands[1])
    new_ins = ['LoadConstant',str(folded_val),'null',output(operation),block(operation)]
    instructions[lines[-1]]=new_ins
#     print('\n----FOLD_CONST_CONST----')
#     print(new_ins)
#     print(instructions[lines[-1]])
#     print(instructions,'****\n')
    if delete: return[instructions[i] for i in range(len(instructions)) if i not in lines[:-1]]
    return instructions