/*
 *
 * Nano RISC-V 32i emulator
 * Copyright (C) Dmitry 'MatrixS_Master' Solovyev, 2020
 *
 * This work is licensed under the MIT License. See included LICENSE file
 *
 * */

#ifndef RISCV_H_
#define RISCV_H_

#define RV_NUMREGS 32

#define RV_ENCODE_SYM_DONT_CARE ' '
#define RV_ENCODE_SYM_IMM_START '<'

// I made this to make sign extend easily optimizable by a compiler - should be converted into 3 or 4 instructions
#define RV_EXTEND(X,B) ((int32_t)( ((X) & (1U << (B))) ? ((X) | (((1U << (32 - ((B)+1))) - 1) << ((B)+1))) : (X) ))

typedef enum {
    RV_LUI,
    RV_AUIPC,
    RV_JAL,
    RV_JALR,
    RV_BEQ,
    RV_BNE,
    RV_BLT,
    RV_BGE,
    RV_BLTU,
    RV_BGEU,
    RV_LB,
    RV_LH,
    RV_LW,
    RV_LBU,
    RV_LHU,
    RV_SB,
    RV_SH,
    RV_SW,
    RV_ADDI,
    RV_SLTI,
    RV_SLTIU,
    RV_XORI,
    RV_ORI,
    RV_ANDI,
    RV_SLLI,
    RV_SRLI,
    RV_SRAI,
    RV_ADD,
    RV_SUB,
    RV_SLL,
    RV_SLT,
    RV_SLTU,
    RV_XOR,
    RV_SRL,
    RV_SRA,
    RV_OR,
    RV_AND,
    RV_FENCE,
    RV_ECALL,
    RV_EBREAK
} riscv_op;

static const char* riscv_encode[] = {
//  "12345678123456781234567812345678",
    "[ZYXWVUTSRQPONMLKJIH     0110111",
    "[ZYXWVUTSRQPONMLKJIH     0010111",
    "PFEDCBA@?>=GONMLKJIH     1101111",
    "GFEDCBA@?>=<     000     1100111",
    "HFEDCBA          000@?>=G1100011",
    "HFEDCBA          001@?>=G1100011",
    "HFEDCBA          100@?>=G1100011",
    "HFEDCBA          101@?>=G1100011",
    "HFEDCBA          110@?>=G1100011",
    "HFEDCBA          111@?>=G1100011",
    "GFEDCBA@?>=<     000     0000011",
    "GFEDCBA@?>=<     001     0000011",
    "GFEDCBA@?>=<     010     0000011",
    "GFEDCBA@?>=<     100     0000011",
    "GFEDCBA@?>=<     101     0000011",
    "GFEDCBA          000@?>=<0100011",
    "GFEDCBA          001@?>=<0100011",
    "GFEDCBA          010@?>=<0100011",
    "GFEDCBA@?>=<     000     0010011",
    "GFEDCBA@?>=<     010     0010011",
    "GFEDCBA@?>=<     011     0010011",
    "GFEDCBA@?>=<     100     0010011",
    "GFEDCBA@?>=<     110     0010011",
    "GFEDCBA@?>=<     111     0010011",
    "0000000          001     0010011",
    "0000000          101     0010011",
    "0100000          101     0010011",
    "0000000          000     0110011",
    "0100000          000     0110011",
    "0000000          001     0110011",
    "0000000          010     0110011",
    "0000000          011     0110011",
    "0000000          100     0110011",
    "0000000          101     0110011",
    "0100000          101     0110011",
    "0000000          110     0110011",
    "0000000          111     0110011",
    "                 000     0001111",
    "00000000000000000000000001110011",
    "00000000000100000000000001110011"
};

static const char* riscv_names[] = {
    "LUI",
    "AUIPC",
    "JAL",
    "JALR",
    "BEQ",
    "BNE",
    "BLT",
    "BGE",
    "BLTU",
    "BGEU",
    "LB",
    "LH",
    "LW",
    "LBU",
    "LHU",
    "SB",
    "SH",
    "SW",
    "ADDI",
    "SLTI",
    "SLTIU",
    "XORI",
    "ORI",
    "ANDI",
    "SLLI",
    "SRLI",
    "SRAI",
    "ADD",
    "SUB",
    "SLL",
    "SLT",
    "SLTU",
    "XOR",
    "SRL",
    "SRA",
    "OR",
    "AND",
    "FENCE",
    "ECALL",
    "EBREAK"
};

#if 1
static const char* riscv_useregs[] = {
    "100",
    "100",
    "100",
    "110",
    "011",
    "011",
    "011",
    "011",
    "011",
    "011",
    "110",
    "110",
    "110",
    "110",
    "110",
    "011",
    "011",
    "011",
    "110",
    "110",
    "110",
    "110",
    "110",
    "110",
    "110",
    "110",
    "110",
    "111",
    "111",
    "111",
    "111",
    "111",
    "111",
    "111",
    "111",
    "111",
    "111",
    "110",
    "000",
    "000"
};
#endif

typedef enum {
    RVR_ZERO,
    RVR_RA,
    RVR_SP,
    RVR_GP,
    RVR_TP,
    RVR_T0,
    RVR_T1,
    RVR_T2,
    RVR_S0,
    RVR_S1,
    RVR_A0,
    RVR_A1,
    RVR_A2,
    RVR_A3,
    RVR_A4,
    RVR_A5,
    RVR_A6,
    RVR_A7,
    RVR_S2,
    RVR_S3,
    RVR_S4,
    RVR_S5,
    RVR_S6,
    RVR_S7,
    RVR_S8,
    RVR_S9,
    RVR_S10,
    RVR_S11,
    RVR_T3,
    RVR_T4,
    RVR_T5,
    RVR_T6
} riscv_reg;

#if 1
static const char* riscv_regname[32] = {
    "zero",
    "ra",
    "sp",
    "gp",
    "tp",
    "t0",
    "t1",
    "t2",
    "s0/fp",
    "s1",
    "a0",
    "a1",
    "a2",
    "a3",
    "a4",
    "a5",
    "a6",
    "a7",
    "s2",
    "s3",
    "s4",
    "s5",
    "s6",
    "s7",
    "s8",
    "s9",
    "s10",
    "s11",
    "t3",
    "t4",
    "t5",
    "t6"
};
#endif

typedef enum {
    RVSYS_CLOSE = 57,
    RVSYS_WRITE = 64,
    RVSYS_FSTAT = 80,
    RVSYS_EXIT = 93,
    RVSYS_BRK = 214,
} riscv_sys;

#endif /* RISCV_H_ */
