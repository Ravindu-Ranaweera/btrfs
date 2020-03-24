#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <conio.h>
#include<stdlib.h>
#define M 5
using namespace std;
struct node{
	int n; /* n < M No. of keys in node will always less than order of B tree */
	int keys[M-1]; /*array of keys*/
	string fileName[M-1];
	struct node *p[M]; /* (n+1 pointers will be in use) */
};

struct node *root=NULL;
enum KeyStatus { Duplicate,SearchFailure,Success,InsertIt,LessKeys };
class setPartition;
char memPartition1[1000][100];  /*virtual memory disk 2D array*/
char memPartition2[1000][100];
int rowCount=0, columnCount=0 ,copyRowCount=0, copyColumnCount=0, numOfFile=0; 
vector<setPartition *> objArray;

void insert(int key);
void display(struct node *root,int);
void DelNode(int x);
void search(int x);
enum KeyStatus ins(struct node *r, int x, int* y, struct node** u);
int searchPos(int x,int *key_arr, int n);
enum KeyStatus del(struct node *r, int x);
int getFileSize(string a);
void copyFile(string file01, string file02, int temp);
void createFile(int a, string s);

class setPartition{		/*create file objects and store file id in the btree*/
	int fileId;
	string fileName;
	int startRow;
	int endRow;
	public:
		setPartition(){
		}
		setPartition(int &fileId, string &fileName);
		void setValue();
		void setMemory();
		void copyPartition(string name, int Id);
		string getFileName();
		int getFileId();
		void copyFile();
		void clearPartition();	
		void updatePartition();
};

setPartition::setPartition(int &fileId, string &fileName){   	/*set file id and name*/
	this->fileId=fileId;
	this->fileName=fileName;
	void setMemory();
	cout << fileName<<endl;
}

void setPartition::setValue(){ /*if we delete file delete file data in btree*/
	fileId=0;
	fileName.clear();
}

void setPartition::updatePartition(){ /*if weupdate file update memory*/
		startRow=rowCount;
		stringstream ss;
		ss <<  fileName<< ".txt";
		string file = ss.str();		
		ifstream infile(file.c_str());
		char ch;
		
		while(infile.get(ch)){
			cout<< ch;
			if(columnCount>100){
				rowCount++;
				columnCount=0;
			}else{
				memPartition1[rowCount][columnCount]=ch;
				columnCount++;	
			}
		}
		endRow=rowCount;
		rowCount++;
		columnCount=0;
}

void setPartition::clearPartition(){  /*clear 2d array*/
	for(int i=startRow; i< endRow; i++){
		for(int j=0; j<100 ; j++){
			memPartition1[rowCount][columnCount]='\0';
			memPartition2[rowCount][columnCount]='\0';
			}
		}	
}

void setPartition::setMemory(){ /*set first hard disk*/
		startRow=rowCount;
				stringstream ss;
			    ss <<  fileName<< ".txt";
				string file = ss.str();
				
				ifstream infile(file.c_str());
				char ch;
		
			while(infile.get(ch)){
				
		
//				cout<< ch;
				if(columnCount>100){
					rowCount++;
					columnCount=0;
				}else{
					memPartition1[rowCount][columnCount]=ch;
					columnCount++;	
				}
			}
			endRow=rowCount;
			rowCount++;
			columnCount=0;
			copyPartition(fileName,fileId);

}

void setPartition::copyPartition(string name, int Id){ /*copy the one hard disk to aother one*/
			stringstream ss;
		ss <<  name<< ".txt";
		string file = ss.str();
				
		ifstream infile(file.c_str());
		char ch;
		
		while(infile.get(ch)){
				
		
//		cout<< ch;
			if(copyColumnCount>100){
				copyRowCount++;
				copyColumnCount=0;
			}else{
				memPartition2[copyRowCount][copyColumnCount]=ch;
				columnCount++;	
			}
		}
			copyRowCount++;
			copyColumnCount=0;	
}

string setPartition::getFileName(){
	return fileName;
}


int setPartition::getFileId(){
	return fileId;
}



