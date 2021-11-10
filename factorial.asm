run_imp_actual(long*):
	LD a1, 8(a0)
	SD a1, 16(a0)
	LI a1, 1
	SD a1, 32(a0)
	LD a1, 32(a0)
	SD a1, 24(a0)
	.LBB_0:
	LI a1, 1
	SD a1, 48(a0)
	LD a1, 16(a0)
	LD a2, 48(a0)
	SGT a1, a1, a2
	SD a1, 32(a0)
	LD a1, 32(a0)
	BEQZ a1, .LBB_2
	.LBB_1:
	LD a1, 24(a0)
	LD a2, 16(a0)
	MUL a1, a1, a2
	SD a1, 32(a0)
	LD a1, 32(a0)
	SD a1, 24(a0)
	LI a1, 1
	SD a1, 48(a0)
	LD a1, 16(a0)
	LD a2, 48(a0)
	SUB a1, a1, a2
	SD a1, 32(a0)
	LD a1, 32(a0)
	SD a1, 16(a0)
	JAL x0, .LBB_0
	.LBB_2:
	LI a1, 0
	SD a1, 32(a0)
	LD a1, 32(a0)
	SD a1, 16(a0)
	LD a1, 24(a0)
	SD a1, 0(a0)
	

run_imp(long*):
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
	call    run_imp_actual(long*)
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

