#include "y86emul.h"

int main(int argc, char*argv[])
{
	FILE *file;

	/*Check for right number of args*/
	if(argc != 2)
	{
		fprintf(stderr, "ERROR: Wrong number of arguments.\n");
		/*print_help();*/
		return 1;
	}

	/*Check for help mode*/
	if(strcmp(argv[1], "-h") == 0)
	{
		/*print_help();*/
		return 0;
	}

	/*Open the file*/
	file = fopen(argv[1], "r");

	/*Check if file was found*/
	if(file == 0)
	{
		fprintf(stderr, "ERROR: File %s not found.\n", argv[1]);
		return 2;
	}

	run_program(file);

	return 0;
}

/* ----------------------------------------------------------------------------/
 * - Controls flow of emulator. First, build memory, then insert data, then
 * - execute instructions.
 * ---------------------------------------------------------------------------*/
void run_program(FILE *file)
{
	void *base;
	unsigned char line[100];
	unsigned char *instrs;
	int i;

	/*Take care of size*/
	fgets(line, 100, file);	
	base = get_size(line);

	/*read lines and place data*/
	instrs = (unsigned char *) read_lines(file, base);

	pipeline(base, instrs);
}

/* ---------------------------------------------------------------------------/
 * Mallocs virtual memory returns pointer to that result.
 * --------------------------------------------------------------------------*/
void *get_size(unsigned char line[100])
{
	int size = 0;
	void * base;
	unsigned char trash[10];
	int stat;
	
	stat = sscanf(line, "%s %x", trash, &size);

	base = malloc(size);

	return (void *) base;

}

/* ---------------------------------------------------------------------------/
 * Read lines and put data into virtual memory.
 * --------------------------------------------------------------------------*/
long read_lines(FILE *file, void *base)
{
	unsigned char line[1000];
	unsigned char directive[10];
	unsigned char *bss[2];
	unsigned char byte;
	unsigned char str[1000];
	unsigned char *str_ptr;
	unsigned char byte_chars[3];
	long num;
	long addr_offset;
	long instr_addr;
	void *addr;
	int i;
	long test;

	while(fgets(line, 1000, file) != NULL)
	{
		sscanf(line, "%s", directive);
		if(strcmp(directive, ".long") == 0)
		{
			sscanf(line, "%s %lx %d", directive, &addr, &num);
			addr = (void *)((long) base + addr_offset);
			put_long((long *) addr, num);
		}
		else if(strcmp(directive, ".byte") == 0)
		{
			sscanf(line, "%s %lx %x", directive, &addr_offset, &byte);
			addr = (unsigned char *) (long) base + addr_offset;
			put_byte(addr, byte);
		}
		else if(strcmp(directive, ".string") == 0)
		{
			sscanf(line, "%s %lx \"%[^\"]\"", directive, &addr_offset, str);
			addr = (unsigned char *) ((long) base + addr_offset);
			str_ptr = calloc(strlen(str), sizeof(char));
			for(i = 0; i <= strlen(str); i++)
			{
				str_ptr[i] = str[i];
			}
			put_string(addr, str_ptr);
		}
		else if(strcmp(directive, ".bss") == 0)
		{
		}
		else if(strcmp(directive, ".text") == 0)
		{
			sscanf(line, "%s %lx %s", directive, &addr_offset, str);
			addr = (unsigned char *) ((long) base + addr_offset);
			instr_addr = (long) addr;
			memset(byte_chars, 0, 2);
			for(i = 0; i < strlen(str); i++)
			{
				if(strlen(byte_chars) == 0)
				{
					byte_chars[0] = str[i];
				}
				else if(strlen(byte_chars) == 1)
				{
					byte_chars[1] = str[i];
				}
				if(strlen(byte_chars) == 2)
				{
					byte = (char) strtol(byte_chars, NULL, 16);
					put_byte(addr, byte);
					addr = (unsigned char *)((long) addr + 1);
					memset(byte_chars, 0, 2);
				}
			}
			put_byte(addr, '\0');
			addr = (unsigned char *)((long) addr + 1);
		}
		else
		{
			fprintf(stderr, "ERROR: Invalid directive %s.\n", directive);
			return -1;
		}
	}
	return instr_addr;
}


/* ---------------------------------------------------------------------------/
 * Begin fetch, decode and execute (pipelining if there is time.
 * --------------------------------------------------------------------------*/
