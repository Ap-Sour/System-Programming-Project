#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "functions.h"


int main(int argc,char **argv)
{																//ORISMATA
	int lines=0,K=10;											//To K se periptwsh poy den dothei exei default timh
	long int curid,previd,t_id;									//To id tou current keimenou kai to id tou prohgoumenou gia epalitheush
	size_t size_buffer=20;
	char c;								
	char *buffer, *token ,*token2, *temp=NULL;					//Strings gia thn anagnwsh apo ta texts kai to stdin
	char** map;													//Domh apothikeushs olwn twn keimenwn 
	Trie *trie_head=NULL;
	FILE *docfile=NULL;
	

	buffer=malloc(size_buffer *sizeof(char));									//Anagnwsh twn inline parameters
	if( (argc<2) && (argc>5)){printf("Error,wrong number of inline arguments\n");exit(1);}

	while(( c=getopt(argc,argv, "i:k:")) != -1)
	{
		switch(c)
		{
			case 'k':
				K=atoi(optarg);
				break;
			case 'i':
				if((docfile=fopen(optarg,"r"))==NULL){perror("error opening document file\n");exit(1);}
				break;
			default:
				break;	
		}
	}

	if(docfile==NULL)															//Anoigma docfile
	{
		printf("Error handling file \n");
		exit(1);
	}

	while((c=getc(docfile)) != EOF)												//Oi grammes tou arxeiou
	{	
		if(c=='\n')
		{
			lines++;
		}	
	}
	printf("The file has:%d texts \n",lines);
	map=malloc(lines * sizeof(char*));
	rewind(docfile);
/////////////////////////Elegxoi ws edw/////////////////////
	

	previd=-1;																	//An to curid-previd!=1 tote den briskontai se auksousa seira me diafora 1
	while(-1 != getline(&buffer,&size_buffer,docfile))							//Apospash kathe grammhs,elegxos kai ekxwrhsh stis domes
	{

		curid=strtol(buffer,&temp,10);
		if((curid-previd) != 1)													//Elegxos gia thn seira tou keimenou
		{
			printf("Text not in the correct order\n"); 
			printf("Curid is %ld and previd is %ld \n",curid,previd); 
			exit(1);
		}	
		map[curid]=malloc(sizeof(char)*(strlen(temp)+1));						//Eisagwgh sto map
		strcpy(map[curid],temp);
		//printf("The id is: %ld \n", curid);
		//printf("The line is: %s \n",map[curid]);

		trie_head=addTrie(map[curid],curid,trie_head);							//Eisagwgh sto Trie

		previd=curid;
	}
////////////////////////MIN PEIRAKSEIS KATI ETSI /////////////////////


	printf("Type something \n");
	while(-1 != getline(&buffer,&size_buffer,stdin))							//Anagnwsh querry
	{
		token=strtok(buffer," \t\n");
		if(token==NULL){printf("Problem occured \n"); exit(1);}

		if((strcmp(token,"/df"))==0)											//Periptwsh /df
		{
			token=strtok(NULL," \t\n");
			if(token!=NULL)														//Elegxos gia kenh grammh
			{
				printf("Searching for %s\n",token );
				search_DFTrie(token,trie_head);
			}
		}
		else if((strcmp(token,"/tf")==0))										//Periptwsh /tf
		{
			token2=strtok(NULL," \t\n");
			temp=NULL;
			if(token2!=NULL)													//Elegxos gia kenh grammh
			{
				t_id=strtol(token2,&temp,10);
				token=strtok(NULL," \t\n");
				if(token!=NULL)
				{
					printf("Searching for %s\n",token );
					search_TFTrie(token,trie_head,t_id);
				}
				
			}
			
		}
		else if((strcmp(token,"/exit")==0))										//Periptwsh /exit
		{
			printf("About to exit the programm\n");
			break;
		}
		else 																	//Periptwsh lathos querry
		{
			printf("Not an expected querry,sorry \n");
			exit(1);
		}
		
		printf("Type something else \n");
	}

	printf("Freeing the allocated memory \n");
	int i;
	for(i=0; i<lines; i++)
	{
		free(map[i]);
	}
	free(map);
	freeTrie(trie_head);
	fclose(docfile);
	free(buffer);

	return 0;

}