/**
 * Gedare Bloom
 * Shumba Brown
 * Skylar Grammas
 * Deeshai Escoffery
 * Tashambra Williams
 * cpu.c
 *
 * Implementation of simulated processor.
 */

#include "cpu.h"
#include "memory.h"
#include "stdio.h"
#include "syscall.h"
#include <math.h>


struct cpu_context cpu_ctx;
struct cpu_counter cpu_cntr;
struct instructionCache iCache;
struct dataCache dCache;

int fetch( struct IF_ID_buffer *out )
{
    // Grab pc from global state.
    uint32_t pc = cpu_ctx.PC;

    // Grab instruction at pc from memory.
    uint32_t instruction = instructionCache(&pc);

    // Increment PC.
    pc = pc + 1;



    out->next_pc = pc;
    out->instruction = instruction;
    return 0;
}

int decode( struct IF_ID_buffer *in, struct ID_EX_buffer *out )
{
    struct instruction instrn;
    parse_instruction(&in->instruction, &instrn);

    out->next_pc = in->next_pc;
    out->read_data_1 = cpu_ctx.GPR[instrn.rs];
    out->read_data_2 = cpu_ctx.GPR[instrn.rt];
    cpu_cntr.cycle += 1;

    switch (instrn.op) {
    //r-type
    case 0:
        switch (instrn.funct) {
        //add
        case 32:
            out->control_signals.RegDst = 1;
            out->control_signals.ALU_src = 0;
            out->control_signals.MemToReg = 0;
            out->control_signals.RegWrite = 1;
            out->control_signals.MemRead = 0;
            out->control_signals.MemWrite = 0;
            out->control_signals.Branch = 0;
            out->control_signals.ALU_op = 2;
            out->control_signals.jump = -1;
            out->op = 32;
            out->shamt = 0;
            out->rt = instrn.rt;
            out->rd = instrn.rd;
            out->rs = instrn.rs;
            cpu_cntr.add_c += 1;
            break;
        //and
        case 36:
            out->control_signals.RegDst = 1;
            out->control_signals.ALU_src = 0;
            out->control_signals.MemToReg = 0;
            out->control_signals.RegWrite = 1;
            out->control_signals.MemRead = 0;
            out->control_signals.MemWrite = 0;
            out->control_signals.Branch = 0;
            out->control_signals.ALU_op = 0;
            out->control_signals.jump = -1;
            out->op = 36;
            out->shamt = 0;
            out->rt = instrn.rt;
            out->rd = instrn.rd;
            out->rs = instrn.rs;
            cpu_cntr.and_c += 1;
            break;
        //nor
        case 39:
            out->control_signals.RegDst = 1;
            out->control_signals.ALU_src = 0;
            out->control_signals.MemToReg = 0;
            out->control_signals.RegWrite = 1;
            out->control_signals.MemRead = 0;
            out->control_signals.MemWrite = 0;
            out->control_signals.Branch = 0;
            out->control_signals.ALU_op = 12;
            out->control_signals.jump = -1;
            out->op = 39;
            out->shamt = 0;
            out->rt = instrn.rt;
            out->rd = instrn.rd;
            out->rs = instrn.rs;
            cpu_cntr.nor_c += 1;
            break;
        //or
        case 37:
            out->control_signals.RegDst = 1;
            out->control_signals.ALU_src = 0;
            out->control_signals.MemToReg = 0;
            out->control_signals.RegWrite = 1;
            out->control_signals.MemRead = 0;
            out->control_signals.MemWrite = 0;
            out->control_signals.Branch = 0;
            out->control_signals.ALU_op = 1;
            out->control_signals.jump = -1;
            out->op = 37;
            out->shamt = 0;
            out->rt = instrn.rt;
            out->rd = instrn.rd;
            out->rs = instrn.rs;
            cpu_cntr.or_c += 1;
            break;
        //slt
        case 42:
            out->control_signals.RegDst = 1;
            out->control_signals.ALU_src = 0;
            out->control_signals.MemToReg = 0;
            out->control_signals.RegWrite = 1;
            out->control_signals.MemRead = 0;
            out->control_signals.MemWrite = 0;
            out->control_signals.Branch = 0;
            out->control_signals.ALU_op = 7;
            out->control_signals.jump = -1;
            out->op = 42;
            out->shamt = 0;
            out->rt = instrn.rt;
            out->rd = instrn.rd;
            out->rs = instrn.rs;
            cpu_cntr.slt_c += 1;
            break;
        //sll
        case 0:
            out->control_signals.RegDst = 1;
            out->control_signals.ALU_src = 0;
            out->control_signals.MemToReg = 0;
            out->control_signals.RegWrite = 1;
            out->control_signals.MemRead = 0;
            out->control_signals.MemWrite = 0;
            out->control_signals.Branch = 0;
            out->control_signals.ALU_op = 3;
            out->control_signals.jump = -1;
            out->op = 0;
            out->shamt = instrn.shamt;
            out->rt = instrn.rt;
            out->rd = instrn.rd;
            out->rs = instrn.rs;
            if (instrn.rt == 0 && instrn.rd == 0) { //nop
                cpu_cntr.nop_c += 1;
            } else { //sll operation
                cpu_cntr.sll_c += 1;
            }
            //cpu_cntr.sll_c += 1;
            break;
        //srl
        case 2:
            out->control_signals.RegDst = 1;
            out->control_signals.ALU_src = 0;
            out->control_signals.MemToReg = 0;
            out->control_signals.RegWrite = 1;
            out->control_signals.MemRead = 0;
            out->control_signals.MemWrite = 0;
            out->control_signals.Branch = 0;
            out->control_signals.ALU_op = 8;
            out->control_signals.jump = -1;
            out->op = 2;
            out->shamt = instrn.shamt;
            out->rt = instrn.rt;
            out->rd = instrn.rd;
            out->rs = instrn.rs;
            cpu_cntr.srl_c += 1;
            break;
        //sub
        case 34:
            out->control_signals.RegDst = 1;
            out->control_signals.ALU_src = 0;
            out->control_signals.MemToReg = 0;
            out->control_signals.RegWrite = 1;
            out->control_signals.MemRead = 0;
            out->control_signals.MemWrite = 0;
            out->control_signals.Branch = 0;
            out->control_signals.ALU_op = 6;
            out->control_signals.jump = -1;
            out->op = 34;
            out->shamt = 0;
            out->rt = instrn.rt;
            out->rd = instrn.rd;
            out->rs = instrn.rs;
            cpu_cntr.sub_c += 1;
            break;
        //sra
        case 3:
            out->control_signals.RegDst = 1;
            out->control_signals.ALU_src = 0;
            out->control_signals.MemToReg = 0;
            out->control_signals.RegWrite = 1;
            out->control_signals.MemRead = 0;
            out->control_signals.MemWrite = 0;
            out->control_signals.Branch = 0;
            out->control_signals.ALU_op = 9;
            out->control_signals.jump = -1;
            out->op = 3;
            out->shamt = instrn.shamt;
            out->rt = instrn.rt;
            out->rd = instrn.rd;
            out->rs = instrn.rs;
            cpu_cntr.sra_c += 1;
            break;
        //xor
        case 38:
            out->control_signals.RegDst = 1;
            out->control_signals.ALU_src = 0;
            out->control_signals.MemToReg = 0;
            out->control_signals.RegWrite = 1;
            out->control_signals.MemRead = 0;
            out->control_signals.MemWrite = 0;
            out->control_signals.Branch = 0;
            out->control_signals.ALU_op = 4;
            out->control_signals.jump = -1;
            out->op = 38;
            out->shamt = 0;
            out->rt = instrn.rt;
            out->rd = instrn.rd;
            out->rs = instrn.rs;
            cpu_cntr.xor_c += 1;
            break;
        //syscall
        case 12:
            out->control_signals.RegDst = 1;
            out->control_signals.ALU_src = 0;
            out->control_signals.MemToReg = 0;
            out->control_signals.RegWrite = 0;
            out->control_signals.MemRead = 0;
            out->control_signals.MemWrite = 0;
            out->control_signals.Branch = 0;
            out->control_signals.ALU_op = 13;
            out->control_signals.jump = -1;
            out->op = 12;
            out->shamt = 0;
            out->rt = instrn.rt;
            out->rd = instrn.rd;
            out->rs = instrn.rs;
            cpu_cntr.syscall_c += 1;
            syscall();
            break;
        //jr (8)
        case 8:
            out->control_signals.RegDst = -1;
            out->control_signals.ALU_src = -1;
            out->control_signals.MemToReg = -1;
            out->control_signals.RegWrite = 0;
            out->control_signals.MemRead = 0;
            out->control_signals.MemWrite = 0;
            out->control_signals.Branch = 0;
            out->control_signals.ALU_op = 5;
            out->control_signals.jump = -1;
            out->op = 8;
            out->shamt = 0;
            out->rt = instrn.rt;
            out->rd = instrn.rd;
            out->rs = instrn.rs;
            cpu_cntr.jr_c += 1;
            break;
        }
        break;
    //i-type
    //addi
    case 8:
        out->control_signals.RegDst = 1;
        out->control_signals.ALU_src = 1;
        out->control_signals.MemToReg = 0;
        out->control_signals.RegWrite = 1;
        out->control_signals.MemRead = 0;
        out->control_signals.MemWrite = 0;
        out->control_signals.Branch = 0;
        out->control_signals.ALU_op = 2;
        out->op = 8;
        out->shamt = 0;
        out->rt = instrn.rt;
        out->rd = instrn.rd;
        out->rs = instrn.rs;
        cpu_cntr.addi_c += 1;
        break;
    //andi
    case 12:
        out->control_signals.RegDst = 0;
        out->control_signals.ALU_src = 1;
        out->control_signals.MemToReg = 0;
        out->control_signals.RegWrite = 1;
        out->control_signals.MemRead = 0;
        out->control_signals.MemWrite = 0;
        out->control_signals.Branch = 0;
        out->control_signals.ALU_op = 0;
        out->control_signals.jump = -1;
        out->op = 12;
        out->shamt = 0;
        out->rt = instrn.rt;
        out->rd = instrn.rd;
        out->rs = instrn.rs;
        cpu_cntr.andi_c += 1;
        break;
    //beq
    case 4:
        out->control_signals.RegDst = -1;
        out->control_signals.ALU_src = 0;
        out->control_signals.MemToReg = -1;
        out->control_signals.RegWrite = 0;
        out->control_signals.MemRead = 0;
        out->control_signals.MemWrite = 0;
        out->control_signals.Branch = 1;
        out->control_signals.ALU_op = 6;
        out->control_signals.jump = -1;
            out->op = 4;
        out->shamt = 0;
        out->rt = instrn.rt;
        out->rd = instrn.rd;
        out->rs = instrn.rs;
        cpu_cntr.beq_c += 1;
        break;
    //bne
    case 5:
        out->control_signals.RegDst = -1;
        out->control_signals.ALU_src = 0;
        out->control_signals.MemToReg = -1;
        out->control_signals.RegWrite = 0;
        out->control_signals.MemRead = 0;
        out->control_signals.MemWrite = 0;
        out->control_signals.Branch = 1;
        out->control_signals.ALU_op = 6;
        out->control_signals.jump = -1;
            out->op = 5;
        out->shamt = 0;
        out->rt = instrn.rt;
        out->rd = instrn.rd;
        out->rs = instrn.rs;
        cpu_cntr.beq_c += 1;
        break;
    //lui
    case 15:
        out->control_signals.RegDst = 0;
        out->control_signals.ALU_src = 1;
        out->control_signals.MemToReg = 0;
        out->control_signals.RegWrite = 1;
        out->control_signals.MemRead = 0;
        out->control_signals.MemWrite = 0;
        out->control_signals.Branch = 0;
        out->control_signals.ALU_op = 14;
        out->control_signals.jump = -1;
        out->op = 15;
        out->shamt = 0;
        out->rt = instrn.rt;
        out->rd = instrn.rd;
        out->rs = instrn.rs;
        cpu_cntr.lui_c += 1;
        break;
    //lw
    case 35:
        out->control_signals.RegDst = 0;
        out->control_signals.ALU_src = 1;
        out->control_signals.MemToReg = 1;
        out->control_signals.RegWrite = 1;
        out->control_signals.MemRead = 1;
        out->control_signals.MemWrite = 0;
        out->control_signals.Branch = 0;
        out->control_signals.ALU_op = 2;
        out->control_signals.jump = -1;
            out->op = 35;
        out->shamt = 0;
        out->rt = instrn.rt;
        out->rd = instrn.rd;
        out->rs = instrn.rs;
        cpu_cntr.lw_c += 1;
        break;
    //ori
    case 13:
        out->control_signals.RegDst = 0;
        out->control_signals.ALU_src = 1;
        out->control_signals.MemToReg = 0;
        out->control_signals.RegWrite = 1;
        out->control_signals.MemRead = 0;
        out->control_signals.MemWrite = 0;
        out->control_signals.Branch = 0;
        out->control_signals.ALU_op = 1;
        out->control_signals.jump = -1;
            out->op = 13;
        out->shamt = 0;
        out->rt = instrn.rt;
        out->rd = instrn.rd;
        out->rs = instrn.rs;
        cpu_cntr.ori_c += 1;
        break;
    //slti
    case 10:
        out->control_signals.RegDst = 0;
        out->control_signals.ALU_src = 1;
        out->control_signals.MemToReg = 0;
        out->control_signals.RegWrite = 1;
        out->control_signals.MemRead = 0;
        out->control_signals.MemWrite = 0;
        out->control_signals.Branch = 0;
        out->control_signals.ALU_op = 7;
        out->control_signals.jump = -1;
            out->op = 10;
        out->shamt = 0;
        out->rt = instrn.rt;
        out->rd = instrn.rd;
        out->rs = instrn.rs;
        cpu_cntr.slti_c += 1;
        break;
    //xori
    case 14:
        out->control_signals.RegDst = 0;
        out->control_signals.ALU_src = 1;
        out->control_signals.MemToReg = 0;
        out->control_signals.RegWrite = 1;
        out->control_signals.MemRead = 0;
        out->control_signals.MemWrite = 0;
        out->control_signals.Branch = 0;
        out->control_signals.ALU_op = 4;
        out->control_signals.jump = -1;
            out->op = 14;
        out->shamt = 0;
        out->rt = instrn.rt;
        out->rd = instrn.rd;
        out->rs = instrn.rs;
        cpu_cntr.xori_c += 1;
        break;
    //sw
    case 43:
        out->control_signals.RegDst = -1;
        out->control_signals.ALU_src = 1;
        out->control_signals.MemToReg = -1;
        out->control_signals.RegWrite = 0;
        out->control_signals.MemRead = 0;
        out->control_signals.MemWrite = 1;
        out->control_signals.Branch = 0;
        out->control_signals.ALU_op = 2;
        out->control_signals.jump = -1;
            out->op = 43;
        out->shamt = 0;
        out->rt = instrn.rt;
        out->rd = instrn.rd;
        out->rs = instrn.rs;
        cpu_cntr.sw_c += 1;
        break;
    //j-type
    //j
    case 2:
        out->control_signals.RegDst = -1;
        out->control_signals.ALU_src = -1;
        out->control_signals.MemToReg = -1;
        out->control_signals.RegWrite = 0;
        out->control_signals.MemRead = -1;
        out->control_signals.MemWrite = 0;
        out->control_signals.Branch = -1;
        out->control_signals.ALU_op = -1;
        out->control_signals.jump = 1;
        out->op = 2;
        out->shamt = 0;
        out->rt = instrn.rt;
        out->rd = instrn.rd;
        out->rs = instrn.rs;
        cpu_cntr.j_c += 1;
        break;
    //jal (3)
    default:
        out->control_signals.RegDst = 2;
        out->control_signals.ALU_src = -1;
        out->control_signals.MemToReg = 2;
        out->control_signals.RegWrite = 1;
        out->control_signals.MemRead = -1;
        out->control_signals.MemWrite = 0;
        out->control_signals.Branch = -1;
        out->control_signals.ALU_op = -1;
        out->control_signals.jump = 1;
        out->op = 3;
        out->shamt = 0;
        out->rt = instrn.rt;
        out->rd = instrn.rd;
        out->rs = instrn.rs;
        cpu_cntr.jal_c += 1;
        break;
    }

    SIGN_EXTEND(&instrn.bit_number, &out->sign_extend);

    out->regDst0 = instrn.rt;
    out->regDst1 = instrn.rd;
    out->bit_address = instrn.bit_address;

    if(out->control_signals.jump == 1) {
        if (instrn.bit_address < 1024 && out->op !=8) {
            cpu_ctx.PC = instrn.bit_address;
        } else {
            printf("error");
        }
        if (out->op == 8) {
            cpu_ctx.PC = cpu_ctx.GPR[instrn.rs];
        }
    }
    return 0;
}

