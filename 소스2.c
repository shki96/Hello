#include <stdio.h>

#include <string.h>

#include <ctype.h>

#include <math.h>

#include <stdlib.h>



struct reg

{

	char reg_name[3];

	char reg_num[4];

}Reg[20]; // �������Ϳ� ���� �̸��� ��ȣ�� �����ϴ� ����ü



struct ins

{

	char instruct[3];

	char dest[2];

	char sour[2];

	char word_type[2];

	char ins_code[3];

	char ins_len[2];

	char mod_reg[9];

}Instr[100], modInstr[100], Instr2[100], modInstr2[100]; // �� �ν�Ʈ������ ������ �����ϴ� ����ü, obj ����ü

int MaxI, Maxll, InstrP = 0;



struct symbol_tbl

{

	char symbol[10];

	char word_type[2];

	int lc;

	char data[10];

} Symbol[30], Symbol2[30];  // �ɺ� ���̺�

int MaxS = 0;



struct sentence

{

	char label[10];

	char _operator[10];

	char operand[3][10];

} Sen, Sen2; // ���� �ڵ� �ӽ� ���� ����ü

int LC;

int LC2;



FILE *ObjSave;  // objcode.txt ����

FILE *SymbolSave; // symbol.txt ����



				  // ���Ͽ��� �������Ϳ� �ν�Ʈ������ ������ �д´�.

void Initialize()

{

	int i = 0, j = 1;

	FILE *regi, *inst;



	regi = fopen("reg.tbl", "r");

	inst = fopen("inst.tbl", "r");

	while (!feof(regi))

	{

		fscanf(regi, "%s%s\n", Reg[i].reg_name, Reg[i].reg_num);

		i++;

	} // �������� ���̺��� �ۼ��Ѵ�.



	while (!feof(inst))

	{

		fscanf(inst, "%6s%2s%2s%4s%3s%2s%9s\n", Instr[j].instruct, Instr[j].dest, Instr[j].sour,

			Instr[j].word_type, Instr[j].ins_code, Instr[j].ins_len, Instr[j].mod_reg);

		j++;

	} // ��ɾ� ���̺��� �ۼ��Ѵ�.



	while (!feof(inst))

	{

		fscanf(inst, "%6s%2s%2s%4s%3s%2s%9s\n", Instr2[j].instruct, Instr2[j].dest, Instr2[j].sour,

			Instr2[j].word_type, Instr2[j].ins_code, Instr2[j].ins_len, Instr2[j].mod_reg);

		j++;

	} // ��ɾ� ���̺��� �ۼ��Ѵ�.

	MaxI = j - 1;

	Maxll = j - 1;

	fclose(regi);

	fclose(inst);

}



int Analyze(char *operand)

{

	int i = 0;

	char *regist[] = { "AX", "BX", "CX" ,"DX", "AL", "BL", "CL", "DL", "AH", "BH", "CH", "DH", 0x00 };

	// �������� �̸��� ����



	if (isdigit(operand[0])) return 0; // immediate ��巹�� ��带 ����

	else

	{

		while (regist[i] != 0x00)

		{

			if (!strcmp(operand, regist[i]))

			{

				if (i<4) return 1; // �������� ��� + ����

				else return 2; // �������� ��� + ����Ʈ

			}

			else i++;

		}

	}

	return 3;   // �޸� ���� ���

}



#define MAX_INS 1   // ��ɾ��� �ִ� ������ ����

int Mov_Chk(char *sen) // ���۷����� ��巹�� ��带 ����

