//****************************** Compile with Dev-C++ compiler for avoiding errors associated with the compiler *********************************
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 99

typedef struct inode{			//I am making a  simple linked list to store the data from the json file so i can manipulate thme
	char *word;
	struct inode *nxt;
}Inode;

typedef Inode* T;

char **load(char **,int *);							//loading data drom json file
char **seperateKeysValues(char **, int );			//seperating keys and values from the json file
char *stringInicial(char *);						
void delete_Quotes(char **,int);
void create_List(T *, char ** , int ,int *);
void print_List(T ,int *);
void delete_Insert_Node(T *);
int Position_Slashes(T );
void insert_Slashes(T *,int );
void change_Lines(char **, int ,int *);
char **strings_End(char **,int );					//making the format of the spceial characters at the end 
void put_Spaces(char **,int );
void put_Dots(char **,int );						//putting dots where it is need to be put
void create_New_File(char **,int );					//creating a txt archive to


int main(){
	
	int n,i,k,list_len,lines;
	char **a,*ch;
	T head,temp1,temp2,temp3,insert,del;
	
	head=malloc(sizeof(Inode));						
	
	a=load(a,&n);
	a=seperateKeysValues(a,n);
	a[1]=stringInicial(a[1]);
	
		
	change_Lines(a,n,&lines);
	
	create_List(&head,a,n,&list_len);			//creating linked list with the data from the json archive
	
	delete_Insert_Node(&head);
	
	i=0;
	while(1){
		i++;
		if(i==lines-2){			//I insert slashes in every line that is needed
			break;
		}
		temp1=head;
		k=Position_Slashes(temp1);	 //inserting slashes between starDate and endDate
		insert_Slashes(&head,k);
		
	}
	
	print_List(head,&list_len);
	temp1=head;
	for(i=0;i<list_len;i++){        //switch from linked list to array now  I can  manipulate the chars easy
		strcpy(a[i],temp1->word);
		temp1=temp1->nxt;
		
	}
	
	list_len=list_len+2;			//making the length of array +2 so we will able to make the right format for the ending 
	a=strings_End(a,list_len);
	
	put_Spaces(a,list_len);			//putting space at the beginning of the strings
	
	put_Dots(a,list_len);           //putting - where is needed
	
	delete_Quotes(a,list_len);
	
	printf("\n\nThe transformed array (Data generated from the json archive):\n\n");
	for(i=0;i<list_len;i++){
		printf("%s",a[i]);    
	}
	printf("\n\n");
	system("pause");
	create_New_File(a,list_len);	
	return 0;
}

char **load(char **arr,int *len){
	FILE *fp;
	int i,j,n=0,realen;
	char *word,*ch;
	fp=fopen("test.json","r");        //open the json file
	word=malloc(sizeof(char)*99);
	if(!word){
		printf("error");
		exit(0);
	}
	arr=malloc(sizeof(char *));           //allocating dynamic the memory for the array of the strings
	while(fscanf(fp,"%s",word)!=EOF){     //read from the json file  each word at a time and store it in an array of strings
		n++;
		arr=realloc(arr,sizeof(char *)*n);
		arr[n-1]=malloc(sizeof(char )*99);
		strcpy(arr[n-1],word);
	}
	
	for(i=0;i<n;i++){
		realen=strlen(arr[i]);				//realen is the actual size of each string stored in a[i]
		ch=strstr(arr[i],"},");				//checking if there is "},"  in the string  because there is where we want to chage lines in the final array
		if(ch){
			i=i+1;
		}       	   
		for(j=0;j<realen;j++){
			if(arr[i][j]=='{' || arr[i][j]=='}' || arr[i][j]=='[' || arr[i][j]==']' || arr[i][j]==','){
				arr[i][j]='*';
			}
		}
	}
	
	*len=n;
	free(word);
	fclose(fp);
	
	return arr;    //return the array of strings in main
}

