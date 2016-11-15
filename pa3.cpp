#include <iostream> 
#include <string>
#include <cmath>
using namespace std;

#define PI 3.14159 //needed for converting degrees to radians
#define EMPTY 0.0

//This structure holds one stack element and points to the next
//The operand might could be unary, in which case op1 does not matter

struct StackElement{
	double opdata;
	char oper;
	StackElement *next;
};


//All the memory pages are managed within this class

class Calculator{

	string inp;
	int pos; //track the current position in the string
	int strLen;
	StackElement *head; //the head of the StackElement linked list

public:

	// The constructor for initializing the class and marking all pages 
	// as free 
	Calculator(string str){
		head = NULL;
		inp = str;
		stripSpaces();
		pos = 0;
		strLen = inp.size();
	}

	void stripSpaces(void){

		int k=0;
		int sz = inp.size();

		for (int i = 0; i < sz; i++){
			if (inp.at(i) != ' '){
				inp[k] = inp[i];
				k++;
			}
		}
		inp.resize(k);

	}

	// Check that the parentheses are balanced before starting any computation

	int checkParenths(){

		int count = 0;
		//cout << "Computing " << inp << " of length " << strLen << "\n";
		if (inp[0] != '('){
			cout << "Expression must begin with a ( \n";
			return -1;
		}

		for (int i = 0; i < strLen; i++){
			if (inp[i] == '(')
				count++;
			else if (inp[i] == ')')
				count--;
			if (count < 0) {
				cout << "ERROR. Found a ) before a ( \n";
				return -1;
			}
		}

		return count;
	}

	// Pushes a new element on to the stack;

	void push(StackElement *el){
		el->next = head;
		head = el;
	}

	// Pop the top element

	StackElement* pop(){
		StackElement *el = head;
		if (head != NULL)
			head = head->next;
		return el;
	}

	bool isNum(int p){
		if (inp[p] >= '0' && inp[p] <= '9')
			return true;
		return false;
	}

	//looking for something like (-2776)

	bool isNegNum(int p){
		if (inp[p] == '-' && inp[p-1] == '(' && isNum(p+1))
			return true;
		return false;
	}

	//Get the next element from the tokenizer

	StackElement* getNext(){

		StackElement *el;
		bool neg = false; //stays false unless a negative number is found

		while (inp.at(pos) == '(') //skip open paren
			pos++;

		el = new StackElement;
		el->opdata = EMPTY;
		el->next = NULL;

		//first check if a negative number (or positive) might be in the input line

		if (isNum(pos) || isNegNum(pos)){
			std::size_t d = 0;
			el->opdata = stod(inp.substr(pos), &d);
			if (inp[pos] == '-') pos++; //increment one extra to consume the closing parenth for neg number
			pos += d;

			//check the operator that follows

			switch (inp[pos]){

			case '+':
			case '-':
			case '*':
			case '/':
			case '^':
			case ')':
				el->oper = inp[pos];
				pos++;
				break;
			default: //bad input
				el->oper = 'E';
				break;
			}
		}
		else {  //check for the unary operators

			switch (inp[pos]){

			case 'c': //looking for cos
				if (inp.compare(pos, 3, "cos") == 0){
					el->oper = 'c';
					pos += 3;
				}
				else
					el->oper = 'E';
				break;

			case 'l': //looking for log2
				if (inp.compare(pos, 3, "log") == 0){
				el->oper = 'l';
				pos += 3;
				}
				else
					el->oper = 'E';
				break;

			case 's': //looking for sin or sqrt
				if (inp.compare(pos, 3, "sin") == 0){
					el->oper = 's';
					pos += 3;
				}
				else if (inp.compare(pos, 4, "sqrt") == 0){
					el->oper = 'q';
					pos += 4;
				}
				else
					el->oper = 'E';
				break;

			case ')': //A previous operation is being closed out
			case '+': //operand for a number on stack already
			case '-':
			case '*':
			case '/':
			case '^':
				el->oper = inp[pos];
				pos++;
				break;
	
			default: //Bad input
				el->oper = 'E';
				break;
			} //end switch
		} //end else

		return el;
	}

	//Perform the next computation

	int updateResult(StackElement *el){

		cout << "Performing operation: " << head->opdata << " " << head->oper << " " << el->opdata << endl;

		switch (head->oper){
		case '+':
			head->opdata = head->opdata + el->opdata;
			break;
		case '-':
			head->opdata = head->opdata - el->opdata;
			break;
		case '*':
			head->opdata = head->opdata * el->opdata;
			break;
		case '/':
			if (el->opdata != 0){
				head->opdata = head->opdata / el->opdata;
			}
			else{
				cout << "\nERROR: Divide by zero\n";
				return -1;
			}
			break;
		case '^':
			head->opdata = pow(head->opdata,el->opdata);
			break;
		case 's':
			if (el->opdata == 90)
				head->opdata = 1;
			else
				head->opdata = sin(el->opdata * PI / 180.0);
			break;
		case 'c':
			if (el->opdata == 90)
				head->opdata = 0;
			else
				head->opdata = cos(el->opdata * PI / 180.0);
			break;
		case 'l':
			if (el->opdata < 0){
				cout << "\n ERROR: Log of negative\n";
				return -1;
			}
			else
				head->opdata = log2(el->opdata);
			break;
		case 'q':
			if (el->opdata < 0){
				cout << "\n ERROR: Square root of negative\n";
				return -1;
			}
			else
				head->opdata = pow(el->opdata, 0.5);
			break;
		}
		delete el; //release the element processed 
		return 1;
	}

	void processIt(){

		StackElement *el;

		//Get tokens until end of line reached
		while ( pos < strLen){

			el = getNext();

			if (el->oper == 'E'){
				cout << "\n Bad input received. Please check and re-enter\n";
				return;
			}
			else if (el->oper == ')'){ //perform a calculation
				if (el->opdata == EMPTY){ //close out a previous calc using top stack element
					delete el;
					el = pop();
				}
				if (updateResult(el) == -1) //Some computation error
					return;
			}
			else if (el->opdata == EMPTY){ 
				//only an operator was received, if unary, put it on stack else update last stack element
				if (el->oper == 's' || el->oper == 'c' || el->oper == 'l' || el->oper == 'q')
					push(el);
				else {
					head->oper = el->oper;
					delete el;
				}
			}
			else push(el); //Received a number and an operand. Put it on stack

		}

		cout << "\n The result is: " << head->opdata << endl;
		delete head;

	}

};


// Main program with switch statement

int main() {


	std::string str;
	Calculator* calc;
	
	cout << "Please enter a string or 0 to exit: ";
	std::getline(std::cin, str);

	while (str.at(0) != '0'){

		calc = new Calculator(str);
		if (calc->checkParenths() == 0)
			calc->processIt();
		else
			cout << "ERROR. Unbalanced Parentheses. Please try again\n";

		delete calc;

		cout << "\nPlease enter a new string or 0 to exit: ";	
		std::getline(std::cin, str);
		
	}
	return 0;
}







