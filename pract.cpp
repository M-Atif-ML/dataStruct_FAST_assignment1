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
	
	void insert_at_begin(char val){
	    Node* newNode = new Node(val);
	    if(head == nullptr){
		head = newNode;
		tail = newNode;
		return;
	    }
	    newNode->next = head;
	    head->prev = newNode;
	    head = newNode;
	    size++;
	}
	void deleteBack(Node *current_pointer){
	    if (current_pointer == nullptr) return;

	    if (current_pointer->prev != nullptr)
		current_pointer->prev->next = current_pointer->next;
	    else
		head = current_pointer->next;  

	    if (current_pointer->next != nullptr)
		current_pointer->next->prev = current_pointer->prev;
	    else
		tail = current_pointer->prev;  

	    delete current_pointer;
	    size--;
	}

	void formation(string line){
		for(int i =0  ; i < line.length();i++){
			insert_at_end(line[i]);	
		}
	
	}
	
	Node * get_head(){
		return head;
	}
	Node * get_tail(){
		return tail;
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
	
	void insertChar(char character){
		Node *newNode = new Node(character);
		
		lineNode *tempLine = headLine;
		
		
		int count_y = 0 ;
		
		while(count_y != cursor_y){
			tempLine = tempLine->next;
			count_y++;
		}
		
		
		
		Node * head = tempLine->data.get_head();
		Node * tail = tempLine->data.get_tail();
		if(cursor_x == 0){
		    tempLine->data.insert_at_begin(character);
		    return;
		}
				
		
		
		
		Node *temp = head;
		int count_x = 0;
		
		while(count_x != cursor_x-1){
			temp = temp->next;	
			count_x++;
		}
		Node *nextNode = temp->next;
		temp->next = newNode;
		newNode->prev = temp;
		newNode->next =nextNode;
		if(nextNode != nullptr){
			newNode->next = nextNode;
			nextNode->prev = newNode;
			return;
		} 
		newNode->next = nullptr;
		
	}
	
	void deleteFront(){
		lineNode *tempLine = headLine;
		
		for(int i  =0 ; i < cursor_y;i++){
			tempLine =tempLine->next;		
		}
		Node*head= tempLine->data.get_head();
		
		for(int i = 0;i<cursor_x;i++){
			head = head->next;
		}
		headLine->data.deleteBack(head);
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
		insertChar(character);

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
				if(input =='0')
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
		else if(user_choice == 'r'){
			editor.deleteFront();
		}
		else if(user_choice == 'n' ){
			should_run = false;
		}
	}

		
	return 0;
}



