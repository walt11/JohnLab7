/*
#########################
Name:		John Walter
Date:		10/12/16
Class:		ECE 3220
Assignment:	Lab 7
#########################

In order to use the graphing ability, python 2.7.6 and matplotlib and TK must be installed
on your machine. Python 2.7.6 is usually the default python install within linux.
To install matplotlib within a linux machine, run the command:

	sudo apt-get install python-matplotlib

To install TK, run:

	sudo apt-get install python-tk

For other operating systems, trying googling it. 
*/


#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
using namespace std;

// For graphing, set this to 1 (see comment above)
// Otherwise set to 0

int set_graphing = 1;

// These were originally defined in main but I moved them to be global variables
// in order to make it easier for graphing
int flag = 0;
int &graph_flag = flag;
int filenumber = 0;
int &graph_filenum = filenumber;


class Signal{
	private:
		int max;
		double average;
		double *data = NULL;
		int length;
		double *array = NULL;
		int check;
	public:
		// open file, read data into private variables, close file
		Signal();
		Signal(int); //receives file number
		Signal(const char *); // receives filename
		~Signal();
		void offset(double);
		void scale(double);
		void center();
		void normalize();
		void statistics();
		void exit();
		int check_file_success();
		//void free_pointers();
		void sig_info();
		void save_file(int,int);
		void print_signal(double *);
		void mean();
		void deletePointers();
		//void graph();

};

void Signal::deletePointers(){
	if(array != NULL){
		cout << "Deleting array\n";
		delete[] array;
	}
	if(data != NULL){
		cout << "Deleting data\n";
		delete[] data;
	}
}

int Signal::check_file_success(){
	//cout << data << endl;
	if(data == NULL){
		return -1;
	}else{
		return 0;
	}
}

void Signal::save_file(int flag, int num){
	char filename[20];
	if(num == 0){
		strcpy(filename,"graph.txt");
	}else if(flag == 0){
		cout << "No transformations have been done." << endl;
		return;
	}else if(flag == 1){
		if(num<9){ // includes a zero in the name
			sprintf(filename,"Offset_data_0%d.txt",num);
		}else{ //doesn't include a zero in the name
			sprintf(filename,"Offset_data_%d.txt",num);
		}
	}else if(flag == 2){
		if(num<9){ // includes a zero in the name
			sprintf(filename,"Scale_data_0%d.txt",num);
		}else{ //doesn't include a zero in the name
			sprintf(filename,"Scale_data_%d.txt",num);
		}
	}else if(flag == 3){
		if(num<9){ // includes a zero in the name
			sprintf(filename,"Center_data_0%d.txt",num);
		}else{ //doesn't include a zero in the name
			sprintf(filename,"Center_data_%d.txt",num);
		}
	}else{
		if(num<9){ // includes a zero in the name
			sprintf(filename,"Normalize_data_0%d.txt",num);
		}else{ //doesn't include a zero in the name
			sprintf(filename,"Normalize_data_%d.txt",num);
		}
	}
	//cout << "Writing new signal to file." << endl;
	int i;
	
	FILE *fp = fopen(filename,"w");
	if(fp == NULL){
		printf("ERROR opening file\n");
	}else{
		fprintf(fp,"%d %d\n",length,max);
		for(i=0;i<length;i++){
			fprintf(fp,"%.4f\n",array[i]);
		}
		fclose(fp);
	}
}

void Signal::sig_info(){
	cout << "Signal Info\n\tMax: ";
	cout << max << endl;
	cout << "\n\tLength: ";
	cout << length << endl;
	cout << "\n\tAverage: ";
	cout << average << endl;
}

void Signal::print_signal(double *x){
	//cout << "Inside print" <<endl;
	int i;
	for(i=0;i<length;i++){
		cout << x[i];
		cout << " ";
	}
}


// This function generates a file containing the mean and max of each raw file
void Signal::statistics(){
	char file_name[] = "Statistics_File.txt";

	FILE * fp = fopen(file_name,"w");
	fprintf(fp,"%.4f %d",average,max);
	cout << "Mean and max written to file" << endl;
	cout << "The mean is: ";
	cout << average << endl;
	cout << "The max is: ";
	cout << max << endl; 
	fclose(fp);
	
}
void graph(){
	char filename[20];

	if(graph_filenum<9){ // includes a zero in the name
		sprintf(filename,"Raw_data_0%d.txt",graph_filenum);
	}else{ //doesn't include a zero in the name
		sprintf(filename,"Raw_data_%d.txt",graph_filenum);
	}

	
	FILE *f = fopen("names.txt","w");
	fprintf(f,"%s\n",filename);
	fprintf(f,"%s\n","graph.txt");
	fprintf(f,"%d",graph_flag);
	fclose(f);
	system("python graph.py");
}

void Signal::mean(){
	double total = 0.0;
	int i=0;
	for(i=0;i<length;i++){
		total = total+data[i];
	}
	average = total/(double)length; //length is type int
}

// This function offsets the signal based on an offset value inputted
void Signal::offset(double offset_value){
	cout << "Offset started" << endl;
	int i;
	// assures that memory is not malloced more than once
	if(array == NULL){
		cout << "Malloced for array\n";
		array = new double [length];
	}
	for(i=0;i<length;i++){
		array[i] = data[i] + offset_value;
	}
	cout << "The signal now is: ";
	print_signal(array);
	
	// if graphing is enabled 
	if(set_graphing == 1){
		cout << "\nGraphing...\n";
		save_file(0,0);
		graph();
	}
}

