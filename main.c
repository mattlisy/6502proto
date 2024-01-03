#include <stdio.h>
#include <stdlib.h> 
#include <stdint.h> 

#define BYTE uint8_t
#define WORD uint16_t


#define LDA_IM 0xA9
#define LDA_ZP 0xA5

typedef struct {

	// regs 
	BYTE a;
	BYTE x;
	BYTE y;

	// address pointers
	WORD pc; 			
	WORD sp;

	// flags
	BYTE c : 1; 
	BYTE z : 1;
	BYTE i : 1;
	BYTE d : 1;
	BYTE b : 1;
	BYTE v : 1;
	BYTE n : 1; 

} CPU;

typedef struct {
	BYTE data[65536];
} MEMORY;

void MEMORY_init(MEMORY* mem) {
	
	for (int i = 0; i < 1024*64; i++) {
		mem->data[i] = 0;
	}


}


BYTE CPU_fetch(CPU* cpu, MEMORY* mem) {

	return mem->data[cpu->pc++]; 

}

void CPU_execute(CPU* cpu, MEMORY* mem, int cycles) {
	for (int i = 0; i < cycles; i++) {
		
		BYTE instruction = CPU_fetch(cpu, mem);
		printf("%i, %i\n", instruction, cpu->pc);
		switch(instruction) {
		
			case LDA_IM:
				cpu->a = CPU_fetch(cpu, mem);
				cpu->z = (cpu->a == 0);
				cpu->n = (cpu->a & (1<<7));
				i++;
				printf("a: %i\n", cpu->a);
				break;

			case LDA_ZP:

				BYTE zeropageaddress = CPU_fetch(cpu, mem);
				i++;
				cpu->a = mem->data[zeropageaddress];

				cpu->z = (cpu->a == 0);
				cpu->n = (cpu->a & (1<<7));
				i++;


		}

	}

}

void CPU_reset(CPU* cpu, MEMORY* mem) {

	cpu->pc = 0xFFFC; // reset vector
	cpu->sp = 0x0100;

	cpu->c = cpu->z = cpu->i = cpu->d = cpu->b = cpu->v = cpu->n = 0;

	cpu->a = cpu->x = cpu->y = 0;	

	
	MEMORY_init(mem);
		
}

int main() {

	CPU cpu;
	MEMORY mem;

	CPU_reset(&cpu, &mem);
	
	mem.data[0xFFFC] = LDA_IM;
	mem.data[0xFFFD] = 7;

	CPU_execute(&cpu, &mem, 2);

}
