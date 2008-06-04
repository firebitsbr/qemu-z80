/*
 *  Alpha emulation cpu definitions for qemu.
 *
 *  Copyright (c) 2007 Jocelyn Mayer
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#if !defined (__CPU_ALPHA_H__)
#define __CPU_ALPHA_H__

#include "config.h"

#define TARGET_LONG_BITS 64

#include "cpu-defs.h"

#include <setjmp.h>

#include "softfloat.h"

#define TARGET_HAS_ICE 1

#define ELF_MACHINE     EM_ALPHA

#define ICACHE_LINE_SIZE 32
#define DCACHE_LINE_SIZE 32

#define TARGET_PAGE_BITS 12

#define VA_BITS 43

/* Alpha major type */
enum {
    ALPHA_EV3  = 1,
    ALPHA_EV4  = 2,
    ALPHA_SIM  = 3,
    ALPHA_LCA  = 4,
    ALPHA_EV5  = 5, /* 21164 */
    ALPHA_EV45 = 6, /* 21064A */
    ALPHA_EV56 = 7, /* 21164A */
};

/* EV4 minor type */
enum {
    ALPHA_EV4_2 = 0,
    ALPHA_EV4_3 = 1,
};

/* LCA minor type */
enum {
    ALPHA_LCA_1 = 1, /* 21066 */
    ALPHA_LCA_2 = 2, /* 20166 */
    ALPHA_LCA_3 = 3, /* 21068 */
    ALPHA_LCA_4 = 4, /* 21068 */
    ALPHA_LCA_5 = 5, /* 21066A */
    ALPHA_LCA_6 = 6, /* 21068A */
};

/* EV5 minor type */
enum {
    ALPHA_EV5_1 = 1, /* Rev BA, CA */
    ALPHA_EV5_2 = 2, /* Rev DA, EA */
    ALPHA_EV5_3 = 3, /* Pass 3 */
    ALPHA_EV5_4 = 4, /* Pass 3.2 */
    ALPHA_EV5_5 = 5, /* Pass 4 */
};

/* EV45 minor type */
enum {
    ALPHA_EV45_1 = 1, /* Pass 1 */
    ALPHA_EV45_2 = 2, /* Pass 1.1 */
    ALPHA_EV45_3 = 3, /* Pass 2 */
};

/* EV56 minor type */
enum {
    ALPHA_EV56_1 = 1, /* Pass 1 */
    ALPHA_EV56_2 = 2, /* Pass 2 */
};

enum {
    IMPLVER_2106x = 0, /* EV4, EV45 & LCA45 */
    IMPLVER_21164 = 1, /* EV5, EV56 & PCA45 */
    IMPLVER_21264 = 2, /* EV6, EV67 & EV68x */
    IMPLVER_21364 = 3, /* EV7 & EV79 */
};

enum {
    AMASK_BWX      = 0x00000001,
    AMASK_FIX      = 0x00000002,
    AMASK_CIX      = 0x00000004,
    AMASK_MVI      = 0x00000100,
    AMASK_TRAP     = 0x00000200,
    AMASK_PREFETCH = 0x00001000,
};

enum {
    VAX_ROUND_NORMAL = 0,
    VAX_ROUND_CHOPPED,
};

enum {
    IEEE_ROUND_NORMAL = 0,
    IEEE_ROUND_DYNAMIC,
    IEEE_ROUND_PLUS,
    IEEE_ROUND_MINUS,
    IEEE_ROUND_CHOPPED,
};

/* IEEE floating-point operations encoding */
/* Trap mode */
enum {
    FP_TRAP_I   = 0x0,
    FP_TRAP_U   = 0x1,
    FP_TRAP_S  = 0x4,
    FP_TRAP_SU  = 0x5,
    FP_TRAP_SUI = 0x7,
};

/* Rounding mode */
enum {
    FP_ROUND_CHOPPED = 0x0,
    FP_ROUND_MINUS   = 0x1,
    FP_ROUND_NORMAL  = 0x2,
    FP_ROUND_DYNAMIC = 0x3,
};

