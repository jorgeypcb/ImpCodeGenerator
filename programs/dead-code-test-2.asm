
.globl run_imp_actual

run_imp_actual:
	LD s1, 0(a0)
	LD s2, 8(a0)
	LD s3, 16(a0)
	LI s4, 1
	
	
	
	
	LI s9, 3
	LI s10, 4
	
	
	ADD s8, s9, s10
	
	
	
	ADD s6, s3, s8
	
	
	
	ADD s4, s2, s6
	
	
	
	SD s1, 0(a0)
	SD s2, 8(a0)
	SD s3, 16(a0)
	ret
