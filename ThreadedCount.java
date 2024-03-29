/**
 * @author Garrett Brown
 * Date: 3/22/24
 *
 */

import java.io.File;

import java.io.FileNotFoundException;
import java.io.PrintStream;
import java.util.ArrayList;
import java.util.Random;
import java.util.Scanner;
public class ThreadedCount implements Runnable{

	// variables initialized
	private int count = 0;
	private ArrayList<Customer> myList;
	private int start;
	private int end;

	// used to define partial list for multithreading
	public ThreadedCount(ArrayList<Customer> myList, int start, int end) {
		this.myList = myList;
		this.start = start;
		this.end = end;
	}
	// sychronized will lock the value, similar to mutex
	public synchronized int readList(int start, int end){
		int count = 0;
		for (int i = start; i < end; ++i)
			if (myList.get(i).getBalance() < 1000)
				++count;
		return count;
	}
	// reads in the list from file
	public void run(){
		count = readList(start,end);
	}
	// gets count value
	public int getCount(){ return count;}
}
