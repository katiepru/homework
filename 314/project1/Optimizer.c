/*
 *********************************************
 *  314 Principles of Programming Languages  *
 *  Spring 2013                              *
 *  Authors: Ulrich Kremer                   *
 *           Hans Christian Woithe           *
 *********************************************
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "InstrUtils.h"
#include "Utils.h"

int main()
{
	Instruction *head;
	Instruction *instr1, *instr2, *instr3;
	int opt_flag, opt_calc;

	head = ReadInstructionList(stdin);
	if (!head) {
		ERROR("No instructions\n");
		exit(EXIT_FAILURE);
	}

	instr1 = head;
	while(instr1 != NULL)
	{
		if(instr1->opcode == LOADI)
		{
			instr2 = instr1->next;
			if(instr2 != NULL && instr2->opcode == LOADI)
			{
				instr3 = instr2->next;
				if(instr3->opcode == ADD)
				{
					opt_calc = instr1->field1 + instr2->field1;
				}
				else if(instr3->opcode == SUB)
				{
					opt_calc = instr1->field1 - instr2->field1;
				}
				else if(instr3->opcode == MUL)
				{
					opt_calc = instr1->field1 * instr2->field1;
				}
				else
				{
					continue;
				}
				printf("opt calc is %d\n", opt_calc);
			}
		}
	}

	PrintInstructionList(stdout, head);
	DestroyInstructionList(head);
	return EXIT_SUCCESS;
}