/* Internal processor registers */
/* XXX: TOFIX: most of those registers are implementation dependant */
enum {
    /* Ebox IPRs */
    IPR_CC           = 0xC0,
    IPR_CC_CTL       = 0xC1,
    IPR_VA           = 0xC2,
    IPR_VA_CTL       = 0xC4,
    IPR_VA_FORM      = 0xC3,
    /* Ibox IPRs */
    IPR_ITB_TAG      = 0x00,
    IPR_ITB_PTE      = 0x01,
    IPT_ITB_IAP      = 0x02,
    IPT_ITB_IA       = 0x03,
    IPT_ITB_IS       = 0x04,
    IPR_PMPC         = 0x05,
    IPR_EXC_ADDR     = 0x06,
    IPR_IVA_FORM     = 0x07,
    IPR_CM           = 0x09,
    IPR_IER          = 0x0A,
    IPR_SIRR         = 0x0C,
    IPR_ISUM         = 0x0D,
    IPR_HW_INT_CLR   = 0x0E,
    IPR_EXC_SUM      = 0x0F,
    IPR_PAL_BASE     = 0x10,
    IPR_I_CTL        = 0x11,
    IPR_I_STAT       = 0x16,
    IPR_IC_FLUSH     = 0x13,
    IPR_IC_FLUSH_ASM = 0x12,
    IPR_CLR_MAP      = 0x15,
    IPR_SLEEP        = 0x17,
    IPR_PCTX         = 0x40,
    IPR_PCTR_CTL     = 0x14,
    /* Mbox IPRs */
    IPR_DTB_TAG0     = 0x20,
    IPR_DTB_TAG1     = 0xA0,
    IPR_DTB_PTE0     = 0x21,
    IPR_DTB_PTE1     = 0xA1,
    IPR_DTB_ALTMODE  = 0xA6,
    IPR_DTB_IAP      = 0xA2,
    IPR_DTB_IA       = 0xA3,
    IPR_DTB_IS0      = 0x24,
    IPR_DTB_IS1      = 0xA4,
    IPR_DTB_ASN0     = 0x25,
    IPR_DTB_ASN1     = 0xA5,
    IPR_MM_STAT      = 0x27,
    IPR_M_CTL        = 0x28,
    IPR_DC_CTL       = 0x29,
    IPR_DC_STAT      = 0x2A,
    /* Cbox IPRs */
    IPR_C_DATA       = 0x2B,
    IPR_C_SHIFT      = 0x2C,

    IPR_ASN,
    IPR_ASTEN,
    IPR_ASTSR,
    IPR_DATFX,
    IPR_ESP,
    IPR_FEN,
    IPR_IPIR,
    IPR_IPL,
    IPR_KSP,
    IPR_MCES,
    IPR_PERFMON,
    IPR_PCBB,
    IPR_PRBR,
    IPR_PTBR,
    IPR_SCBB,
    IPR_SISR,
    IPR_SSP,
    IPR_SYSPTBR,
    IPR_TBCHK,
    IPR_TBIA,
    IPR_TBIAP,
    IPR_TBIS,
    IPR_TBISD,
    IPR_TBISI,
    IPR_USP,
    IPR_VIRBND,
    IPR_VPTB,
    IPR_WHAMI,
    IPR_ALT_MODE,
    IPR_LAST,
};

typedef struct CPUAlphaState CPUAlphaState;

typedef struct pal_handler_t pal_handler_t;
struct pal_handler_t {
    /* Reset */
    void (*reset)(CPUAlphaState *env);
    /* Uncorrectable hardware error */
    void (*machine_check)(CPUAlphaState *env);
    /* Arithmetic exception */
    void (*arithmetic)(CPUAlphaState *env);
    /* Interrupt / correctable hardware error */
    void (*interrupt)(CPUAlphaState *env);
    /* Data fault */
    void (*dfault)(CPUAlphaState *env);
    /* DTB miss pal */
    void (*dtb_miss_pal)(CPUAlphaState *env);
    /* DTB miss native */
    void (*dtb_miss_native)(CPUAlphaState *env);
    /* Unaligned access */
    void (*unalign)(CPUAlphaState *env);
    /* ITB miss */
    void (*itb_miss)(CPUAlphaState *env);
    /* Instruction stream access violation */
    void (*itb_acv)(CPUAlphaState *env);
    /* Reserved or privileged opcode */
    void (*opcdec)(CPUAlphaState *env);
    /* Floating point exception */
    void (*fen)(CPUAlphaState *env);
    /* Call pal instruction */
    void (*call_pal)(CPUAlphaState *env, uint32_t palcode);
};

#define NB_MMU_MODES 4

struct CPUAlphaState {
    uint64_t ir[31];
    float64  fir[31];
    float_status fp_status;
    uint64_t fpcr;
    uint64_t pc;
    uint64_t lock;
    uint32_t pcc[2];
    uint64_t ipr[IPR_LAST];
    uint64_t ps;
    uint64_t unique;
    int saved_mode; /* Used for HW_LD / HW_ST */

#if TARGET_LONG_BITS > HOST_LONG_BITS
    /* temporary fixed-point registers
     * used to emulate 64 bits target on 32 bits hosts
     */
    target_ulong t0, t1, t2;
#endif
    /* */
    double ft0, ft1, ft2;

    /* Those resources are used only in Qemu core */
    CPU_COMMON

