#ifndef GLOABLS_CPP
#define GLOBALS_CPP
#include <sqlite3.h>
static int callbackFileOut(void *data, int argc, char **argv, char **azColName);
static int callbackFileOut(void *data, int argc, char **argv, char **azColName);

static int callbackFileOut(void *data, int argc, char **argv, char **azColName){
	fstream fout;
	stringstream sstream;
	string filename = "songDbOutput.txt";
	string str;

	if(OpenFileOut(fout, filename)) {
		for(int i = 0; i < argc; i++){
			sstream << setw(20) << left << azColName[i] << argv[i]  ? argv[i] : "NULL\n";
			str = sstream.str();
			sstream.str(string());
			str += "\n";
			fout << str.c_str();
		}
		fout.close();
	}
	else {
		cout << "Error while opening file for output." << endl;
	}
	
	return 0;
}

static int callback(void *data, int argc, char **argv, char **azColName){
	for(int i = 0; i < argc; i++){
		printf("%-20s  %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	cout << endl;
	return 0;
}
#endif