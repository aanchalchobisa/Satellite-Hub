#include "MemoryUnit.h"
//#include <unistd.h>

using namespace std;

#define SEND_TIME(bytes) ((bytes)/2)*(291+1)+15+1+1+291
#define REQUEST_TIME(bytes) ((bytes)/2)*(291+1)+15+1+1+1+291
#define STORAGE_TIME(bytes) 100+((2+((bytes)/2))*1333333)+15+1+1

Transaction Transactions[100];

int read_files( char const * foo_file )
{

	string line[100];
	int i;
	int temp;
	int strsize;

	temp = 0;
	ifstream file;

	/* Open the each file */
	file.open(foo_file, ios::in);

	/* Get rid of the first line, the headers */
	getline(file, line[0], '\r');
	line[0] = "";

	/* Import each line of s single file into an array of strings */
	while ( getline(file, line[temp], '\r') )
	{
		strsize = line[temp].length();
		if (strsize < 5) 
		{ 
			break; 
		}
		temp++;
	}

	/* Parse out the strings for each element of the struct 
		Skip the first line as it is a header                */
	for (i = 0; i < temp; i++)
	{
		string parsed = "";
		stringstream input(line[i]);

		/* Get the request start time */
		getline(input, parsed, ',');
		
		Transactions[i].arrival_time = stoi(parsed);
			/* Get the device id */
		getline(input, parsed, ',');
		Transactions[i].device = stoi(parsed);

		/* Get the transaction type */
		getline(input, parsed, ',');
		/* If the transaction is a send enter a 0, 1 for request */
		if (parsed == "SEND")
		{
			Transactions[i].operation = 0;
		}
		else
		{
			Transactions[i].operation = 1;
		}

		/* Get the size of the transaction */
		getline(input, parsed, ',');
		Transactions[i].size = stoi(parsed);

		/* Get the tag of the transaction */
		getline(input, parsed, '\r');
		Transactions[i].tag = stoi(parsed);

		/* Initialize the transaction finish time to zero */
		Transactions[i].start = 0;
		Transactions[i].finish = 0;


	}

	/* Close the file */
	file.close();

	return(temp);
}

// This function is passed a data size and returns the starting address of the place to store it.
// If there's no room, it returns -1.
int is_space(int size, LUT m1){
	int i = 1;
	int j = 0;
	size = size/128; // = 1, 4 or 8
	while(i <= 80){
		// Search in blocks of 'size', starting at each slot i.
		if(i+(size-1) > 80) return -1;
		for(j = 0; j < size ; j++){
			//if((i+j) > 80) {
			//	cout<<"i, j"<<i<<j<<endl;
			//	return -1;// Reached the end without finding space;
			//} 
			if(m1.occupied[i+j]) j = size; 		// Occupied slot. This block won't work. Try next i;
			else if(j == (size - 1)) return i;	// Block is empty. Return starting slot i.
		}

		i++;
	}

	cout<< "IS_SPACE FUNCTION ERROR INVESTIGATION"<< endl;
	cout<< m1.occupied[80]<<endl;
	cout<< "size/128 = " << size<< endl;
	cout<< "i = " << i << endl;
	cout<< "j = " << j << endl;
	cout << "Error in is_space function." << endl;
}


