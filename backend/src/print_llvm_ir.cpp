#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "syntax_tree.h"
#include "llvm_ir.h"
#include "defines.h"
#include "print_llvm_ir.h"

static void printGlobals(LLVMContext* context, FILE* llvm_ir_code);
static void printFunctions(LLVMContext* context, FILE* llvm_ir_code);
static void printFunctionBody(LLVMContext* context, LLVMFunction* function, FILE* llvm_ir_code);
static void printBlock(LLVMContext* context, LLVMBlock* block, FILE* llvm_ir_code);

void printLLVMIR(LLVMContext* context, FILE* llvm_ir_code){
    if (context == NULL || llvm_ir_code == NULL) return;

    printGlobals(context, llvm_ir_code);
    printFunctions(context, llvm_ir_code);
}

static void printGlobals(LLVMContext* context, FILE* llvm_ir_code){
    for (int i = 0; i < context->global_amount; i++){
        fprintf(llvm_ir_code, "@gl_%d = global i32 %d\n", context->globals[i].index, context->globals[i].value);
    }

    if (context->global_amount > 0) fprintf(llvm_ir_code, "\n");
}

static void printFunctions(LLVMContext* context, FILE* llvm_ir_code){
    for (int i = 0; i < context->function_amount; i++){
        fprintf(llvm_ir_code, "define i32 @func_%d(", context->functions[i].index);

        for (int j = 0; j < context->functions[i].arguments_amount; j++){
            if (j != 0) fprintf(llvm_ir_code, ", ");
            fprintf(llvm_ir_code, "i32 %%%d", j);
        }

        fprintf(llvm_ir_code, ") {\n");

        printFunctionBody(context, context->functions + i, llvm_ir_code);

        fprintf(llvm_ir_code, "}\n\n");
    }
}

static void printFunctionBody(LLVMContext* context, LLVMFunction* function, FILE* llvm_ir_code){
    for (int i = 0; i < function->block_amount; i++){
        printBlock(context, function->blocks + i, llvm_ir_code);
    }
}

static void printBlock(LLVMContext* context, LLVMBlock* block, FILE* llvm_ir_code){
    fprintf(llvm_ir_code, "%d:\n", block->index);

    for (int i = 0; i < block->instruction_amount; i++){
        LLVMInstruction* instruction = block->instructions + i;

        switch (instruction->operation){
            case LLVM_ALLOCA:           PRINT_ALLOCA(llvm_ir_code, (*instruction)); break;
            case LLVM_STORE:            PRINT_STORE(llvm_ir_code, (*instruction)); break;
            case LLVM_LOAD:             PRINT_LOAD(llvm_ir_code, (*instruction)); break;
            case LLVM_RETURN:           PRINT_RETURN(llvm_ir_code, (*instruction)); break;
            case LLVM_ADD:              PRINT_BIN_OPERATION(llvm_ir_code, (*instruction)); break;
            case LLVM_SUB:              PRINT_BIN_OPERATION(llvm_ir_code, (*instruction)); break;
            case LLVM_MUL:              PRINT_BIN_OPERATION(llvm_ir_code, (*instruction)); break;
            case LLVM_DIV:              PRINT_BIN_OPERATION(llvm_ir_code, (*instruction)); break;
            case LLVM_EQ:               PRINT_CMP_OPERATION(llvm_ir_code, (*instruction)); break;
            case LLVM_NEQ:              PRINT_CMP_OPERATION(llvm_ir_code, (*instruction)); break;
            case LLVM_LT:               PRINT_CMP_OPERATION(llvm_ir_code, (*instruction)); break;
            case LLVM_LTEQ:             PRINT_CMP_OPERATION(llvm_ir_code, (*instruction)); break;
            case LLVM_GT:               PRINT_CMP_OPERATION(llvm_ir_code, (*instruction)); break;
            case LLVM_GTEQ:             PRINT_CMP_OPERATION(llvm_ir_code, (*instruction)); break;
            case LLVM_CONDITIONAL_JUMP: PRINT_CONDITIONAL_JUMP(llvm_ir_code, (*instruction)); break;
            case LLVM_JUMP:             PRINT_JUMP(llvm_ir_code, (*instruction)); break;
            case LLVM_IN:               PRINT_IN(llvm_ir_code, (*instruction)); break;
            case LLVM_OUT:              PRINT_OUT(llvm_ir_code, (*instruction)); break;
            case LLVM_BREAK:            PRINT_JUMP(llvm_ir_code, (*instruction)); break;
            case LLVM_CONTINUE:         PRINT_JUMP(llvm_ir_code, (*instruction)); break;
            case LLVM_CALL:             PRINT_CALL(llvm_ir_code, (*instruction)); break;
            default: break;
        }
    }
}
