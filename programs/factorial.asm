
.globl run_imp_actual

; Number of variables in imp program: 4

run_imp_actual:
	LD s1, 0(a0)
	LD s2, 8(a0)
	LD s3, 16(a0)
	LD s4, 24(a0)
	
	LI s2, 1
	.LBB_3
	SGT s5, s3, s7
	BEQZ s5, .LBB_6
	.LBB_4
	MUL s5, s4, s3
	
	SUB s5, s3, s7
	
	JAL x0, .LBB_3
	.LBB_6
	LI s1, 0
	LI s2, 1
	SD s1, 0(a0)
	SD s2, 8(a0)
	SD s3, 16(a0)
	SD s4, 24(a0)
	ret
