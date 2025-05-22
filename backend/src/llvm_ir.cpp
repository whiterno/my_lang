#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "syntax_tree.h"
#include "llvm_ir.h"
#include "defines.h"

static void addGlobal(LLVMContext* context, Node* node);
static void addFunction(LLVMContext* context, Node* node);
static int countLocalVariables(Node* node);
static LLVMLocal* setArguments(LLVMFunction* function, Node* node);
static LLVMLocal* setLocalVariables(LLVMFunction* function, Node* node);
static void setLocalVar(LLVMLocal* local_variables, Node* node, int* index, int arg_amount);
static int countBlocks(Node* node);
static void setInstructions(LLVMContext* context, int index, Node* node, LLVMBlock* blocks);
static void setIfBranch(LLVMContext* context, int index, Node* node, LLVMBlock* blocks);
static void setWhileBranch(LLVMContext* context, int index, Node* node, LLVMBlock* blocks);
static void setBreakContinue(LLVMBlock* blocks, LLVMOperand continue_index, LLVMOperand break_index, int from_index, int to_index);
static void setVarDecl(LLVMContext* context, int index, Node* node, LLVMBlock* block);
static void setKeywords(LLVMContext* context, int index, Node* node, LLVMBlock* block);
static void instructionsCtor(LLVMContext* context, int index);
static void insertInstruction(LLVMBlock* block, LLVMInstruction instruction);
static LLVMBlock* setBlocks(LLVMContext* context, int index, Node* node);
static int setBlock(LLVMContext* context, int index, Node* node, LLVMBlock* blocks);
static int findLocalRegister(LLVMContext* context, int index, int var_index);
static int findGlobal(LLVMContext* context, int var_index);
static LLVMOperand setExpression(LLVMContext* context, int index, Node* node, LLVMBlock* block);
static LLVMOperand setCallArguments(LLVMContext* context, int index, Node* node, LLVMBlock* block);
static int countCallArguments(Node* node);
static LLVMOperation getInstructionType(Node* node);
static void llvmContextDtor(LLVMContext* context);
static LLVMContext* llvmContextCtor();

LLVMContext* generateLLVMIR(Node* node){
    if (node == NULL) return NULL;

    LLVMContext* context = llvmContextCtor();
    if (context == NULL) return NULL;

    while (node){
        if (_TYPE(_L(node)) == VAR_DECLARATION)
            addGlobal(context, _R(_L(node)));

        if (_TYPE(_L(node)) == FUNCTION_DEFINITION)
            addFunction(context, _L(node));

        node = _R(node);
    }

    return context;
}

static LLVMContext* llvmContextCtor(){
    LLVMContext* context = (LLVMContext*)calloc(1, sizeof(LLVMContext));
    CHECK_ALLOC(context);

    GLOBALS = NULL;
    GLOBAL_AMOUNT = 0;

    FUNCS = NULL;
    FUNC_AMOUNT = 0;

    return context;
}

static void addGlobal(LLVMContext* context, Node* node){
    if (node == NULL) return;

    GLOBAL_AMOUNT++;
    GLOBALS = (LLVMGlobal*)realloc(context->globals, context->global_amount * sizeof(LLVMGlobal));
    CHECK_ALLOC(context->globals);

    GLOBALS[GLOBAL_AMOUNT - 1].index = _INX(_R(node));
    GLOBALS[GLOBAL_AMOUNT - 1].value = _NUM(_L(node));
}

static void addFunction(LLVMContext* context, Node* node){
    if (node == NULL) return;

    FUNC_AMOUNT++;
    FUNCS = (LLVMFunction*)realloc(context->functions, context->function_amount * sizeof(LLVMFunction));
    CHECK_ALLOC(context->functions);

    size_t index = context->function_amount - 1;
    FUNC_INDEX = _INX(node);
    V_REG_AMOUNT = 0;

    L_VAR_AMOUNT = countLocalVariables(_R(_R(node)));
    ARG_AMOUNT = countCallArguments(_L(_R(node)));
    L_VARS = setLocalVariables(context->functions + index, node);

    BLOCK_AMOUNT = countBlocks(node) + 1;
    BLOCKS = setBlocks(context, index, node);
}

