/**
 * Gedare Bloom
 * Shumba Brown
 * Skylar Grammas
 * Deeshai Escoffery
 * Tashambra Williams
 * single-cycle.c
 *
 * Drives the simulation of a single-cycle processor
 */

#include "cpu.h"
#include "memory.h"
#include "syscall.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define DEBUG

int main( int argc, char *argv[] )
{
	FILE *f;
	struct IF_ID_buffer if_id;
	struct ID_EX_buffer id_ex;
	struct EX_MEM_buffer ex_mem;
	struct MEM_WB_buffer mem_wb;
	int i;

	/* Initialize registers and memory to 0 */
	cpu_ctx.PC = 0;
	for ( i = 0; i < 32; i++ ) {
		cpu_ctx.GPR[i] = 0;
	}

	for ( i = 0; i < 1024; i++ ) {
		instruction_memory[i] = 0;
		data_memory[i] = 0;
		stack_memory[i] = 0;
	}

	/* Initialize the counters */
  cpu_cntr.cycle = 0;
  cpu_cntr.add_c = 0;
  cpu_cntr.and_c = 0;
  cpu_cntr.nor_c = 0;
  cpu_cntr.or_c = 0;
  cpu_cntr.slt_c = 0;
  cpu_cntr.sll_c = 0;
  cpu_cntr.srl_c = 0;
  cpu_cntr.sub_c = 0;
  cpu_cntr.sra_c = 0;
  cpu_cntr.xor_c = 0;
  cpu_cntr.syscall_c = 0;
  cpu_cntr.add_c = 0;
  cpu_cntr.andi_c = 0;
  cpu_cntr.beq_c = 0;
  cpu_cntr.bne_c = 0;
  cpu_cntr.lui_c = 0;
  cpu_cntr.lw_c = 0;
  cpu_cntr.ori_c = 0;
  cpu_cntr.slti_c = 0;
  cpu_cntr.xori_c = 0;
  cpu_cntr.sw_c = 0;
  cpu_cntr.j_c = 0;
  cpu_cntr.jal_c = 0;
  cpu_cntr.jr_c = 0;

	/* Read memory from the input file */
	f = fopen(argv[1], "r");
	assert (f);
	for ( i = 0; i < 1024; i++ ) {
		fread(&data_memory[i], sizeof(uint32_t), 1, f);
#if defined(DEBUG)
		printf("%u\n", data_memory[i]);
#endif
	}
	for ( i = 0; i < 1024; i++ ) {
		fread(&instruction_memory[i], sizeof(uint32_t), 1, f);
#if defined(DEBUG)
		printf("%u\n", instruction_memory[i]);
#endif
	}
	fclose(f);

	while(1) {
#if defined(DEBUG)
		printf("FETCH from PC=%x\n", cpu_ctx.PC);
#endif
		fetch( &if_id );
		decode( &if_id, &id_ex );
		execute( &id_ex, &ex_mem );
		memory( &ex_mem, &mem_wb );
		writeback( &mem_wb );
		if ( cpu_ctx.PC == 0 ) break;
		if ( cpu_ctx.PC == 1024 ) {
		  printf("Exceeded memory range.\n");
			break;
		}

		for ( i = 0; i < 32; i++ ) {
		printf("reg %d: %d\n", i, cpu_ctx.GPR[i]);
	}
	}

    for ( i = 0; i < 1024; i++ ) {
#if defined(DEBUG)
		printf("%u\n", data_memory[i]);
#endif
	}


	printf("Cycle counter :%d\n", cpu_cntr.cycle);
	printf("add :%d\n", cpu_cntr.add_c);
	printf("and :%d\n", cpu_cntr.and_c);
	printf("nor :%d\n", cpu_cntr.nor_c);
	printf("or :%d\n", cpu_cntr.or_c);
	printf("slt :%d\n", cpu_cntr.slt_c);
	printf("sll :%d\n", cpu_cntr.sll_c);
	printf("srl :%d\n", cpu_cntr.srl_c);
	printf("sub :%d\n", cpu_cntr.sub_c);
	printf("sra :%d\n", cpu_cntr.sra_c);
	printf("xor :%d\n", cpu_cntr.xor_c);
	printf("syscall: %d\n", cpu_cntr.syscall_c);
	printf("addi :%d\n", cpu_cntr.addi_c);
	printf("andi :%d\n", cpu_cntr.andi_c);
	printf("beq :%d\n", cpu_cntr.beq_c);
	printf("bne :%d\n", cpu_cntr.bne_c);
	printf("lui :%d\n", cpu_cntr.lui_c);
	printf("lw :%d\n", cpu_cntr.lw_c);
	printf("ori :%d\n", cpu_cntr.ori_c);
	printf("slti :%d\n", cpu_cntr.slti_c);
	printf("xori :%d\n", cpu_cntr.xori_c);
	printf("sw :%d\n", cpu_cntr.sw_c);
	printf("j : %d\n", cpu_cntr.j_c);
	printf("jal :%d\n", cpu_cntr.jal_c);
	printf("jr :%d\n", cpu_cntr.jr_c);

	return 0;
}