int main(){
	int val, opt,lineCount=0;
	ifstream file("textDetails.txt");
    int fileId;
    string fileName, line;
    
    while(!file.eof()){				
		getline(file,line);
		lineCount++;
		if(line.empty()){
//			cout <<numOfFile << endl;
			setPartition *tmp = new setPartition(fileId, fileName);
			objArray.push_back(tmp);
			objArray[numOfFile]->setMemory();
			insert(fileId);
			lineCount=0;
			numOfFile++;	
		}else{
									
			if(lineCount == 1){
				fileId = atoi(line.c_str());
			}else if(lineCount == 2){
				fileName = line;
			}
		}
	}
			
	int choice;
	printf("*****Creation of B tree for node %d\n\n",M);
	while(true)
	{
		cout<<"\n\t\t --- B-TREE FILE SYSTEM ---\n\n";
        cout<<"\t1. Create File\n";
        cout<<"\t2. Deletion File\n";
        cout<<"\t3. Searching File\n";
        cout<<"\t4. Show File List\n";
        cout<<"\t5. Get File Size\n ";
		cout<<"\t6. Copy File\n";       
        cout<<"\t7. Exit\n Enter your choice: ";
		scanf("%d",&choice);
	
		switch(choice)
		{
			case 1: {
				cout << "Enter File ID : ";
			cin >>  fileId;
			cout << fileId;
			insert(fileId);
			cout << "Enter File NAME :  ";
    		cin >> fileName;
    		stringstream ss1;
			ss1 <<  fileName<< ".txt";
			string file = ss1.str();
			createFile(fileId,file);
			break;
		}

			case 2:{
        		int pos, newId, count=0;
			
	            cout<<"Enter the file name to :";
	            cin >> fileName ;
	            
	            for (int i = 0; i < numOfFile; ++i)
					{
						if(objArray[i]->getFileName() == fileName ){
							newId = objArray[i]->getFileId();
							count++;
						}							
					}
				DelNode(newId);
				break;
			}
			case 3:{
        		int pos, newId, count=0;
			
	            cout<<"Enter the file name to SEARCH:";
	            cin >> fileName ;
	            
	            for (int i = 0; i < numOfFile; ++i)
					{
						if(objArray[i]->getFileName() == fileName ){
							newId = objArray[i]->getFileId();
//							cout << newId;
							count++;
						}							
					}
	            search(newId);
	            	if(count==0){
	            	cout<<"Sorry.... Given File is not present in B-Tree\n";
				}	
				break;
			}
			case 4:{
				printf("Btree is :\n");
				display(root,0);
				break;
			}
			case 5:{
				int pos, newId, count=0;
			
	            cout<<"Enter the file name to get size:";
	            cin >> fileName ;
	            
	            for (int i = 0; i < numOfFile; ++i)
					{
						if(objArray[i]->getFileName() == fileName ){
							newId = objArray[i]->getFileId();
							count++;
						}							
					}
				cout << "size of " <<fileName <<".txt : " << getFileSize(fileName) << " bytes." << endl;
				break;
			}
			case 6:{
				int temp, flag=0;
        	string file1;
			cout << "Copy file to another file" << endl;
			cout << "Enter The File Name : " ;
			cin >> file1;
			
			for (int i = 0; i < numOfFile; ++i)	{
				if(objArray[i]->getFileName() == file1 ){
				cout << "\nThis File Is In BTREE\n"	;
				temp = objArray[i]->getFileId();			
				flag++	;		
				}							
			}
			if(flag==0){
				cout << "\nInvalid File Name\n";
				break;
			}
			stringstream ss1;
			ss1 <<  file1<< ".txt";
			string file01 = ss1.str();
		
			stringstream ss2;
			ss2 <<  file1<< "copy.txt";
			string file02 = ss2.str();
			cout << file01<<" "<<file02<<endl<<endl;
        	copyFile(file01,file02,temp);
			break;
		}
		case 7:		
				exit(1);
				break;
			
			default:
				printf("Wrong choice\n");
				break;
			system("pause");
		}/*End of switch*/
	}/*End of while*/
	
	

	
return 0;
}/*End of main()*/

void insert(int key)
{
	struct node *newnode;
	int upKey;
	enum KeyStatus value;
	value = ins(root, key, &upKey, &newnode);
	if (value == Duplicate)
		printf("Key already available\n");
	if (value == InsertIt)
	{
		struct node *uproot = root;
		root=(struct node*)malloc(sizeof(struct node));
		root->n = 1;
		root->keys[0] = upKey;
		root->p[0] = uproot;
		root->p[1] = newnode;
	}/*End of if */
}/*End of insert()*/

enum KeyStatus ins(struct node *ptr, int key, int *upKey,struct node**newnode)
{
	struct node *newPtr, *lastPtr;
	int pos, i, n,splitPos;
	int newKey, lastKey;
	enum KeyStatus value;
	if (ptr == NULL)
	{
		*newnode = NULL;
		*upKey = key;
		return InsertIt;
	}
	