{

	int k = MaxI;

	int i = 0, j = 0, l = 0, wp = 0;

	char op[6][10];

	char *opcode[] = { "mov", "" };

	while (sen[wp] != '\n')

	{

		while (sen[wp] == ' ' || sen[wp] == '\t' || sen[wp] == ', ')wp++;// ����, ��, �޸��� ���

		while (sen[wp] != ' ' && sen[wp] != '\n' && sen[wp] != ', ')

		{

			*(op[j] + i) = sen[wp];

			i++;

			wp++;

		}

		*(op[j] + i) = '\0';

		i = 0;

		j++;

	}

	i = 0;

	while (strcmp(opcode[i], ""))  // ��ϵ� ��ɾ��� ������ ��

	{

		if (stricmp(opcode[i], op[0]))i++; //mov ��ɾ�� op[0]�� �ٸ���, ���� ��ɾ� ��ȸ

		else

		{

			strcpy(Sen._operator, op[0]);  // ��ɾ _operator�� ����

			for (l = 1; l<j; l++)

				strcpy(Sen.operand[l - 1], op[l]); // ��ɾ� �� ������ operand�� ����

			break;

		}

	}

	if (i == MAX_INS)//i=1�̸�, ��ɾ ã�� ���� ��(��ɾ� 1�� ����)

	{

		strcpy(Sen.label, op[0]);   // �� ��� ù �ܾ ���̺� ���

		strcpy(Sen._operator, op[1]);  // �ι�° �ܾ ��ɾ ���

		for (l = 2; l<j; l++)

			strcpy(Sen.operand[l - 2], op[l]); // �� ���� ������ operand�� ����

	}// �� ������ �м��Ͽ� ���̺�, ���۷����Ϳ� ���۷���� �з��Ѵ�.

	strcpy(Instr[0].instruct, op[0]);  // ��ɾ instruct�� ����

	switch (Analyze(op[1])) // ������ �м�

	{

	case 0: strcpy(Instr[0].dest, "i");  // �� ��� �Է� ���

		break;

	case 1: strcpy(Instr[0].dest, "r"); // �������� ��� + ����

		strcpy(Instr[0].word_type, "w");

		break;

	case 2: strcpy(Instr[0].dest, "r"); // �������͹�� + ����Ʈ

		strcpy(Instr[0].word_type, "b");

		break;

	case 3: strcpy(Instr[0].dest, "m"); // �޸� ���� ���

		break;

	}

	switch (Analyze(op[2]))   // ����� �м�

	{

	case 0: strcpy(Instr[0].sour, "i"); // �� ��� �Է� ���

		break;

	case 1: strcpy(Instr[0].sour, "r"); // �������� ��� + ����

		strcpy(Instr[0].word_type, "w");

		break;

	case 2: strcpy(Instr[0].sour, "r"); // �������� ��� + ����Ʈ

		strcpy(Instr[0].word_type, "b");

		break;

	case 3: strcpy(Instr[0].sour, "m"); // �޸� ���� ���

		break;

	}

	while (stricmp(Instr[k].instruct, Instr[0].instruct) || strcmp(Instr[k].dest, Instr[0].dest)

		|| strcmp(Instr[k].sour, Instr[0].sour) || strcmp(Instr[k].word_type, Instr[0].word_type))

	{    // instruct, �� ���� ��ɾ��,inst.tbl���� ������ ��ɾ ��

		k--;  // ���� ���� ã�� ������,k�� �����ϸ� ��

	}

	return k; // ã�� �ε��� k�� ����

}



int Add_Chk(char *sen) // ���۷����� ��巹�� ��带 ����

