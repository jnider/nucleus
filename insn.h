#ifndef NUCLEUS_INSN_H
#define NUCLEUS_INSN_H

#include <stdio.h>
#include <stdint.h>

#include <capstone/capstone.h>

#include <string>
#include <vector>

#include "edge.h"

#define INS_FLAG_CFLOW 0x001
#define INS_FLAG_COND 0x002
#define INS_FLAG_INDIRECT 0x004
#define INS_FLAG_JMP 0x008
#define INS_FLAG_CALL 0x010
#define INS_FLAG_RET 0x020
#define INS_FLAG_NOP  0x040

enum OperandType
{
    OP_TYPE_NONE,
    OP_TYPE_REG,
    OP_TYPE_IMM,
    OP_TYPE_MEM,
    OP_TYPE_FP
};

union AArch64Value {
    arm64_reg    reg;
    int32_t      imm;
    double       fp;
    arm64_op_mem mem;
};

union ARMValue {
    arm_reg    reg;
    int32_t    imm;
    double     fp;
    arm_op_mem mem;
};

union MIPSValue {
    mips_reg    reg;
    int32_t     imm;
    double      fp;
    mips_op_mem mem;
};

union PPCValue {
    ppc_reg    reg;
    int32_t    imm;
    ppc_op_mem mem;
};

union X86Value {
    x86_reg    reg;
    int64_t    imm;
    double     fp;
    x86_op_mem mem;
};

class Operand
{
public:
	Operand() : type(OP_TYPE_NONE) {}
	virtual ~Operand() {}

public:
	OperandType type;
	uint8_t size;
};

class AArch64Operand : public Operand
{
public:
    AArch64Operand() { value.reg = ARM64_REG_INVALID; }
    AArch64Operand(const AArch64Value &v) { value.mem.base = v.mem.base;
      value.mem.index = v.mem.index; value.mem.disp = v.mem.disp; }

public:
    AArch64Value value; /* Only set if the arch is aarch64 */
};

class ARMOperand: public Operand
{
public:
    ARMOperand() { value.reg = ARM_REG_INVALID; }
    ARMOperand(const ARMValue &v) { value.mem.base = v.mem.base; value.mem.index = v.mem.index;
      value.mem.scale = v.mem.scale; value.mem.disp = v.mem.disp; }

public:
    ARMValue value; /* Only set if the arch is arm */
};

class MIPSOperand : public Operand
{
public:
    MIPSOperand() { value.reg = MIPS_REG_INVALID; }
    MIPSOperand(const MIPSValue &v) { value.mem.base = v.mem.base; value.mem.disp = v.mem.disp; }

public:
    MIPSValue value; /* Only set if the arch is mips */
};

class PPCOperand : public Operand
{
public:
    PPCOperand() { value.reg = PPC_REG_INVALID; }
    PPCOperand(const PPCValue &v) { value.mem.base = v.mem.base; value.mem.disp = v.mem.disp; }
public:
    PPCValue value; /* Only set if the arch is ppc */
};

class X86Operand : public virtual Operand
{
public:
    X86Operand() { value.reg = X86_REG_INVALID; }
    X86Operand(const X86Value &v) { value.mem.segment = v.mem.segment; value.mem.base = v.mem.base;
      value.mem.index = v.mem.index; value.mem.scale = v.mem.scale;
      value.mem.disp = v.mem.disp; }
public:
    X86Value value; /* Only set if the arch is x86 */

};



class Instruction {
public:
  Instruction() : id(0), start(0), size(0), addr_size(0), target(0), flags(0), invalid(false), privileged(false), trap(false) {}
  Instruction(const Instruction &i) : id(i.id), start(i.start), size(i.size), addr_size(i.addr_size), target(i.target), flags(i.flags),
                                      mnem(i.mnem), op_str(i.op_str), operands(i.operands), invalid(i.invalid), privileged(i.privileged), trap(i.trap) {}

  void           print     (FILE *out);
  Edge::EdgeType edge_type ();

  unsigned int         id;
  uint64_t             start;
  uint8_t              size;
  uint8_t              addr_size;
  uint64_t             target;
  unsigned short       flags;
  std::string          mnem;
  std::string          op_str;
  std::vector<Operand> operands;
  bool                 invalid;
  bool                 privileged;
  bool                 trap;
};

class X86Instruction : public Instruction {
public:
  static const uint8_t MAX_LEN = 16;

  X86Instruction() : Instruction() {}
  X86Instruction(const X86Instruction &i) : Instruction(i) {}
};

#endif /* NUCLEUS_INSN_H */