static int countLocalVariables(Node* node){
    if (node == NULL) return 0;

    int amount = 0;
    if (_TYPE(node) == VAR_DECLARATION)
        amount++;

    return amount + countLocalVariables(_L(node)) + countLocalVariables(_R(node));
}

static LLVMLocal* setLocalVariables(LLVMFunction* function, Node* node){
    if (node == NULL) return NULL;
    if (function->local_variables_amount + function->arguments_amount == 0) return NULL;

    int amount = function->local_variables_amount + function->arguments_amount;
    LLVMLocal* local_variables = (LLVMLocal*)calloc(amount, sizeof(LLVMLocal));
    CHECK_ALLOC(local_variables);

    node = _R(node);
    int index = 0;

    setLocalVar(local_variables, node, &index, function->arguments_amount);

    return local_variables;
}

static void setLocalVar(LLVMLocal* local_variables, Node* node, int* index, int arg_amount){
    if (node == NULL) return;

    if (_TYPE(node) == VAR_DECLARATION){
        if (_TYPE(_P(node)) == KEYWORD && _KWD(_P(node)) == COMMA) local_variables[*index].variable_index = _INX(_R(node));
        else local_variables[*index].variable_index = _INX(_R(_R(node)));
        local_variables[*index].register_index = *index + arg_amount + 1;

        (*index)++;

        return;
    }

    setLocalVar(local_variables, _L(node), index, arg_amount);
    setLocalVar(local_variables, _R(node), index, arg_amount);
}

static int countBlocks(Node* node){
    if (node == NULL) return 0;

    int amount = 0;
    if (_TYPE(node) == KEYWORD && _KWD(node) == WHILE)
        amount += 3;
    else if (_TYPE(node) == KEYWORD && _KWD(node) == IF)
        amount += 2;

    return amount + countBlocks(_L(node)) + countBlocks(_R(node));
}

static void instructionsCtor(LLVMContext* context, int index){
    assert(context);

    for (int i = 0; i < BLOCK_AMOUNT; i++){
        BLOCKS[i].instructions = (LLVMInstruction*)calloc(BASE_INSTRUCTION_AMOUNT, sizeof(LLVMInstruction));
        CHECK_ALLOC(BLOCKS[i].instructions);

        BLOCKS[i].instruction_capacity = BASE_INSTRUCTION_AMOUNT;
        BLOCKS[i].instruction_amount = 0;
    }
}

static void insertInstruction(LLVMBlock* block, LLVMInstruction instruction){
    if (block->instruction_amount == block->instruction_capacity){
        block->instruction_capacity *= 2;
        block->instructions = (LLVMInstruction*)realloc(block->instructions, block->instruction_capacity * sizeof(LLVMInstruction));
        CHECK_ALLOC(block->instructions);
    }

    block->instructions[block->instruction_amount] = instruction;
    block->instruction_amount++;
}

static LLVMBlock* setBlocks(LLVMContext* context, int index, Node* node){
    if (node == NULL) return NULL;

    LLVMBlock* blocks = (LLVMBlock*)calloc(BLOCK_AMOUNT, sizeof(LLVMBlock));
    CHECK_ALLOC(blocks);

    context->functions[index].blocks = blocks;

    instructionsCtor(context, index);

    V_REG_AMOUNT = ARG_AMOUNT + 1;
    BLOCK_AMOUNT = 0;

    for (int i = 0; i < L_VAR_AMOUNT + ARG_AMOUNT; i++, V_REG_AMOUNT++){
        insertInstruction(&blocks[0], ALLOCA_INSTRUCTION);
    }
    for (int i = 0; i < ARG_AMOUNT; i++){
        insertInstruction(&blocks[0], STORE_INSTRUCTION);
    }

    setBlock(context, index, _R(_R(node)), blocks);

    return blocks;
}

