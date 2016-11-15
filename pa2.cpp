#include <iostream> 
#include <iomanip>
#include <string>
using namespace std;

#define NUMPAGES 32
int algorithmChoice; // worst fit =0, best fit = 1
int pickAlgorithm(string algo);

//This structure tracks the page fragments available. It is one node within the linked list

struct Fragment{
	int start;
	int size;
	Fragment *next;
};


//All the memory pages are managed within this class

class MemoryManager{
	string pages[NUMPAGES];
	int pageSize;
	Fragment *head; //the head of the fragment linked list
	Fragment *runner; //the iterator for the linked list
	int count; //number of elements within the linked list
	
public:

	// The constructor for initializing the class and marking all pages 
	// as free 
	MemoryManager(){
		head = NULL;
		runner = NULL;
		for (int i = 0; i < NUMPAGES; i++)
			pages[i] = "Free";
		buildFragList();
		pageSize = 4;
		count = 0;
	}

	// computes the number of pages needed for a defined memory size in KB
	int pagesNeeded(int kbSize){
		int pg = kbSize / pageSize;
		if (kbSize%pageSize != 0) pg++;
		return pg;
	}

	//Used to mark the free pages when a program terminates
	void markFreePages(int start, int kbSize){
		int size = pagesNeeded(kbSize);
		for (int i = start; i < (start + size); i++)
			pages[i] = "Free";
	}

	//Marks the pages as used when a program is added successfully
	void markBusyPages(int start, int kbSize, string prog){
		int size = pagesNeeded(kbSize);
		for (int i = start; i < (start + size); i++)
			pages[i] = prog;
	}

	//Shows the pages available and used by programs
	void printMemoryStatus(){
		for (int i = 0; i < 4; i++){
			for (int j = 0; j < 8; j++)
				cout << setw(7) << pages[8 * i + j] << " ";
			cout << endl;
		}
	}


	//The fragment count of the linked list
	int getCount(void){
		return count;
	}

	//Used for updating the fragment list on addition or removal of a program
	void buildFragList(){
		deleteList();
		count = 0;
		int i = 0;
		while (i < NUMPAGES){
			while (pages[i] != "Free" && i < NUMPAGES) //Find the next free block
				i++;
			if (i < NUMPAGES && pages[i] == "Free"){
				Fragment* frag = new Fragment;
				count++;
				frag->start = i;
				frag->next = NULL;
				frag->size = 0;
				while (i < NUMPAGES && pages[i] == "Free"){
					i++;
					frag->size++;
				}
				addFragment(frag);
			} //end if
		} //end while
	}

	//Adds a new fragment object to the linked list
	void addFragment(Fragment* frag){
		if (head == NULL)
			head = frag;
		else{
			runner = head;
			while (runner->next != NULL)
				runner = runner->next;
			runner->next = frag;
		}
	}

	//Used for cleanup of the list on exit
	void deleteList(void){
		while (head != NULL){
			runner = head;
			head = head->next;
			delete runner;
		}
	}

	//Displays the number of fragments available and their position
	void printList(void){
		runner = head;
		cout << endl;
		if (head == NULL) {
			cout << "No free fragments available\n";
			return;
		}
		cout << endl << "There are " << count << " fragment(s) available\n";
		do {
			cout << "pos=  " << runner->start << "   pages=    " << runner->size << endl;
			runner = runner->next;
		} while (runner != NULL);
		cout << endl;
	}


	// Find largest fragment to use with the worst fit algorithm

	Fragment *getLargestFragment(int size){

		Fragment *fr= NULL;
		int largest = 0;
		runner = head;
		while (runner!= NULL){
			if (runner->size > largest){
				largest = runner->size;
				fr = runner;
			}
			runner = runner->next;
		}

		//check if largest fragment found is large enough for the program
		if (largest >= pagesNeeded(size))
			return fr;
		else return NULL;
	}

	// Find best fitting fragment to use with the best fit algorithm

	Fragment *getBestFitFragment(int size){

		Fragment *fr = NULL;
		int bestSize = 5000;
		int needed = pagesNeeded(size);
		runner = head;
		while (runner != NULL){
			if (runner->size >= needed && runner->size < bestSize){
				bestSize = runner->size;
				fr = runner;
			}
			runner = runner->next;
		}
		return fr;
	}
};


// A linked list node to record the programs and their position
struct Program{
	string name;
	int start;
	int kbSize;
	Program *next;
};


// A class for maintaining a program linked list

class ProgramList{
	Program* head; //Head of the list
	Program* runner; //Iterator for the list
	MemoryManager *memMgr; //Pointer to the memory manager for the programs

public:
	
	ProgramList(){
		head = NULL;
		runner = NULL;
	}
	
