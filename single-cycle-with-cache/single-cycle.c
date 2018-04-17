/**
 * Gedare Bloom
 * Shumba Brown
 * Skylar Grammas
 * Deeshai Escoffery
 * Tashambra Williams
 * single-cycle-with-cache.c
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
	int i, j;

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

    for ( i = 0; i < 256; i++ ) {
        iCache.way1[i].valid = 0;
        for ( j = 0; j<4; j++ ) {
        	dCache.way[j][i].valid = 0;
        }
    }

    for ( i = 0; i < 4; i++ ) {
    	dCache.LRU[i] = i;
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
		// printf("%u\n", data_memory[i]);
#endif
	}
	for ( i = 0; i < 1024; i++ ) {
		fread(&instruction_memory[i], sizeof(uint32_t), 1, f);
#if defined(DEBUG)
		// printf("%u\n", instruction_memory[i]);
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
	return 0;
}
