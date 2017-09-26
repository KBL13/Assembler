#include <stdio.h>
#include <string.h>

struct codeEntry 
{
	int address;
	char label [80];
	char instruction[80];
};


int registerNum(char* instructString);

int main (int argc, char *argv[])
{
	struct codeEntry arr[100];

	char buffer[80];

	//instructions to be compared later for checks
	char ignore1[] = ".data";
	char ignore2[] = ".text";
	char directive1[] = ".space";
	char directive2[] = ".word";

	char la[] = "la";
	char add[] = "add";
	char sll[] = "sll";
	char nor[] = "nor";
	char addi[] = "addi";
	char ori[] = "ori";
	char lui[] = "lui";
	char sw[] = "sw";
	char lw[] = "lw";
	char bne[] = "bne";
	char j[] = "j";

	int instructArray[100];
	int addr = 0; //keeping track of address
	int count = 0; //keeps track of how many elements are entered into the array of structs

	//First past to allocate memory and pass data into the array of structs
	while (fgets(buffer,80,stdin) !=NULL)
	{
		//all temporary strings
		char a[80];
		char b[80];

		char random1[80]; 
		char random2[80];
		char random3[80];

		int random4; //variable used for .space and .word

		if(sscanf(buffer,"%[^:]:\t%[^\n]",a,b) == 2)
		{
			if (strncmp(b,la,2) == 0) //If the instruction is la create two separate instructions
			{
				strcpy(arr[count].label,a); //lui later
				strcpy(arr[count].instruction,b);
				arr[count].address = addr;	

				count = count + 1;
				addr = addr + 4;

				strcpy(arr[count].label,a); //ori later
				strcpy(arr[count].instruction,b);
				arr[count].address = addr; //store data and increment for loop

				count = count + 1;
				addr = addr + 4;
			}
			else 
			{
				if (strncmp(b,directive1,6) == 0 || strncmp(b,directive2,5) == 0)//if .space or .word do separate instructions
				{
					if (strncmp(b,directive1,6) == 0)
					{
						strcpy(arr[count].label,a);
						strcpy(arr[count].instruction,b);
						arr[count].address = addr;
						if(sscanf(b,"%s\t%d",random1,&random4) == 2)
						{
							addr = addr + random4;
						}
						count = count + 1;
					}
					else
					{
						strcpy(arr[count].label,a);
						strcpy(arr[count].instruction,b);
						arr[count].address = addr;
						if(sscanf(b,"%s\t%d",random1,&random4) == 2)
						{
							addr = addr + (random4*4);
						}
						count = count + 1;						
					}
				}
				else
				{
					strcpy(arr[count].label,a);
					strcpy(arr[count].instruction,b);
					arr[count].address = addr;

					addr = addr + 4;
					count = count + 1;
				}
			}
		}
		else if(sscanf(buffer,"\t%[^\n]",a) == 1)
		{
			if (strcmp(a,ignore1) == 0 || strcmp(a,ignore2) == 0)
			{
				//we do nothing
			}
			else
			{
				if (strncmp(a,la,2) == 0)//if we found la do separate instructions
				{
					strcpy(arr[count].instruction,a);
					arr[count].address = addr;

					count = count + 1;
					addr = addr + 4;

					strcpy(arr[count].instruction,a);
					arr[count].address = addr;

					count = count + 1;
					addr = addr + 4;
				}
				else
				{
					if (strncmp(a,directive1,6) == 0 || strncmp(a,directive2,5) == 0)
					{
						if (strncmp(a,directive1,6) == 0)
						{
							if(sscanf(a,"%s\t%d",random1,&random4) == 2)
							{
								addr = addr + random4;
							}
						}
						else
						{
							if(sscanf(a,"%s\t%d",random1,&random4) == 2)
							{
								addr = addr + (random4*4);
							}
						}
					}
					else
					{
						strcpy(arr[count].instruction,a);
						arr[count].address = addr;

						addr = addr + 4;
						count = count + 1;
					}
				}
			}
		}

		else
		{
			//do nothing
		}

	}
	//Second pass begins to start decoding
	int i;
	for (i = 0; i < count; i++)
	{
		//temp strings
		char c[80]; 
		char d[80];
		char e[80];
		char f[80];
		char g[80];
		char h[80];


		char temp1[80];
		char temp2[80];
		char temp3[80];

		unsigned int temp = 0;
		//while unnecessary decides if each instruction are r-type, i-type, j-type. Was really helpful during debugging
		if (strncmp(arr[i].instruction,add,3) == 0 || strncmp(arr[i].instruction,nor,3) == 0 || strncmp(arr[i].instruction,sll,3) == 0)
		{
			//add block
			if (strncmp(arr[i].instruction,add,3) == 0)
			{
				if(sscanf(arr[i].instruction,"%s\t%s",c,d) == 2)
				{
					if (sscanf(d,"%[^,],%[^,],%s",e,f,g) == 3)
					{
							strcpy(temp1,e);
							strcpy(temp2,f);
							strcpy(temp3,g);
							instructArray[i] = 32 + 0 + (registerNum(temp1) << 11) + (registerNum(temp3) << 16) + (registerNum(temp2) << 21);
					}
				}
			}

			//nor block
			if (strncmp(arr[i].instruction,nor,3) == 0)
			{
				if(sscanf(arr[i].instruction,"%s\t%s]",c,d) == 2)
				{
					if (sscanf(d,"%[^,],%[^,],%s",e,f,g) == 3)
					{
							strcpy(temp1,e);
							strcpy(temp2,f);
							strcpy(temp3,g);
							instructArray[i] = 39 + 0 + (registerNum(temp1) << 11) + (registerNum(temp3) << 16) + (registerNum(temp2) << 21);
					}
				}							
			}

			//sll block
			if (strncmp(arr[i].instruction,sll,3) == 0)
			{
				if(sscanf(arr[i].instruction,"%s\t%s",c,d) == 2)
				{
					if (sscanf(d,"%[^,],%[^,],%d",e,f,&temp) == 3)
					{
						strcpy(temp1,e);
						strcpy(temp2,f);
						instructArray[i] = 0 + (temp << 6) + (registerNum(temp1) << 11) + (registerNum(temp2) << 16);
					}
				}					
			}
		}

		//while unnecessary decides if each instruction are r-type, i-type, j-type. Was really helpful during debugging 
		if (strncmp(arr[i].instruction,addi,4) == 0 || strncmp(arr[i].instruction,ori,3) == 0 || strncmp(arr[i].instruction,lui,3) == 0 || strncmp(arr[i].instruction,sw,2) == 0 || strncmp(arr[i].instruction,lw,2) == 0 || strncmp(arr[i].instruction,bne,3) == 0 || strncmp(arr[i].instruction,la,2) == 0)
		{
			if (strncmp(arr[i].instruction,addi,4) == 0)
			{
				if(sscanf(arr[i].instruction,"%s\t%s",c,d) == 2)
				{
					if (sscanf(d,"%[^,],%[^,],%d",e,f,&temp) == 3)
					{
						strcpy(temp1,e);
						strcpy(temp2,f);
						instructArray[i] = (temp & 65535) + (registerNum(temp1) << 16) + (registerNum(temp2) << 21) + (8 << 26);
					}
				}
			}

			//or immed block	
			if(strncmp(arr[i].instruction,ori,3) == 0)
			{
				if(sscanf(arr[i].instruction,"%s\t%s",c,d) == 2)
				{
					if (sscanf(d,"%[^,],%[^,],%d",e,f,&temp) == 3) 
					{
						strcpy(temp1,e);
						strcpy(temp2,f);
						instructArray[i] = (temp & 65535) + (registerNum(temp1) << 16) + (registerNum(temp2) << 21) + (13 << 26);
					}
				}				
			}

			//load upper immed block
			if (strncmp(arr[i].instruction,lui,3) == 0)
			{
				if(sscanf(arr[i].instruction,"%s\t%s",c,d) == 2) //split string into components to be loaded into array as an int
				{
					if (sscanf(d,"%[^,],%d",e,&temp) == 2) 
					{
						strcpy(temp1,e);
						instructArray[i] = (temp & 65535) + (registerNum(temp1) << 16) + (15 << 26);
					}
				}					
			}

			//store word
			if (strncmp(arr[i].instruction,sw,2) == 0)
			{
				if(sscanf(arr[i].instruction,"%s\t%s",c,d) == 2)
				{
					if (sscanf(d,"%[^,],%d(%[^)]",e,&temp,f) == 3) 
					{
						strcpy(temp1,e);
						strcpy(temp2,f);
						instructArray[i] = (temp & 65535) + (registerNum(temp1) << 16) + (registerNum(temp2) << 21) + (43 << 26);
					}
				}				
			}

			//load word block
			if (strncmp(arr[i].instruction,lw,2) == 0)
			{
				if(sscanf(arr[i].instruction,"%s\t%s",c,d) == 2)
				{
					if (sscanf(d,"%[^,],%d(%[^)]",e,&temp,f) == 3) 
					{
						strcpy(temp1,e);
						strcpy(temp2,f);
						instructArray[i] = (temp & 65535) + (registerNum(temp1) << 16) + (registerNum(temp2) << 21) + (35 << 26); //load int instruction into array
					}
				}					
			}

			//branch block
			if(strncmp(arr[i].instruction,bne,3) == 0)
			{
				if(sscanf(arr[i].instruction,"%s\t%s",c,d) == 2)
				{
					if (sscanf(d,"%[^,],%[^,],%s",e,f,g) == 3)//split into components 
					{
						strcpy(temp1,e);
						strcpy(temp2,f);
						strcpy(temp3,g);
						int r;
						int lebron;
						for (r = 0; r < count; r++) //for block to find where the label is
						{
							if(strcmp(arr[r].label,temp3) == 0)
							{
								lebron = (arr[r].address - arr[i].address); //find displacement to get address
							}
						}							
						instructArray[i] = (lebron & 65535) + (registerNum(temp2) << 16) + (registerNum(temp1) << 21) + (5 << 26); //store instruction into array
					}
				}				
			}

			//load address block
			if (strncmp(arr[i].instruction,la,2) == 0)
			{
				int kobe;
				int top;
				int lower;
				if(sscanf(arr[i].instruction,"%s\t%s",c,d) == 2)//
				{
					if (sscanf(d,"%[^,],%s",e,f) == 2) //splits string into the $rt and the label
					{
						strcpy(temp1,e);
						strcpy(temp2,f);
					}
				}
				int j;
				for (j = 0; j < count; j++) //finds the address of the label
				{
					if(strcmp(arr[j].label,temp2) == 0)
					{
						kobe = arr[j].address; //stores address once found
					}
				}
				top = kobe >> 16; //top 16 bits
				lower = kobe & 0x0000FFFF; //lower 16 bits
				instructArray[i] = (top & 65535) + (1 << 16) + (15 << 26); //instruction for lui
				instructArray[i+1] = (lower & 65535) + (registerNum(temp1) << 16) + (1 << 21) + (13 << 26); //instruction for ori
				i = i + 1; //increment i to skip
			}		
		}

		//jump instruction block
		if (strncmp(arr[i].instruction,j,1) == 0)
		{
			int random5;
			if(sscanf(arr[i].instruction,"%s\t%s",c,d) == 2)
			{
				if (sscanf(d,"%[^,],%s",e,f) == 2) 
				{
					strcpy(temp1,e);
					strcpy(temp2,f);
				}
				int l;
				for (l = 0; l < count; l++)
				{
					if(strcmp(arr[l].label,temp2) == 0)
					{
						random5 = arr[l].address - arr[i].address;
					}
				}
				instructArray[i] = ((random5 & 65535) << 2) + (2 << 26);
			}						
		}
	}

	//prints the memory address and the hexadecimal for machine code
	int q;
	for (q = 0; q < count; q++)
	{
		if (strncmp(arr[q].instruction,directive1,6) == 0 || strncmp(arr[q].instruction,directive2,5) == 0)
		{
			//print nothing
		}
		else
		{
			printf("0x%06X: ", arr[q].address); //prints out in hex
			printf("0x%08X\n", instructArray[q]);
		}
	}

	return 0;
}

