
.globl run_imp_actual

run_imp_actual:
	LD s1, 0(a0)
	LD s2, 8(a0)
	LI s3, 100000000
	
	
	.LBB_2
	LI s5, 0
	
	
	SGT s3, s1, s5
	
	
	BEQZ s3, .LBB_4
	.LBB_3
	LI s5, 1
	
	
	SUB s3, s1, s5
	
	
	
	JAL x0, .LBB_2
	.LBB_4
	
	
	SD s1, 0(a0)
	SD s2, 8(a0)
	ret