void pipeline(void *base, unsigned char *instrs)
{
	unsigned char curr[7];
	long registers[8];
	int pc = 0;
	struct Node *mem_vals;
	unsigned char *str;
	int halt = 0;
	int flags[3];
	int i;
	
	/*Set all flags to 0*/
	flags[OF] = 0;
	flags[ZF] = 0;
	flags[SF] = 0;

	for(i = 0; i< 8; i++)
	{
		registers[i] = 0;
	}
	
	while(!halt)
	{
		mem_vals = NULL;
		fetch(curr, instrs, &pc);
		/*decode(curr);*/
		halt = execute(curr, registers, mem_vals, flags, &pc, base, instrs);
		writeback(registers, (long *) base, mem_vals);
	}
}

/*Functions for fetch, decode, exec, and weriteback*/

/* ---------------------------------------------------------------------------/
 * Fetch instruction and populate curr. Modify program counter as needed.
 * --------------------------------------------------------------------------*/
void fetch(unsigned char curr[7], unsigned char *instrs, int *pc)
{
	int i;
	/*noop, halt or ret*/
	if(instrs[*pc] == 0 || instrs[*pc] == 16 || instrs[*pc] == 144)
	{
		curr[0] = instrs[*pc];
		for(i = 1; i < 6; i++)
		{
			curr[i] = 0;
		}
		++*pc;
	}
	/*rrmovl, op1, push, and pop*/
	else if(instrs[*pc] == 32 || (instrs[*pc] >= 96 && instrs[*pc] <= 100) || 
		instrs[*pc] == 160 || instrs[*pc] == 176)
	{
		for(i = 0; i < 2; i++)
		{
			curr[i] = instrs[*pc];
			++*pc;
		}
		for(i = 2; i < 6; i++)
		{
			curr[i] = 0;
		}
	}
	/*irmolv, rmovl, mrmovl, readX, and writeX*/
	else if(instrs[*pc] == 48 || instrs[*pc] == 64 || instrs[*pc] == 80 || 
		instrs[*pc] == 192 || instrs[*pc] == 193 || instrs[*pc] == 208 ||
		instrs[*pc] == 209)
	{
		for(i = 0; i < 6; i++)
		{
			curr[i] = instrs[*pc];
			++*pc;
		}
	}
	/*jmp and call*/
	else if(instrs[*pc] == 128 || (instrs[*pc] >= 112 && instrs[*pc] <= 117))
	{
		for(i = 0; i < 5; i++)
		{
			curr[i] = instrs[*pc];
			++*pc;
		}

		curr[i] = 0;
	}
	else
	{
		fprintf(stderr, "You fucked up\n");
	}
}

/* ---------------------------------------------------------------------------/
 * Execute current instruction. Returns status code.
 * --------------------------------------------------------------------------*/
