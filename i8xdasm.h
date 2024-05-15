
#ifndef __I8XDASM_H
#define __I8XDASM_H

#ifdef __cplusplus
extern "C" {
#endif

#define i80Dasm(instr, op, f) i8xDasm (instr, op, f)

int i8xDasm (char *instr, const unsigned char op[3], int f);

#define I8XDASM_ASPC	0x200

#define I8XDASM_85	0x100
#define i85Dasm(instr, op, f) i8xDasm (instr, op, (f) | I8XDASM_85)

#ifdef __cplusplus
}
#endif

#endif
