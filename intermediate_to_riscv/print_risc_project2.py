import sys

run_imp = '''
.globl run_imp_actual

run_imp:
	addi    sp, sp, -2032
	sd      ra, 2024(sp)                    # 8-byte Folded
	sd      s0, 2016(sp)                    # 8-byte Folded
	lui     a1, 32
	addiw   a1, a1, -2000
	sub     sp, sp, a1
	mv      s0, a0
	addi    a0, sp, 16
	addi    a2, zero, 120
	mv      a1, s0
	call    memcpy@plt
	addi    a0, sp, 16
	call    run_imp_actual
	addi    a1, sp, 16
	addi    a2, zero, 120
	mv      a0, s0
	call    memcpy@plt
	lui     a0, 32
	addiw   a0, a0, -2000
	add     sp, sp, a0
	ld      s0, 2016(sp)                    # 8-byte Folded Reload
	ld      ra, 2024(sp)                    # 8-byte Folded Reload
	addi    sp, sp, 2032
	ret
'''

def print_riscv_instruction(instruction,register_allocation=False):
    op,i0,i1 = instruction[:3] #assigns the instruction components
    output = instruction[3].strip('\n\t')
    binaryops = {'Plus':'ADD','Minus':'SUB','Times':'MUL','Greater':'SGT','And':'AND','Or':'OR'}
    jumpops = {'JumpIfZero':'BEQZ','JumpIfNonzero':'BNEZ'}

    stack = lambda i : str(8*int(i)) + '(a0)' #converts stack index 
    load_stack = lambda i, a: "LD "+a+", "+ stack(i) + "\n\t"  #load ith value from stack to register a
    save_stack = lambda i, a: "SD "+a+", "+ stack(i) + "\n\t"  #save register a to ith index in stack
    binaryop = lambda out_reg, ai, aj, opname: opname + " " + out_reg + ", " + ai +", " + aj + "\n\t" #perform operation a1 op a2, put result in output register
    unaryop = lambda arg1, arg2, opname: opname + " " + arg1 + ", " + arg2 + '\n\t'
        
    if register_allocation:
        def check_variable1(var):
            register_numb = lambda i : str(int(i) + 1)
            if int(var) > 10:
                return load_stack(var, 'a1')
            else:
                current_register = "s"+register_numb(var)
                save_register = "LD " +"s"+register_numb(var)+ " " + stack(var) + "\n\t"
                return save_register

        def check_variable2(var):
            register_numb = lambda i : str(int(i) + 1)
            if int(var) > 10:
                return load_stack(var, 'a2')
            else:
                save_register = "LD " +"s"+register_numb(var)+ " " + stack(var) + "\n\t"
                return save_register

        def check_binary_op(var, operator):
            register1_numb = lambda i : str(int(i) + 1)
            register2_numb = lambda i : str(int(i) + 2)
            register3_numb = lambda i : str(int(i) + 3)
            if int(var) > 10:
                return binaryop('a1','a1','a2', operator)
            else:
                return binaryop("s"+register3_numb(var), "s"+register1_numb(var), "s"+register2_numb(var), operator)

        def check_unary_opGE(var, operator):
            register1_numb = lambda i : str(int(i) + 1)
            register2_numb = lambda i : str(int(i) + 3)
            if int(var) > 10:
                return unaryop('a3','a1',operator)
            else:
                return unaryop("s"+register2_numb(var), "s"+register1_numb(var), operator)

        def check_binary_opE(var):
            register1_numb = lambda i : str(int(i) + 1)
            register2_numb = lambda i : str(int(i) + 2)
            register3_numb = lambda i : str(int(i) + 3)
            register4_numb = lambda i : str(int(i) + 4)

            if int(var) > 10:
                instruction_set = binaryop('a3','a1','a2','SLT') + \
                binaryop('a4','a1','a2','SGT') + \
                binaryop('a2','a3','a4','XOR') + \
                unaryop('a1','a2','NOT')
                return instruction_set
            else:
                instruction_set = binaryop("s"+register3_numb(var),"s"+register1_numb(var),"s"+register2_numb(var),'SLT') + \
                binaryop("s"+register4_numb(var),"s"+register1_numb(var),"s"+register2_numb(var),'SGT') + \
                binaryop("s"+register2_numb(var),"s"+register3_numb(var),"s"+register4_numb(var),'XOR') + \
                unaryop("s"+register1_numb(var),"s"+register2_numb(var),'NOT')
                return instruction_set

        def check_unary_opNOT(var, operator):
            register1_numb = lambda i : str(int(i) + 1)
            if int(var) > 10:
                return unaryop('a1','a1',operator)
            else:
                return unaryop("s"+register1_numb(var), "s"+register1_numb(var), operator)

        def check_unary_opLI(var, operator):
            register1_numb = lambda i : str(int(i) + 1)
            if int(var) > 10:
                return unaryop('a1',var,operator)
            else:
                return unaryop("s"+register1_numb(var), var, operator)

        def store_output(var): 
            register_numb = lambda i : str(int(i) + 1)
            if int(var) > 10:
                return save_stack(var, 'a1')
            else:
                save_register = "SD " +"s"+register_numb(var)+ " " + stack(var) + "\n\t"
                return save_register

        if op in binaryops: 
            riscv = check_variable1(i0) + \
            check_variable2(i1) + \
            check_binary_op(i0, binaryops[op]) + \
            store_output(output)


        elif op in jumpops:
            riscv = check_variable1(i0) + \
            unaryop('a1',i1,jumpops[op])

        elif op == 'GreaterEq': #a3 = (a1 < a2), then a1 = not a3, save a1 to stack output
            riscv = check_variable1(i0) + \
            check_variable2(i1) + \
            check_binary_op(i0, 'SLT') + \
            check_unary_opGE(i0, 'NOT') + \
            store_output(output)

        elif op == 'Equal': #a3 = (a1 < a2), a4 = (a1 > a2), a2 = (a3 xor a4), a1 = not a2, save a1 to stack output
            riscv = check_variable1(i0) + \
            check_variable2(i1) + \
            check_binary_opE(i0) + \
            store_output(output)

        elif op == 'Not': 
            riscv = check_variable1(i0) + \
            check_unary_opNOT(i0,'NOT') + \
            store_output(output)

        elif op == 'LoadConstant':
            riscv = check_unary_opLI(i0,'LI') + \
            store_output(output)

        elif op == 'Label':
            riscv = i0 + '\n\t'

        elif op == 'Move':
            riscv = check_variable1(i0) + \
            store_output(output)

        elif op == 'Jump':
            riscv = 'JAL x0, ' + i0

        else: assert False, 'operation not found'

        return riscv
    else:    
        if op in binaryops: 
            riscv = load_stack(i0,'a1') +         load_stack(i1,'a2') +         binaryop('a1','a1','a2',binaryops[op]) +         save_stack(output,'a1')

        elif op in jumpops:
            riscv = load_stack(i0,'a1') +         unaryop('a1',i1,jumpops[op])

        elif op == 'GreaterEq': #a3 = (a1 < a2), then a1 = not a3, save a1 to stack output
            riscv = load_stack(i0,'a1') +         load_stack(i1,'a2') +         binaryop('a3','a1','a2','SLT') +         unaryop('a1','a3','NOT') +         save_stack(output,'a1')

        elif op == 'Equal': #a3 = (a1 < a2), a4 = (a1 > a2), a2 = (a3 xor a4), a1 = not a2, save a1 to stack output
            riscv = load_stack(i0,'a1') +         load_stack(i1,'a2') +         binaryop('a3','a1','a2','SLT') +         binaryop('a4','a1','a2','SGT') +         binaryop('a2','a3','a4','XOR') +         unaryop('a1','a2','NOT') +         save_stack(output,'a1')

        elif op == 'Not': 
            riscv = load_stack(i0,'a1') +         unaryop('a1','a1','NOT') +         save_stack(output,'a1')

        elif op == 'LoadConstant':
            riscv = unaryop('a1',i0,'LI') +         save_stack(output,'a1')

        elif op == 'Label':
            riscv = i0 + ':\n\t'

        elif op == 'Move':
            riscv = load_stack(i0,'a1') +         save_stack(output,'a1')

        elif op == 'Jump':
            riscv = 'JAL x0, ' + i0 + '\n\t'

        else: assert False, 'operation not found'

        return riscv

