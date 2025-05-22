#ifndef PRINT_LLVM_IR_H
#define PRINT_LLVM_IR_H

#include "llvm_ir.h"
#include "stdio.h"

void printLLVMIR(LLVMContext* context, FILE* llvm_ir_code);

#define PRINT_INSTRUCTION(llvm_ir_code, instruction)                \
    do {                                                            \
        switch (instruction.operation){                             \
            case LLVM_ADD: fprintf(llvm_ir_code, "add"); break;     \
            case LLVM_SUB: fprintf(llvm_ir_code, "sub"); break;     \
            case LLVM_MUL: fprintf(llvm_ir_code, "mul"); break;     \
            case LLVM_DIV: fprintf(llvm_ir_code, "div"); break;     \
            case LLVM_EQ:  fprintf(llvm_ir_code, "eq");  break;     \
            case LLVM_NEQ: fprintf(llvm_ir_code, "ne"); break;      \
            case LLVM_LT:  fprintf(llvm_ir_code, "slt");  break;    \
            case LLVM_LTEQ:fprintf(llvm_ir_code, "sle");break;      \
            case LLVM_GT:  fprintf(llvm_ir_code, "sgt");  break;    \
            case LLVM_GTEQ:fprintf(llvm_ir_code, "sge");break;      \
            case LLVM_CONDITIONAL_JUMP:fprintf(llvm_ir_code, "br");break; \
            case LLVM_JUMP: fprintf(llvm_ir_code, "br"); break;     \
            default:       fprintf(llvm_ir_code, "error");break;    \
        }                                                           \
    } while (0)

#define PRINT_OPERAND(llvm_ir_code, operand)                                                            \
    do {                                                                                                \
        switch (operand.type){                                                                          \
            case LLVM_CONSTANT: fprintf(llvm_ir_code, "%d", operand.value.immediate_value); break;      \
            case LLVM_REGISTER: fprintf(llvm_ir_code, "%%%d", operand.value.register_index); break;     \
            case LLVM_GLOBAL:   fprintf(llvm_ir_code, "@gl_%d", operand.value.register_index); break;   \
            default: break;                                                                             \
        }                                                                                               \
    } while (0)

#define PRINT_BIN_OPERATION(llvm_ir_code, instruction)                                                  \
    do {                                                                                                \
        fprintf(llvm_ir_code, "\t");                                                                    \
        PRINT_OPERAND(llvm_ir_code, instruction.operand1);                                              \
        fprintf(llvm_ir_code, " = ");                                                                   \
        PRINT_INSTRUCTION(llvm_ir_code, instruction);                                                   \
        fprintf(llvm_ir_code, " i32 ");                                                                 \
        PRINT_OPERAND(llvm_ir_code, instruction.operand2);                                              \
        fprintf(llvm_ir_code, ", ");                                                                    \
        PRINT_OPERAND(llvm_ir_code, instruction.operand3);                                              \
        fprintf(llvm_ir_code, "\n");                                                                    \
    } while (0)

#define PRINT_CMP_OPERATION(llvm_ir_code, instruction)                                                  \
    do {                                                                                                \
        fprintf(llvm_ir_code, "\t");                                                                    \
        PRINT_OPERAND(llvm_ir_code, instruction.operand1);                                              \
        fprintf(llvm_ir_code, " = icmp ");                                                              \
        PRINT_INSTRUCTION(llvm_ir_code, instruction);                                                   \
        fprintf(llvm_ir_code, " i32 ");                                                                 \
        PRINT_OPERAND(llvm_ir_code, instruction.operand2);                                              \
        fprintf(llvm_ir_code, ", ");                                                                    \
        PRINT_OPERAND(llvm_ir_code, instruction.operand3);                                              \
        fprintf(llvm_ir_code, "\n");                                                                    \
    } while (0)

