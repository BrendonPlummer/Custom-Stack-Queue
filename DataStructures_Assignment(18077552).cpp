#include <iostream>

class Node
{
public:
	Node(int value, Node* nextptr = NULL, Node* prevptr = NULL, int currentpriority = 0) //Nodes allows the implementation of a linked list, they hold given data and also hold the memory address of the Next/Prev element in the list
	{
		this->value = value;           
		next = nextptr; 
		prev = prevptr; 
		priority = currentpriority;		//These nodes also allow for a priority to be set indivudally 
	}

#pragma region Getters & Setters 
	//Getters and setters allow us to retrieve or edit information relating the node without having direct access to its attributes
	//Each function either returns a requested attribute, or sets the nodes attributes to a new given value
	int getVal(void)				
	{								
		return this->value;
	}

	Node* getNext(void) 
	{
		return this->next;
	}

	Node* getPrev(void) 
	{
		return this->prev;
	}

	int getPriority(void)
	{
		return priority;
	}

	void setVal(int value) 
	{
		this->value = value;
	}

	void setPrev(Node* prevptr) 
	{
		this->prev = prevptr;
	}

	void setNext(Node* nextptr) 
	{
		this->next = nextptr;
	}

	void setPriority(int priority) 
	{
		this->priority = priority;
	}

#pragma endregion

private:
	Node* next;
	Node* prev;
	int priority;
	int value;
};


class Stack
{
public:
	Stack(void) 
	{
		top = nullptr;  //Creates stack by assigning a *top pointer to null, nodes are added to the top and taken from the top, follows a FILO ordering
	}

	~Stack(void) 
	{
		while (top != nullptr) Pop(); //Custom deconstructor needed, deletes all nodes in the stack from memory
		delete top;					  //Removes the top* pointer from memory, prevents memory being held unnecessarily
	}

	void Push(int value) 
	{
		Node* tmp = new Node(value, top); //Creates a new node, points it to the current *top pointer
		top = tmp;						  //The *top pointer is then assigned to the new node, making it the new top of stack
	}

	Node* NodePop(void) 
	{
		Node* tmp = top;		//Assigns a *tmp pointer to *top
		if (top != nullptr)     //If top is NULL, then the stack must be empty already
		{
			top = top->getNext();						//Assigns the *top pointer to the next item in the stack, making it the new top of the Stack
			if(top != nullptr) tmp->setNext(nullptr);   //If the newly assigned *top pointer is now NULL< the stack must already be empty, if not then set tmps next value to null
		}												//Thus removing the node completely from the stack 
		return tmp;   //Returns the newly removed node from the stack
	}

	int Pop(void) 
	{
		Node* tmp = NodePop();  //Assigns a *tmp pointer to the newly removed Node from the stack
		int returnVal = 0;

		if (tmp != nullptr)               //Checking that tmp is not NULL, if *tmp is pointing to NULL, the stack must be already empty
			returnVal = tmp->getVal();	  //If not NULL, assigns the vlaue of the tmp node to the 'returnVal' variable, ready for returning
		else
			throw "Stack Empty";		//This is the error that is thrown if the stack is already empty before carrying out operation

		delete tmp;			//Deletes the *tmp pointer from memory
		return returnVal;	//Returns the 'returnVal' variable, now containing the Nodes data 
	}

private:

	Node* top;
};


class Queue
{
public:
	Queue(void)
	{
		front = back = nullptr; //Creates a *front and *back pointer, thus initialising the queue, the *front and *back pointers allow for easier navigation of the queue
	}	                        //Nodes are enqueued at the back of queue, and dequeued from the front, follows a FIFO ordering
								

	~Queue(void) 
	{
		while (front != nullptr) { delete NodeDequeue(); } //Custom deconstructor required, deletes all nodes in the queue from memory
		delete front; delete back; //Removes the front* and back pointer* from memory, after rest of queue is deleted, prevents memory addresses being held unnecessarily 
	}

	void Enqueue(int i, int priority = 0) 
	{
		Node* tmp = new Node(i, back);		//Creates a new node, pointed at the back of the queue
		back = tmp;							//Assigns the 'back' pointer to the new back of queue 
		if (front == nullptr) front = back; //If this case is true then the queue was empty before enqueue function is called, in this case it assigns the front and back pointers to the new element as it is both the front and the back of queue
		else {
			tmp = back->getNext(); //Moves the tmp pointer to the orignal back of queue
			tmp->setPrev(back);    //Assigns *prev to the new back of the queue
		}						   
	}

