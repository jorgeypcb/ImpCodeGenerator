
.globl run_imp_actual

; Number of variables in imp program: 3

run_imp_actual:
	LD s1, 0(a0)
	LD s2, 8(a0)
	LD s3, 16(a0)
	LI s2, 1
	LI s3, 1
	LI s1, 9
	SD s1, 0(a0)
	SD s2, 8(a0)
	SD s3, 16(a0)
	ret