def print_run_imp_actual(instructions,name,fold_const,elim,register_allocation):
    run_imp_actual = 'run_imp_actual:\n\t'
    if fold_const:
        cfg=make_cfg('../programs/'+name)
        _,varmap=load_il('../programs/'+name)
        allvars=list(varmap)
        iteration_function=iterate_reaching_definitions
        rd_iterations= fixed_point_iteration(iteration_function,cfg=cfg,allvars=allvars)
        rd_final=rd_iterations[list(rd_iterations)[-1]]    
        instructions=fixed_point_iteration(fold,cfg=cfg,rd=rd_final,program_name=name)
        instructions=instructions[list(instructions)[-1]]
    if elim:
        _,varmap=load_il(name)
        cleanup_iter=fixed_point_iteration(cleanup,init_instr=instructions,varmap=varmap)
        instructions=cleanup_iter[list(cleanup_iter)[-1]]
    for i in instructions:
        run_imp_actual+=print_riscv_instruction(i)
    run_imp_actual += 'ret'
    return run_imp_actual
    
with open(str(sys.argv[1]), 'r') as f:
    name=sys.argv[1]
    operations = f.readlines()
    instructions = [o.split(' ') for o in operations]
    
fold_const,elim,register_allocation=sys.argv[2:]

print(print_run_imp_actual(instructions,name,fold_const,elim,register_allocation))
print(run_imp)