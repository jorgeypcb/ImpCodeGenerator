
.globl run_imp_actual

// Number of variables in imp program: 3

run_imp_actual:
	LD s1, 0(a0)
	LD s2, 8(a0)
	LD s3, 16(a0)
	LI s4, 1
	
	
	LI s4, 2
	
	
	LI s8, 3
	
	
	ADD s6, s3, s8
	
	
	
	ADD s4, s2, s6
	
	
	
	SD s1, 0(a0)
	SD s2, 8(a0)
	SD s3, 16(a0)
	ret
