.extern handleSupervisorTrap
.global supervisorTrap
.align 4
supervisorTrap:
    addi sp, sp, -256
    .irp index 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
    sd x\index, \index * 8(sp)
    .endr

    call handleSupervisorTrap

    .irp index 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
    ld x\index, \index * 8(sp)
    .endr
    addi sp, sp, 256
    sret

.global loadArgumentRegisters
.type loadArgumentRegisters, @function
.align 4
loadArgumentRegisters:
    ld a1, 11 * 8(fp)
    sd a1, 8(a0)
    ld a2, 12 * 8(fp)
    sd a2, 16(a0)
    ld a3, 13 * 8(fp)
    sd a3, 24(a0)
    ld a4, 14 * 8(fp)
    sd a4, 32(a0)
    ld a5, 10 * 8(fp)
    sd a5, 0(a0)
    ld a0, 10 * 8(fp)
    ld a5, 15 * 8(fp)
    ret

.global _ZN4kTCB13contextSwitchEPNS_7ContextES1_
.type _ZN4kTCB13contextSwitchEPNS_7ContextES1_, @function
.align 4
_ZN4kTCB13contextSwitchEPNS_7ContextES1_:
    sd ra, 0(a0)
    sd sp, 8(a0)
    ld ra, 0(a1)
    ld sp, 8(a1)
    ret

.global pushAllRegisters
.type pushAllRegisters, @function
.align 4
pushAllRegisters:
    addi sp, sp, -256
    .irp index 3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
    sd x\index, \index * 8(sp)
    .endr
    ret

.global popAllRegisters
.type popAllRegisters, @function
.align 4
popAllRegisters:
    .irp index 3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
    ld x\index, \index * 8(sp)
    .endr
    addi sp, sp, 256
    ret
