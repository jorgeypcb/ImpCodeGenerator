
.globl run_imp_actual

run_imp_actual:
	LD s1, 0(a0)
	LD s2, 8(a0)
	LD s3, 16(a0)
	LD s4, 24(a0)
	LD s5, 32(a0)
	LD s6, 40(a0)
	
	
	LI s7, 0
	
	
	.LBB_3:
	LI s9, 1
	
	
	SGT s7, s2, s9
	
	
	BEQZ s7, .LBB_13
	.LBB_4:
	
	
	LI s7, 0
	
	
	.LBB_6:
	LI s9, 1
	
	
	SGT s7, s5, s9
	
	
	BEQZ s7, .LBB_9
	.LBB_7:
	LI s9, 2
	
	
	SUB s7, s5, s9
	
	
	
	LI s9, 1
	
	
	ADD s7, s4, s9
	
	
	
	JAL x0, .LBB_6
	.LBB_9:
	LI s9, 0
	
	
	SLT s7, s5, s6
	SGT s8, s5, s6
	XOR s6, s7, s8
	NOT s5, s6
	
	
	BEQZ s7, .LBB_11
	
	
	JAL x0, .LBB_12
	.LBB_11:
	LI s8, 3
	LI s11, 1
	
	
	ADD s9, s2, s11
	
	
	
	MUL s7, s8, s9
	
	
	
	.LBB_12:
	LI s9, 1
	
	
	ADD s7, s6, s9
	
	
	
	JAL x0, .LBB_3
	.LBB_13:
	
	
	SD s1, 0(a0)
	SD s2, 8(a0)
	SD s3, 16(a0)
	SD s4, 24(a0)
	SD s5, 32(a0)
	SD s6, 40(a0)
	ret