{

	int k = Maxll;

	int i = 0, j = 0, l = 0, wp = 0;

	char op[6][10];

	char *opcode[] = { "add", "" };

	while (sen[wp] != '\n')

	{

		while (sen[wp] == ' ' || sen[wp] == '\t' || sen[wp] == ', ')wp++;// ����, ��, �޸��� ���

		while (sen[wp] != ' ' && sen[wp] != '\n' && sen[wp] != ', ')

		{

			*(op[j] + i) = sen[wp];

			i++;

			wp++;

		}

		*(op[j] + i) = '\0';

		i = 0;

		j++;

	}

	i = 0;

	while (strcmp(opcode[i], ""))  // ��ϵ� ��ɾ��� ������ ��

	{

		if (stricmp(opcode[i], op[0]))i++; //add ��ɾ�� op[0]�� �ٸ���, ���� ��ɾ� ��ȸ

		else

		{

			strcpy(Sen2._operator, op[0]);  // ��ɾ _operator�� ����

			for (l = 1; l<j; l++)

				strcpy(Sen2.operand[l - 1], op[l]); // ��ɾ� �� ������ operand�� ����

			break;

		}

	}

	if (i == MAX_INS)//i=1�̸�, ��ɾ ã�� ���� ��(��ɾ� 1�� ����)

	{

		strcpy(Sen2.label, op[0]);   // �� ��� ù �ܾ ���̺� ���

		strcpy(Sen2._operator, op[1]);  // �ι�° �ܾ ��ɾ ���

		for (l = 2; l<j; l++)

			strcpy(Sen2.operand[l - 2], op[l]); // �� ���� ������ operand�� ����

	}// �� ������ �м��Ͽ� ���̺�, ���۷����Ϳ� ���۷���� �з��Ѵ�.

	strcpy(Instr2[0].instruct, op[0]);  // ��ɾ instruct�� ����

	switch (Analyze(op[1])) // ������ �м�

	{

	case 0: strcpy(Instr2[0].dest, "i");  // �� ��� �Է� ���

		break;

	case 1: strcpy(Instr2[0].dest, "r"); // �������� ��� + ����

		strcpy(Instr2[0].word_type, "w");

		break;

	case 2: strcpy(Instr2[0].dest, "r"); // �������͹�� + ����Ʈ

		strcpy(Instr2[0].word_type, "b");

		break;

	case 3: strcpy(Instr2[0].dest, "m"); // �޸� ���� ���

		break;

	}

	switch (Analyze(op[2]))   // ����� �м�

	{

	case 0: strcpy(Instr2[0].sour, "i"); // �� ��� �Է� ���

		break;

	case 1: strcpy(Instr2[0].sour, "r"); // �������� ��� + ����

		strcpy(Instr2[0].word_type, "w");

		break;

	case 2: strcpy(Instr2[0].sour, "r"); // �������� ��� + ����Ʈ

		strcpy(Instr2[0].word_type, "b");

		break;

	case 3: strcpy(Instr2[0].sour, "m"); // �޸� ���� ���

		break;

	}

	while (stricmp(Instr2[k].instruct, Instr2[0].instruct) || strcmp(Instr2[k].dest, Instr2[0].dest)

		|| strcmp(Instr2[k].sour, Instr2[0].sour) || strcmp(Instr2[k].word_type, Instr2[0].word_type))

	{    // instruct, �� ���� ��ɾ��,inst.tbl���� ������ ��ɾ ��

		k--;  // ���� ���� ã�� ������,k�� �����ϸ� ��

	}

	return k; // ã�� �ε��� k�� ����

}



void PassI(char *buf)