char **seperateKeysValues(char **a , int n){
	char *ch1,*ch2,*ch3,*ch4, *ch5,*ch6,*ch7,*ch8;
	int i;
	
	for(i=0;i<n;i++){
		if(a[i][0]!=' '){
			ch1=strstr(a[i],"name");    //i have to do this because i cant do strcmp(a[i],""name"")!=0 ,checking for substring -> name
			ch2=strstr(a[i],"edition");
			ch3=strstr(a[i],"startDate");
			ch4=strstr(a[i],"endDate");
			ch5=strstr(a[i],"location");
			ch6=strstr(a[i],"city");
			ch7=strstr(a[i],"country");
			ch8=strstr(a[i],"state");
			if(ch1 || ch2  || ch3 || ch4 || ch5  || ch6 || ch7 ||ch8){	//if the name is not substring the pointer ch1  is an NULL pointer etc
				strcpy(a[i],"*");
			}		
		}		
	}
	return a;
}

char *stringInicial(char *a){                                        //making the right format for the starting sritng 
	char ch1[N]="{\n", ch2[N]=": [\n    \"", nuevo[N]="   meetUps ";
	int realen;
	strcpy(a,nuevo);
	realen=strlen(nuevo);
	nuevo[2]='"';
	nuevo[realen-1]='"';
	
	strcat(ch1,nuevo);
	strcat(ch1,ch2);
	
	strcpy(a,ch1);
	
	return a;
}

char **strings_End(char **a,int n){       //making the right format for the ending 
	int i;
	strcpy(a[n-2],"\n ]");
	strcpy(a[n-1],"\n}");

	return a;
}

void put_Spaces(char **a,int n){
	int i;
	for(i=1;i<n-2;i++){
		if(strcmp(a[i],"\n")==0 && strcmp(a[i-1],"|")!=0){
			strcpy(a[i],"\"\n    ");
		}
	}
	for(i=0;i<n-2;i++){
		if(strcmp(a[i],"\"\n    ")==0){       //if we have meeting at the same date in two diferent places this finds it and places "|" for the format
			if(strcmp(a[i],a[i+3])==0){
				strcat(a[i-1],"|");
				strcpy(a[i],"\n");
			}
			
		}
	}
}

void put_Dots(char **a,int n){
	int i,j,realen;
	char *ch,*ch1;
	for(i=0;i<n;i++){
		realen=strlen(a[i]);
		for(j=0;j<realen;j++){
			if(strcmp(a[i+1],"/")!=0){		//we do not want - between start Date and end Date thats why i am doing this
				if(a[i][j]=='*'){
					a[i][j]='·';
				}
			}	
		}
	}
	 
	for(i=2;i<n;i++){
		realen=strlen(a[i]);
		for(j=0;j<realen;j++){
			if(strcmp(a[i+2],"\"\n    ")==0 ||strcmp(a[i+3],"\"\n    ")==0 ||strcmp(a[i+4],"\"\n    ")==0){
				ch=strstr(a[i],"20");         // I dont want after dates to have , so I avoid that by assuming that all dates have 20 (2010, 2016,2020 etc) 
				if(a[i][j]=='·' && !ch){
					a[i][j]=',';
				}
			}
		}
	}
	for(i=0;i<n;i++){
		realen=strlen(a[i]);
		for(j=0;j<realen;j++){
			if(strcmp(a[i+1],"|")==0){
				if(a[i][j]=='·'){
					a[i][j]=' ';
				}
			}
			ch=strstr(a[i],"20");	   //if we have a date at pos i and at pos i+1 we haven t date the we put , at the string in pos i+1
			ch1=strstr(a[i+1],"20");
			if(ch && !ch1 && strcmp(a[i+2],"|")!=0){
				if(a[i+1][j]=='·'){
					a[i+1][j]=',';
				}
			}
		}
	}

}
void delete_Quotes(char **a,int n){
	int i,j,realen;
	for(i=1;i<n-2;i++){
		realen=strlen(a[i]);
		for(j=0;j<realen;j++){
			if(a[i][j]=='"' ||a[i][j]=='*'){
			a[i][j]=' ';
			}
		}
	}
	for(i=1;i<n-2;i++){
		if(strcmp(a[i]," \n    ")==0 && strcmp(a[i-1],"|")!=0 ){   //quotes at the end and at the start of every line excepts the lines that end in "|"
			strcpy(a[i],"\"\n    \"");
		}
	}
	strcpy(a[n-2],"\"\n ]");									//puting the last " at the last string of the json archive
	
	for(i=0;i<n;i++){
		if(strcmp(a[i],"\"\n    \"")==0){
			strcpy(a[i],"\",\n    \"");
		}
	}
	

}