int execute( struct ID_EX_buffer *in, struct EX_MEM_buffer *out )
{
    // RegDst MUX
    struct MUX_input reg_dst_m_in;
    struct MUX_output reg_dst_m_out;

    reg_dst_m_in.in0 = in->regDst0;
    reg_dst_m_in.in1 = in->regDst1;
    reg_dst_m_in.in2 = 31;
    reg_dst_m_in.select = in->control_signals.RegDst;
    MUX(&reg_dst_m_in, &reg_dst_m_out);
    out->registerDestination = reg_dst_m_out.output;
    // ALUSRC
    struct MUX_input alu_src_m_in;
    struct MUX_output alu_src_m_out;

    alu_src_m_in.in0 = in->read_data_2;
    alu_src_m_in.in1 = in->sign_extend;
    alu_src_m_in.select = in->control_signals.ALU_src;

    MUX(&alu_src_m_in, &alu_src_m_out);


    struct ALU_input pre;
    struct ALU_output post;
    if (((in->control_signals.ALU_op == 3) && (in->op == 0)) || ((in->control_signals.ALU_op == 8) && (in->op == 2)) || ((in->control_signals.ALU_op == 9) && (in->op == 3))) { //if sll or srl or sra
        pre.in1 = cpu_ctx.GPR[in->rt];
    } else {
        pre.in1 = in->read_data_1;
    }
    if (((in->control_signals.ALU_op == 3) && (in->op == 0)) || ((in->control_signals.ALU_op == 8) && (in->op == 2)) || ((in->control_signals.ALU_op == 9) && (in->op == 3))) { //if sll or srl or sra
        pre.in2 = in->shamt;
    } else {
        pre.in2 = alu_src_m_out.output;
    }

    if (in->op == 8) {
        cpu_ctx.PC = cpu_ctx.GPR[in->rs];
    }

    pre.control = in->control_signals.ALU_op;


    ALU(&pre, &post);
    if (in->op == 4) { //beq
        if (post.result == 0) {
            post.zero = 1;
        }
    } else if (in->op == 5) { //bne
        if (post.result != 0) {
            post.zero = 1;
        }
    }

    out->control_signals = in->control_signals;
    out->next_pc = in->next_pc;
    out->ALU_result = post.result;
    out->read_data_2 = in->read_data_2;
    out->ALU_zero = post.zero;
    out->sign_extend = in->sign_extend;
    out->registerDestination = reg_dst_m_out.output;

    return 0;
}

