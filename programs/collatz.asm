
.globl run_imp_actual

; Number of variables in imp program: 6

run_imp_actual:
	LD s1, 0(a0)
	LD s2, 8(a0)
	LD s3, 16(a0)
	LD s4, 24(a0)
	LD s5, 32(a0)
	LD s6, 40(a0)
	
	
	LI s1, 0
	
	.LBB_3
	LI s1, 0
	
	
	BEQZ s7, .LBB_13
	.LBB_4
	LI s1, 0
	
	LI s1, 0
	
	.LBB_6
	LI s1, 0
	
	
	BEQZ s7, .LBB_9
	.LBB_7
	
	
	SUB s7, s5, s9
	
	
	
	LI s2, 1
	
	JAL x0, .LBB_6
	.LBB_9
	
	
	SLT s7, s5, s6
	SGT s8, s5, s6
	XOR s6, s7, s8
	NOT s5, s6
	
	
	BEQZ s7, .LBB_11
	LI s1, 0
	
	JAL x0, .LBB_12
	.LBB_11
	LI s4, 3
	
	.LBB_12
	
	
	ADD s7, s6, s9
	
	
	
	JAL x0, .LBB_3
	.LBB_13
	
	
	SD s1, 0(a0)
	SD s2, 8(a0)
	SD s3, 16(a0)
	SD s4, 24(a0)
	SD s5, 32(a0)
	SD s6, 40(a0)
	ret