void change_Lines(char **a, int n,int *lines){
	int i,counter=0;
	for(i=0;i<n;i++){
		if(strcmp(a[i],"},")==0){
			strcpy(a[i],"\n");
			counter++;
		}
	}
	
	*lines=counter;
}

void create_List(T *head, char **a, int n, int *len){
	int i,w=1;
	T first,temp,insert;
	temp=malloc(sizeof(Inode));
	first=*head;
	head=malloc(sizeof(Inode));
	first->word=malloc(sizeof(char )*N);
	strcpy(first->word,a[1]);              //head of the list the inicial string a[1]
	first->nxt=NULL;
	temp=first;
	
	for(i=2;i<n;i++){
		if(strcmp(a[i],"*")!=0){
			w++;
			insert=malloc(sizeof(Inode));
			insert->word=malloc(sizeof(char)*N);
			strcpy(insert->word,a[i]);
			insert->nxt=NULL;
			temp->nxt=insert;
			temp=temp->nxt;
		}
	}
	*len=w;
}

void print_List(T head,int *len){
	T temp;
	int k=0;
	temp=head;
	for(;temp!=NULL;temp=temp->nxt){
		
		k++;
	}
	printf("\n");
	*len=k;
}

void delete_Insert_Node(T *head){
	T temp1,temp2,temp3,del,insert;
	int i,k=0;
	temp1=*head;
	for(;temp1!=NULL;temp1=temp1->nxt){
		temp2=temp1->nxt;
		temp3=temp2->nxt;
		temp3=temp3->nxt;
		temp2=temp3;
		k++;
		if(strcmp(temp1->word,temp2->word)==0){
			break;
		}
	}
	temp1=*head;
	for(i=0;i<k-1;i++){          //moving temp1 in the position in the list where I want to change a node
		temp1=temp1->nxt;
		temp2=temp1->nxt;
	}
	insert=malloc(sizeof(Inode));				//inserting "| " in the list
	insert->word=malloc(sizeof(char)*N);
	strcpy(insert->word,"|");
	insert->nxt=NULL;
	
	temp1->nxt=insert;
	insert->nxt=temp2;
	
	temp1=*head;
	for(i=0;i<k-2;i++){          
		temp1=temp1->nxt;
		
	}
	del=temp1->nxt;
	temp1->nxt=del->nxt;
	free(del);
	printf("\t\t\t\tConverting json  archive to HTML\n\n");
	system("pause");
}

int Position_Slashes(T head){
	int pos=0;
	T temp1, temp2;
	char *ch1,*ch2;
	temp1=head;
	for(;temp1!=NULL;temp1=temp1->nxt){
		temp2=temp1->nxt;
		ch1=strstr(temp1->word,"20");	//because i want to find dates there is sure that the string will have substring "20" because we have the format 2016, 2020 we arrange meeting in this century
		ch2=strstr(temp2->word,"20");
		pos++;
		if(ch1 && ch2){	            //finding if there is endDate and startDate in order to put slash between them
			break;
		}
	}
	return pos;
}

void insert_Slashes(T *head,int pos){
	T temp1, first,insert;
	int i;
	for(i=0, temp1=*head; i<pos-1 && temp1!=NULL; i++,temp1=temp1->nxt);
	
	insert=malloc(sizeof(Inode));				//inserting "/ " in the list as new nodes
	
	insert->word=malloc(sizeof(char)*N);
	strcpy(insert->word,"/");
	insert->nxt=NULL;
	
	insert->nxt=temp1->nxt;
	temp1->nxt=insert;
	
}

void create_New_File(char **a,int n){             //creating a txt file with the transformed array inside, if is need to
	FILE *fpw;
	fpw=fopen("converted for creating HTML.txt","w+");
	if(fpw==NULL){
		printf("error");
		exit(0);
	}
	
	int i;
	for(i=0;i<n;i++){
		fprintf(fpw,"%s",a[i]);
	}
	
	printf("\n\n ---->Text  file ,with the transformed array, has been created in the same file with the json archive and the c program!\n\n ---->This txt  can be used to create the HTML!!\n\n");
	fclose(fpw);
}