{

	int i, ii;

	static int j = 0;

	static int jj = 0;



	i = Mov_Chk(buf);

	// ���� �ڵ��� ��ɾ�� �ּ� ����� ���� ����.

	if (i) // mov ��ɾ ���� ó��

	{

		printf("%04X:%s", LC, buf); // �ּҰ�, ���� �ڵ� ���

		LC += atoi(Instr[i].ins_len); // LC=LC+ ��ɾ� ����

	}

	else // ��ȣ�� ���� ó��

	{

		if (!stricmp(Sen._operator, "dw"))

			strcpy(Symbol[j].word_type, "w");  // dw ��� w�� ����

		else if (!stricmp(Sen._operator, "db"))

			strcpy(Symbol[j].word_type, "b"); // db��� b�� ����

		strcpy(Symbol[j].symbol, Sen.label); // ���̺��� �ɺ��� ����

		strcpy(Symbol[j].data, Sen.operand[0]); // ���̺��� ���� ����

		Symbol[j].lc = LC;    // �ּҰ��� ����

		printf("%04X:%s", LC, buf);  // �ּҰ�, ���� �ڵ� ���

		if (*Symbol[j].word_type == 'w') LC += 2; // �ּҰ��� w=2,b=1��ŭ ����

		else if (*Symbol[j].word_type == 'b') LC += 1;

		j++;  //j�� �������� ������ �ɺ� �ε����� ������.

		MaxS++;

	}



	ii = Add_Chk(buf);

	// ���� �ڵ��� ��ɾ�� �ּ� ����� ���� ����.

	if (ii) // add ��ɾ ���� ó��

	{

		// printf("%04X:%s", LC, buf); // �ּҰ�, ���� �ڵ� ���

		LC2 += atoi(Instr2[ii].ins_len); // LC=LC+ ��ɾ� ����

	}

	else // ��ȣ�� ���� ó��

	{

		if (!stricmp(Sen2._operator, "dw"))

			strcpy(Symbol2[jj].word_type, "w");  // dw ��� w�� ����

		else if (!stricmp(Sen2._operator, "db"))

			strcpy(Symbol2[jj].word_type, "b"); // db��� b�� ����

		strcpy(Symbol2[jj].symbol, Sen2.label); // ���̺��� �ɺ��� ����

		strcpy(Symbol2[jj].data, Sen2.operand[0]); // ���̺��� ���� ����

		Symbol2[jj].lc = LC2;    // �ּҰ��� ����

								 // printf("%04X:%s", LC, buf);  // �ּҰ�, ���� �ڵ� ���

		if (*Symbol2[jj].word_type == 'w') LC2 += 2; // �ּҰ��� w=2,b=1��ŭ ����

		else if (*Symbol2[jj].word_type == 'b') LC2 += 1;

		jj++;  //j�� �������� ������ �ɺ� �ε����� ������.

		MaxS++;

	}

}



int btoi(char *dig)

{

	int i = 0, ret = 0;



	while (*(dig + i) != '\0')

	{

		if (*(dig + i) == '1') ret += pow(2, strlen(dig + i) - 1);

		//dig+i�� ���̴� dig(8)+i(0)���� -1�ϸ� 7�̴�.

		// �������� i�� �����ϹǷ�,7,6,5,4,3,2,1,0���� �پ���.

		i++;

	}

	return ret;

}



void PassII(char *buf)