	n = ptr->n;
	pos = searchPos(key, ptr->keys, n);
	if (pos < n && key == ptr->keys[pos])
		return Duplicate;
	value = ins(ptr->p[pos], key, &newKey, &newPtr);
	if (value != InsertIt)
		return value;
	/*If keys in node is less than M-1 where M is order of B tree*/
	if (n < M - 1)
	{
		pos = searchPos(newKey, ptr->keys, n);
		/*Shifting the key and pointer right for inserting the new key*/
		for (i=n; i>pos; i--)
		{
			ptr->keys[i] = ptr->keys[i-1];
			ptr->p[i+1] = ptr->p[i];
		}
		/*Key is inserted at exact location*/
		ptr->keys[pos] = newKey;
		ptr->p[pos+1] = newPtr;
		++ptr->n; /*incrementing the number of keys in node*/
		return Success;
	}/*End of if */
	/*If keys in nodes are maximum and position of node to be inserted is
	last*/
	if (pos == M - 1)
	{
		lastKey = newKey;
		lastPtr = newPtr;
	}
	else /*If keys in node are maximum and position of node to be inserted is not last*/
	{
		lastKey = ptr->keys[M-2];
		lastPtr = ptr->p[M-1];
		for (i=M-2; i>pos; i--)
		{
			ptr->keys[i] = ptr->keys[i-1];
			ptr->p[i+1] = ptr->p[i];
		}
		ptr->keys[pos] = newKey;
		ptr->p[pos+1] = newPtr;
	}
	splitPos = (M - 1)/2;
	(*upKey) = ptr->keys[splitPos];
	
	(*newnode)=(struct node*)malloc(sizeof(struct node));/*Right node after split*/
	ptr->n = splitPos; /*No. of keys for left splitted node*/
	(*newnode)->n = M-1-splitPos;/*No. of keys for right splitted node*/
	for (i=0; i < (*newnode)->n; i++)
	{
	(*newnode)->p[i] = ptr->p[i + splitPos + 1];
	if(i < (*newnode)->n - 1)
	(*newnode)->keys[i] = ptr->keys[i + splitPos + 1];
	else
	(*newnode)->keys[i] = lastKey;
	}
	(*newnode)->p[(*newnode)->n] = lastPtr;
	return InsertIt;
}/*End of ins()*/

void display(struct node *ptr, int blanks)
{
	if (ptr)
	{
		int i;
		for(i=1;i<=blanks;i++)
			printf(" ");
		for (i=0; i < ptr->n; i++)
			printf("%d ",ptr->keys[i]);
		printf("\n");
		for (i=0; i <= ptr->n; i++)
			display(ptr->p[i], blanks+10);
	}/*End of if*/
}/*End of display()*/

void search(int key)
{
	int pos, i, n;
	struct node *ptr = root;
	printf("Search path:\n");
	while (ptr)
	{
		n = ptr->n;
		for (i=0; i < ptr->n; i++)
			printf(" %d",ptr->keys[i]);
		printf("\n");
		pos = searchPos(key, ptr->keys, n);
		if (pos < n && key == ptr->keys[pos])
		{
			printf("Key %d found in position %d of last dispalyednode\n",key,i);
			return;
		}
		ptr = ptr->p[pos];
	}
	printf("Key %d is not available\n",key);
}/*End of search()*/

int searchPos(int key, int *key_arr, int n)
{
	int pos=0;
	while (pos < n && key > key_arr[pos])
	pos++;
	return pos;
}/*End of searchPos()*/

void DelNode(int key)
{
	struct node *uproot;
	enum KeyStatus value;
	value = del(root,key);
	switch (value)
	{
	case SearchFailure:
		printf("Key %d is not available\n",key);
		break;
	case LessKeys:
		uproot = root;
		root = root->p[0];
		free(uproot);
		break;
}/*End of switch*/
}/*End of delnode()*/

enum KeyStatus del(struct node *ptr, int key)
{
	int pos, i, pivot, n ,min;
	int *key_arr;
	enum KeyStatus value;
	struct node **p,*lptr,*rptr;
	
	if (ptr == NULL)
		return SearchFailure;
	/*Assigns values of node*/
	n=ptr->n;
	key_arr = ptr->keys;
	p = ptr->p;
	min = (M - 1)/2;/*Minimum number of keys*/
	
