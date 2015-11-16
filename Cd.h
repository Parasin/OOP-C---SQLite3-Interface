#ifndef CD_H
#define CD_H
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

class Cd {
	private:
		int id;
		int artist_id;
		string title;
		string catalogue;
		
	public:
		Cd(){}
		Cd(const Cd &obj) {
			id        = obj.id;
			artist_id = obj.artist_id;
			title     = obj.title;
			catalogue = obj.catalogue;
		}
		Cd(const int id, const int artist_id, const string title, const string catalogue) {
			this->id        = id;
			this->artist_id = artist_id;
			this->title     = title;
			this->catalogue = catalogue;
		}
		Cd(const string *data[]) {
			id        = atoi(data[0].c_str());
			artist_id = atoi(data[1].c_str());
			title     = data[2];
			catalogue = data[3];
		}
		~Cd(){}
		
		/* Mutators */
		void setId(const int id) {this->id = id;}
		void setArtistId(const int artist_id) {this->artist_id = artist_id;}
		void setTitle(const string title) {this->title = title;}
		void setCatalogue(const string catalogue) {this->catalogue = catalogue;}
		
		/* Accessors */
		int getId () const {return id;}
		int getArtistId () const {return artist_id;}
		string getTitle () const {return title;}
		string getCatalogue () const {return catalogue;}
		
		/* Prototypes */
		bool ExecuteQuery(string);
		bool Insert(string []);
		bool Update(string []);
		bool Delete();
		bool CreateTable();
};
#endif