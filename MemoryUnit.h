
#include <cctype>
#include <iostream>
#include <fstream>
#include <queue>
#include <stdio.h>
#include <sstream>
#include <string>
#include <cstdlib>
#include <cstdint>
#include <cmath>

// 2B field located in the M1 memory unit which corresponds to each Tag from 1 to 32
// For example, Tag 1's line is located at byte location 1, Tag 2 at byte location 3,
// and Tag x at byte location 2x-1.
struct Line{
	unsigned int address : 7;			// Starting address in memory where the corresponding
										// data is located. 0 means this data doesn't exist.
										// 1-80 are locations in M3 units, 81 is storage.
	unsigned int size : 2; 				// 1=128B, 2=512B, 3=1024B
	unsigned int last_transaction : 7; // Most recent transaction

	//Constructor to initialize to zero
	Line(){
		address = 0;
		size = 0;
		last_transaction = 0;
	}
};


// Table consisting of one Line corresponding to each data Tag
struct LUT{
	unsigned int counter : 7; 	// Increments with each new transaction
	Line lines[32];				// One line for each tag
	bool occupied[80];			// Records which data slots are empty (0) or occupied (1)

	// Constructor initializes to zero
	LUT(){
		counter = 0x00;
		for(int i = 0 ; i < 80 ; i++) occupied[i] = 0;
	}
	
};

// Transaction Info
struct Transaction{
	uint64_t arrival_time;
	int device;
	int operation;
	int size;
	int tag;
	uint64_t start;
	//__int128 finish;
	uint64_t finish;

	// Constructor for creating a Transaction
	void initialize(int a, int d, int o, int s, int t){
		arrival_time = a;
		device = d;
		operation = o;
		size = s;
		tag = t;
		start = 0;
		finish = 0;
	}
};