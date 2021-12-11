from cfg import*

programs='collatz','arith','factorial','fib','dead-code-test-1','dead-code-test-2'

for p in programs:
    c=make_cfg('../programs/'+p)
    visualize(c,p+'.cfg',True)
