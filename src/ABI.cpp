#include "../h/ABI.h"
#include "../h/kMemoryAllocator.h"
#include "../h/kSemaphore.h"
#include "../h/kTCB.h"
#include "../lib/console.h"

struct ArgumentRegisters {
    uint64 a0, a1, a2, a3, a4;
};

extern "C" void loadArgumentRegisters(ArgumentRegisters*);

void passArgumentsToSyscall(uint64 a0, uint64 a1, uint64 a2, uint64 a3, uint64 a4)
{
    asm volatile("ecall");
}

extern "C" __attribute__((unused)) void handleSupervisorTrap()
{
    uint64 scause;
    asm volatile("csrr %[scauseASM], scause" : [scauseASM] "=r"(scause));

    if (scause == (0x08UL) || scause == (0x09UL)) {
        uint64 sepc, sstatus;
        asm volatile("csrr %[sepcASM], sepc\n\t"
                     "csrr %[sstatusASM], sstatus"
                     : [sepcASM] "=r"(sepc), [sstatusASM] "=r"(sstatus));
        sepc = sepc + 4;
        ArgumentRegisters registers{};
        loadArgumentRegisters(&registers);

        switch (registers.a0) {
        case 0x01:
            kMemoryAllocator::instance().mem_alloc(registers.a1);
            asm volatile("sd a0, 10 * 8(fp)");
            break;

        case 0x02:
            kMemoryAllocator::instance().mem_free((void*) registers.a1);
            asm volatile("sd a0, 10 * 8(fp)");
            break;

        case 0x11:
            kTCB::createThread((kTCB**) registers.a1, (void (*)(void*)) registers.a2,
                               (void*) registers.a3, (uint64*) registers.a4, true);
            asm volatile("sd a0, 10 * 8(fp)");
            break;

        case 0x12:
            kTCB::exitThread();
            asm volatile("sd a0, 10 * 8(fp)");
            break;

        case 0x13:
            kTCB::dispatch();
            break;

        case 0x21:
            kSemaphore::createSemaphore((kSemaphore**) registers.a1, (unsigned) registers.a2);
            asm volatile("sd a0, 10 * 8(fp)");
            break;

        case 0x22:
            kSemaphore::closeSemaphore((kSemaphore*) registers.a1);
            asm volatile("sd a0, 10 * 8(fp)");
            break;

        case 0x23:
            ((kSemaphore*) registers.a1)->wait();
            asm volatile("sd a0, 10 * 8(fp)");
            break;

        case 0x24:
            ((kSemaphore*) registers.a1)->signal();
            asm volatile("sd a0, 10 * 8(fp)");
            break;

        case 0x41:
            __getc();
            asm volatile("sd a0, 10 * 8(fp)");
            break;

        case 0x42:
            __putc(registers.a1);
            break;

        case 0x101:
            kTCB::createThread((kTCB**) registers.a1, (void (*)(void*)) registers.a2,
                               (void*) registers.a3, (uint64*) registers.a4, false);
            asm volatile("sd a0, 10 * 8(fp)");
            break;

        case 0x102:
            kTCB::deleteThread((kTCB*) registers.a1);
            asm volatile("sd a0, 10 * 8(fp)");
            break;

        case 0x103:
            kTCB::startThread((kTCB*) registers.a1);
            asm volatile("sd a0, 10 * 8(fp)");
            break;

        default:
            break;
        }

        asm volatile("csrw sepc, %[sepcASM]\n\t"
                     "csrw sstatus, %[sstatusASM]"
                     :
                     : [sepcASM] "r"(sepc), [sstatusASM] "r"(sstatus));
    }
    else if (scause == 0x8000000000000001UL) {
        asm volatile("csrc sip, 0x02");
    }
    else if (scause == 0x8000000000000009UL) {
        console_handler();
        asm volatile("csrc sip, 0x02");
    }
    else {
        uint64 pc;
        asm volatile("csrr %[pcreg], stval" : [pcreg] "=r"(pc));
        asm volatile("csrc sip, 0x02");
    }
}
