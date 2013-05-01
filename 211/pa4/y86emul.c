#include "y86emul.h"

int main(int argc, char *argv[])
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
	char line[100];
	char *instrs;
	int i;

	/*Take care of size*/
	fgets(line, 100, file);	
	base = get_size(line);

	/*read lines and place data*/
	instrs = (char *) read_lines(file, base);

	pipeline(base, instrs);
}

/* ---------------------------------------------------------------------------/
 * Mallocs virtual memory returns pointer to that result.
 * --------------------------------------------------------------------------*/
void *get_size(char line[100])
{
	int size = 0;
	void * base;
	char trash[10];
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
	char line[1000];
	char directive[10];
	char *bss[2];
	char byte;
	char str[1000];
	char *str_ptr;
	char byte_chars[3];
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
			addr = (char *) (long) base + addr_offset;
			put_byte(addr, byte);
		}
		else if(strcmp(directive, ".string") == 0)
		{
			sscanf(line, "%s %lx %s", directive, &addr_offset, str);
			addr = (char *) ((long) base + addr_offset);
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
			addr = (char *) ((long) base + addr_offset);
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
					addr = (char *)((long) addr + 1);
					memset(byte_chars, 0, 2);
				}
			}
			put_byte(addr, '\0');
			addr = (char *)((long) addr + 1);
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
void pipeline(void *base, char *instrs)
{
	char curr[7];
	long registers[8];
	long reg_vals[8];
	int pc = 0;
	struct Node *mem_vals;
	char *str;
	int halt = 0;
	int flags[3];
	int i;
	
	while(!halt)
	{
		mem_vals = NULL;
		for(i = 0; i < 8; i++)
		{
			reg_vals[i] = registers[i];
		}
		puts("fetching");
		fetch(curr, instrs, &pc);
		/*decode(curr);*/
		puts("execing");
		halt = execute(curr, registers, mem_vals, reg_vals, flags, &pc, base);
		puts("writing");
		writeback(registers, reg_vals, (long *) base, mem_vals);
	}
}

/*Functions for fetch, decode, exec, and weriteback*/

/* ---------------------------------------------------------------------------/
 * Fetch instruction and populate curr. Modify program counter as needed.
 * --------------------------------------------------------------------------*/
