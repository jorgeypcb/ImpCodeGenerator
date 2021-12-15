
.globl run_imp_actual

; Number of variables in imp program: 5

run_imp_actual:
	LD s1, 0(a0)
	LD s2, 8(a0)
	LD s3, 16(a0)
	LD s4, 24(a0)
	LD s5, 32(a0)
	LI s1, 0
	LI s3, 0
	.LBB_3
	LI s8, 10
	
	
	SGT s6, s8, s1
	
	
	BEQZ s6, .LBB_6
	.LBB_4
	
	
	ADD s6, s1, s1
	
	
	
	LI s8, 1
	
	
	ADD s6, s3, s8
	
	
	
	JAL x0, .LBB_3
	.LBB_6
	
	
	SUB s6, s1, s3
	
	
	
	LI s8, 1
	
	
	SLT s6, s4, s5
	SGT s7, s4, s5
	XOR s5, s6, s7
	NOT s4, s5
	
	
	BEQZ s6, .LBB_9
	
	
	JAL x0, .LBB_10
	.LBB_9
	LI s5, 0
	.LBB_10
	
	
	MUL s6, s5, s5
	
	
	
	SD s1, 0(a0)
	SD s2, 8(a0)
	SD s3, 16(a0)
	SD s4, 24(a0)
	SD s5, 32(a0)
	ret
