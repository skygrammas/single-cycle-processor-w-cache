/**
 * Gedare Bloom
 * Shumba Brown
 * Skylar Grammas
 * Deeshai Escoffery
 * Tashambra Williams
 * cpu.h
 *
 * Definitions for the processor.
 */

#include <stdint.h>

struct cpu_context {
	uint32_t PC;
	uint32_t GPR[32];
};

struct cpu_counter {
    int cycle;
    int add_c;
    int and_c;
    int nor_c;
    int or_c;
    int slt_c;
    int sll_c;
    int srl_c;
    int sub_c;
    int sra_c;
    int xor_c;
    int syscall_c;
    int addi_c;
    int andi_c;
    int beq_c;
    int bne_c;
    int lui_c;
    int lw_c;
    int ori_c;
    int slti_c;
    int xori_c;
    int sw_c;
    int j_c;
    int jal_c;
    int jr_c;
    int nop_c;
};

extern struct cpu_context cpu_ctx;
extern struct cpu_counter cpu_cntr;

struct Control {
    uint32_t MemToReg;
    uint32_t RegWrite;
    uint32_t Branch;
    uint32_t MemRead;
    uint32_t MemWrite;
    uint32_t ALU_src;
    uint32_t RegDst;
    uint32_t ALU_op;
    uint32_t jump;
};

struct IF_ID_buffer {
	uint32_t instruction;
	uint32_t next_pc;
};

struct ID_EX_buffer {
    uint32_t next_pc;
    uint32_t read_data_1;
    uint32_t read_data_2;
    struct Control control_signals;
    uint32_t sign_extend;
    uint32_t regDst1;
    uint32_t regDst0;
    uint32_t op;
    uint32_t shamt;
    uint32_t rt;
    uint32_t rd;
    uint32_t rs;
    uint32_t bit_address;
};

struct EX_MEM_buffer {
    uint32_t next_pc;
    struct Control control_signals;
    int ALU_result;
    uint32_t add_result;
    uint32_t read_data_2;
    int ALU_zero;
    int32_t sign_extend;
    uint32_t registerDestination;
};

struct MEM_WB_buffer {
	uint32_t rd;
	uint32_t registerDestination;
    uint32_t data;
    uint32_t ALU_result;
    uint32_t next_pc;
    struct Control control_signals;
};

struct instruction {
    uint32_t op;
    uint32_t shamt;
    uint32_t rs;
    uint32_t rt;
    uint32_t rd;
    uint32_t funct;
    uint32_t bit_address;
    uint32_t bit_number;
};

struct ALU_input {
 uint32_t in1, in2;
 uint32_t control;
};
struct ALU_output {
 uint32_t result;
 unsigned int zero: 1;
};

struct MUX_input {
 uint32_t in0, in1, in2, select;
};
struct MUX_output {
 uint32_t output;
};

struct instructionCache {
 block way1[256];
};

struct dataCache {
 block way1[256];
 block way2[256];
 block way3[256];
 block way4[256];
};

struct L1Cache {
 instructionCache iCache;
 dataCache dCache;
};

struct Address {
 uint32_t tag;
 uint32_t offset;
 uint32_t index;
};

struct block {

 blockArray data[block_size];
 uint32_t tag;
 uint32_t valid;
 uint32_t LRU; //2 bits
};

struct blockArray {
 uint32_t LRU_1;
 uint32_t LRU_2;
 uint32_t LRU_3;
 uint32_t LRU_4;
};

int ALU(struct ALU_input *in, struct ALU_output *out);
int MUX(struct MUX_input *in, struct MUX_output *out);
int SIGN_EXTEND(uint32_t *in, uint32_t *out);

int fetch( struct IF_ID_buffer *out );
int decode( struct IF_ID_buffer *in, struct ID_EX_buffer *out );
int execute( struct ID_EX_buffer *in, struct EX_MEM_buffer *out );
int memory( struct EX_MEM_buffer *in, struct MEM_WB_buffer *out );
int writeback( struct MEM_WB_buffer *in );

int parse_instruction(uint32_t *bits, struct instruction *fields);
int parse_address(uint32_t *requested_address, struct Address *fields);
int instructionCache(uint32_t *address, struct instructionCache *iCache);
int dataCache(uint32_t *address, struct dataCache *dCache);
