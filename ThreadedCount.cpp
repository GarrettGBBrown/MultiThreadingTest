/**
 * @author Garrett Brown
 * Date: 3/22/24
 *
 */
#include <iomanip>
#include <iostream>       // std::cout
#include <thread>         // used for std::thread
#include <ctime>    
#include <chrono>  // measure the tile via using some functions included here
#include <vector>
#include <fstream>
#include <omp.h>  //openMP
#include <mutex>

using namespace std;
using namespace std::chrono;

	// class to contain all data about customer bank accounts
	class Customer{
	private:
		string id;
		double balance;
	public:
		Customer() {
			id = " ";
			balance = -1;
		}
		Customer(string newId, double newBalance){
			id = newId;
			balance = newBalance;
		}
		// getters, setters
		string getID(){return id; }
		void setId(string nId){id = nId;}
		double getBalance() {return balance;}
		void setBalance(double nBalance) {balance = nBalance;}
		
	};
// function declaration for counting below $1000 as balance
int countBelow(int from, int to);
// list of all customers for multithreading
vector<Customer> customers;
int counter;
mutex mx;

	
	// reads from input file, example accounts.txt
	vector<Customer> readFromFile(){
		ifstream input;

		input.open("accounts.txt");

		vector<Customer> inputVector = vector<Customer>();

		// initialize variables
		string tempId = "";
		double tempBalance = 0;

		// while input is not at end of file
		while(!input.eof()){
			input >> tempId;
			input >> tempBalance;
			inputVector.push_back(Customer(tempId,tempBalance));

			// shows that file is still running
			if (inputVector.size() % 30000 == 0) {
				cout << ">>";
			}
			if (inputVector.size() % 1000000 == 0) {
				cout << "\n";
			}		
		}
		cout << endl;
		input.close();
		return inputVector;
	}

	// finds # of customers with a balance below 1000
	int countBelow(int from, int to){
		int localCounter = 0 ;
		for (int i = 0; i < to; ++i) {
			if (customers[i].getBalance() < 1000)
				localCounter++;
		}
		mx.lock(); // locks value to avoid race condition
		counter = localCounter; 
		mx.unlock(); // unlocks value to avoid race condition
	return localCounter; // not needed but void had issues in linux
	
	}

int main (){
//initialize counter
counter = 0;
customers = readFromFile();
// number of accounts
cout <<"the Accounts Size is: " << customers.size();

// divides customer list into 4 cores for multithreading
int Dim = customers.size();
int workload = Dim / 4;
int dim1 = workload;
int dim2 = workload * 2;
int dim3 = workload * 3;
int dim4 = Dim -1;

auto start = high_resolution_clock::now(); // starts runtime clock
// runs different threads
thread th1(countBelow, 0, dim1);
thread th2(countBelow, (dim1 + 1), dim2);
thread th3(countBelow, (dim2 + 1), dim3);
thread th4(countBelow, (dim3 + 1), dim4);

auto end = high_resolution_clock::now(); // ends runtime clock
th1.join(); th2.join(); th3.join(); th4.join();

auto duration = duration_cast<milliseconds>(end - start);

// outputs findings to terminal
cout << "\n This prossess took: "<< duration.count();
cout << "\n Counted: " << counter<< endl;
return 0;

}