int memory( struct EX_MEM_buffer *in, struct MEM_WB_buffer *out )
{
    // If MemRead then get data from data memory
    if (in->control_signals.MemRead == 1) {
      if (in->ALU_result >= 1024 && in->ALU_result < 0) {
      } else {
        printf("ALUUU:::: %d", in->ALU_result);

        uint32_t result = in->ALU_result;
        out->data = dataCache(&result);
        printf("resutl:::: %x", result);
        // out->data = data_memory[in->ALU_result];
      }
    }
    // If MemWrite then write to the data memeory
    if (in->control_signals.MemWrite == 1) {
      data_memory[in->ALU_result] = in->read_data_2;
    }

    // If Branch then update pc
    if (in->ALU_zero == 1 && in->control_signals.Branch == 1) {
        cpu_ctx.PC = in->next_pc + in->sign_extend;
    } else {
        cpu_ctx.PC += 1;
    }

    out->registerDestination = in->registerDestination;
    out->ALU_result = in->ALU_result;
    out->control_signals = in->control_signals;

    out->next_pc = in->next_pc;
    return 0;
}

int writeback( struct MEM_WB_buffer *in )
{
    // If mem to reg then write data in register.
    printf("rd:%d\n", in->registerDestination);

    if (in->control_signals.RegWrite == 1) {
        cpu_ctx.GPR[in->registerDestination] = in->ALU_result;
    }
    if (in->control_signals.RegWrite == 0) {
        //cpu_ctx.GPR[in->registerDestination] = in->ALU_result;
    }
    if (in->control_signals.MemToReg == 2) {
        cpu_ctx.GPR[in->registerDestination] = in->next_pc + 1;
    }
    if (in->control_signals.MemToReg == 1) {
        cpu_ctx.GPR[in->registerDestination] = in->data;
    }

    return 0;
}

