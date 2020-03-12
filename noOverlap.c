#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct file
{
	char line[210];/*This must be larger if the word is longer.*/
	struct file *next;
}_fe;


_fe *load_cmp(FILE *fpin)
{
	int i = 0;
	char ch;
	_fe *pre_head,*tail,*find;
	pre_head = (_fe*)malloc(sizeof(_fe));
	tail = pre_head;
	
	_fe *kanji = (_fe*)malloc(sizeof(_fe));
	while((ch = fgetc(fpin)) != 0xa)
		kanji->line[i++] = ch; /*Without EOF judgement,if file have only one line,a pointer have not been free.*/
	kanji->line[i] = '\0';
	kanji->next = NULL;
	tail->next = kanji;
	tail = kanji;

	while(1)
	{
		find = pre_head;
		_fe *kanji = (_fe*)malloc(sizeof(_fe));
		for(i = 0;(ch = fgetc(fpin)) != 0xa;i++)
		{
			if(ch == EOF)
			{
				free(kanji);
				return pre_head;
			}
			kanji->line[i] = ch;
		}
		kanji->line[i] = '\0';
		while(find != tail)
		{
			find = find->next;
			if(strcmp(kanji->line,find->line) == 0)
			{
				printf("%s\n",kanji->line);
				free(kanji);
				i = -1;
				break;
			}
		}
		if(i >= 0)
		{
			kanji->next = NULL;
			tail->next = kanji;
			tail = kanji;
		}
	}
}

void free_load_p(_fe *head)
{
	_fe *pointer = head;
	while(head)
	{
		pointer = pointer->next;
		free(head);
		head = pointer;
	}
}

void output_file(FILE *fpout,_fe *head)
{
	_fe *suf_head = head;
	_fe *pointer = suf_head->next;
	while(pointer)
	{
		fputs(pointer->line,fpout);
		fputc(0xa,fpout);
	//	fprintf(fpout,"%s\n",pointer->line);
		pointer = pointer->next;
	}
}

int main()
{
	FILE *fpin,*fpout;
/*	char input[60] = "";
	printf("Please input the file which to be processed:");
	scanf("%s",input);
*/	if((fpin = fopen("pyPhrase.org","r")) == NULL || (fpout = fopen("tmp.txt","w")) == NULL)
	{
		printf("Can't open file.");
		return 0;
	}
	_fe *head = load_cmp(fpin);
	output_file(fpout,head);
	free_load_p(head);
	fclose(fpin);
	fclose(fpout);
	return 0;
}
