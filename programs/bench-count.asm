
.globl run_imp_actual

; Number of variables in imp program: 2

run_imp_actual:
	LD s1, 0(a0)
	LD s2, 8(a0)
	LI s1, 100000000
	.LBB_2
	LI s3, 1
	
	BEQZ s3, .LBB_4
	.LBB_3
	LI s1, 99999999
	JAL x0, .LBB_2
	.LBB_4
	LI s2, 100000000
	SD s1, 0(a0)
	SD s2, 8(a0)
	ret
