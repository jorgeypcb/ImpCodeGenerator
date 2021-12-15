
.globl run_imp_actual

run_imp_actual:
	LD s1, 0(a0)
	LD s2, 8(a0)
	LD s3, 16(a0)
	LD s4, 24(a0)
	LD s5, 32(a0)
	LD s6, 40(a0)
	LI s7, 0
	
	
	LI s7, 1
	
	
	LI s7, 0
	
	
	.LBB_4:
	
	
	SGT s7, s3, s6
	
	
	BEQZ s7, .LBB_10
	.LBB_5:
	
	
	ADD s7, s1, s2
	
	
	
	
	
	
	
	LI s9, 1
	
	
	SUB s7, s3, s9
	
	
	
	LI s7, 0
	
	
	JAL x0, .LBB_4
	.LBB_10:
	
	
	SD s1, 0(a0)
	SD s2, 8(a0)
	SD s3, 16(a0)
	SD s4, 24(a0)
	SD s5, 32(a0)
	SD s6, 40(a0)
	ret
