//===-- NVPTXutil.cpp - Functions exported to CodeGen --*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the functions that can be used in CodeGen.
//
//===----------------------------------------------------------------------===//

#include "NVPTXutil.h"
//===-- NVPTX.h - Top-level interface for NVPTX representation --*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the entry points for global functions defined in
// the LLVM NVPTX back-end.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_TARGET_NVPTX_H
#define LLVM_TARGET_NVPTX_H

#include "llvm/Value.h"
#include "llvm/Module.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Target/TargetMachine.h"
#include "MCTargetDesc/NVPTXBaseInfo.h"
#include <cassert>
#include <iosfwd>

namespace llvm
{
class NVPTXTargetMachine;
class FunctionPass;
class formatted_raw_ostream;

namespace NVPTXCC
{
enum CondCodes
{
    EQ,
    NE,
    LT,
    LE,
    GT,
    GE
};
}

inline static const char *NVPTXCondCodeToString(NVPTXCC::CondCodes CC)
{
    switch (CC)
    {
    case NVPTXCC::NE:
        return "ne";
    case NVPTXCC::EQ:
        return "eq";
    case NVPTXCC::LT:
        return "lt";
    case NVPTXCC::LE:
        return "le";
    case NVPTXCC::GT:
        return "gt";
    case NVPTXCC::GE:
        return "ge";
    }
    llvm_unreachable("Unknown condition code");
}

FunctionPass *createNVPTXISelDag(NVPTXTargetMachine &TM,
                                 llvm::CodeGenOpt::Level OptLevel);
FunctionPass *createVectorElementizePass(NVPTXTargetMachine &);
FunctionPass *createLowerStructArgsPass(NVPTXTargetMachine &);
FunctionPass *createNVPTXReMatPass(NVPTXTargetMachine &);
FunctionPass *createNVPTXReMatBlockPass(NVPTXTargetMachine &);

bool isImageOrSamplerVal(const Value *, const Module *);

extern Target TheNVPTXTarget32;
extern Target TheNVPTXTarget64;

namespace NVPTX
{
enum DrvInterface
{
    NVCL,
    CUDA,
    TEST
};

// A field inside TSFlags needs a shift and a mask. The usage is
// always as follows :
// ((TSFlags & fieldMask) >> fieldShift)
// The enum keeps the mask, the shift, and all valid values of the
// field in one place.
enum VecInstType
{
    VecInstTypeShift = 0,
    VecInstTypeMask = 0xF,

    VecNOP = 0,
    VecLoad = 1,
    VecStore = 2,
    VecBuild = 3,
    VecShuffle = 4,
    VecExtract = 5,
    VecInsert = 6,
    VecDest = 7,
    VecOther = 15
};

enum SimpleMove
{
    SimpleMoveMask = 0x10,
    SimpleMoveShift = 4
};
enum LoadStore
{
    isLoadMask = 0x20,
    isLoadShift = 5,
    isStoreMask = 0x40,
    isStoreShift = 6
};

namespace PTXLdStInstCode
{
enum AddressSpace
{
    GENERIC = 0,
    GLOBAL = 1,
    CONSTANT = 2,
    SHARED = 3,
    PARAM = 4,
    LOCAL = 5
};
enum FromType
{
    Unsigned = 0,
    Signed,
    Float
};
enum VecType
{
    Scalar = 1,
    V2 = 2,
    V4 = 4
};
}
}
} // end namespace llvm;

// Defines symbolic names for NVPTX registers.  This defines a mapping from
// register name to register number.
#define GET_REGINFO_ENUM
#include "NVPTXGenRegisterInfo.inc"

// Defines symbolic names for the NVPTX instructions.
#define GET_INSTRINFO_ENUM
#include "NVPTXGenInstrInfo.inc"

#endif


using namespace llvm;

namespace llvm
{

bool isParamLoad(const MachineInstr *MI)
{
    if ((MI->getOpcode() != NVPTX::LD_i32_avar) &&
            (MI->getOpcode() != NVPTX::LD_i64_avar))
        return false;
    if (MI->getOperand(2).isImm() == false)
        return false;
    if (MI->getOperand(2).getImm() != NVPTX::PTXLdStInstCode::PARAM)
        return false;
    return true;
}

#define DATA_MASK     0x7f
#define DIGIT_WIDTH   7
#define MORE_BYTES    0x80

static int encode_leb128(uint64_t val, int *nbytes,
                         char *space, int splen)
{
    char *a;
    char *end = space + splen;

    a = space;
    do
    {
        unsigned char uc;

        if (a >= end)
            return 1;
        uc = val & DATA_MASK;
        val >>= DIGIT_WIDTH;
        if (val != 0)
            uc |= MORE_BYTES;
        *a = uc;
        a++;
    }
    while (val);
    *nbytes = a - space;
    return 0;
}

#undef DATA_MASK
#undef DIGIT_WIDTH
#undef MORE_BYTES

uint64_t encode_leb128(const char *str)
{
    union
    {
        uint64_t x;
        char a[8];
    } temp64;

    temp64.x = 0;

    for (unsigned i=0,e=strlen(str); i!=e; ++i)
        temp64.a[i] = str[e-1-i];

    char encoded[16];
    int nbytes;

    int retval = encode_leb128(temp64.x, &nbytes, encoded, 16);

    (void)retval;
    assert(retval == 0 &&
           "Encoding to leb128 failed");

    assert(nbytes <= 8 &&
           "Cannot support register names with leb128 encoding > 8 bytes");

    temp64.x = 0;
    for (int i=0; i<nbytes; ++i)
        temp64.a[i] = encoded[i];

    return temp64.x;
}

} // end namespace llvm