static int setBlock(LLVMContext* context, int index, Node* node, LLVMBlock* blocks){
    int block_index = BLOCK_AMOUNT++;

    blocks[block_index].index = V_REG_AMOUNT++;
    if (node == NULL) return blocks[block_index].index;

    if (V_REG_AMOUNT - 1 == L_VAR_AMOUNT + 2 * ARG_AMOUNT + 1) {
        blocks[block_index].index = ARG_AMOUNT;
        V_REG_AMOUNT--;
    }
    setInstructions(context, index, node, blocks);

    return blocks[block_index].index;
}

static void setInstructions(LLVMContext* context, int index, Node* node, LLVMBlock* blocks){
    if (node == NULL) return;

    if (_TYPE(node) == KEYWORD && _KWD(node) == SEMICOLON){
        setInstructions(context, index, _L(node), blocks);

        if (_TYPE(_L(node)) == KEYWORD && (_KWD(_L(node)) == IF)){
            setIfBranch(context, index, node, blocks);
        }
        else if (_TYPE(_L(node)) == KEYWORD && (_KWD(_L(node)) == WHILE)){
            setWhileBranch(context, index, node, blocks);
        }
        else setInstructions(context, index, _R(node), blocks);

        return;
    }

    switch(_TYPE(node)){
        case(VAR_DECLARATION):{
            setVarDecl(context, index, _R(node), blocks + BLOCK_AMOUNT - 1);
            break;
        }
        case(KEYWORD):{
            setKeywords(context, index, node, blocks + BLOCK_AMOUNT - 1);
            break;
        }
        default: break;
    }
}

static void setIfBranch(LLVMContext* context, int index, Node* node, LLVMBlock* blocks){
    if (node == NULL) return;

    int parent = BLOCK_AMOUNT - 1;

    LLVMOperand condition = setExpression(context, index, _L(_L(node)), blocks + parent);
    LLVMInstruction branch = {.operation = LLVM_CONDITIONAL_JUMP};
    branch.operand1 = condition;

    branch.operand2 = LOCAL_OP(setBlock(context, index, _R(_L(node)), blocks));

    LLVMInstruction jump = {.operation = LLVM_JUMP};
    jump.operand1 = LOCAL_OP(V_REG_AMOUNT);
    insertInstruction(blocks + BLOCK_AMOUNT - 1, jump);

    branch.operand3 = LOCAL_OP(setBlock(context, index, _R(node), blocks));

    insertInstruction(blocks + parent, branch);
}

static void setWhileBranch(LLVMContext* context, int index, Node* node, LLVMBlock* blocks){
    if (node == NULL) return;

    LLVMOperand condition_index = LOCAL_OP(V_REG_AMOUNT);
    LLVMInstruction to_condition_branch = {.operation = LLVM_JUMP};
    to_condition_branch.operand1 = condition_index;

    insertInstruction(blocks + BLOCK_AMOUNT - 1, to_condition_branch);

    BLOCK_AMOUNT++;
    blocks[BLOCK_AMOUNT - 1].index = V_REG_AMOUNT++;

    int condition_array_index = BLOCK_AMOUNT - 1;

    LLVMOperand condition = setExpression(context, index, _L(_L(node)), blocks + BLOCK_AMOUNT - 1);
    LLVMInstruction condition_branch = {.operation = LLVM_CONDITIONAL_JUMP};
    condition_branch.operand1 = condition;
    condition_branch.operand2 = LOCAL_OP(V_REG_AMOUNT);

    int while_body_array_index = BLOCK_AMOUNT;
    LLVMOperand while_body_index = LOCAL_OP(V_REG_AMOUNT);

    setBlock(context, index, _R(_L(node)), blocks);

    condition_branch.operand3 = LOCAL_OP(setBlock(context, index, _R(node), blocks));

    setBreakContinue(blocks, condition_index, condition_branch.operand3, while_body_array_index, BLOCK_AMOUNT - 1);

    insertInstruction(blocks + condition_array_index, condition_branch);
    insertInstruction(blocks + BLOCK_AMOUNT - 2, to_condition_branch);
}

