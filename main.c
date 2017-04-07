#include "main.h"

int isError;
extern struct list table;
extern struct binarycode *binaryTable;
extern struct symbol *symbolTable;

int main(int argc, char *argv[])
{
	int IC, DC;
	char *fileName;
	FILE *f;
	int i, len;
	
	if(argc == 1)
	{
		fprintf(stderr, "Error - No files entered \n");
		return 0;
	}

	for(i = 1; i < argc; i++)
	{
		isError = FALSE;
		fileName = argv[i];
		len = strlen(fileName);
		
		f = fopen(strcat(fileName, ".as") , "r");
		fileName[len] = 0;
		
		if(!f)
		{
        		fprintf(stderr, "Error - File %s doesn't exist \n", fileName);
			continue;
		}
		
		firstLoop(f, &IC, &DC); /* we send IC and DC to the first loop and the first loop will put in it the right values */
		/*buildSymbolTable();*/
		secondLoop();
		
		if(!isError)
		{
			f = fopen(strcat(fileName, ".ob") , "w");
			fileName[len] = 0;
			FILE_ERROR(object)
			createObject(f,IC,DC);
		 
			/*if(table.extHead)
			{
				f = fopen(strcat(fileName, ".ext") , "w"); 
				fileName[len] = 0;
				FILE_ERROR(extern)
				createExtern(f);
			}
			
			if(table.entHead)
			{
				f = fopen(strcat(fileName, ".ent") , "w");  
				fileName[len] = 0;
				FILE_ERROR(entry)
				createEntry(f);
			}*/
		}

		/*freeAll();
		fclose(f);*/
	}
	
	return 0;
}

void firstLoop(FILE *f, int *ICp, int *DCp)
{
	char assemblyCommand[assembly_line_max+1];
	int IC = IC_start, DC = 0;
	
	while(fgets(assemblyCommand,assembly_line_max,f))
	{			
		deleteEnter(assemblyCommand);
		
		if(isBlankOrComment(assemblyCommand))
			continue;

		else if(isExt(assemblyCommand))
		  	addExt(assemblyCommand);
		
		else if(isEnt(assemblyCommand))
		  	addEnt(assemblyCommand);
		
		else if(isCode(assemblyCommand))
			IC += addCmd(assemblyCommand, IC);	

		else if(isData(assemblyCommand))
		  	DC += addData(assemblyCommand, DC);
		
		else if(isStr(assemblyCommand))
		  	DC += addStr(assemblyCommand,DC);
		
		else
		{
  			fprintf(stderr, "Error - Illegal Command \n");
 			isError = TRUE;
 		}
	}
	
	printf("%d,%d \n", IC, DC); /* !!! TEMP ONLY */
	
	fixAddresses(IC);
	
	*ICp = IC;
	*DCp = DC;
}

void freeAll()
{
	struct binarycode *binaryCur = binaryTable;
        struct ent *entCur = table.entHead;  
	struct ext *extCur = table.extHead;
	struct cmd *cmdCur = table.cmdHead; 
	struct data *dataCur = table.dataHead;
        /*struct symbol *symbolCur = symbolTable; */

	
	while(binaryCur)
	{
		struct binarycode *temp = binaryCur;
		binaryCur = temp->next;
		free(temp);
	}
	binaryTable=NULL;
	
	while(extCur)
	{
		struct ext *temp;		
		free(extCur->symbol);
		temp = extCur;
		extCur = temp->next;
		free(temp);
	}
	table.extHead=NULL;
	
	while(entCur)
	{
		struct ent *temp;		
		free(entCur->symbol);
		temp = entCur;
		entCur = temp->next;
		free(temp);
	}
	table.entHead=NULL;
	
	while(cmdCur)
	{
		struct cmd *temp;
		if(cmdCur->firstOperand)
			free(cmdCur->firstOperand);
		if(cmdCur->secndOperand)
			free(cmdCur->secndOperand);
		if(cmdCur->symbol)
			free(cmdCur->symbol);
		temp = cmdCur;
		cmdCur = temp->next;
		free(temp);
	}
	table.cmdHead=NULL;

	while(dataCur)
	{
		struct data *temp;		
		if(dataCur->symbol);
			/*free(dataCur->symbol);*/
		temp = dataCur;
		dataCur = temp->next;
		free(temp);
	}
	table.dataHead=NULL;
	
	/*while(symbolCur)
	{
		struct symbol *temp;
		if(symbolCur->name);
			/*free(symbolCur->name);*/
	/*	temp = symbolCur;
		symbolCur = temp->next;
		free(temp);
	}
	symbolCur=NULL;
	*/
}

void deleteEnter(char *assemblyCommand)
{
	int i = 0;
	while(assemblyCommand[i] != '\0')
		i++;
	
	if(assemblyCommand[--i] == '\n')
		assemblyCommand[i] = '\0';
}

void createObject(FILE *f,int IC, int DC)
{
	struct binarycode *cur = binaryTable;
        fprintf(f, "%X   %X \n", IC - IC_start, DC);
        
        while(cur)
        {
                fprintf(f,"%X   %s \n", cur->address, cur->binary);
                cur = cur->next;
        }
}
/*		
void createEntry(FILE *f)
{
        struct ent *cur = table.entHead;
        while(cur)
        {
                struct symbol *symbolCur = symbolTable;
                while(symbolCur)
                {
                    if(strcmp(symbolCur->name, cur->symbol) == 0)
                        fprintf(f,"%s   %X \n", symbolCur->name, symbolCur->address);
                        symbolCur = symbolCur->next;
                }
                cur = cur->next;
        }
}
		
void createExtern(FILE *f)
{
        struct cmd *cur = table.cmdHead;
        while(cur)
        {
                if(cur->group > 0)
                        if(cur->firstAddressing == ADDRESS)
                                if(getEntryAddress(cur->secndOperand) == 0)
                                	fprintf(f,"%s   %X \n", symbolCur->name, cur->address+1);
        
                if(cur->group > 1)
                {
                        if(cur->secndAddressing == ADDRESS)
                                if(getEntryAddress(cur->secndOperand) == 0)
                                        fprintf(f,"%s   %X \n", symbolCur->name, cur->address+2);
                cur = cur->next;
        }
}*/
