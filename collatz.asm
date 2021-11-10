run_imp_actual:
	LD a1, 0(a0)
	SD a1, 8(a0)
	LI a1, 0
	SD a1, 48(a0)
	LD a1, 48(a0)
	SD a1, 40(a0)
	.LBB_0:
	LI a1, 1
	SD a1, 64(a0)
	LD a1, 8(a0)
	LD a2, 64(a0)
	SGT a1, a1, a2
	SD a1, 48(a0)
	LD a1, 48(a0)
	BEQZ a1, .LBB_2
	.LBB_1:
	LD a1, 8(a0)
	SD a1, 32(a0)
	LI a1, 0
	SD a1, 48(a0)
	LD a1, 48(a0)
	SD a1, 24(a0)
	.LBB_3:
	LI a1, 1
	SD a1, 64(a0)
	LD a1, 32(a0)
	LD a2, 64(a0)
	SGT a1, a1, a2
	SD a1, 48(a0)
	LD a1, 48(a0)
	BEQZ a1, .LBB_5
	.LBB_4:
	LI a1, 2
	SD a1, 64(a0)
	LD a1, 32(a0)
	LD a2, 64(a0)
	SUB a1, a1, a2
	SD a1, 48(a0)
	LD a1, 48(a0)
	SD a1, 32(a0)
	LI a1, 1
	SD a1, 64(a0)
	LD a1, 24(a0)
	LD a2, 64(a0)
	ADD a1, a1, a2
	SD a1, 48(a0)
	LD a1, 48(a0)
	SD a1, 24(a0)
	JAL x0, .LBB_3
	.LBB_5:
	LI a1, 0
	SD a1, 64(a0)
	LD a1, 32(a0)
	LD a2, 64(a0)
	SLT a3, a1, a2
	SGT a4, a1, a2
	XOR a2, a3, a4
	NOT a1, a2
	SD a1, 48(a0)
	LD a1, 48(a0)
	BEQZ a1, .LBB_6
	LD a1, 24(a0)
	SD a1, 8(a0)
	JAL x0, .LBB_7
	.LBB_6:
	LI a1, 3
	SD a1, 56(a0)
	LI a1, 1
	SD a1, 80(a0)
	LD a1, 8(a0)
	LD a2, 80(a0)
	ADD a1, a1, a2
	SD a1, 64(a0)
	LD a1, 56(a0)
	LD a2, 64(a0)
	MUL a1, a1, a2
	SD a1, 48(a0)
	LD a1, 48(a0)
	SD a1, 8(a0)
	.LBB_7:
	LI a1, 1
	SD a1, 64(a0)
	LD a1, 40(a0)
	LD a2, 64(a0)
	ADD a1, a1, a2
	SD a1, 48(a0)
	LD a1, 48(a0)
	SD a1, 40(a0)
	JAL x0, .LBB_0
	.LBB_2:
	LD a1, 40(a0)
	SD a1, 16(a0)
	

.globl run_imp_actual

run_imp:
	addi    sp, sp, -2032
	sd      ra, 2024(sp)                    # 8-byte Folded
	sd      s0, 2016(sp)                    # 8-byte Folded
	lui     a1, 32
	addiw   a1, a1, -2000
	sub     sp, sp, a1
	mv      s0, a0
	addi    a0, sp, 16
	addi    a2, zero, 120
	mv      a1, s0
	call    memcpy@plt
	addi    a0, sp, 16
	call    run_imp_actual
	addi    a1, sp, 16
	addi    a2, zero, 120
	mv      a0, s0
	call    memcpy@plt
	lui     a0, 32
	addiw   a0, a0, -2000
	add     sp, sp, a0
	ld      s0, 2016(sp)                    # 8-byte Folded Reload
	ld      ra, 2024(sp)                    # 8-byte Folded Reload
	addi    sp, sp, 2032
	ret