int ALU(struct ALU_input *in, struct ALU_output *out)
{

    switch (in->control) {
        // AND (and, andi, lw, sw)
        case 0:
            out->result = in->in1 & in->in2;
            out->zero = 0;
            break;
        // OR (or, ori)
        case 1:
            out->result = in->in1 | in->in2;
            out->zero = 0;
            break;
        // ADD (add, addi)
        case 2:
            out->result = in->in1 + in->in2;
            out->zero = 0;
            break;
        // SHIFT LEFT (sll)
        case 3:
            out->result =  in->in1 << in->in2;
            out->zero = 0;
            break;
        // XOR (xor, xori)
        case 4:
            out->result = in->in1 ^ in->in2;
            out->zero = 0;
            break;
        // SUBTRACT (sub, beq, bne)
        case 6:
            out->result = in->in1 - in->in2;
            out->zero = 0;
            break;
        // SET ON LESS THAN (slt, slti)
        case 7:
            if (in->in1 < in->in2) {
                out->result = 1;
            } else {
                out->result = 0;
            }
            out->zero = 0;
            break;
        // SHIFT RIGHT LOGICAL (srl)
        case 8:
            out->result = in->in1 >> in->in2;
            out->zero = 0;
            break;
        // SHIFT RIGHT ARITHMETIC (sra)
        case 9:
            if ((int)in->in1 < 0) {
                for (int i = 0; i < in->in2; i++) {
                    in->in1 = in->in1 >> 1;
                    in->in1 = in->in1 | 0x80000000;
                }
                out->result = in->in1;
            } else {
                out->result = in->in1 >> in->in2;
            }
            break;
        // NOT OR (nor)
        case 12:
            out->result = ~(in->in1 | in->in2);
            out->zero = 0;
            break;
        // LOAD UPPER IMMEDIATE (lui)
        case 14:
            out->result = (in->in2 << 16);
            out->zero = 0;
            break;
    }
    return 0;
}

