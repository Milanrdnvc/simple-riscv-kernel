#ifndef _riscv_hpp_
#define _riscv_hpp_

#include "../lib/hw.h"

class RISCV {
public:
    // read register scause
    static size_t r_scause();

    // write register scause
    static void w_scause(size_t scause);

    // read register sepc
    static size_t r_sepc();

    // write register sepc
    static void w_sepc(size_t sepc);

    // read register stvec
    static size_t r_stvec();

    // write register stvec
    static void w_stvec(size_t stvec);

    // read register stval
    static size_t r_stval();

    // write register stval
    static void w_stval(size_t stval);

    enum BitMaskSip {
        SIP_SSIP = (1 << 1),
        SIP_STIP = (1 << 5),
        SIP_SEIP = (1 << 9),
    };

    // mask set register sip
    static void ms_sip(size_t mask);

    // mask clear register sip
    static void mc_sip(size_t mask);

    // read register sip
    static size_t r_sip();

    // write register sip
    static void w_sip(size_t sip);

    enum BitMaskSstatus {
        SSTATUS_SIE = (1 << 1),
        SSTATUS_SPIE = (1 << 5),
        SSTATUS_SPP = (1 << 8),
    };

    // mask set register sstatus
    static void ms_sstatus(size_t mask);

    // mask clear register sstatus
    static void mc_sstatus(size_t mask);

    // read register sstatus
    static size_t r_sstatus();

    // write register sstatus
    static void w_sstatus(size_t sstatus);

    // read register a0
    static size_t r_a0();

    // write register a0
    static void w_a0(size_t val);

    // read register a1
    static size_t r_a1();

    // write register a1
    static void w_a1(size_t val);

    // read register a2
    static size_t r_a2();

    // write register a2
    static void w_a2(size_t val);

    // read register a3
    static size_t r_a3();

    // write register a3
    static void w_a3(size_t val);

    // read register a4
    static size_t r_a4();

    // write register a4
    static void w_a4(size_t val);

    // interrupt routine
    static void interruptRoutine();

    // pop sstatus.spp and sstatus.spie bits
    static void popSppSpie();

    // pop sstatuis.spp and sstatus.spie bits but remain in system mode
    static void systemPopSppSpie();

    // change a0 on the stack
    static void returnSysCall(size_t retVal);

    // halt the CPU
    static void halt();

private:
    // interrupt handler
    static void handleInterruptRoutine();
};

inline size_t RISCV::r_stvec() {
    size_t volatile stvec;
    __asm__ volatile("csrr %[stvec], stvec" : [stvec] "=r" (stvec));
    return stvec;
}

inline void RISCV::w_stvec(size_t stvec) {
    __asm__ volatile("csrw stvec, %[stvec]" : : [stvec] "r" (stvec));
}

inline size_t RISCV::r_sip() {
    uint64 volatile sip;
    __asm__ volatile ("csrr %[sip], sip" : [sip] "=r" (sip));
    return sip;
}

inline void RISCV::w_sip(size_t sip) {
    __asm__ volatile ("csrw sip, %[sip]" : : [sip] "r" (sip));
}

inline void RISCV::ms_sip(size_t mask) {
    __asm__ volatile ("csrs sip, %[mask]" : : [mask] "r" (mask));
}

inline void RISCV::mc_sip(size_t mask) {
    __asm__ volatile ("csrc sip, %[mask]" : : [mask] "r" (mask));
}

inline uint64 RISCV::r_sepc() {
    size_t volatile sepc;
    __asm__ volatile ("csrr %[sepc], sepc" : [sepc] "=r" (sepc));
    return sepc;
}

inline void RISCV::w_sepc(size_t sepc) {
    __asm__ volatile ("csrw sepc, %[sepc]" : : [sepc] "r" (sepc));
}

inline uint64 RISCV::r_sstatus() {
    size_t volatile sstatus;
    __asm__ volatile ("csrr %[sstatus], sstatus" : [sstatus] "=r" (sstatus));
    return sstatus;
}

inline void RISCV::w_sstatus(size_t sstatus) {
    __asm__ volatile ("csrw sstatus, %[sstatus]" : : [sstatus] "r" (sstatus));
}

inline uint64 RISCV::r_scause() {
    size_t volatile scause;
    __asm__ volatile ("csrr %[scause], scause" : [scause] "=r" (scause));
    return scause;
}

inline void RISCV::w_scause(size_t scause) {
    __asm__ volatile ("csrw scause, %[scause]" : : [scause] "r" (scause));
}

inline size_t RISCV::r_a0() {
    size_t volatile a0;
    __asm__ volatile ("mv %0, a0" : "=r" (a0));
    return a0;
}

inline void RISCV::w_a0(size_t val) {
    __asm__ volatile ("mv a0, %0" : : "r" (val));
}

inline size_t RISCV::r_a1() {
    size_t volatile a1;
    __asm__ volatile ("mv %0, a1" : "=r" (a1));
    return a1;
}

inline void RISCV::w_a1(size_t val) {
    __asm__ volatile ("mv a1, %0" : : "r" (val));
}

inline size_t RISCV::r_a2() {
    size_t volatile a2;
    __asm__ volatile ("mv %0, a2" : "=r" (a2));
    return a2;
}

inline void RISCV::w_a2(size_t val) {
    __asm__ volatile ("mv a2, %0" : : "r" (val));
}

inline size_t RISCV::r_a3() {
    size_t volatile a3;
    __asm__ volatile ("mv %0, a3" : "=r" (a3));
    return a3;
}

inline void RISCV::w_a3(size_t val) {
    __asm__ volatile ("mv a3, %0" : : "r" (val));
}

inline size_t RISCV::r_a4() {
    size_t volatile a4;
    __asm__ volatile ("mv %0, a4" : "=r" (a4));
    return a4;
}

inline void RISCV::w_a4(size_t val) {
    __asm__ volatile ("mv a4, %0" : : "r" (val));
}

inline void RISCV::mc_sstatus(size_t mask) {
    __asm__ volatile ("csrc sstatus, %[mask]" : : [mask] "r" (mask));
}

inline void RISCV::ms_sstatus(size_t mask) {
    __asm__ volatile ("csrs sstatus, %[mask]" : : [mask] "r" (mask));
}

inline void RISCV::returnSysCall(size_t retVal) {
    __asm__ volatile("mv t0, %0" : : "r" (retVal));
    __asm__ volatile ("sw t0, 80(x8)");
}

inline void RISCV::halt() {
    __asm__ volatile("li t0, 0x5555");
    __asm__ volatile("li t1, 0x100000");
    __asm__ volatile("sw t0, 0(t1)");
}

// read register stval
inline size_t RISCV::r_stval() {
    size_t volatile stval;
    __asm__ volatile ("csrr %[stval], stval" : [stval] "=r" (stval));
    return stval;
}

// write register stval
inline void RISCV::w_stval(size_t stval) {
    __asm__ volatile ("csrw stval, %[stval]" : : [stval] "r" (stval));
}

#endif
