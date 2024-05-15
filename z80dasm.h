
#ifndef __Z80DASM_H
#define __Z80DASM_H

#ifdef __cplusplus
extern "C" {
#endif

int Z80Dasm (char *instr, const unsigned char *op, int f, int pc);

#define Z80DASM_ASPC	0x100
#define Z80DASM_PCVAL	0x200
#define Z80DASM_IPSEP	0x400

#ifdef __cplusplus
}
#endif

#endif