{

	int i, j = 0, k = 0;

	ObjSave = fopen("objCode.txt", "a+");

	i = Mov_Chk(buf);

	if (i) //i!=0�� ��, �� ��ɾ��� ���,

	{

		modInstr[InstrP] = Instr[i];

		printf("%04x:%3s", LC, Instr[i].ins_code); // LC�� ��ɾ� �ڵ� ���

		fprintf(ObjSave, "%04x:%3s", LC, Instr[i].ins_code);

		if (!strcmp(Instr[i].dest, "r"))  // �������� r�̸�

		{

			while (stricmp(Reg[j].reg_name, Sen.operand[0]))  // �������� �������� �̸��� ���� ����

				j++;

			strncpy(strchr(modInstr[InstrP].mod_reg, '?'), Reg[j].reg_num, 3);

		}



		j = 0;

		if (!strcmp(Instr[i].sour, "r"))// ������� r�̸�

		{

			while (stricmp(Reg[j].reg_name, Sen.operand[1]))  // ������� �������� �̸��� ���� ����

				j++;

			strncpy(strchr(modInstr[InstrP].mod_reg, '?'), Reg[j].reg_num, 3);

			//???�� �������� ǥ�� ������ ä��

		}



		if (strcmp(Instr[i].dest, "m") && strcmp(Instr[i].sour, "m")) // ������, ������� �Ѵ� m�� �ƴϸ�

		{

			printf("%02X\t\t%s", btoi(modInstr[InstrP].mod_reg), buf);

			//Instr�� ��ɾ� �ڵ带 16���� ��ȯ �� ���, ���� �ڵ� ���

			fprintf(ObjSave, "%02X\t\t%s", btoi(modInstr[InstrP].mod_reg), buf);

		}

		else// ������ or ������� ��ȣ�� �� ���

		{

			if (!strcmp(Instr[i].dest, "m"))  // �������� m�� ���

				while (strcmp(Symbol[k].symbol, Sen.operand[0]))k++;

			// �ɺ��� ���� operand�� ã��. �� ��ȣ�� ã��

			else if (!strcmp(Instr[i].sour, "m"))  // ������� m�� ���

				while (strcmp(Symbol[k].symbol, Sen.operand[1]))k++;

			// �ɺ��� ���� operand�� ã��. �� ��ȣ�� ã��

			if (Symbol[k].lc == (atoi(Symbol[k].data))) // ������, ���ġ ���ʿ�

			{

				printf(" %02X\t%04X\t%s", btoi(modInstr[InstrP].mod_reg), Symbol[k].lc, buf);

				fprintf(ObjSave, " %02X\t%04X\t%s", btoi(modInstr[InstrP].mod_reg), Symbol[k].lc, buf);

			}

			else // �ٸ��� ���ġ �ʿ�

			{

				printf(" %02X\t%04XR\t%s", btoi(modInstr[InstrP].mod_reg), Symbol[k].lc, buf);

				fprintf(ObjSave, " %02X\t%04XR\t%s", btoi(modInstr[InstrP].mod_reg), Symbol[k].lc, buf);

				//Instr�� ��ɾ� �ڵ带 16���� ��ȯ �� ���, ���� �ڵ� ���

			}

		}

		LC += atoi(Instr[i].ins_len);  //LC=LC+ ��ɾ� ����

	}



	int ii, jj = 0, kk = 0;

	ii = Add_Chk(buf);

	if (ii) //ii!=0�� ��, �� ��ɾ��� ���,

	{

		modInstr2[InstrP] = Instr2[ii];

		printf("%04x:%3s", LC2, Instr2[ii].ins_code); // LC�� ��ɾ� �ڵ� ���

		fprintf(ObjSave, "%04x:%3s", LC2, Instr2[ii].ins_code);

		if (!strcmp(Instr2[ii].dest, "r"))  // �������� r�̸�

		{

			while (stricmp(Reg[jj].reg_name, Sen2.operand[0]))  // �������� �������� �̸��� ���� ����

				jj++;

			strncpy(strchr(modInstr2[InstrP].mod_reg, '?'), Reg[jj].reg_num, 3);

		}



		jj = 0;

		if (!strcmp(Instr2[ii].sour, "r"))// ������� r�̸�

		{

			while (stricmp(Reg[jj].reg_name, Sen2.operand[1]))  // ������� �������� �̸��� ���� ����

				jj++;

			strncpy(strchr(modInstr2[InstrP].mod_reg, '?'), Reg[jj].reg_num, 3);

			//???�� �������� ǥ�� ������ ä��

		}



		if (strcmp(Instr2[ii].dest, "m") && strcmp(Instr2[ii].sour, "m")) // ������, ������� �Ѵ� m�� �ƴϸ�

		{

			printf("%02X\t\t%s", btoi(modInstr2[InstrP].mod_reg), buf);

			//Instr�� ��ɾ� �ڵ带 16���� ��ȯ �� ���, ���� �ڵ� ���

			fprintf(ObjSave, "%02X\t\t%s", btoi(modInstr2[InstrP].mod_reg), buf);

		}

		else// ������ or ������� ��ȣ�� �� ���

		{

			if (!strcmp(Instr2[ii].dest, "m"))  // �������� m�� ���

				while (strcmp(Symbol2[kk].symbol, Sen2.operand[0]))kk++;

			// �ɺ��� ���� operand�� ã��. �� ��ȣ�� ã��

			else if (!strcmp(Instr2[ii].sour, "m"))  // ������� m�� ���

				while (strcmp(Symbol2[kk].symbol, Sen2.operand[1]))kk++;

			// �ɺ��� ���� operand�� ã��. �� ��ȣ�� ã��

			if (Symbol2[kk].lc == (atoi(Symbol2[kk].data))) // ������, ���ġ ���ʿ�

			{

				printf(" %02X\t%04X\t%s", btoi(modInstr2[InstrP].mod_reg), Symbol2[kk].lc, buf);

				fprintf(ObjSave, " %02X\t%04X\t%s", btoi(modInstr2[InstrP].mod_reg), Symbol2[kk].lc, buf);

			}

			else // �ٸ��� ���ġ �ʿ�

			{

				printf(" %02X\t%04XR\t%s", btoi(modInstr2[InstrP].mod_reg), Symbol2[kk].lc, buf);

				fprintf(ObjSave, " %02X\t%04XR\t%s", btoi(modInstr2[InstrP].mod_reg), Symbol2[kk].lc, buf);

				//Instr�� ��ɾ� �ڵ带 16���� ��ȯ �� ���, ���� �ڵ� ���

			}

		}

		LC2 += atoi(Instr2[ii].ins_len);  //LC=LC+ ��ɾ� ����

	}



	if (i == 0)// ��ȣ�� ���,

	{

		k = 0;

		while (strcmp(Symbol[k].symbol, Sen.label))k++; // symbol[k]�� label�� ���� ������ ����. ���� ����

		if (!strcmp(Symbol[k].word_type, "w")) // Ÿ���� w��

		{

			printf("%04X:%04X\t\t%s", LC, atoi(Symbol[k].data), buf);

			fprintf(ObjSave, "%04X:%04X\t\t%s", LC, atoi(Symbol[k].data), buf);

		}

		if (!strcmp(Symbol[k].word_type, "b")) // Ÿ���� b��

		{

			printf("%04X:%02X\t\t\t%s", LC, atoi(Symbol[k].data), buf);

			fprintf(ObjSave, "%04X:%02X\t\t\t%s", LC, atoi(Symbol[k].data), buf);

		}

		if (*Symbol[k].word_type == 'w')LC += 2;//w�� �ּ� �� 2����,b�� �ּ� �� 1����

		else if (*Symbol[k].word_type == 'b')LC += 1;

	}



	//###################################################################################

	InstrP++;

	fclose(ObjSave);

}