int execute(unsigned char curr[7], long registers[8], struct Node *memvals, 
	int flags[3], int *pc, unsigned char *base, unsigned char *instrs)
{
	struct Node *node;
	int reg1, reg2;
	void *mem1, *mem2;
	long val1, val2;
	int i;
	unsigned char byte[2];
	unsigned char num[4];

	switch((int) curr[0])
	{
		case 0: 
			/*Do nothing*/
			return AOK;
		case 16:
			/*Halt*/
			return HLT;
		case 32:
			/*rrmovl*/
			reg1 = curr[1]/0x10;
			reg2 = curr[1] % 0x10;
			registers[reg2] = registers[reg1];
			return AOK;
		case 48:
			/*irmovl*/
			reg1 = curr[1] % 0x10;
			val1 = get_long((long *) &curr[2]);
			registers[reg1] = val1;
			return AOK;
		case 64:
			/*rmmovl*/
			reg1 = curr[1]/0x10;
			reg2 = curr[1] % 0x10;
			val1 = get_long((long *) &curr[2]);
			node = create_node(((long) base + val1) + registers[reg2], 
				registers[reg1], 0);
			if(memvals == NULL)
			{
				memvals = node;
			}
			else
			{
				node->next = memvals->next;
				memvals->next = node;
			}
			return AOK;
		case 80:
			/*mrmovl*/
			reg1 = curr[1]/0x10;
			reg2 = curr[1] % 0x10;
			val1 = get_long((long *) &curr[2]);
			registers[reg1] = get_long((long *)(registers[reg2] + 
				(long) base + val1));
			return AOK;
		case 96:
			/*addl*/
			reg1 = curr[1]/0x10;
			reg2 = curr[1] % 0x10;
			registers[reg2] = registers[reg1] + registers[reg2];
			/*FIXME overflow*/
			return AOK;
		case 97:
			/*subl = Rb -= Ra*/
			reg1 = curr[1]/0x10;
			reg2 = curr[1] % 0x10;
			registers[reg2] = registers[reg2] - registers[reg1];
			/*FIXME: Check overflow*/
			/*Set flags*/
			if(registers[reg2] == 0)
			{
				flags[ZF] = 1;
			}
			else if(registers[reg2] < 0)
			{
				flags[SF] = 1;
				flags[ZF] = 0;
			}
			else if(registers[reg2] > 0)
			{
				flags[SF] = 0;
				flags[ZF] = 0;
			}
			return AOK;
		case 98:
			/*andl - bitwise and*/
			reg1 = curr[1]/0x10;
			reg2 = curr[1] % 0x10;
			registers[reg2] = registers[reg1] & registers[reg2];
			return AOK;
		case 99:
			/*xorl - bitwise xor*/
			reg1 = curr[1]/0x10;
			reg2 = curr[1] % 0x10;
			registers[reg2] = registers[reg1] ^ registers[reg2];
			return AOK;
		case 100:
			/*mull*/
			reg1 = curr[1]/0x10;
			reg2 = curr[1] % 0x10;
			registers[reg2] = registers[reg1] * registers[reg2];
			/*FIXME: Check overflow*/
			/*FIXME: Set other flags?*/
			return AOK;
		case 112:
			/*jmp*/
			/*Get destination*/
			val1 = 0;
			val1 = get_long((long *) &curr[1]);
			/*FIXME: Do error checking here*/
			*pc = val1 - ((long) instrs - (long)base);
			return AOK;
		case 113:
			/*jle*/
			/*check if jump should occur*/
			if(flags[ZF] == 1 || flags[SF] == 1)
			{
				/*Get destination*/
				val1 = 0;
				val1 = get_long((long *) &curr[1]);
				/*FIXME: Do error checking here*/
				*pc = val1 - ((long) instrs - (long)base);
			}
			return AOK;
		case 114:
			/*jl*/
			/*check if jump should occur*/
			if(flags[SF] == 1)
			{
				/*Get destination*/
				val1 = 0;
				val1 = get_long((long *) &curr[1]);
				/*FIXME: Do error checking here*/
				*pc = val1 - ((long) instrs - (long)base);
			}
			return AOK;
		case 115:
			/*je*/
			/*check if jump should occur*/
			if(flags[ZF] == 1)
			{
				/*Get destination*/
				val1 = 0;
				val1 = get_long((long *) &curr[1]);
				/*FIXME: Do error checking here*/
				*pc = val1 - ((long) instrs - (long)base);
			}
			return AOK;
		case 116:
			/*jne*/
			/*check if jump should occur*/
			if(flags[ZF] == 0)
			{
				/*Get destination*/
				val1 = get_long((long *) &curr[1]);
				*pc = val1 - ((long) instrs - (long)base);
			}
			return AOK;
		case 117:
			/*jge*/
			/*check if jump should occur*/
			if(flags[SF] == 0)
			{
				/*Get destination*/
				val1 = 0;
				val1 = get_long((long *) &curr[1]);
				/*FIXME: Do error checking here*/
				*pc = val1 - ((long) instrs - (long)base);
			}
			return AOK;
		case 118:
			/*jg*/
			/*check if jump should occur*/
			if(flags[SF] == 0 && flags[ZF] == 0)
			{
				/*Get destination*/
				val1 = 0;
				val1 = get_long((long *) &curr[1]);
				/*FIXME: Do error checking here*/
				*pc = val1 - ((long) instrs - (long)base);
			}
			return AOK;
		case 128:
			/*Call - push then jump*/
			val1 = get_long((long *) &curr[1]);
			registers[4] = registers[4] - 4;
			val2 = *pc + (long) instrs - (long) base;
			put_long((long *) (long) base + registers[4], val2);
			*pc = val1 - ((long) instrs - (long)base);
			return AOK;
		case 144:
			/*ret - pop and jump*/
			val1 = get_long((long *) (long) base + registers[4]);
			registers[4] = registers[4] + 4;
			*pc = val1 - ((long) instrs - (long)base);
			return AOK;
		case 160:
			/*pushl*/
			reg1 = curr[1]/0x10;
			/*Decrement esp*/
			registers[4] = registers[4] - 4;
			put_long((long *) (long) base + registers[4], registers[reg1]);
			return AOK;
		case 176:
			/*popl*/
			reg1 = curr[1]/0x10;
			registers[reg1] = get_long((long *) (long) base +registers[4]);
			registers[4] = registers[4] + 4;
			return AOK;
		case 192:
			/*readb*/
			scanf("%s", byte);
			reg1 = curr[1]/0x10;
			val1 = get_long((long *) &curr[2]);
			node = create_node(val1 + registers[reg1], strtol(byte, NULL, 16), 
				1);
			/*insert into memval linked list*/
			node->next = memvals->next;
			memvals->next = node;
			return AOK;
		case 193:
			/*readw*/
			scanf("%d", &val2);
			reg1 = curr[1]/0x10;
			val1 = get_long((long *) &curr[2]);
			put_long((long *) (val1 + (long) base), val2);
			return AOK;

		case 208:
			/*writeb*/
			reg1 = curr[1]/0x10;
			val1 = get_long((long *) &curr[2]);
			printf("%c", get_byte((unsigned char *)((long) base + registers[reg1] + val1)));
			return AOK;
		case 209:
			/*writew*/
			reg1 = curr[1]/0x10;
			val1 = get_long((long *) &curr[2]);
			/*FIXME: Back ti get string*/
			printf("%d\n", get_long((long *)
				((long) base + registers[reg1] + val1)));
			return AOK;
	}
}