	int Dequeue(void) 
	{
		Node* tmp = NodeDequeue();      //Assigns a *tmp pointer to the now dequeued node
		int returnVal = 0;				
		if (tmp != nullptr)				//Checks that *tmp is not pointing to NULL, if it has then the queue must already be empty, will throw an error
			returnVal = tmp->getVal();  //If its not pointing to NULL, it will set the data of the *tmp pointer to the 'returnVal' variable
		else 
			throw "Queue Empty";        //This is the error that is thrown when there is no node in the queue

		if (front == nullptr) back = front; //If the *front is then pointing to NULL, the queue must now be empty, sets the front and back to NULL
		delete tmp;			//Deletes the *tmp pointer from memory
		return returnVal;   //Returns the returnVal variable, containing the data of the node that has just been dequeued
	}

protected:

	Node* back;
	Node* front;

private:

	virtual Node* NodeDequeue(void) //Virtual as to allow for specific method definitions in inheriting classes
	{
		Node* tmp = front;		//Assigns a tmp pointer to front of queue, queue follows FIFO ordering so the front node should be dequeued first 
		if (front != nullptr)   //If front pointer is pointing to NULL, then the queue must be empty
		{ 
			front = front->getPrev();		//Assigns the front pointer to the previous node in the queue
			if (front != nullptr)			//If the front pointer does not point to NULL after new assignment, then elements still exist in the queue
			{
				front->setNext(nullptr); //Sets the new front node *next to be pointing to NULL
				tmp->setPrev(nullptr);   //Removes the final linkage left on the node before returning
			}
		}
		return tmp; //Returns node that has been removed from the queue
	}
};


class Scheduler : public Queue
{
public:

	Scheduler(void) //Custom constructor, same as the Queue class
	{
		front = back = nullptr; //Initialises a Front* and Back* pointer, pointed at NULL
	};							//Similar to the queue class, nodes are enqueued at the back and dequeued from the front (FIFO), however the enqueue method is overridden to accomodate for prioritisation

	~Scheduler()
	{
		while (front != nullptr) { delete NodeDequeue(); } //Custom deconstructor required, deletes all nodes in the queue from memory
		delete front; delete back;  //Removes the front* and back pointer* from memory, after rest of queue is deleted, prevents memory addresses being held unnecessarily 
	}

	void Enqueue(int i, int priority = 0) //Enqueue method has been overridden for this class, incorporates the priority of the nodes as an input value
	{
		if (priority > MAX_PRIORITY)	//Confirms the priority is not above the system specified maximum, this can be changed by editing the MAX_PRIORITY constant
			throw "Priority Too High";  //If it is too high, throws an exception

		Node* tmp = new Node(i, nullptr, nullptr, priority); //Initialises a node, pointing at NULL, with its given data and priority

		//There are 4 possible cases that will decide where the node belongs, each case is commented below

		if (front == nullptr) //Case: List empty 
		{
			//With the front* pointer pointing at NULL, the queue must currently be empty
			back = tmp;		//With the queue being empty, the first node that is added will be both the front and the back of the queue 
			front = back;
		}
		else if (tmp->getPriority() >= front->getPriority()) //Case: New node has highest/higher priority
		{
			//If the Node has the highest priority, or is of higher prority or at MAX_PRIORITY it will be placed at the front of the list
			if (front->getPriority() == 10) 
			{
				//If the node at the front is already at MAX_PRIORITY, then following FIFO it will go behind the last node entered with MAX_PRIORITY
				Node* position = front;												//Starting the loop at the front of the Queue

				do {																//As the front is already confirmed at MAX_PRIORITY, 
					position = position->getPrev();									//I will move the position* pointer to the previous Node in the Queue
				} while (position != nullptr && position->getPriority() == 10);     //This then checks that the node is not NULL and also whether it is < MAX_PRIORITY. If not, it continues the loop

				//Using the position* pointer, of which points to the node with a priority < MAX_PRIORITY, we can insert the new node
				tmp->setNext(position->getNext());     //Firstly the new node is pointed to the next node in the list
				tmp->setPrev(position);				   //It is then also pointed to the node held by the position* pointer, this inserts it in its new space
				position->getNext()->setPrev(tmp);     //Next I must amend the orignal linkages of the node pointed to by position*, it points the node in front of position to the new node
				position->setNext(tmp);				   //And then points the position node to the new node, thus completing the Insert

				position = nullptr; //The position pointer is then pointed to nothing 
				delete position;	//And removed from the memory, again preventing unneccessary pointers from staying in the system
			}
			else
			{
				//If the node at front* is not already at max, then we can assume the node has the Highest priority in the queue
				tmp->setNext(nullptr); //Points the new node to NULL
				front->setNext(tmp);   //Points the old front* to the newly added node, making it the new front of the list
				tmp->setPrev(front);   //The new nodes prev* pointer is then also pointed to the old front of the list
				front = tmp;		   //The front* pointer is reassigned to the new front of the list
			}
		}
		else if (tmp->getPriority() >= back->getPriority())//Case: New node has priority higher than the back most node
		{
			//If the new nodes priority is higher than the backmost node, but lower than the frontmost node, we can assume it lies somewhere in between
			tmp->setNext(back); //Initially pointing the new node to the back to the queue to begin its insertion
			while (tmp->getNext() != nullptr && tmp->getPriority() > tmp->getNext()->getPriority())	//Checks that the next node in the queue is not NULL, and also compares the new nodes priority to that which
			{																						//is next in the queue, if the priority of the new node is lower than its next item then we have found its new location
				tmp->setNext(tmp->getNext()->getNext()); //Points the new node to the next node in the queue to be checked
			}
			tmp->setPrev(tmp->getNext()->getPrev()); //Once the loop has ended, I can assume the node is pointed to the appropriate node that is next in Queue
			tmp->getNext()->setPrev(tmp);			 //I reassign the old linkages to the new node, first setting the next node in queue to point at the new node
			tmp->getPrev()->setNext(tmp);			 //Then pointing the previous node in the queue to the new node, thus completing the insertion
		}
		else //Case: New node has the lowest priority
		{
			//If no other statement has been satisfied prior to this, the new node must have the lowest priority, therefore must be added to the back of the queue
			tmp->setNext(back); //Points the new node to the current back of the queue
			back->setPrev(tmp);	//Points the current back of the queue to the new node
			back = tmp;			//Reassigns the back* pointer to the new back of queue
		}
	}

