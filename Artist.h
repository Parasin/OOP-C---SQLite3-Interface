#ifndef ARTIST_H
#define ARTIST_H
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>
using namespace std;

class Artist {
	private:
		int id;
		string name;
		
	public:
		Artist(){}
		Artist(const Artist &obj) {
			id   = obj.id;
			name = obj.name;
		}
		Artist(const int id, const string name) {
			this->id   = id;
			this->name = name;
		}
		Artist(const string *data[]) {
			id   = atoi(data[0]->c_str());
			name = data[1]->c_str();
		}
		~Artist(){}
		
		/* Mutators */
		void setArtistId(const int id) {this->id = id;}
		void setName(const string name) {this->name = name;}
		
		/* Accessors */
		int getArtistId () const {return id;}
		string getName () const {return name;}
		
		/* Prototypes */
		bool ExecuteQuery(string);
		bool Insert(string []);
		bool Update(string []);
		bool Delete();
		bool CreateTable();
};
#endif