//This function scales the signal based on a scale factor inputted
void Signal::scale(double scale_factor){
	cout << "Scale started" << endl;
	// assures that memory is not malloced more than once 
	if(array == NULL){
		cout << "Malloced for array\n";
		array = new double [length];
	}
	int i;
	//printf("%f\n",scale_factor);
	for(i=0;i<length;i++){
		array[i] = data[i] * scale_factor;
	}
	cout << "The signal now is: ";
	print_signal(array);

	// if graphing is enabled
	if(set_graphing == 1){
		cout << "\nGraphing...\n";
		save_file(0,0);
		graph();
	}
}


//This function centers the signal 
void Signal::center(){
	// Since centering is the same as offsetting the signal by a factor
	// of -mean, the offset can be used with an offset value of -mean
	cout << "Centering data" << endl;
	offset(-average);
}

// This function normalizes the singal
void Signal::normalize(){
	// Since normalizing is the same as scaling the signal by a factor
	// of 1/max, the scaleSignal() function can be used with a scale value of 1/max
	cout << "Normalizing data" << endl;
	scale((1/(double)max));
}



Signal::Signal(int file){
	double cell;
	int i;
	char file_name[20];
	int check = 0;
	if(file<9){
		sprintf(file_name,"Raw_data_0%d.txt",file);
	}else{
		sprintf(file_name,"Raw_data_%d.txt",file);
	}
	FILE *fp = fopen(file_name,"r");
	if (fp == NULL){
		return;
	}
	fscanf(fp,"%d %d",&length,&max);
	data = new double [length];
	for(i=0;i<length;i++){
		fscanf(fp,"%lf",&cell);
		data[i] = cell;
	}
	cout << "File read.\n";
	cout << "The signal from file is: ";
	print_signal(data);
	fclose(fp);
}

Signal::Signal(){
	max = 0;
	data = NULL;
	length = 0;
	average = 0;
	array = NULL;
}

Signal::Signal(const char * file_name){
	double cell;
	int i;
	FILE *fp = fopen(file_name,"r");
	if (fp == NULL){
		return;
	}
	fscanf(fp,"%d %d",&length,&max);
	data = new double [length];
	for(i=0;i<length;i++){
		fscanf(fp,"%lf",&cell);
		data[i] = cell;
	}
	cout << "File read.\n";
	cout << "The signal from file is: ";
	print_signal(data);
	fclose(fp);
}

Signal::~Signal(){
	/* Because of the way I implemented the while loop in main, the malloced memory
	cannot be freed until the termination of the program, and therefore I wrote a seperate method to
	delete the arrays
	*/
}



int main(int argc, char *argv[]){
	char filename[20];
	//int filenumber = 0;
	int choice, i;
	//int flag = 0;
	
	if(argc == 1){
		cout << "No arguments given" << endl;
		cout << "Enter filename> " << endl;
		cin >> filename;
	}
	else if(argc == 3){
		if(strcmp(argv[1],"-n") == 0){
			//cout << "File number given" << endl;
			if(isdigit(*argv[2])){
				filenumber = atoi(*(argv+2));
			}else{
				cout << "ERROR: '-n' expects a number not a string\nExiting.\n";
				return -1;
			}
			//cout << filenumber;
		}
		else if(strcmp(argv[1],"-f") == 0){
			cout << "File name given" << endl;

			if(isdigit(*argv[2])){
				cout << "ERROR: '-f' expects a string not a number\nExiting.\n";
				return -1;
			}else{
				strcpy(filename,argv[2]);	
			}
		}
	}else{
		cout << "ERROR with arguments\nExpecting:\n\t'./a.out -n filenumber'\n\t'./a.out -f filename.txt'\n" << endl;
		return -1;
	}


	Signal sig1;
	if(filenumber != 0){
		sig1 = Signal(filenumber);
		if(sig1.check_file_success() == -1){
			cout << "File given was not found.\nExiting.\n";
			return -1;
		}

	}else{
		sig1 = Signal(filename);
		if(sig1.check_file_success() == -1){
			cout << "File given was not found.\nExiting.\n";
			return -1;
		}
	}

	//get file number if filename is given
	if(filenumber == 0){
		char *split;
		split = strtok (filename,"_.");
    	split = strtok (NULL, "_.");
    	split = strtok(NULL, "_.");
    	filenumber = atoi(split);
    }
    //cout << filenumber << endl;
	sig1.mean();
	while(1){
		cout << "\nSelect:\n\t(0) Get Info\n\t(1) Offset\n\t(2) Scale\n\t(3) Center\n\t(4) Normalize\n\t(5) Statistic\n\t(6) Save To File\n\t(7) Exit\n> ";
		cin >> choice;
		while(choice > 8 | choice < 0){
			cout <<"Invalid. Enter again: ";
			cin >> choice;
		}
		switch(choice){
			case 0:{
				sig1.sig_info();
				break;}
			case 1: {
				flag = 1;
				double offset_num = 0.0;
				cout << "Enter offset value: ";
				cin >> offset_num;
				cout << "Calling offset" << endl;
				sig1.offset(offset_num);
				break;}
			case 2: {
				flag = 2;
				double scale_num = 0.0;
				cout << "Enter a scale factor: ";
				cin >> scale_num;
				sig1.scale(scale_num);
				break;}
			case 3:{
				flag = 3;
				sig1.center();
				break;}
			case 4:{
				flag = 4;
				sig1.normalize();
				break;}
			case 5:{
				sig1.statistics();
				break;}
			case 6:{
				sig1.save_file(flag,filenumber);
				break;}
			case 7:{
				sig1.deletePointers();
				return 0;
				break;}
		}	
	}
}