	int Dequeue(void) //Works almost identical to the Queue class Dequeue method, with the addition of an 'ageing' process
	{
		Node* tmp = NodeDequeue();		//Assigns a *tmp pointer to the now dequeued node
		int returnVal = 0;
		if (tmp != nullptr)				//Checks that *tmp is not pointing to NULL, if it has then the queue must already be empty, will throw an error
			returnVal = tmp->getVal();  //If its not pointing to NULL, it's safe to set the data of the *tmp pointer to the 'returnVal' variable
		else
			throw "Priority Queue Empty"; //This is the error that is thrown if the priority queue is already empty
		
		if (front == nullptr) //If the *front is then pointing to NULL, the queue must now be empty
			back = front;     //The front and the back must therefore both be pointed to NULL
		else
		{ 
			//If the queue is not empty, we must 'age' each of the processess currently in the queue to prevent blocking, this is done whenever a process is removed from the queue
			Node* position = back; //Setting a position* pointer to the back fo the queue to begin 
			while (position != nullptr && position->getPriority() < MAX_PRIORITY) //If the process that position is pointing to is not NULL, and isnt already at MAX_PRIORITY we can 'age' it
			{
				position->setPriority(position->getPriority() + 1); //'Ageing' simply increases the priority of a process gradually as it spends more time in the queue
				position = position->getNext();						//The position* pointer is then assigned to the next process to continue 'ageing'
			}
			position = nullptr; //The position pointer is then pointed to nothing 
			delete position;	//And removed from the memory, again preventing unneccessary pointers from staying in the system
		}
		delete tmp; 
	}

private:

	const unsigned int MAX_PRIORITY = 10; //Const value for the priority limit, allows for easy editing of the max value 

	Node* NodeDequeue(void) //Had no need to edit the method of the virtual funciton for the original queue, kept as virtual to adhear to brief specification
	{
		Node* tmp = front;		//Assigns a tmp pointer to front of queue, queue follows FIFO ordering so the front node should be dequeued first 
		if (front != nullptr)   //If front pointer is pointing to NULL, then the queue must be empty
		{
			front = front->getPrev();		//Assigns the front pointer to the previous node in the queue
			if (front != nullptr)			//If the front pointer does not point to NULL after new assignment, then elements still exist in the queue
			{
				front->setNext(nullptr); //Sets the new front node *next to be pointing to NULL
				tmp->setPrev(nullptr);   //Removes the final linkage left on the node before returning
			}
		}
		return tmp; //Returns node that has been removed from the queue
	}
};