    int user_mode_only; /* user mode only simulation */
    uint32_t hflags;

    int error_code;
    int interrupt_request;

    uint32_t features;
    uint32_t amask;
    int implver;
    pal_handler_t *pal_handler;
};

#define CPUState CPUAlphaState
#define cpu_init cpu_alpha_init
#define cpu_exec cpu_alpha_exec
#define cpu_gen_code cpu_alpha_gen_code
#define cpu_signal_handler cpu_alpha_signal_handler

/* MMU modes definitions */
#define MMU_MODE0_SUFFIX _kernel
#define MMU_MODE1_SUFFIX _executive
#define MMU_MODE2_SUFFIX _supervisor
#define MMU_MODE3_SUFFIX _user
#define MMU_USER_IDX 3
static inline int cpu_mmu_index (CPUState *env)
{
    return (env->ps >> 3) & 3;
}

#if defined(CONFIG_USER_ONLY)
static inline void cpu_clone_regs(CPUState *env, target_ulong newsp)
{
    if (newsp)
        env->ir[30] = newsp;
    /* FIXME: Zero syscall return value.  */
}
#endif

#include "cpu-all.h"

enum {
    FEATURE_ASN    = 0x00000001,
    FEATURE_SPS    = 0x00000002,
    FEATURE_VIRBND = 0x00000004,
    FEATURE_TBCHK  = 0x00000008,
};

enum {
    EXCP_RESET            = 0x0000,
    EXCP_MCHK             = 0x0020,
    EXCP_ARITH            = 0x0060,
    EXCP_HW_INTERRUPT     = 0x00E0,
    EXCP_DFAULT           = 0x01E0,
    EXCP_DTB_MISS_PAL     = 0x09E0,
    EXCP_ITB_MISS         = 0x03E0,
    EXCP_ITB_ACV          = 0x07E0,
    EXCP_DTB_MISS_NATIVE  = 0x08E0,
    EXCP_UNALIGN          = 0x11E0,
    EXCP_OPCDEC           = 0x13E0,
    EXCP_FEN              = 0x17E0,
    EXCP_CALL_PAL         = 0x2000,
    EXCP_CALL_PALP        = 0x3000,
    EXCP_CALL_PALE        = 0x4000,
    /* Pseudo exception for console */
    EXCP_CONSOLE_DISPATCH = 0x4001,
    EXCP_CONSOLE_FIXUP    = 0x4002,
};

/* Arithmetic exception */
enum {
    EXCP_ARITH_OVERFLOW,
};

enum {
    PALCODE_CALL = 0x00000000,
    PALCODE_LD   = 0x01000000,
    PALCODE_ST   = 0x02000000,
    PALCODE_MFPR = 0x03000000,
    PALCODE_MTPR = 0x04000000,
    PALCODE_REI  = 0x05000000,
    PALCODE_INIT = 0xF0000000,
};

enum {
    IR_V0   = 0,
    IR_T0   = 1,
    IR_T1   = 2,
    IR_T2   = 3,
    IR_T3   = 4,
    IR_T4   = 5,
    IR_T5   = 6,
    IR_T6   = 7,
    IR_T7   = 8,
    IR_S0   = 9,
    IR_S1   = 10,
    IR_S2   = 11,
    IR_S3   = 12,
    IR_S4   = 13,
    IR_S5   = 14,
    IR_S6   = 15,
#define IR_FP IR_S6
    IR_A0   = 16,
    IR_A1   = 17,
    IR_A2   = 18,
    IR_A3   = 19,
    IR_A4   = 20,
    IR_A5   = 21,
    IR_T8   = 22,
    IR_T9   = 23,
    IR_T10  = 24,
    IR_T11  = 25,
    IR_RA   = 26,
    IR_T12  = 27,
#define IR_PV IR_T12
    IR_AT   = 28,
    IR_GP   = 29,
    IR_SP   = 30,
    IR_ZERO = 31,
};

CPUAlphaState * cpu_alpha_init (const char *cpu_model);
int cpu_alpha_exec(CPUAlphaState *s);
/* you can call this signal handler from your SIGBUS and SIGSEGV
   signal handlers to inform the virtual CPU of exceptions. non zero
   is returned if the signal was handled by the virtual CPU.  */
int cpu_alpha_signal_handler(int host_signum, void *pinfo,
                             void *puc);
int cpu_alpha_mfpr (CPUState *env, int iprn, uint64_t *valp);
int cpu_alpha_mtpr (CPUState *env, int iprn, uint64_t val, uint64_t *oldvalp);
void cpu_loop_exit (void);
void pal_init (CPUState *env);
void call_pal (CPUState *env, int palcode);

#endif /* !defined (__CPU_ALPHA_H__) */