//passes in registers and decides a value
int registerNum(char* instructString) 
{
	char t0[] = "$t0";
	char t1[] = "$t1";
	char t2[] = "$t2";
	char t3[] = "$t3";
	char t4[] = "$t4";
	char t5[] = "$t5";
	char t6[] = "$t6";
	char t7[] = "$t7";

	char s0[] = "$s0";
	char s1[] = "$s1";
	char s2[] = "$s2";
	char s3[] = "$s3";
	char s4[] = "$s4";
	char s5[] = "$s5";
	char s6[] = "$s6";
	char s7[] = "$s7";

	char zero[] = "$0";	
	char one[] = "$1";

	if(strcmp(instructString, t0) == 0)
	{
		return 8;
	}
	if(strcmp(instructString, t1) == 0)
	{
		return 9;
	}
	if(strcmp(instructString, t2) == 0)
	{
		return 10;
	}
	if(strcmp(instructString, t3) == 0)
	{
		return 11;
	}
	if(strcmp(instructString, t4) == 0)
	{
		return 12;
	}
	if(strcmp(instructString, t5) == 0)
	{
		return 13;
	}
	if(strcmp(instructString, t6) == 0)
	{
		return 14;
	}
	if(strcmp(instructString, t7) == 0)
	{
		return 15;
	}
	//$s registers start
	if(strcmp(instructString, s0) == 0)
	{
		return 16;
	}
	if(strcmp(instructString, s1) == 0)
	{
		return 17;
	}
	if(strcmp(instructString, s2) == 0)
	{
		return 18;
	}
	if(strcmp(instructString, s3) == 0)
	{
		return 19;
	}
	if(strcmp(instructString, s4) == 0)
	{
		return 20;
	}
	if(strcmp(instructString, s5) == 0)
	{
		return 21;
	}
	if(strcmp(instructString, s6) == 0)
	{
		return 22;
	}
	if(strcmp(instructString, s7) == 0)
	{
		return 23;
	}
	if(strcmp(instructString, zero) == 0)
	{
		return 0;
	}
	if(strcmp(instructString, one) == 0)
	{
		return 1;
	}

}