int MUX(struct MUX_input *in, struct MUX_output *out)
{
    switch (in->select) {
        case 0:
            out->output = in->in0;
            break;
        case 1:
            out->output = in->in1;
            break;
        case 2:
            out->output = in->in2;
            break;
    }
    return 0;
}

int parse_instruction(uint32_t *bits, struct instruction *fields) {
    fields->op = *bits >> 26;

    fields->rs = *bits << 6;
    fields->rs = fields->rs >> 27;

    fields->rt = *bits << 11;
    fields->rt = fields->rt >> 27;

    fields->rd = *bits << 16;
    fields->rd = fields->rd >> 27;

    fields->shamt = *bits << 21;
    fields->shamt = fields->shamt >> 27;

    fields->funct = *bits << 26;
    fields->funct = fields->funct >> 26;

    fields->bit_number = *bits << 16;
    fields->bit_number = fields->bit_number >> 16;

    fields->bit_address = *bits << 6;
    fields->bit_address = fields->bit_address >> 6;

    return 0;
};

int SIGN_EXTEND(uint32_t *in, uint32_t *out)
{
    if ((*in >> 15) == 1) {
        *out = *in | 0xFFFF0000;
    } else {
        *out = *in | 0x00000000;
    }
    return 0;
}

int parse_instruction_address(uint32_t *requested_address, struct Address *fields)
{
    fields->index = (*requested_address << 20) >> 22;
    fields->offset = (*requested_address << 28) >> 30;
    fields->tag = *requested_address >> 12;
    return 0;
}