/* ---------------------------------------------------------------------------/
 * Write back to memory and registers
 * --------------------------------------------------------------------------*/
void writeback(long registers[8], long *base, struct Node *memvals)
{
	struct Node *tmp;

	/*Write to memory*/
	tmp = memvals;
	while(tmp != NULL)
	{
		if(tmp->byte == 1)
		{
			put_byte((unsigned char *) tmp->addr, (char) tmp->data);
		}
		else
		{
			put_long((long *) tmp->addr, tmp->data);
		}
		memvals = tmp;
		tmp = tmp->next;
		free(memvals);
	}
	memvals = NULL;
}

/*Helper functions to get and put data to and from memory*/

/* ---------------------------------------------------------------------------/
 * Put num into addr in memory.
 * --------------------------------------------------------------------------*/
void put_long(long *addr, long num)
{
	memcpy(addr, &num, 4);
}

/* ---------------------------------------------------------------------------/
 * Returns long stored in addr.
 * --------------------------------------------------------------------------*/
long get_long(long *addr)
{
	long num = 0;

	memcpy(&num, addr, 4);

	return num;
}

/* ---------------------------------------------------------------------------/
 * Put num into addr in memory.
 * --------------------------------------------------------------------------*/
void put_byte(unsigned char *addr, unsigned char num)
{
	memcpy(addr, &num, 1);
}

/* ---------------------------------------------------------------------------/
 * Returns unsigned char stored in addr.
 * --------------------------------------------------------------------------*/
unsigned char get_byte(unsigned char *addr)
{
	unsigned char num;

	memcpy(&num, addr, 1);
	return num;
}

/* ---------------------------------------------------------------------------/
 * Put str into addr in memory.
 * --------------------------------------------------------------------------*/
void put_string(unsigned char *addr, unsigned char *str)
{
	int i = 0;
	while(i <= strlen(str))
	{
		addr[i] = str[i];
		i++;
	}
}

/* ---------------------------------------------------------------------------/
 * Returns string of length len stored in addr.
 * --------------------------------------------------------------------------*/
unsigned char *get_string(unsigned char *addr, int len)
{
	unsigned char *str = calloc(len, sizeof(char));
	int i = 0;
	
	while(i <= len)
	{
		str[i] = addr[i];
		i++;
	}
	return str;
}

/*End helper functions for getting and putting data*/

/*Begin linked list functions*/

/* ---------------------------------------------------------------------------/
 * Malloc a Node.
 * --------------------------------------------------------------------------*/
struct Node *create_node(long addr, long data, int byte)
{
	struct Node *node = malloc(sizeof(struct Node));
	node->addr = addr;
	node->data = data;
	node->byte = byte;
	node->next = NULL;
}

/* ---------------------------------------------------------------------------/
 * Free a Node.
 * --------------------------------------------------------------------------*/
void delete_node(long addr, struct Node *head)
{
	struct Node *ptr;
	struct Node *tmp;

	ptr = head;

	if(head->addr == addr)
	{
		head = head->next;
		free(ptr);
		return;
	}

	while(ptr->next != NULL)
	{
		if(ptr->next->addr == addr)
		{
			tmp = ptr->next;
			ptr->next = ptr->next->next;
			free(tmp);
			return;
		}
	}
}

/*End linked list list functions*/
