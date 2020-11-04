#include <stdio.h>
#include <stdlib.h>
#include "functions.h"


/////////////////////////////////////////////////////////////////////POSTING LIST FUNCTIONS////////////////////////////////


PostingList* addPostingList(PostingList *head,int t_id)
{
	PostingList *temp;
	if(head==NULL)											//An to head einai NULL dhmioyrgia kai anathesh
	{
		head=malloc(sizeof(PostingList));
		head->textid=t_id;
		head->count=1;
		head->next=NULL;
		return head;
	}
	temp=head;

	while(temp->textid!= t_id)								//Elegxos gia to an yparxei hdh node me auto to id, an oxi tote eisagwgh neou
	{

		if(temp->next==NULL)
		{
			temp->next=malloc(sizeof(PostingList));
			temp->next->textid=t_id;
			temp->next->count=1;
			temp->next->next=NULL;
			return head;
		}
		else
		{
			temp=temp->next;
		}	
	}
	temp->count++;
	return head;
}


int findPostingList(PostingList *head,int t_id)				//Synarthsh eyreshs count gia to posting list me id==t_id,xrhsimopoieitai apo thn search_TFTrie
{
	int flag=0;
	while((head!=NULL) && (flag==0))
	{
		if(head->textid==t_id)
		{
			return head->count;
		}
		else
		{
			head=head->next;
		}
	}
	printf("The word is not in the %d text \n",t_id );
	exit(1);
	return 0;
}



void freePostingList(PostingList *head)
{
	if(head!=NULL)
	{
		if(head->next!=NULL)
		{
			freePostingList(head->next);
		}
		free(head);
		head=NULL;
	}
}

/////////////////////////////TRIE SINARTISEIS///////////////////////////////////////////////////////////////////////

Trie* addTrie(char* map,int id,Trie *trie_head)								/////INSERT/////
{
	int flag=0,i=0;
	char ch;
	Trie *temp=NULL;
	temp=trie_head;
	
	while((map[i]) != 0)													//Mexri telos toy string
	{
		if((map[i]!=' ') && (map[i]!='\t'))
		{
			ch=map[i];
			if(temp==NULL)													//periptwsh adeias kefalhs(mono mia fora prepei na symbei)
			{
				printf("Eimai sto prwto node tou trie \n");
				trie_head=malloc(sizeof(Trie));
				temp=trie_head;
				temp->horiz=NULL;
				temp->vert=NULL;
				temp->letter=ch;
				temp->plist=NULL;
			}
			else
			{
				if(flag)													//flag==0 shmainei oti einai prwto gramma leksis
				{
					if(temp->vert==NULL)									//An den briskomaste sto prwto gramma lekshs o temp prepei na proxwrhsei sta paidia epo-
					{														//menos an den yparxei paidi dhmiourgeitai alliws temp=temp->vert
						temp->vert=malloc(sizeof(Trie));
						temp=temp->vert;
						temp->horiz=NULL;
						temp->vert=NULL;
						temp->letter=ch;
						temp->plist=NULL;
					}
					else
					{
						temp=temp->vert;
					}
					
				}
				while(temp->letter != ch) 									//Oso den brisketai to gramma proxwraei stous horizontal nodes(aderfia)
				{
					if(temp->horiz==NULL)									//Sto telos autou tou while o temp tha deixnei ston kombo me letter==ch
					{
						temp->horiz=malloc(sizeof(Trie));
						temp=temp->horiz;
						temp->letter=ch;
						temp->vert=NULL;
						temp->horiz=NULL;
						temp->plist=NULL;
					}
					else
					{
						temp=temp->horiz;
					}
				}
			}
			flag=1;
		}
		else
		{
			if(flag)																//An to flag einai 1 shmainei oti molis teleiwse leksh --> uptade tou plist
			{
				temp->plist=addPostingList(temp->plist,id);
			}

			flag=0;
			temp=trie_head;
		}
		
		i++;
	}
	if(flag)
	{
		temp->plist=addPostingList(temp->plist,id);
	}

	return trie_head;

}




void search_DFTrie(char* buffer ,Trie* head)
{
	int i,nutext,flag;
	Trie *temp=NULL, *prevtemp=NULL;
	PostingList *templist=NULL;


	temp=head;
	 
	while(buffer[i] != 0)
	{
		flag=0;																	//To flag deixnei oti brethike to gramma
		if(temp!=NULL)
		{
			if (temp->letter==buffer[i])										//An brethei tote bes sto apo katw
			{
				prevtemp=temp;
				temp=temp->vert;
			}
			else
			{
				while((flag==0) && (temp->horiz!=NULL) )							//An den brethei oso den brisketai kai ta dipla einai kena bes dipla
				{
					temp=temp->horiz;
					if(temp->letter==buffer[i])
					{
						flag=1;
						prevtemp=temp;
						temp=temp->vert;
					}
				}
				if(flag==0)															//An meta thn anazhthsh se olous tous orizontious den brethei to gramma tote failed
				{
					printf("Requested word not found \n");
					exit(1);
				}
			}
		}
		else 																		//An o temp ftasei se null xwris na exei brethei to gramma tote failed
		{
			printf("Requested word not found \n");
			exit(1);
		}
		i++;

	}
	if(prevtemp->plist != NULL)														//Elegxos tou posting list, an einai null shmainei oti h leksh den dothike apo ta text
	{	
		templist=prevtemp->plist;
		nutext=1;
		while(templist->next!=NULL)													//Euresh toy arithmou twn keimenwn opou yparxei h leksh
		{
			templist=templist->next;
			nutext++;
		}
		printf("%s %d \n",buffer,nutext);	
	}
	else
	{
		printf("Requested word not found \n");
	}
}






void search_TFTrie(char* buffer ,Trie* head,int t_id)							//H anazhthsh symbainei me ton idio tropo me to search_DFTrie, allazh h ektypwsh
{
	int i,nutext,flag,count;
	Trie *temp=NULL, *prevtemp=NULL;
	PostingList *templist=NULL;


	temp=head;
	while(buffer[i] != 0)
	{
		flag=0;
		if(temp!=NULL)
		{
			if (temp->letter==buffer[i])										//An brethei tote bes sto apo katw
			{
				prevtemp=temp;
				temp=temp->vert;
			}
			else
			{
				while((temp->horiz!=NULL) && (flag==0))							//An den brethei oso den brisketai kai ta dipla einai kena bes dipla
				{
					temp=temp->horiz;
					if(temp->letter==buffer[i])
					{
						flag=1;
						prevtemp=temp;
						temp=temp->vert;
					}
				}
				if(flag==0)
				{
					printf("patata \n");
					printf("Requested word not found \n");
					exit(1);
				}
			}
		}
		else
		{
			printf("Requested word not found \n");
			printf("ntomata \n");
			exit(1);
		}
		i++;

	}
	if(prevtemp->plist != NULL)													///Euresh twn arithmwn poy yparxei se keimeno me id==t_id h leksh
	{	
		count=findPostingList(prevtemp->plist,t_id);
		printf("%d %s %d \n",t_id,buffer,count );
	}
	else
	{
		printf("Requested word not found \n");
		printf("sos \n");
	}
}




void freeTrie(Trie *head)														//Free ths desmeumenhs mnhmhs tou trie
{
	if(head!=NULL)
	{
		if(head->vert!=NULL)
		{
			freeTrie(head->vert);
		}
		if(head->horiz!=NULL)
		{
			freeTrie(head->horiz);
		}
		if(head->plist!=NULL)
		{
			freePostingList(head->plist);
		}
		free(head);
		head=NULL;
	}

}