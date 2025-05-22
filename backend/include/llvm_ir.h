#ifndef LLVM_IR_H
#define LLVM_IR_H

const int BASE_INSTRUCTION_AMOUNT = 10;

enum LLVMOperandType{
    LLVM_CONSTANT,
    LLVM_REGISTER,
    LLVM_GLOBAL,
    LLVM_ARGUMENTS
};

enum LLVMOperation{
    LLVM_ADD,
    LLVM_SUB,
    LLVM_MUL,
    LLVM_DIV,

    LLVM_EQ,
    LLVM_NEQ,
    LLVM_LT,
    LLVM_LTEQ,
    LLVM_GT,
    LLVM_GTEQ,

    LLVM_ALLOCA,
    LLVM_LOAD,
    LLVM_STORE,

    LLVM_CALL,

    LLVM_JUMP,
    LLVM_CONDITIONAL_JUMP,

    LLVM_BREAK,
    LLVM_CONTINUE,

    LLVM_RETURN,

    LLVM_IN,
    LLVM_OUT,
    LLVM_HLT,

    LLVM_ERROR
};

typedef struct LLVMOperand LLVMOperand;

typedef struct LLVMCall{
    int function_index;
    int argument_amount;
    LLVMOperand* arguments;
} LLVMCall;

typedef struct LLVMOperand{
    LLVMOperandType type;

    union{
        int immediate_value;
        int register_index;
        int global_index;
        LLVMCall call;
    } value;

} LLVMOperand;

typedef struct LLVMInstruction{
    LLVMOperation operation;
    LLVMOperand operand1;
    LLVMOperand operand2;
    LLVMOperand operand3;
} LLVMInstruction;

typedef struct LLVMLocal{
    int variable_index;
    int register_index;
} LLVMNameTable;

typedef struct LLVMBlock{
    int index;
    LLVMInstruction* instructions;
    int instruction_amount;
    int instruction_capacity;
} LLVMBlock;

typedef struct LLVMGlobal{
    int index;
    int value;
} LLVMGlobal;

typedef struct LLVMFunction{
    int index;

    int virtual_register_amount;
    int local_variables_amount;
    int arguments_amount;
    LLVMLocal* local_variables;

    int block_amount;
    LLVMBlock* blocks;
} LLVMFunction;

typedef struct LLVMContext{
    LLVMGlobal* globals;
    int global_amount;

    LLVMFunction* functions;
    int function_amount;
} LLVMContext;

#define V_REG_AMOUNT context->functions[index].virtual_register_amount
#define L_VAR_AMOUNT context->functions[index].local_variables_amount
#define ARG_AMOUNT context->functions[index].arguments_amount
#define BLOCK_AMOUNT context->functions[index].block_amount

#define L_VARS context->functions[index].local_variables
#define ARGS context->functions[index].arguments
#define BLOCKS context->functions[index].blocks

#define FUNC_INDEX context->functions[index].index

#define FUNC context->functions + index

#define GLOBALS context->globals
#define GLOBAL_AMOUNT context->global_amount

#define FUNCS context->functions
#define FUNC_AMOUNT context->function_amount

#define ALLOCA_INSTRUCTION {.operation = LLVM_ALLOCA,                                   \
                            .operand1 = {.type = LLVM_REGISTER,                         \
                                         .value.register_index = V_REG_AMOUNT}}
#define STORE_INSTRUCTION {.operation = LLVM_STORE,                                     \
                            .operand1 = {.type = LLVM_REGISTER,                         \
                                         .value.register_index = i},                    \
                            .operand2 = {.type = LLVM_REGISTER,                         \
                                         .value.register_index = ARG_AMOUNT + 1 + i}}

#define GLOBAL_OP(inx) {.type = LLVM_GLOBAL, .value.global_index = inx}
#define LOCAL_OP(inx) {.type = LLVM_REGISTER, .value.register_index = inx}
#define CONST_OP(val) {.type = LLVM_CONSTANT, .value.immediate_value = val}

LLVMContext* generateLLVMIR(Node* node);

#endif