int instructionCache(uint32_t *address) {
    struct Address current_request;
    parse_instruction_address(address, &current_request);
    uint32_t data;
    if ((iCache.way1[current_request.index].tag == current_request.tag) && (iCache.way1[current_request.index].valid)) {
        data = iCache.way1[current_request.index].data[current_request.offset];
    } else {
        iCache.way1[current_request.index].tag = current_request.tag;
        iCache.way1[current_request.index].valid = 1;
        for (int i = 0; i < 4; i++){ // i < offset
            uint32_t index =  (uint32_t) floor(*address/4) * 4 ;
            iCache.way1[current_request.index].data[i] = instruction_memory[index + i];
        }
        data = iCache.way1[current_request.index].data[current_request.offset];
    }
    return data;
}

int parse_data_address(uint32_t *requested_address, struct Address *fields)
{
    fields->index = (*requested_address << 22) >> 22;
    fields->tag = *requested_address >> 8;


    return 0;
}
int dataCache(uint32_t *address) {
    struct Address current_request;
    uint32_t data;
    parse_data_address(address, &current_request);
    if ((dCache.way[0][current_request.index].tag == current_request.tag) && (dCache.way[0][current_request.index].valid)) {
        //then it's a hit
        //increment LRU metadata
        data = dCache.way[0][current_request.index].data;
        incrementLRU(0);
        //return data
    } else if ((dCache.way[1][current_request.index].tag == current_request.tag) && (dCache.way[1][current_request.index].valid)) {
        //then it's a hit
        //increment LRU metadata
        data = dCache.way[1][current_request.index].data;
        incrementLRU(1);
        //return data
    } else if ((dCache.way[2][current_request.index].tag == current_request.tag) && (dCache.way[2][current_request.index].valid)) {
        //then it's a hit
        //increment LRU metadata
        data = dCache.way[2][current_request.index].data;
        incrementLRU(2);
        //return data
    } else if ((dCache.way[3][current_request.index].tag == current_request.tag) && (dCache.way[3][current_request.index].valid)) {
        //then it's a hit
        //increment LRU metadata
        data = dCache.way[3][current_request.index].data;
        incrementLRU(3);
        //return data
    } else {
        //it's a miss
        //add to cache
        printf("address:::::: %x\n", *address);
        dCache.way[dCache.LRU[0]][current_request.index].data = data_memory[*address];
        data = dCache.way[dCache.LRU[0]][current_request.index].data;
        int temp;
        temp = dCache.LRU[0];
        dCache.LRU[0] = dCache.LRU[1];
        dCache.LRU[1] = dCache.LRU[2];
        dCache.LRU[2] = dCache.LRU[3];
        dCache.LRU[3] = temp;
        //edit LRU metadata



    }
    printf("dataCache\n");
    return data;
}

int incrementLRU(uint32_t way) {
    int i = 0;

    while (dCache.LRU[i] != way) {
        i++;
    }

    int temp = dCache.LRU[i];

    for (int j =i; j < 3; j++) {
        dCache.LRU[j] = dCache.LRU[j+1];
    }
    dCache.LRU[3] = temp;

    return 0;
}