static void setBreakContinue(LLVMBlock* blocks, LLVMOperand continue_index, LLVMOperand break_index, int from_index, int to_index){
    for (int i = from_index; i < to_index; i++){
        for (int j = 0; j < blocks[i].instruction_amount; j++){
            if (blocks[i].instructions[j].operation == LLVM_BREAK){
                blocks[i].instructions[j].operation = LLVM_JUMP;
                blocks[i].instructions[j].operand1 = break_index;
            }
            else if (blocks[i].instructions[j].operation == LLVM_CONTINUE){
                blocks[i].instructions[j].operation = LLVM_JUMP;
                blocks[i].instructions[j].operand1 = continue_index;
            }
        }
    }
}

static void setVarDecl(LLVMContext* context, int index, Node* node, LLVMBlock* block){
    if (node == NULL) return;

    LLVMOperand result = setExpression(context, index, _L(node), block);

    int register_index = findLocalRegister(context, index, _INX(_R(node)));
    LLVMOperand operand2 = {};
    if (register_index == -1){
        register_index = findGlobal(context, _INX(_R(node)));
        operand2 = GLOBAL_OP(register_index);
    }
    else operand2 = LOCAL_OP(register_index);

    LLVMInstruction store = {.operation = LLVM_STORE,
                             .operand1 = result,
                             .operand2 = operand2};
    insertInstruction(block, store);
}

static void setKeywords(LLVMContext* context, int index, Node* node, LLVMBlock* block){
    if (node == NULL) return;

    switch (_KWD(node)){
        case ASSIGN:{
            LLVMOperand result = setExpression(context, index, _R(node), block);

            int reg_index = findLocalRegister(context, index, _INX(_L(node)));
            LLVMOperand operand2 = {};
            if (reg_index == -1){
                reg_index = findGlobal(context, _INX(_L(node)));
                operand2 = GLOBAL_OP(reg_index);
            }
            else operand2 = LOCAL_OP(reg_index);

            LLVMInstruction store = {.operation = LLVM_STORE,
                                    .operand1 = result,
                                    .operand2 = operand2};
            insertInstruction(block, store);
            break;
        }
        case IN:{
            LLVMInstruction in = {.operation = LLVM_IN};
            in.operand1 = setExpression(context, index, _L(node), block);
            insertInstruction(block, in);
            break;
        }
        case OUT:{
            LLVMInstruction out = {.operation = LLVM_OUT};
            out.operand1 = setExpression(context, index, _L(node), block);
            insertInstruction(block, out);
            break;
        }
        case RETURN:{
            LLVMInstruction ret = {.operation = LLVM_RETURN};
            ret.operand1 = setExpression(context, index, _R(node), block);
            insertInstruction(block, ret);
            break;
        }
        case BREAK:{
            LLVMInstruction break_instruction = {.operation = LLVM_BREAK};
            insertInstruction(block, break_instruction);
            break;
        }
        case CONTINUE:{
            LLVMInstruction continue_instruction = {.operation = LLVM_CONTINUE};
            insertInstruction(block, continue_instruction);
            break;
        }
        default: break;
    }
}

static int findLocalRegister(LLVMContext* context, int index, int var_index){
    if (context == NULL) return -1;

    for (int i = 0; i < L_VAR_AMOUNT + ARG_AMOUNT; i++){
        if (L_VARS[i].variable_index == var_index)
            return L_VARS[i].register_index;
    }

    return -1;
}