	//Initialized the memory manager object
	void setMemMgr(MemoryManager *mgr){
		memMgr = mgr;
	}
	// Adds a new program
	bool addProgram(string name, int size){

		Fragment *frag;
		// Check if a program name exists already. If so, the addition fails
		if (checkDuplicate(name) == true){
			cout << "A program by this name exists already. Unable to add.\n";
			return false;
		}

		//Requests a fragment from the memory manager based on the algorithm type
		if (algorithmChoice == 0)
			frag = memMgr->getLargestFragment(size);
		else
			frag = memMgr->getBestFitFragment(size);
		// Failure in case of a program being too large
		if (frag == NULL){
			cout << "Unable to accommodate a program of this size\n";
			return false;
		}
		// Allocates the memory for the program
		memMgr->markBusyPages(frag->start,size,name);
		//Adds a new program object to the linked list
		Program *prog = new Program;
		prog->name = name;
		prog->kbSize = size;
		prog->start = frag->start;
		prog->next = NULL;
		if (head == NULL)
			head = prog;
		else{
			runner = head;
			while (runner->next != NULL)
				runner = runner->next;
			runner->next = prog;
		}
		memMgr->buildFragList();
		cout << "Program added successfully. Pages used = " << memMgr->pagesNeeded(size) << endl;
		//printProgList();
		return true;
	}


	//Removes a program from the linked list and frees up the memory in the memory manager
	bool removeProgram(string name){

		Program *temp;

			runner = head;
			if (head == NULL) 
				return false;
			else if (head->name == name){
				head = head->next;
				memMgr->markBusyPages(runner->start, runner->kbSize, "Free");
				cout << "Program successfully removed. Number of pages reclaimed = " << memMgr->pagesNeeded(runner->kbSize) << endl;
				delete runner;
				memMgr->buildFragList();
				return true;
			}
			else {
				while (runner->next != NULL){
					temp = runner->next;
					if (temp->name == name){
						runner->next = temp->next;
						memMgr->markBusyPages(temp->start, temp->kbSize, "Free");
						cout << "Program removed. Number of pages reclaimed = " << memMgr->pagesNeeded(temp->kbSize) << endl;
						delete temp;
						memMgr->buildFragList();
						return true;
					} //end if
					else
						runner = runner->next;
				} //end while
			}  //end else

			return false; //Unable to locate program by that name
	}

	//Releases linked list memory on exit
	void deleteList(void){
		while (head != NULL){
			runner = head;
			head = head->next;
			delete runner;
		}
	}

	//checks for a duplicate program name
	bool checkDuplicate(string name){
		if (head == NULL) 
			return false;
		else{
			runner = head;
			while (runner != NULL){
				if (runner->name == name)
					return true;
				else runner = runner->next;
			}
			return false;
		}
	}


	//test method
	void test(){
		memMgr->markBusyPages(0, 10, "P1");
		memMgr->markBusyPages(7, 22, "P2");
		memMgr->markBusyPages(22, 8, "P3");

		memMgr->buildFragList();
	}

	//Debugging method to see the current list of programs
	void printProgList(void){
		runner = head;
		cout << endl;
		if (head == NULL) {
			cout << "No more programs\n";
			return;
		}
		do {
			cout << runner->name << "     " << runner->start << "  "<< runner->kbSize << endl;
			runner = runner->next;
		} while (runner != NULL);
		cout << endl;
	}
};

//Parses command line to set the algorithm type
int setAlgorithm(string algorithm){

	if (algorithm[0] == 'w' || algorithm[0] == 'W'){
		algorithmChoice = 0;
		cout << endl << "Using Worst fit algorithm" << endl;
	}
	else if (algorithm[0] == 'b' || algorithm[0] == 'B'){
		algorithmChoice = 1;
		cout << endl << "Using best fit algorithm" << endl;
	}
	else{
		cout << endl << "Please input a valid algorithm choice" << endl;
		return 2;
	}

	return algorithmChoice;
}


// Main program with switch statement

int main(int argc, char* argv[]) {

	if (argc != 2){
		cout << "Please provide algorithm choice on command line, best or worst" << endl;
		return 0;
	}

	if (setAlgorithm(argv[1]) ==2)
		return 0;

	int menuSelection = 0;
	ProgramList progList;
	MemoryManager memMgr;
	string progName;
	int progSize;

	progList.setMemMgr(&memMgr);

	while (menuSelection != 5){
		cout << endl << " 1. Add program\n 2. Kill program\n 3. Fragmentation\n 4. Print memory\n 5. Exit\n" << endl;
		cout << "Please make your selection -- ";
		cin >> menuSelection;
		cout << endl;

		switch (menuSelection){
		case(1) :
			cout << "Attempting to add program" << endl;
			cout << "Please enter program name:  ";
			cin >> progName;
			cout << "Please enter program size:  ";
			cin >> progSize;
			progList.addProgram(progName, progSize);
			break;

		case(2) :
			cout << "Attempting to kill program" << endl;
			cout << "Please enter program name:   ";
			cin >> progName;
			if (progList.removeProgram(progName) == false)
				cout << "Unable to locate program with the provided name\n";
			break;

		case(3) :
			cout << "Printing fragment info" << endl;
			memMgr.printList();
			break;

		case(4) :
			cout << "Printing memory status" << endl;
			memMgr.printMemoryStatus();
			break;

		case(5) :
			progList.deleteList();
			memMgr.deleteList();
			cout << "Good bye and have a nice day" << endl;
			break;

		default:
			cout << "Invalid choice. Please try again" << endl;

		}
	}

	return 0;
}







