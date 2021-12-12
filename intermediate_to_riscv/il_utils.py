#helper functions
op=lambda i: i[0]
i0=lambda i: i[1]
i1=lambda i: i[2]
output=lambda i: i[3]
block=lambda i: i[4].strip('\n')
block_of_label=lambda lab,instructions: [block(i) for i in instructions if (op(i)=='Label' and i0(i)==lab)][0]
instruction=lambda i: (op(i),i0(i),i1(i),output(i),block(i))
jumpto_uncond=lambda i, instructions: block_of_label(i0(i),instructions)
jumpto_cond=lambda i, instructions: block_of_label(i1(i),instructions)
instructions_in_block = lambda b,instructions: [instruction(i) for i in instructions if block(i)==b] #extract all instructions from block b
block_contains_op = lambda l, op_, instructions: True if op_ in [op(i) for i in instructions_in_block(l)] else False

def instruction_number_of_label(instructions,l):
    for ni,i in enumerate(instructions):
        if block(i)==l: return ni
    assert False, 'block label not found'
    
def const(s,l,instructions): #Return last constant assigned to variable v in block l if possible, else False
    if (not l) or l=='?': return False
    b=instructions_in_block(l,instructions)
    blockstart=instruction_number_of_label(instructions,l)
    next_ins=lambda i: b[int(i)+1] if (int(i)+1)<len(b) else False
    next_op=lambda i: op(next_ins(i)) if next_ins(i) else False
    next_out=lambda i: output(next_ins(i)) if next_ins(i) else False
    for ni,i in enumerate(b):
        const=i0(i)
        if (op(i),next_op(ni))==('LoadConstant','Move'):
            if s==next_out(ni): return const,(ni,str(int(ni+1)))
        elif op(i)=='LoadConstant' and output(i)==s: return const,(ni,None)
    return False

#starting at some line in a block that uses a temporary variable as a parameter,
#find the most recent value of a constant assigned to that temporary variable if possible
def temp_const_val(stack_loc,l,instructions,line_used):
#     print('temp_const_val\nstack_loc',stack_loc,'\nl',l,'\nline_used',line_used,'\n')
    blockstart=instruction_number_of_label(instructions,l)
    i=line_used-1
    while i>=blockstart:
        ins=instructions[i]
#         print('op(ins)',op(ins),'\ni0(ins)',output(ins),'==stack_loc\n','\n-----\n')
        if (op(ins)=='LoadConstant' and output(ins)==stack_loc): 
            line_defined = i
            val=i0(ins)
#             print('RETURN',val)
            return val,line_defined
        i-=1
    return False

def print_ins(ins):
    space=lambda x, n: n-len(x)
    def empty(z): 
        e=''
        for i in range(z):e+=' '
        return e
    fill=lambda x, width: x+empty(space(x,width))
    for i in ins:print(fill(i[0],12),'\t',fill(i[1],6),'\t',fill(i[2],6),'\t',fill(i[3],6),'\t',fill(i[4].strip('\n'),6))        

def load_il(insname,varsname):
    with open(insname,'r') as f:
        operations=f.readlines()
        instructions=[o.split(' ') for o in operations]
    with open(varsname,'r') as f:
        v=f.readlines()
        v=[i.split(' ') for i in v]
        stack_var_map={i[0]:i[1].strip('\n') for i in v}
    return instructions,stack_var_map