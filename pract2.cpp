#include<iostream>
using namespace std;

class Block {
	public:
	int BlockId;
	string data;
	float workDone;
	float prevKey;
	float currKey;
	Block *Next1,*Next2;
	
	Block(){
		Next1 = nullptr;
		Next2 = nullptr;
	}
	
	Block(int id,string data,int work,int prevKey){
		BlockId = id;
		this->data= data;
		workDone = work;
		this->prevKey = prevKey;
		currKey = 0;
		
		Next1 = nullptr;
		Next2 = nullptr;
	}
};

class BlockChain {

	private:
	Block *head;
	static int idCount;
	int sum_ascii(string data){
		int sum = 0;
		
		for(int  i =0 ; i < data.length();i++){
			sum += int(data[i]);
		}
		
		return sum;
	
	}
	void calculateCurrentKey(Block *block,int id,float workDone,string data,float prevKey){
		
		block->currKey = (int)(id * 37 + workDone*19+sum_ascii(data)+prevKey) % 100000;
		
	}
	public:
	
	BlockChain(){
		head= nullptr;
	}
	void createGenesisBlock(){
	
		string data;
		
		cout<<"Enter data:";
		cin>>data;
		int workDone = 0;
		cout<<"Enter work done: ";
		cin>>workDone;
		
		
		
		Block *newBlock = new Block(1,data,workDone,0);
		calculateCurrentKey(newBlock, newBlock->BlockId, newBlock->workDone, newBlock->data, newBlock->prevKey);
		idCount++;
		head= newBlock;
		
	}
	
	void addBlock(string data,int work){
		if(head == nullptr){
			createGenesisBlock();
		}	
		
		Block * temp = head;
		while(temp->Next1!= nullptr){
			temp = temp->Next1;
		}
		
		Block *newBlock = new Block(idCount++, data, work, temp->currKey);
		calculateCurrentKey(newBlock, newBlock->BlockId, newBlock->workDone,newBlock->data, newBlock->prevKey);
		temp->Next1 = newBlock;

		
	}

	
	void forkChain(int blockID, string data1, string data2, int work1, int work2){
		Block * temp = head;
		
		while(temp != nullptr && temp->BlockId != blockID){
			temp = temp->Next1;
		}
		
		if(temp == nullptr){
			cout<<"does not found the block"<<endl;
			return;
		}
		
		Block *newBlock1 = new Block(idCount++,data1,work1,temp->currKey);
		Block *newBlock2 = new Block(idCount++,data2,work2,temp->currKey);
		
		calculateCurrentKey(newBlock1, newBlock1->BlockId, newBlock1->workDone,newBlock1->data, newBlock1->prevKey);
		calculateCurrentKey(newBlock2, newBlock2->BlockId, newBlock2->workDone,newBlock2->data, newBlock2->prevKey);
		
		temp->Next1 = newBlock1;
		temp->Next2 = newBlock2;
	}
	
	void validateChain(){
		Block * iterator = nullptr;
		if(head->Next1 == nullptr && head->Next2 !=nullptr){
			iterator = head->Next2;
		}
		else{
			iterator = head->Next1;
		}
		
		while(iterator->Next1 != nullptr){
			if(iterator->Next1->prevKey != iterator->currKey){
				cout<<"BLock with id"<<iterator->BlockId<<" is Broken"<<endl;
				return;
			}
			
			if(iterator -> Next2 != nullptr){
				Block * temp = iterator;
				temp = temp->Next1;
				while(temp !=  nullptr){
					if(temp->Next1->prevKey != temp->currKey){
						cout<<"BLock with id"<<iterator->BlockId<<" is Broken"<<endl;
						return;
					}
				}
				Block * temp2 = iterator;
				temp2 = temp2->Next2;
				while(temp2 != nullptr){
					if(temp2->Next1->prevKey != temp2->currKey){
						cout<<"BLock with id"<<iterator->BlockId<<" is Broken"<<endl;
						return;
					}
				}
				break;
			
			}
		}
		
	}
	
	void repairChain(int blockID){
		Block * iterator = nullptr;
		bool isFound = false;
		if(head->Next1 == nullptr && head->Next2 !=nullptr){
			iterator = head->Next2;
		}
		else{
			iterator = head->Next1;
		}
		
		
		while(iterator->Next1 != nullptr){
			if(iterator->BlockId == blockID){
				isFound = true;
				
			}
			
			if(iterator -> Next2 != nullptr){
				Block * temp = iterator;
				temp = temp->Next1;
				while(temp !=  nullptr){
					if(temp->BlockId == blockID){
						isFound =true;
				
					}
					
					if(isFound){
						calculateCurrentKey(temp,temp->BlockId,temp->workDone,temp->data,temp->prevKey);
						cout<<"In Repair"<<endl;
					}
					temp = temp->Next1;
				}
				Block * temp2 = iterator;
				temp2 = temp2->Next2;
				while(temp2 != nullptr){
					if(temp2->BlockId == blockID){
						isFound =true;
				
					}
					if(isFound){
						calculateCurrentKey(temp2,temp2->BlockId,temp2->workDone,temp2->data,temp2->prevKey);
						cout<<"In Repair"<<endl;
					}
					temp2 = temp2->Next1;
				}
				break;
			
			}
			
			if(isFound){
				calculateCurrentKey(iterator,iterator->BlockId,iterator->workDone,iterator->data,iterator->prevKey);
				cout<<"In Repair"<<endl;
			}
			iterator = iterator->Next1;
		}
		if(!isFound){
			cout<<"Couldnot find the block"<<endl;
			return;
		}
	}
	
	
	void recalculte(Block * starting){
		Block *iterator=starting;
		
		int key = iterator->currKey;
		
		while(iterator != nullptr){
			
			if(iterator->Next2 !=nullptr){
				Block * temp = iterator;
					temp = temp->Next1;
			
				while(temp!= nullptr){
					calculateCurrentKey(temp, temp->BlockId, -11111,temp->data, key);
					temp = temp->Next1;
				}
				
				Block * temp2 = iterator;
					temp2 = temp2->Next2;
			
				while(temp2!= nullptr){
					calculateCurrentKey(temp2, temp2->BlockId, temp2->workDone,temp2->data, key);
					temp2 = temp2->Next1;
				}
				
				break;
			
			
			}
			iterator = iterator ->Next1;
		}
	}
	
