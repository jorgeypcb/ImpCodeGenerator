
.globl run_imp_actual

; Number of variables in imp program: 5

run_imp_actual:
	LD s1, 0(a0)
	LD s2, 8(a0)
	LD s3, 16(a0)
	LD s4, 24(a0)
	LD s5, 32(a0)
	LI s1, 0
	
	LI s1, 0
	
	.LBB_3
	LI s2, 1
	
	
	BEQZ s6, .LBB_6
	.LBB_4
	LI s1, 0
	
	LI s2, 1
	
	JAL x0, .LBB_3
	.LBB_6
	LI s1, 0
	
	LI s1, 0
	
	
	BEQZ s6, .LBB_9
	LI s1, 0
	
	JAL x0, .LBB_10
	.LBB_9
	LI s1, 0
	
	.LBB_10
	LI s1, 0
	
	SD s1, 0(a0)
	SD s2, 8(a0)
	SD s3, 16(a0)
	SD s4, 24(a0)
	SD s5, 32(a0)
	ret
