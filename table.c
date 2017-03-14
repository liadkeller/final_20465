#include "table.h"
#include "utils.h"

struct list *table;

void addCmdToList(struct cmd *c, struct list *t)
{
	struct cmd *n;
	n = (struct cmd *) malloc (sizeof struct cmd); // !!! to check if this is the right way to use sizeof
	// !!! malloc - to make sure to free the pointer
	n->encode = c->encode;
	n->opcode = c->opcode;
	n->group = c->group;
	n->firstOperand = c->firstOperand;
	n->secndOperand = c->secndOperand;
	n->firstAddressing = c->firstAddressing;
	n->secndAddressing = c->secndAddressing;
	n->wordsNum = c->wordsNum;
	n->address = c->address;
	n->isSymbol = c->isSymbol;
	n->symbol = c->symbol;
	n->number = c->number;
	n->addressNumber = c->addressNumber;
	n->reg1 = c->reg1;
	n->reg2 = c->reg2;
	n->whichReg = c->whichReg;
	n->encodeType = c->encodeType;
	n->next = NULL;
	
	if(t->cmdHead == NULL)
		cmdHead = n;

	else
	{
		c = cmdHead; // uses c as temp
		while(c->next)
			c = c->next;
		c->next = n;
	}
}

void addDataToList(struct cmd *c, struct list *t);

void addCmd(char *cmd, int address)
{	// we use the phrase cmd. (command) as a code command (mov, lee, inc, ...) or as a general command (code command, data command)
	int i;	
	char inst[op_name_size+1]; // inst. = instruction

	struct cmd *new;
	struct cmd *nextWord, *nextNextWord;

	new->encode = MAIN_COMMAND;
	new->address = address;
	new->isSymbol = isSymbol(cmd);
	if(new->isSymbol)
		new->symbol = getSymbol(cmd);

	i = getCmdStart(cmd);
	strncpy(inst, cmd+i, op_name_size);
	inst[op_name_size] = '\0';
	new->opcode = getOpcode(inst);
	new->group = getGroup(inst);

	if(new->group > 0)
	{
		new->firstOperand = getFirstOperand(cmd+i);
		new->firstAddressing = getFirstAddressing(new->firstOperand)	
	}

	if(new->group > 1)
	{
		new->secndOperand = getSecndOperand(cmd+i);
		new->secndAddressing = getSecndAddressing(new->secndOperand)	
	}
	
	new->encodeType = A; // !!! will be defined as constant

	addCmdToList(new, table);

	
	nextWord->address = address+1;
	nextNextWord->address = address+2;

	if(new->group == 2 && new->firstAddressing = 3 && new->secndAddressing == 3) // 2 = TWO OPERANDS / 3 = ADRESSING MIUN !!! To Define
	{
		new->wordsNum = group-1;
		
		nextWord->encode = TWO_REGISTER;
		addCmdToList(nextWord, table);
	}

	else
		new->wordsNum = group;
	
	if(new->wordsNum = 1)
	{
		nextWord->encode = secndAddressing;
		addCmdToList(nextWord, table);
	}
		
	if(new->wordsNum = 2)
	{
		nextWord->encode = firstAddressing;
		nextNextWord->encode = secndAddressing;
		addCmdToList(nextWord, table);
		addCmdToList(nextNextWord, table);
	}
	
	/*return wordsNum;*/
}

int addData(char *cmd, int address)
{
	int i, dataOrStr, len = strlen(cmd);
	struct data *new;
	int wordsNum;
	
	// first
	new->isFirst = TRUE;
	new->address = address;
	new->isSymbol = isSymbol(cmd);
	if(new->isSymbol)
		new->symbol = getSymbol(cmd);
	
	i = getCmdStart(cmd);
	wordsNum = countWords(cmd+i);
	new->wordsNum = wordsNum;
	// add new to the list as the first word
	
	if(i = isData(cmd, i)) // !!! to make sure isData is local function
	{
		
		
		
	}
	
	if(i = isString(cmd, i)) 
	{
		if(cmd[i] != '"')
			//error	
		i++;
		
		while(cmd[i] != '\0')
		{
			new->isFirst = FALSE;
			new->address++; // !!! לבדוק תקינות של סדר קדימויות
			new->content = cmd[i]; // !!! לבדוק המרה לאינטגר
			// add new to the list
			
			i++;
		}
		// add zero to the end of the string
		new->address++;
		new->content = 0;
		//add
	}
	
	return wordsNum;
}