void fetch(char curr[7], char *instrs, int *pc)
{
	printf("instrs is %p\n", instrs);
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
int execute(char curr[7], long registers[8], struct Node *memvals, 
	long reg_vals[8], int flags[3], int *pc, char *base)
{
	struct Node *node;
	int reg1, reg2;
	void *mem1, *mem2;
	long val1;
	int i;
	char byte[2];
	char num[4];

	for(i = 0; i < strlen(curr); i++)
	{
		printf("%x", curr[i]);
	}
	printf("\n");
	/*Set all flags to 0*/
	flags[OF] = 0;
	flags[ZF] = 0;
	flags[SF] = 0;

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
			reg1 = curr[1]/10;
			reg2 = curr[1] % 10;
			reg_vals[reg2] = registers[reg1];
			return AOK;
		case 48:
			/*irmovl*/
			reg1 = curr[1] % 10;
			val1 = 0;
			for(i = 2; i < 6; i++)
			{
				val1 += curr[i];
			}
			reg_vals[reg1] = val1;
			return AOK;
		case 64:
			/*rmmovl*/
			reg1 = curr[1]/10;
			reg2 = curr[1] % 10;
			val1 = 0;
			for(i = 2; i < 6; i++)
			{
				val1 += curr[i];
			}
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
			reg1 = curr[1]/10;
			reg2 = curr[1] % 10;
			val1 = 0;
			for(i = 2; i < 6; i++)
			{
				val1 += curr[i];
			}
			reg_vals[reg1] = get_long((long *)(registers[reg2] + 
				(long) base + val1));
			return AOK;
		case 96:
			/*addl*/
			reg1 = curr[1]/10;
			reg2 = curr[1] % 10;
			reg_vals[reg1] = registers[reg1] + registers[reg2];
			/*FIXME: Check overflow*/
			/*FIXME: Set other flags?*/
			return AOK;
		case 97:
			/*subl = Ra -= Rb*/
			reg1 = curr[1]/10;
			reg2 = curr[1] % 10;
			reg_vals[reg1] = registers[reg1] - registers[reg2];
			/*FIXME: Check overflow*/
			/*Set flags*/
			if(reg_vals[reg1] == 0)
			{
				flags[ZF] = 1;
			}
			else if(reg_vals[reg1] < 0)
			{
				flags[SF] = 1;
			}
			return AOK;
		case 98:
			/*andl - bitwise and*/
			reg1 = curr[1]/10;
			reg2 = curr[1] % 10;
			reg_vals[reg1] = registers[reg1] & registers[reg2];
			return AOK;
		case 99:
			/*xorl - bitwise xor*/
			reg1 = curr[1]/10;
			reg2 = curr[1] % 10;
			reg_vals[reg1] = registers[reg1] ^ registers[reg2];
			return AOK;
		case 100:
			/*mull*/
			reg1 = curr[1]/10;
			reg2 = curr[1] % 10;
			reg_vals[reg1] = registers[reg1] * registers[reg2];
			/*FIXME: Check overflow*/
			/*FIXME: Set other flags?*/
			return AOK;
		case 112:
			/*jmp*/
			/*Get destination*/
			val1 = 0;
			for(i = 1; i < 4; i++)
			{
				val1 += curr[i];
			}
			/*FIXME: Do error checking here*/
			*pc = (val1 - (long) base);
			return AOK;
		case 113:
			/*jle*/
			/*check if jump should occur*/
			if(flags[ZF] == 1 || flags[SF] == 1)
			{
				/*Get destination*/
				val1 = 0;
				for(i = 1; i < 6; i++)
				{
					val1 += curr[i];
				}
				/*FIXME: Do error checking here*/
				*pc = (val1 - (long) base);
			}
			return AOK;
		case 114:
			/*jl*/
			/*check if jump should occur*/
			if(flags[SF] == 1)
			{
				/*Get destination*/
				val1 = 0;
				for(i = 1; i < 5; i++)
				{
					val1 += curr[i];
				}
				/*FIXME: Do error checking here*/
				*pc = (val1 - (long) base);
			}
			return AOK;
		case 115:
			/*je*/
			/*check if jump should occur*/
			if(flags[ZF] == 1)
			{
				/*Get destination*/
				val1 = 0;
				for(i = 1; i < 5; i++)
				{
					val1 += curr[i];
				}
				/*FIXME: Do error checking here*/
				*pc = (val1 - (long) base);
			}
			return AOK;
		case 116:
			/*jne*/
			/*check if jump should occur*/
			if(flags[ZF] == 0)
			{
				/*Get destination*/
				val1 = 0;
				for(i = 1; i < 5; i++)
				{
					val1 += curr[i];
				}
				/*FIXME: Do error checking here*/
				*pc = (val1 - (long) base);
			}
			return AOK;
		case 117:
			/*jge*/
			/*check if jump should occur*/
			if(flags[SF] == 0)
			{
				/*Get destination*/
				val1 = 0;
				for(i = 1; i < 5; i++)
				{
					val1 += curr[i];
				}
				/*FIXME: Do error checking here*/
				*pc = (val1 - (long) base);
			}
			return AOK;
		case 128:
			/*Call - push then jump*/
			val1 = 0;
			for(i = 0; i < 4; i++)
			{
				val1 += curr[i];
			}
			reg_vals[4] = registers[4] - 4;
			put_long((long *) reg_vals[4], *pc + *base);
			*pc = (val1 - (long) base);
			return AOK;
		case 144:
			/*ret - pop and jump*/
			val1 = get_long((long *) registers[4]);
			reg_vals[4] = registers[4] + 4;
			*pc = (val1 - (long) base);
			return AOK;
		case 160:
			/*pushl*/
			reg1 = curr[1]/10;
			/*Decrement esp*/
			reg_vals[4] = registers[4] - 4;
			put_long((long *) reg_vals[4], registers[reg1]);
			return AOK;
		case 176:
			/*popl*/
			reg1 = curr[1]/10;
			reg_vals[reg1] = get_long((long *) registers[4]);
			reg_vals[4] = registers[4] + 4;
			return AOK;
		case 192:
			/*readb*/
			scanf("%s", byte);
			reg1 = curr[1]/10;
			val1 = 0;
			for(i = 2; i < 7; i++)
			{
				val1 += curr[i];
			}
			node = create_node(val1 + registers[reg1], strtol(byte, NULL, 16), 
				1);
			/*insert into memval linked list*/
			node->next = memvals->next;
			memvals->next = node;
		case 193:
			/*readw*/
			scanf("%s", num);
			reg1 = curr[1]/10;
			for(i = 2; i < 6; i++)
			{
				val1 += curr[i];
			}
			put_string((char *) (val1 + (long) base), num);
			return AOK;

		case 208:
			/*writeb*/
			reg1 = curr[1]/10;
			val1 = 0;
			for(i = 2; i < 6; i++)
			{
				val1 += curr[i];
			}
			printf("0x%x\n", (long) get_byte((char *)(registers[reg1] + val1)));
			return AOK;
		case 209:
			/*writew*/
			reg1 = curr[1]/10;
			val1 = 0;
			for(i = 2; i < 7; i++)
			{
				val1 += curr[i];
			}
			printf("0x%s\n", get_string((char *)
				(registers[reg1] + val1), 4));
			return AOK;
	}
}

/* ---------------------------------------------------------------------------/
 * Write back to memory and registers
 * --------------------------------------------------------------------------*/
void writeback(long registers[8], long reg_vals[8], long *base,
	struct Node *memvals)
{
	struct Node *tmp;
	int i;

	/*Write to registers*/
	for(i = 0; i < 8; i++)
	{
		registers[i] = reg_vals[i];
	}

	/*Write to memory*/
	tmp = memvals;
	while(tmp != NULL)
	{
		if(tmp->byte == 1)
		{
			put_byte((char *) tmp->addr, (char) tmp->data);
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
	long num;

	memcpy(&num, addr, 4);
	return num;
}

/* ---------------------------------------------------------------------------/
 * Put num into addr in memory.
 * --------------------------------------------------------------------------*/
void put_byte(char *addr, char num)
{
	memcpy(addr, &num, 1);
}

/* ---------------------------------------------------------------------------/
 * Returns char stored in addr.
 * --------------------------------------------------------------------------*/
char get_byte(char *addr)
{
	char num;

	memcpy(&num, addr, 1);
	return num;
}

/* ---------------------------------------------------------------------------/
 * Put str into addr in memory.
 * --------------------------------------------------------------------------*/
void put_string(char *addr, char *str)
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
char *get_string(char *addr, int len)
{
	char *str = calloc(len, sizeof(char));
	int i = 0;
	
	while(i <= len)
	{
		str[i] = addr[i];
		printf("addr[i] = %d\n", addr[i]);
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
