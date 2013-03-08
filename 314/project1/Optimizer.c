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
				/*Check to make sure registers match*/
				if((instr3->field2 == instr1->field1 || instr3->field2 == 
					instr2->field1) && (instr3->field2 == instr1->field1 || 
					instr3->field3 == instr2->field1))
				{
					switch(instr3->opcode)
					{
					case ADD:
						opt_calc = instr1->field2 + instr2->field2;
						break;
					case SUB:
						if(instr3->field2 == instr1->field2)
						{
							opt_calc = instr2->field2 - instr1->field2;
						}
						else
						{
							opt_calc = instr1->field2 - instr2->field2;
						}
						break;
					case MUL:
						opt_calc = instr1->field2 * instr2->field2;
						break;
					default:
						continue;
					}
	
					/*Get Register*/
					opt_flag = instr3->field1;
	
					/*Modify instr3 to contain new instr*/
					instr3->opcode = LOADI;
					instr3->field1 = opt_flag;
					instr3->field2 = opt_calc;
					instr3->field3 = 0xFFFFF;
	
					/*Fix pointers*/
					instr3->prev = instr1->prev;
					instr1->prev->next = instr3;

					/*Delete instr1 and instr2*/
					free(instr1);
					free(instr2);
					instr1 = instr3;
					if(instr1->prev != NULL)
					{
						instr1 = instr1->prev;
					}
					continue;
				}
			}
		}
		instr1 = instr1->next;
	}

	PrintInstructionList(stdout, head);
	DestroyInstructionList(head);
	return EXIT_SUCCESS;
}
