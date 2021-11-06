import sys

run_imp = '''
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
call    run_imp_actual(long*)
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

def print_riscv_instruction(instruction):
    op,i0,i1 = instruction[:3] #assigns the instruction components
    output = instruction[3].strip('\n\t')
    binaryops = {'Plus':'ADD','Minus':'SUB','Times':'MUL','Greater':'SGT','And':'AND','Or':'OR'}
    jumpops = {'JumpIfZero':'BEQZ','JumpIfNonzero':'BNEZ'}
    
    stack = lambda i : str(8*int(i)) + '(a0)' #converts stack index 
    load_stack = lambda i, a: "LD "+a+", "+ stack(i) + "\n\t"  #load ith value from stack to register a
    save_stack = lambda i, a: "SD "+a+", "+ stack(i) + "\n\t"  #save register a to ith index in stack
    binaryop = lambda out_reg, ai, aj, opname: opname + " " + out_reg + ", " + ai +", " + aj + "\n\t" #perform operation a1 op a2, put result in output register
    unaryop = lambda arg1, arg2, opname: opname + " " + arg1 + ", " + arg2 + '\n\t'
    
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
        riscv = i0 + '\n\t'
        
    else: assert False, 'operation not found'
    
    return riscv

def print_run_imp_actual(instructions):
    run_imp_actual = 'run_imp_actual(long*):\n\t'
    for i in instructions:
        run_imp_actual+=print_riscv_instruction(i)
    return run_imp_actual
    
with open(str(sys.arvg[1]), 'r') as f:
    operations = f.readlines()
    instructions = [o.split(' ') for o in operations]

print(print_run_imp_actual(instructions))
print(run_imp)