int main(int argc, char * argv[]){

	// Create needed variables
	LUT M1;
	int trans_count;
	int j;
	int i = 0;
	int slot;
	uint64_t num; // scratch pad number
	uint64_t current_time = 1;
	uint64_t communication_count = 0; 
	uint64_t cumulative_latency = 0;


	/* A quick header, small description, and directions */

	cout << "///////////////////////////////////////////////////" << endl;
	cout << "//  ECE - 485 Microprocessor System Design       //" << endl;
	cout << "//  Team Misfit Toys (cheesecake)                //" << endl;
	cout << "//  Christopher Dean                             //" << endl;
	cout << "//  Aanchal Chobisa                              //" << endl;
	cout << "//  Seth Esther                                  //" << endl;
	cout << "///////////////////////////////////////////////////" << endl << endl;
	cout << "This program will take the three traffic files as" << endl;
	cout << "and output the total latency, the unit cost," << endl;
	cout << "and communication cost for each transaction and" << endl;
	cout << "normalized over the three transactions." << endl << endl;
	cout << "Please create the directory C:/Traffic/ on your" << endl;
	cout << "and place the three traffic files in this folder." << endl << endl;
	cout << "When you are ready, press enter to begin." << endl;
	cin.get();
	cout << endl << "Reading in the files now....." << endl << endl;


	trans_count = read_files( argv[1] );


	cout << "File is as below:" << endl;
		
	for (j = 0; j < trans_count; j++)
	{
		cout << "Line #" << j << " : ";
		cout << Transactions[j].arrival_time << " ,";
		cout << Transactions[j].device << " ,";
		cout << Transactions[j].operation << " ,";
		cout << Transactions[j].size << " ,";
		cout << Transactions[j].tag <<endl;
	}
	cout << endl << "End of file " << endl << endl;

	cin.get();
/*	
//Create Transactions array manually (Replace this part with read-from-file)
	Transaction Transactions[24];
	Transactions[0].initialize(0,1,0,128,1);
	Transactions[1].initialize(50,2,0,128,2);
	Transactions[2].initialize(100,3,0,128,3);
	Transactions[3].initialize(150,1,0,1024,4);
	Transactions[4].initialize(200,2,1,128,1);
	Transactions[5].initialize(210,1,1,128,2);
	Transactions[6].initialize(300,3,0,1024,5);
	Transactions[7].initialize(400,3,0,1024,6);
	Transactions[8].initialize(500,3,0,512,7);
	Transactions[9].initialize(1000,1,1,128,3);
	Transactions[10].initialize(1100,2,1,128,3);
	Transactions[11].initialize(2000,3,0,1024,8);
	Transactions[12].initialize(3000,3,0,1024,9);
	Transactions[13].initialize(4000,3,0,1024,10);
	Transactions[14].initialize(5000,3,0,1024,11);
	Transactions[15].initialize(6000,3,0,1024,12);
	Transactions[16].initialize(7000,3,0,1024,13);
	Transactions[17].initialize(8000,3,0,1024,14);
	Transactions[18].initialize(9000,3,0,1024,15);
	Transactions[19].initialize(10000,3,0,1024,16);
	Transactions[20].initialize(11000,3,0,1024,17);
	Transactions[21].initialize(11500,2,1,1024,12);
	Transactions[22].initialize(12000,1,1,1024,10);
	Transactions[23].initialize(12100,3,1,512,7);

*/
// Create and fill queues
	/* Queue usage:
	 * queue.push(object)
	 * queue.pop(object)
	 * queue.size()		<--Returns int
	 * queue.front()	<--Returns object
	 * queue.back()		<--Returns object
	 */
	queue <Transaction> D1_send;
	queue <Transaction> D1_req;
	queue <Transaction> D2_send;
	queue <Transaction> D2_req;
	queue <Transaction> D3_send;
	queue <Transaction> D3_req;
	queue <Transaction> Storage_send;
	queue <Transaction> Storage_req;


	cout << "Total number of transactions: " << trans_count << endl;
// Go through array and put each one in the correct queue
	while (i < (trans_count)){
		
		switch (Transactions[i].device){
			case 1:	if(Transactions[i].operation == 0) D1_send.push(Transactions[i]);
					else if(Transactions[i].operation == 1) D1_req.push(Transactions[i]);
					else cout << "Queue building: Something went wrong 1" << endl;
					break;
			case 2:	if(Transactions[i].operation == 0) D2_send.push(Transactions[i]);
					else if(Transactions[i].operation == 1) D2_req.push(Transactions[i]);
					else cout << "Queue building: Something went wrong 2" << endl;
					break;
			case 3:	if(Transactions[i].operation == 0) D3_send.push(Transactions[i]);
					else if(Transactions[i].operation == 1) D3_req.push(Transactions[i]);
					else cout << "Queue building: Something went wrong 3" << endl;
					break;
			default:	cout << "Queue building: Something went wrong" << endl;

		}

		i++;
	}

// Show that it worked
	cout << "D1_send queue length: " << D1_send.size() << endl;
	cout << "D1_req  queue length: " << D1_req.size() << endl;
	cout << "D2_send queue length: " << D2_send.size() << endl;
	cout << "D2_req  queue length: " << D2_req.size() << endl;
	cout << "D3_send queue length: " << D3_send.size() << endl;
	cout << "D3_req  queue length: " << D3_req.size() << endl;
	cout << "storage_send queue length: " << Storage_send.size() << endl;
	cout << "storage_req  queue length: " << Storage_req.size() << endl;

// Each loop is one clock cycle.
	while(1){

		// Check each queue to see if something takes place at this clock time.


		/*
		 *
		 *		CHECK IF ANY TRANSACTIONS START NOW
		 *
		 */

		// Check if Device 1 SEND is ready to start.
		if( !D1_send.empty() && (D1_send.front().start == 0) && (current_time >= D1_send.front().arrival_time) ){
			slot = is_space(D1_send.front().size, M1);
			if(slot >= 0){
				// Calculate and store start & finish time
				D1_send.front().start = current_time;
				D1_send.front().finish = current_time + SEND_TIME(D1_send.front().size);
				cout << "Device 1 begins sending tag " << D1_send.front().tag;
				cout << " at clock time " << current_time << endl;
			// Put info in LUT
				// Update transaction counter
				M1.counter++;
				// Set occupied bits
				num = (D1_send.front().size/128); // Will be 1, 4, or 8
				for(i = 0; i < num; i++) M1.occupied[slot + i] = 1;
				// Fill in this tag's line
				M1.lines[D1_send.front().tag].address = slot;
				num == 1 ? num = 1 : (num == 4 ? num = 2 : num = 3); // Translate to 2-bit size representation
				M1.lines[D1_send.front().tag].size = num;
				M1.lines[D1_send.front().tag].last_transaction = M1.counter;
			}
			// Else there's no room. Move something to storage.
			// But first check if there's something in the storage queue because its memory won't be marked 
			// clear until it's finished moving to storage, meaning you'll try to send the same thing to
			// storage multiple times.

			// Eviction policy evicts the *largest* available data unit, choosing the earliest arrival.
			// That is, the policy is FIFO but only for 1024-sized blocks.
			else if(Storage_send.empty()){
				i = 1;
				while(i < 32){
					// Is this tag for 1024B data in memory?
					if((M1.lines[i].size == 3) && (M1.lines[i].address < 81)) {
						// Put a new transaction on the storage_send queue
						Transaction *temp = new Transaction;
						temp->initialize(current_time,4,1,1024,i);
						Storage_send.push(*temp);
						i = 32;
					}
					i++;
				}
			}
		}

		// Check if Device 1 REQUEST is ready to start
		if( !D1_req.empty() && (D1_req.front().start == 0) && (current_time >= D1_req.front().arrival_time) ){
			slot = M1.lines[D1_req.front().tag].address; // Starting point
			if( slot > 0){ // Data is here
				// Calculate and store start & finish time
				D1_req.front().start = current_time;
				D1_req.front().finish = current_time + REQUEST_TIME(D1_req.front().size);
				cout << "Device 1 begins receiving tag " << D1_req.front().tag;
				cout << " at clock time " << current_time << endl;
			// Update info in LUT
				// Update transaction counter
				M1.counter++;
				M1.lines[D1_req.front().tag].last_transaction = M1.counter;
			}
			else ;// Data hasn't arrived yet. Do nothing.
		}




		// Check if Device 2 SEND is ready to start.
		if( !D2_send.empty() && (D2_send.front().start == 0) && (current_time >= D2_send.front().arrival_time) ){
			slot = is_space(D2_send.front().size, M1);
			if(slot >= 0){
				// Calculate and store start & finish time
				D2_send.front().start = current_time;
				D2_send.front().finish = current_time + SEND_TIME(D2_send.front().size);
				cout << "Device 2 begins sending tag " << D2_send.front().tag;
				cout << " at clock time " << current_time << endl;
			// Put info in LUT
				// Update transaction counter
				M1.counter++;
				// Set occupied bits
				num = (D2_send.front().size/128); // Will be 1, 4, or 8
				for(i = 0; i < num; i++) M1.occupied[slot + i] = 1;
				// Fill in this tag's line
				M1.lines[D2_send.front().tag].address = slot;
				num == 1 ? num = 1 : (num == 4 ? num = 2 : num = 3); // Translate to 2-bit size representation
				M1.lines[D2_send.front().tag].size = num;
				M1.lines[D2_send.front().tag].last_transaction = M1.counter;
			}
			// Else there's no room. Move something to storage.
			// But first check if there's something in the storage queue because its memory won't be marked 
			// clear until it's finished moving to storage, meaning you'll try to send the same thing to
			// storage multiple times.

			// Eviction policy evicts the *largest* available data unit, choosing the earliest arrival.
			// That is, the policy is FIFO but only for 1024-sized blocks.
			else if(Storage_send.empty()){
				i = 1;
				while(i < 32){
					// Is this tag for 1024B data in memory?
					if((M1.lines[i].size == 3) && (M1.lines[i].address < 81)) {
						// Put a new transaction on the storage_send queue
						Transaction *temp = new Transaction;
						temp->initialize(current_time,4,1,1024,i);
						Storage_send.push(*temp);
						i = 32;
					}
					i++;
				}
			}
		}

		// Check if Device 2 REQUEST is ready to start
		if( !D2_req.empty() && (D2_req.front().start == 0) && (current_time >= D2_req.front().arrival_time) ){
			slot = M1.lines[D2_req.front().tag].address; // Starting point
			if( slot > 0){ // Data is here
				// Calculate and store start & finish time
				D2_req.front().start = current_time;
				D2_req.front().finish = current_time + REQUEST_TIME(D2_req.front().size);
				cout << "Device 2 begins receiving tag " << D2_req.front().tag;
				cout << " at clock time " << current_time << endl;
			// Update info in LUT
				// Update transaction counter
				M1.counter++;
				M1.lines[D2_req.front().tag].last_transaction = M1.counter;
			}
			else ;// Data hasn't arrived yet. Do nothing.
		}




		// Check if Device 3 SEND is ready to start.
		if( !D3_send.empty() && (D3_send.front().start == 0) && (current_time >= D3_send.front().arrival_time) ){
			slot = is_space(D3_send.front().size, M1);
			if(slot >= 0){
				// Calculate and store start & finish time
				D3_send.front().start = current_time;
				D3_send.front().finish = current_time + SEND_TIME(D3_send.front().size);
				cout << "Device 3 begins sending tag " << D3_send.front().tag;
				cout << " at clock time " << current_time << endl;
			// Put info in LUT
				// Update transaction counter
				M1.counter++;
				// Set occupied bits
				num = (D3_send.front().size/128); // Will be 1, 4, or 8
				for(i = 0; i < num; i++) M1.occupied[slot + i] = 1;
				// Fill in this tag's line
				M1.lines[D3_send.front().tag].address = slot;
				num == 1 ? num = 1 : (num == 4 ? num = 2 : num = 3); // Translate to 2-bit size representation
				M1.lines[D3_send.front().tag].size = num;
				M1.lines[D3_send.front().tag].last_transaction = M1.counter;
			}
			// Else there's no room. Move something to storage.
			// But first check if there's something in the storage queue because its memory won't be marked 
			// clear until it's finished moving to storage, meaning you'll try to send the same thing to
			// storage multiple times.

			// Eviction policy evicts the *largest* available data unit, choosing the earliest arrival.
			// That is, the policy is FIFO but only for 1024-sized blocks.
			else if(Storage_send.empty()){
				i = 1;
				while(i < 32){
					// Is this tag for 1024B data in memory?
					if((M1.lines[i].size == 3) && (M1.lines[i].address < 81)) {
						// Put a new transaction on the storage_send queue
						Transaction *temp = new Transaction;
						temp->initialize(current_time,4,1,1024,i);
						Storage_send.push(*temp);
						i = 32;
					}
					i++;
				}
			}
		}

		// Check if Device 3 REQUEST is ready to start
		if( !D3_req.empty() && (D3_req.front().start == 0) && (current_time >= D3_req.front().arrival_time) ){
			slot = M1.lines[D3_req.front().tag].address; // Starting point
			if( slot > 0){ // Data is here
				// Calculate and store start & finish time
				D3_req.front().start = current_time;
				D3_req.front().finish = current_time + REQUEST_TIME(D3_req.front().size);
				cout << "Device 3 begins receiving tag " << D3_req.front().tag;
				cout << " at clock time " << current_time << endl;
			// Update info in LUT
				// Update transaction counter
				M1.counter++;
				M1.lines[D3_req.front().tag].last_transaction = M1.counter;
			}
			else ;// Data hasn't arrived yet. Do nothing.
		}





		/*
		 *
		 *		CHECK IF ANY TRANSACTIONS FINISH NOW
		 *
		 */


		// Check if a Device 1 SEND has finished
		if( !D1_send.empty() && (D1_send.front().start != 0) && (current_time == D1_send.front().finish) ){
			cout << "Device 1 finished sending tag " << D1_send.front().tag;
			cout << " at clock time " << current_time;
			// Add this transaction to the cumulative latency
			num = D1_send.front().finish - D1_send.front().arrival_time;
			cout<< " for a total latency of " << num << " cycles." << endl;
			cumulative_latency += num;
			// Remove this transaction from queue
			D1_send.pop();
		}


		// Check if a Device 1 REQUEST has finished
		if( !D1_req.empty() && (D1_req.front().start != 0) && (current_time == D1_req.front().finish) ){
			cout << "Device 1 finished receiving tag " << D1_req.front().tag;
			cout << " at clock time " << current_time;
			// Add this transaction to the cumulative latency
			num = D1_req.front().finish - D1_req.front().arrival_time;
			cout<< " for a total latency of " << num << " cycles." << endl;
			cumulative_latency += num;
			// Remove this transaction from the queue
			D1_req.pop(); 
		}




		// Check if a Device 2 SEND has finished
		if( !D2_send.empty() && (D2_send.front().start != 0) && (current_time == D2_send.front().finish) ){
			cout << "Device 2 finished sending tag " << D2_send.front().tag;
			cout << " at clock time " << current_time;
			// Add this transaction to the cumulative latency
			num = D2_send.front().finish - D2_send.front().arrival_time;
			cout<< " for a total latency of " << num << " cycles." << endl;
			cumulative_latency += num;
			// Remove this transaction from queue
			D2_send.pop();
		}

		// Check if a Device 2 REQUEST has finished
		if( !D2_req.empty() && (D2_req.front().start != 0) && (current_time == D2_req.front().finish) ){
			cout << "Device 2 finished receiving tag " << D2_req.front().tag;
			cout << " at clock time " << current_time;
			// Add this transaction to the cumulative latency
			num = D2_req.front().finish - D2_req.front().arrival_time;
			cout<< " for a total latency of " << num << " cycles." << endl;
			cumulative_latency += num;
			// Remove this transaction from the queue
			D2_req.pop(); 
		}


		// Check if a Device 3 SEND has finished
		if( !D3_send.empty() && (D3_send.front().start != 0) && (current_time == D3_send.front().finish) ){
			cout << "Device 3 finished sending tag " << D3_send.front().tag;
			cout << " at clock time " << current_time;
			// Add this transaction to the cumulative latency
			num = D3_send.front().finish - D3_send.front().arrival_time;
			cout<< " for a total latency of " << num << " cycles." << endl;
			cumulative_latency += num;
			// Remove this transaction from queue
			D3_send.pop();
		}

		// Check if a Device 3 REQUEST has finished
		if( !D3_req.empty() && (D3_req.front().start != 0) && (current_time == D3_req.front().finish) ){
			cout << "Device 3 finished receiving tag " << D3_req.front().tag;
			cout << " at clock time " << current_time;
			// Add this transaction to the cumulative latency
			num = D3_req.front().finish - D3_req.front().arrival_time;
			cout<< " for a total latency of " << num << " cycles." << endl;
			cumulative_latency += num;
			// Remove this transaction from the queue
			D3_req.pop(); 
		}





		/*
		 *
		 *		STORAGE TRANSACTIONS
		 *
		 */


		// Check if Storage_send.front() has started
		if( !Storage_send.empty() && (Storage_send.front().start == 0)){
			// Begin the transaction
			Storage_send.front().start = current_time;
			Storage_send.front().finish = current_time + STORAGE_TIME(1024);
			cout << " Hub begins sending tag " << Storage_send.front().tag;
			cout << " into storage at clock time " << current_time << endl;
		}


		// Check if Storage transaction is complete
		if( !Storage_send.empty() && (current_time == Storage_send.front().finish)) {
			// "Free" the slots in memory and change the info of this tag line
			for(i = 0; i < 8; i++) M1.occupied[M1.lines[Storage_send.front().tag].address + i] = 0;
			M1.lines[Storage_send.front().tag].address = 81;
			cout << " Hub completes sending tag " << Storage_send.front().tag;
			cout << " into storage at clock time " << current_time << endl;
			communication_count += STORAGE_TIME(1024);
			Storage_send.pop();
		}


		// If all transactions are complete, exit this loop.
		if( D1_send.empty() && 
			D1_req.empty() && 
			D2_send.empty() && 
			D2_req.empty() && 
			D3_send.empty() &&
			D3_req.empty() &&
			Storage_send.empty() &&
			Storage_req.empty()
			) break;

		current_time++;

		// Jump ahead if there's nothing but the storage transaction running
		// This is just to skip ahead, because the storage takes hundreds of millions
		// of cycles and will crash the computer.
		if((!Storage_send.empty()) && 
			(Storage_send.front().finish > 0) &&
			((Storage_send.front().finish - current_time) > 100)){
			// See if any of the devices have transactions going
			if( !D1_send.empty() && (D1_send.front().start != 0)) num = 1; // D1 Running
			else if( !D1_req.empty() && (D1_req.front().start != 0)) num = 1; // D1 Running
			else if( !D2_send.empty() && (D2_send.front().start != 0)) num = 1; // D2 Running
			else if( !D2_req.empty() && (D2_req.front().start != 0)) num = 1; // D2 Running
			else if( !D3_send.empty() && (D3_send.front().start != 0)) num = 1; // D3 Running
			else if( !D3_req.empty() && (D3_req.front().start != 0)) num = 1; // D3 Running
			else current_time = Storage_send.front().finish - 10; // None running, jump ahead to finish time.
		}
	}

	// Report numbers and end program
	cout << "TRANSACTIONS COMPLETE" << endl << endl << endl;
	cout << "BENCHMARK MEASURES " << endl;
	cout << "The unit cost of the system: " << "80" << endl;
	cout << "The cumulative latency: " << cumulative_latency << endl;
	cout << "The total satellite communcation cycles: " << communication_count << endl;
	float cost = (communication_count)/(60*pow(10, 8));
	cout << "Total communication minutes: $" << cost << endl;

	// Get the number of the traffic file
	string filename = argv[1];
	filename = filename.substr(0, filename.size()-4);
	char c = filename.back();

	// Send benchmarks to file
	ofstream benchmarks;
	benchmarks.open ("benchmarks.csv", ios::app);
	benchmarks << "COST, LATENCY, COMMUNICATION COST, For Traffic File " << c << endl;
	benchmarks << "$80, " << cumulative_latency << ", " << "$" << cost << endl;
	benchmarks.close(); 

	return 1;
}