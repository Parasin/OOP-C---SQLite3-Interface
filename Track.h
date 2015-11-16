#ifndef TRACK_H
#define TRACK_H
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

class Track {
	private:
		int cd_id;
		int track_id;
		string title;
		
	public:
		Track(){}
		Track(const Track &obj) {
			cd_id     = obj.cd_id;
			track_id  = obj.track_id;
			title     = obj.title;
		}
		Track(const int cd_id, const int track_id, const string title) {
			this->cd_id    = cd_id;
			this->track_id = track_id;
			this->title    = title;
		}
		Track(const string *data[]) {
			cd_id    = atoi(data[0]->c_str());
			track_id = atoi(data[1]->c_str());
			title    = data[1]->c_str();
		}
		~Track(){}
		
		/* Mutators */
		void setId(const int cd_id) {this->cd_id = cd_id;}
		void setArtistId(const int track_id) {this->track_id = track_id;}
		void setTitle(const string title) {this->title = title;}
		
		/* Accessors */
		int getId () const {return cd_id;}
		int getTrackId () const {return track_id;}
		string getTitle () const {return title;}
		
		/* Prototypes */
		bool ExecuteQuery(const string);
		bool Insert(const string []);
		bool Update(const string []);
		bool Delete();
		bool CreateTable();
};
#endif