	pos = searchPos(key, key_arr, n);
	if (p[0] == NULL)
	{
		if (pos == n || key < key_arr[pos])
			return SearchFailure;
		/*Shift keys and pointers left*/
		for (i=pos+1; i < n; i++)
		{
			key_arr[i-1] = key_arr[i];
			p[i] = p[i+1];
		}
		return --ptr->n >= (ptr==root ? 1 : min) ? Success : LessKeys;
	}/*End of if */
	
	if (pos < n && key == key_arr[pos])
	{
		struct node *qp = p[pos], *qp1;
		int nkey;
		while(1)
		{
			nkey = qp->n;
			qp1 = qp->p[nkey];
			if (qp1 == NULL)
			break;
			qp = qp1;
		}/*End of while*/
		key_arr[pos] = qp->keys[nkey-1];
		qp->keys[nkey - 1] = key;
	}/*End of if */
	value = del(p[pos], key);
	if (value != LessKeys)
		return value;
	
	if (pos > 0 && p[pos-1]->n > min)
	{
		pivot = pos - 1; /*pivot for left and right node*/
		lptr = p[pivot];
		rptr = p[pos];
		/*Assigns values for right node*/
		rptr->p[rptr->n + 1] = rptr->p[rptr->n];
		for (i=rptr->n; i>0; i--)
		{
			rptr->keys[i] = rptr->keys[i-1];
			rptr->p[i] = rptr->p[i-1];
		}
		rptr->n++;
		rptr->keys[0] = key_arr[pivot];
		rptr->p[0] = lptr->p[lptr->n];
		key_arr[pivot] = lptr->keys[--lptr->n];
		return Success;
	}/*End of if */
	if (pos > min)
	{
		pivot = pos; /*pivot for left and right node*/
		lptr = p[pivot];
		rptr = p[pivot+1];
		/*Assigns values for left node*/
		lptr->keys[lptr->n] = key_arr[pivot];
		lptr->p[lptr->n + 1] = rptr->p[0];
		key_arr[pivot] = rptr->keys[0];
		lptr->n++;
		rptr->n--;
		for (i=0; i < rptr->n; i++)
		{
			rptr->keys[i] = rptr->keys[i+1];
			rptr->p[i] = rptr->p[i+1];
		}/*End of for*/
		rptr->p[rptr->n] = rptr->p[rptr->n + 1];
		return Success;
	}/*End of if */
	
	if(pos == n)
		pivot = pos-1;
	else
		pivot = pos;
	
	lptr = p[pivot];
	rptr = p[pivot+1];
	/*merge right node with left node*/
	lptr->keys[lptr->n] = key_arr[pivot];
	lptr->p[lptr->n + 1] = rptr->p[0];
	for (i=0; i < rptr->n; i++)
	{
		lptr->keys[lptr->n + 1 + i] = rptr->keys[i];
		lptr->p[lptr->n + 2 + i] = rptr->p[i+1];
	}
	lptr->n = lptr->n + rptr->n +1;
	free(rptr); /*Remove right node*/
	for (i=pos+1; i < n; i++)
	{
		key_arr[i-1] = key_arr[i];
		p[i] = p[i+1];
	}
	return --ptr->n >= (ptr == root ? 1 : min) ? Success : LessKeys;
}/*End of del()*/

int getFileSize(string name){
	stringstream ss;
	ss << name << ".txt";
	string file = ss.str();	
	long begin, end;
	ifstream myfile (file.c_str());
	begin = myfile.tellg();
	myfile.seekg (0, ios::end);
	end = myfile.tellg();
	myfile.close();

	return (end-begin);	
}

void copyFile(string file01, string file02, int temp){
	std::ifstream ifs (file01.c_str());
	std::ofstream ofs (file02.c_str());
	std::filebuf* inbuf  = ifs.rdbuf();
	std::filebuf* outbuf = ofs.rdbuf();
	char c = inbuf->sbumpc();
	while (c != EOF)
	{
		outbuf->sputc (c);
		c = inbuf->sbumpc();
	}
	ofs.close();
	ifs.close();
	objArray[temp-1]->clearPartition();
	objArray[temp-1]->updatePartition();

}

void createFile(int fileId, string fileName){
		
			string content;
			cout << "Enter File Content :  ";
			cin >> content;
    		ofstream outfile(fileName.c_str());
    		numOfFile++;
    		if (outfile.is_open())
			  {
			  	outfile << content.c_str();
			    outfile.close();
			  }
			  else cout << "Unable to open file";
			
            setPartition *tmp = new setPartition(fileId, fileName);
			objArray.push_back(tmp);
			
			
			
}