	void temperBlock(int blockID,int newKey){
		Block * iterator = head;
		
		while(iterator != nullptr){
		
			if(iterator->BlockId == blockID){
				iterator->currKey = newKey;
				
				cout<<"The block has been tempered"<<endl;
				return;
			}
		
			if(iterator -> Next2 != nullptr){
				Block * temp = iterator;
				temp = temp->Next1;
				while(temp != nullptr){
					if(temp->BlockId == blockID){
						temp->currKey = newKey;
						
						
						return;
					}
					temp = temp->Next1;
				}
				Block * temp2 = iterator;
				temp2 = temp2->Next2;
				while(temp2 != nullptr){
					
					if(temp2->BlockId == blockID){
						temp2->currKey = newKey;
					
						return;
					}
					temp2 = temp2->Next1;	
					
				}
				break;
			}
		
			
			
			iterator = iterator -> Next1;
		} 
		
		
		
		if(!iterator){
			cout<<"Could not found any block with id:"<<blockID<<endl;
		}
		cout<<"Block repaired"<<endl;
		
	}
	
	void displayChain() {
	    Block* temp = head;
	    
	    while (temp != nullptr) {
		cout<<"BlockID: "<<temp->BlockId<<endl; 
		cout<<" | Data: "<<temp->data<<endl;
		cout<<" | Work: "<<temp->workDone<<endl;
		cout<<" | PrevKey: "<<temp->prevKey<<endl;
		cout<<" | CurrKey: "<<temp->currKey<<endl;
		if(temp->Next2 != nullptr){
			Block *fork1 = temp;
			fork1 = fork1->Next1;
			cout<<"***Fork detected "<<endl;

			while(fork1 != nullptr ){
				cout<<"BlockID: "<<fork1->BlockId<<endl; 
				cout<<" | Data: " <<fork1->data<<endl;
				cout<<" | Work: " <<fork1->workDone<<endl;
				cout<<" | PrevKey: "<<fork1->prevKey<<endl;
				cout<<" | CurrKey: "<<fork1->currKey<<endl;
				fork1 = fork1->Next1;
			}
			
			Block *fork2 = temp;
			fork2 = fork2->Next1;
			
			while(fork2 != nullptr ){
				cout<<"BlockID: "<<fork2->BlockId<<endl; 
				cout<<" | Data: " <<fork2->data<<endl;
				cout<<" | Work: " <<fork2->workDone<<endl;
				cout<<" | PrevKey: "<<fork2->prevKey<<endl;
				cout<<" | CurrKey: "<<fork2->currKey<<endl;
				fork2 = fork2->Next2;
			}
			
			 break;
		}
		temp = temp->Next1;
	   }
	   
	   cout<<endl;
	}
};

int BlockChain :: idCount  = 1;

int main(){
    BlockChain obj;
    int choice;
    
    do {
        cout<<"\n=== BLOCKCHAIN TEST MENU ==="<<endl;
        cout<<"1. Create Genesis Block"<<endl;
        cout<<"2. Add Block"<<endl;
        cout<<"3. Create Fork"<<endl;
        cout<<"4. Tamper Block"<<endl;
        cout<<"5. Validate Chain"<<endl;
        cout<<"6. Repair Chain"<<endl;
        cout<<"7. Display Chain"<<endl;
        cout<<"8. Exit"<<endl;
        cout<<"Enter your choice: ";
        cin>>choice;
        
        switch(choice) {
            case 1: {
                obj.createGenesisBlock();
                break;
            }
            case 2: {
                string data;
                int work;
                cout<<"Enter data for new block: ";
                cin>>data;
                cout<<"Enter work done: ";
                cin>>work;
                obj.addBlock(data, work);
                break;
            }
            case 3: {
                int blockID;
                string data1, data2;
                int work1, work2;
                cout<<"Enter block ID to fork from: ";
                cin>>blockID;
                cout<<"Enter data for fork path 1: ";
                cin>>data1;
                cout<<"Enter work for fork path 1: ";
                cin>>work1;
                cout<<"Enter data for fork path 2: ";
                cin>>data2;
                cout<<"Enter work for fork path 2: ";
                cin>>work2;
                obj.forkChain(blockID, data1, data2, work1, work2);
                break;
            }
            case 4: {
                int blockID, newKey;
                cout<<"Enter block ID to tamper: ";
                cin>>blockID;
                cout<<"Enter new key value: ";
                cin>>newKey;
                obj.temperBlock(blockID, newKey);
                break;
            }
            case 5: {
                obj.validateChain();
                break;
            }
            case 6: {
                int blockID;
                cout<<"Enter block ID to repair from: ";
                cin>>blockID;
                obj.repairChain(blockID);
                break;
            }
            case 7: {
                obj.displayChain();
                break;
            }
            case 8: {
                cout<<"Exiting program..."<<endl;
                break;
            }
            default: {
                cout<<"Invalid choice! Please try again."<<endl;
            }
        }
    } while(choice!= 8);
    
    return 0;
}