#define PRINT_CONDITIONAL_JUMP(llvm_ir_code, instruction)                                               \
    do {                                                                                                \
        fprintf(llvm_ir_code, "\t");                                                                    \
        PRINT_INSTRUCTION(llvm_ir_code, instruction);                                                   \
        fprintf(llvm_ir_code, " i1 ");                                                                  \
        PRINT_OPERAND(llvm_ir_code, instruction.operand1);                                              \
        fprintf(llvm_ir_code, ", label ");                                                              \
        PRINT_OPERAND(llvm_ir_code, instruction.operand2);                                              \
        fprintf(llvm_ir_code, ", label ");                                                              \
        PRINT_OPERAND(llvm_ir_code, instruction.operand3);                                              \
        fprintf(llvm_ir_code, "\n");                                                                    \
    } while (0)

#define PRINT_JUMP(llvm_ir_code, instruction)                                                           \
    do {                                                                                                \
        fprintf(llvm_ir_code, "\t");                                                                    \
        PRINT_INSTRUCTION(llvm_ir_code, instruction);                                                   \
        fprintf(llvm_ir_code, " label ");                                                               \
        PRINT_OPERAND(llvm_ir_code, instruction.operand1);                                              \
        fprintf(llvm_ir_code, "\n");                                                                    \
    } while (0)

#define PRINT_ALLOCA(llvm_ir_code, instruction)                                                         \
    do {                                                                                                \
        fprintf(llvm_ir_code, "\t%%%d = alloca i32\n", instruction.operand1.value.register_index);      \
    } while (0)

#define PRINT_STORE(llvm_ir_code, instruction)                                                          \
    do {                                                                                                \
        fprintf(llvm_ir_code, "\tstore i32 ");                                                          \
        PRINT_OPERAND(llvm_ir_code, instruction.operand1);                                              \
        fprintf(llvm_ir_code, ", i32 ptr ");                                                            \
        PRINT_OPERAND(llvm_ir_code, instruction.operand2);                                              \
        fprintf(llvm_ir_code, "\n");                                                                    \
    } while (0)

#define PRINT_LOAD(llvm_ir_code, instruction)                                                           \
    do {                                                                                                \
        fprintf(llvm_ir_code, "\t");                                                                    \
        PRINT_OPERAND(llvm_ir_code, instruction.operand1);                                              \
        fprintf(llvm_ir_code, " = load i32, ptr ");                                                     \
        PRINT_OPERAND(llvm_ir_code, instruction.operand2);                                              \
        fprintf(llvm_ir_code, "\n");                                                                    \
    } while (0)

#define PRINT_RETURN(llvm_ir_code, instruction)                                                         \
    do {                                                                                                \
        fprintf(llvm_ir_code, "\tret i32 ");                                                            \
        PRINT_OPERAND(llvm_ir_code, instruction.operand1);                                              \
        fprintf(llvm_ir_code, "\n");                                                                    \
    } while (0)

#define PRINT_IN(llvm_ir_code, instruction)                                                             \
    do {                                                                                                \
        fprintf(llvm_ir_code, "\t");                                                                    \
        PRINT_OPERAND(llvm_ir_code, instruction.operand1);                                              \
        fprintf(llvm_ir_code, " = i32 input\n");                                                        \
        break;                                                                                          \
    } while (0)

#define PRINT_OUT(llvm_ir_code, instruction)                                                            \
    do {                                                                                                \
        fprintf(llvm_ir_code, "\toutput i32 ");                                                         \
        PRINT_OPERAND(llvm_ir_code, instruction.operand1);                                              \
        fprintf(llvm_ir_code, "\n");                                                                    \
    } while (0)

#define PRINT_CALL(llvm_ir_code, instruction)                                                          \
    do {                                                                                                \
        fprintf(llvm_ir_code, "\t");                                                                    \
        PRINT_OPERAND(llvm_ir_code, instruction.operand1);                                              \
        fprintf(llvm_ir_code, " = call i32 @func_%d(", instruction.operand2.value.call.function_index);  \
        for (int j = 0; j < instruction.operand2.value.call.argument_amount; j++){                      \
            if (j != 0) fprintf(llvm_ir_code, ", ");                                                    \
            PRINT_OPERAND(llvm_ir_code, instruction.operand2.value.call.arguments[j]);                  \
        }                                                                                               \
        fprintf(llvm_ir_code, ")\n");                                                                   \
    } while (0)

#endif
