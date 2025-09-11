#include<iostream>
#include<fstream>
#include <termios.h>
#include <unistd.h>

using namespace std;


 // since i use linux and getch does not work in ubuntu so i have to take this code from internet inorder to improve ux
char getch() {
    char buf = 0;
    struct termios old = {0};
    if (tcgetattr(STDIN_FILENO, &old) < 0) perror("tcgetattr");
    old.c_lflag &= ~ICANON; 
    old.c_lflag &= ~ECHO;   
    if (tcsetattr(STDIN_FILENO, TCSANOW, &old) < 0) perror("tcsetattr");
    if (read(STDIN_FILENO, &buf, 1) < 0) perror("read");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(STDIN_FILENO, TCSADRAIN, &old) < 0) perror("tcsetattr");
    return buf;
}




class Node { // basic class node for characters
	public:
	char data;
	Node *next,*prev;
	int size=0;
	Node(char data = '\0'){
		this->data = data;
		next = nullptr;
		prev = nullptr;
	
	}
	
};

class charList{
	private:
	Node *head,*tail;
	int size = 0;
	public:
	charList(){
		head = nullptr;
		tail = nullptr;
	}
	void insert_at_end(char val){
		Node *newNode = new Node(val);
		if(tail == nullptr){
			head = newNode;
			tail = newNode;
			size++;
			return;	
		}
		size++;
		tail->next = newNode;
		newNode->prev = tail;
		tail = newNode;
		
	
	}
	void formation(string line){
		for(int i =0  ; i < line.length();i++){
			insert_at_end(line[i]);	
		}
	
	}
	
	void insertChar(char character,int x_pos,lineNode* line){
		Node *newNode = new Node(character);
		if(head == nullptr && tail == nullptr){
			head = newNode;
			tail = newNode;
			return;
		}
		if(x_pos > size){
			return;
		}
		
		Node *temp = head;
		int count_x = 0;
		
		while(count_x != x_pos){
			temp = temp->next;	
			count_x++;
		}
		Node *nextNode = temp->next;
		temp->next = newNode;
		newNode->prev = temp;
		if(nextNode != nullptr){
			newNode->next = nextNode;
			nextNode->prev = newNode;
			return;
		} 
		newNode->next = nullptr;
		
	}
	
	void show(int x=-1,int  y =-1){
		int charNumber = 0;
		Node * temp = head;
                int count_x= 0;
                static bool isVisible;
               
                
		while(temp != nullptr ){
			if(count_x == x){
				cout<<"_";
			}
			else{
				cout<<temp->data;
			}
			temp = temp->next;
			charNumber++;
			count_x++;
		}
	
		cout<<endl;
	}
	
	Node *getLineContent(){
		return head;
	}
	
	
	~charList(){
		Node *prev = nullptr;
		while(head != nullptr){
			prev= head;
			head = head->next;
			
			delete prev;
		}
	}
	

};


class lineNode{
	public:
	charList data;
	lineNode * next,*prev;
	string line;
	lineNode(string line){
		data.formation(line);
		
		next = nullptr;
		prev = nullptr;
	}
	
	
};


class textEditor{
	private:
	lineNode *headLine,*tailLine;
	string temp;
	int cursor_x,cursor_y;
	public:
	textEditor(string fileName){
		headLine = nullptr;
		tailLine = nullptr;
		fstream file(fileName,ios::in);
		while(getline(file,temp)){ // just for initialization
			lineNode * newNode= new lineNode(temp);
			if(tailLine == nullptr){
				tailLine = newNode;
				headLine = newNode;
			}
			else{
				tailLine->next = newNode;
				newNode->prev = tailLine;
				tailLine = newNode;
			}
			
			
		}
		
		cursor_x =0;
		cursor_y = 0;
	}
	
	void move_right(){
	
		cursor_x++;
		system("clear");


	}
	
	void move_left(){
		if(cursor_x >0)
			cursor_x--;
		system("clear");

	}
	
	void move_down(){
		cursor_y++;
		system("clear");
	}
	void move_up(){
		if(cursor_y > 0)
			cursor_y--;
		system("clear");
	}
	void insertion(char character){
		headLine->data.insertChar(character,cursor_x);

		showAll();
	}
	void showAll(){
		
		lineNode * temp= headLine;
		Node * lineContent;
		int current_x= 0 ,current_y = 0;
		while(temp != nullptr){
			current_x = 0;
			lineContent = temp->data.getLineContent();
			while(lineContent!= nullptr){
				if(cursor_x == current_x && cursor_y == current_y){
					cout<<'_';
				}
				else{
					cout<<lineContent->data;
				}
				lineContent = lineContent->next;
				current_x++;
			}
			current_y++;
			cout<<endl;
			temp = temp->next;
			
		}
		
	}
	

};



int main(){

	textEditor editor("input.txt");

	

	char user_choice;
	char input;
	bool should_run = true;
	
	while(should_run){
		system("clear");
		editor.showAll();
	
		
  		user_choice = getch();

		if(user_choice == 'd'){
			editor.move_right();
		}
		else if(user_choice == 'i'){
			
			while(true){
				input = getch();
				if(input =='n')
					break;
				system("clear");
				editor.insertion(input);
				
			}
		}
		else if(user_choice == 'a'){
			editor.move_left();
		}
		else if(user_choice == 'w'){
			editor.move_up();
		}
		else if(user_choice == 's'){
			editor.move_down();
		}
		else if(user_choice == 'n' ){
			should_run = false;
		}
	}
	/*while(should_run){
	
		cout<<"Enter you choice: ";
		cin>>user_choice;
		
		switch(user_choice){
			case 'w':
			// moveup;
			break;
			
			case 's':
			//movedown
			break;
			
			case 'a':
			//moveleft
			break;
			
			case 'd':
			//moveright
			break;
			
			case 'i':
			//insert
			break;
			
			
			default:
				should_run =false;
				continue;
		}
	}
	*/
	
		
	return 0;
}


/*
								code written with lots of love by a human@
									    happy checking!
									    	  😉️(just a )
*/