static int findGlobal(LLVMContext* context, int var_index){
    if (context == NULL) return -1;

    for (int i = 0; i < GLOBAL_AMOUNT; i++){
        if (GLOBALS[i].index == var_index)
            return GLOBALS[i].index;
    }

    return -1;
}

static LLVMOperand setExpression(LLVMContext* context, int index, Node* node, LLVMBlock* block){
    if (node == NULL) return {};

    LLVMInstruction instruction = {};

    if (_TYPE(node) == KEYWORD){
        instruction = {.operation = getInstructionType(node)};

        if (instruction.operation == LLVM_IN){
            instruction.operand1 = LOCAL_OP(V_REG_AMOUNT++);
            insertInstruction(block, instruction);
        }
        else{
            LLVMOperand operand2 = setExpression(context, index, _L(node), block);
            LLVMOperand operand3 = setExpression(context, index, _R(node), block);

            instruction.operand1 = LOCAL_OP(V_REG_AMOUNT++);
            instruction.operand2 = operand2;
            instruction.operand3 = operand3;
            insertInstruction(block, instruction);
        }
    }
    else if (_TYPE(node) == CALL){
        instruction = {.operation = LLVM_CALL};
        instruction.operand1 = LOCAL_OP(V_REG_AMOUNT++);
        instruction.operand2 = setCallArguments(context, index, node, block);

        insertInstruction(block, instruction);
    }
    else if (_TYPE(node) == CONSTANT){
        instruction.operand1 = CONST_OP(_NUM(node));
    }
    else if (_TYPE(node) == IDENTIFIER){
        int reg_index = findLocalRegister(context, index, _INX(node));
        if (reg_index == -1){
            printf("");
            reg_index = findGlobal(context, _INX(node));
            instruction.operand2 = GLOBAL_OP(reg_index);
        }
        else instruction.operand2 = LOCAL_OP(reg_index);

        instruction.operand1 = LOCAL_OP(V_REG_AMOUNT++);
        instruction.operation = LLVM_LOAD;

        insertInstruction(block, instruction);
    }

    return instruction.operand1;
}

static LLVMOperand setCallArguments(LLVMContext* context, int index, Node* node, LLVMBlock* block){
    if (node == NULL) return {};

    LLVMOperand result = {.type = LLVM_ARGUMENTS};
    LLVMCall call = {};

    call.function_index = _INX(_R(node));
    call.argument_amount = countCallArguments(_L(node));

    call.arguments = (LLVMOperand*)calloc(call.argument_amount, sizeof(LLVMOperand));
    CHECK_ALLOC(call.arguments);

    node = _L(node);
    for (int i = 0; i < call.argument_amount; i++){
        call.arguments[i] = setExpression(context, index, _L(node), block);
        node = _R(node);
    }

    result.value.call = call;

    return result;
}

static int countCallArguments(Node* node){
    if (node == NULL) return 0;

    int amount = 0;
    while (node){
        node = _R(node);
        amount++;
    }

    return amount;
}

static LLVMOperation getInstructionType(Node* node){
    if (node == NULL) return {};

    switch (_KWD(node)){
        case ASSIGN: return LLVM_STORE;
        case ADD:    return LLVM_ADD;
        case SUB:    return LLVM_SUB;
        case MULT:   return LLVM_MUL;
        case DIV:    return LLVM_DIV;
        case GREATER:return LLVM_GT;
        case LESS:   return LLVM_LT;
        case EQUAL:  return LLVM_EQ;
        case G_OR_EQ:return LLVM_GTEQ;
        case L_OR_EQ:return LLVM_LTEQ;
        case N_EQ:   return LLVM_NEQ;
        case IN:     return LLVM_IN;
        case OUT:    return LLVM_OUT;
        default:     return LLVM_ERROR;
    }
}

static void llvmContextDtor(LLVMContext* context){
    if (context == NULL) return;

    free(context->globals);
    free(context->functions);
    free(context);
}