// ��ȣǥ ���

void Symbol_Print()

{

	int i = 0;

	SymbolSave = fopen("symbol.txt", "w+");

	printf("\n*Symbol Table *\n");

	printf("SYMBOL\tDATA(ADDRESS)\tRELOCATION\n");

	for (i = 0; i<MaxS; i++)

	{ // symbol[k]�� label�� ���� ������ ����. ���� ����

		if (!strcmp(Symbol[i].word_type, "w")) // Ÿ���� w

		{

			printf("%s\t%X\t\t%d\n", Symbol[i].symbol, Symbol[i].lc, (Symbol[i].lc != atoi(Symbol[i].data) ? 1 : 0));

			// �ּ� ��, ���̺� ��, ���� �ڵ�

			fprintf(SymbolSave, "%s\t%X\t\t%d\n", Symbol[i].symbol, Symbol[i].lc, (Symbol[i].lc != atoi(Symbol[i].data) ? 1 : 0));

		}

		if (!strcmp(Symbol[i].word_type, "b")) // Ÿ���� b

		{

			printf("%s\t%X\t\t%d\n", Symbol[i].symbol, Symbol[i].lc, (Symbol[i].lc != atoi(Symbol[i].data) ? 1 : 0));

			// �ּҰ�, ���̺� ��, ���� �ڵ�

			fprintf(SymbolSave, "%s\t%X\t\t%d\n", Symbol[i].symbol, Symbol[i].lc, (Symbol[i].lc != atoi(Symbol[i].data) ? 1 : 0));

		}

	}



}





void main()

{

	char buf[50];

	FILE *in;



	in = fopen("test1.asm", "r");

	Initialize();  // �������� ǥ, ��ɾ� ǥ ����

	printf("\nPass1:\n");

	while (1)   // �н� 1

	{

		fgets(buf, 30, in);

		if (feof(in)) break;

		PassI(buf);

	}

	Symbol_Print();  // ��ȣǥ ���



	rewind(in);    // ������ �ʱ�ȭ

	LC = 0;

	LC2 = 0;

	ObjSave = fopen("objCode.txt", "w+");

	printf("\nPass2:\n");

	while (1)   // �н� 2

	{

		fgets(buf, 30, in);

		if (feof(in)) break;

		PassII(buf);

	}

	fclose(in);

}