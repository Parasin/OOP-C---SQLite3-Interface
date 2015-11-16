#include "Cd.cpp"
#include "Artist.cpp"
#include "Track.cpp"
#include <vector>
#define DB "song.db"

/* Prototypes */
bool ConnectDB();
bool DisconnectDB();
bool OpenFileIn(fstream &, string);
bool OpenFileOut (fstream &, string);
void Menu();
void DisplayTables();
void DisplayMenuOptions();
void ParseCds(fstream &);
void ParseArtists(fstream &);
void ParseTracks(fstream &);
bool Select(string []);
static int callback(void *, int , char **, char **);
static int callbackFileOut(void *, int, char **, char **);
Cd * CdFactory();
Cd * CdFactory(const string data[]);
Artist * ArtistFactory();
Artist * ArtistFactory(const string data[]);
Track * TrackFactory();
Track * TrackFactory(const string data[]);

/* Global Variables */
fstream file;
const string fileName[4] = {"artist.txt", "cd.txt", "track.txt"};
vector<Cd*> cdVector;
vector<Artist*> artistVector;
vector<Track*> trackVector;

int main() {
	cdVector.push_back(CdFactory());
	artistVector.push_back(ArtistFactory());
	trackVector.push_back(TrackFactory());
	
	// Connect to database
	if (ConnectDB()) {
		cout << "Connected to database successfully!" << endl << endl;
		
		// Create all tables that are not already present
		cout << "Attempting to create tables..." << endl;
		bool flag = false;
		if (cdVector[0]->CreateTable() && artistVector[0]->CreateTable() && trackVector[0]->CreateTable()) {
			cout << "Successfully created all tables!" << endl << endl;
			flag = true;
		}
		else {
			cout << "Tables already present, skipping table creation and automatic population" << endl << endl;
		}
		
		// If the tables did not exist prior to the program running, populate them now.
		// If the tables were already present, they do not need to be populated with default data.
		if (flag) {
			// Read each file and do the required operations on each one
			for (int i = 0; i < 3; i++) {
				// Start by reading the file
				if (OpenFileIn(file, fileName[i])) {
					cout << "File:" << fileName[i] << " opened successfully!" << endl;
					cout << "Reading data from file..." << endl;
					
					if (i == 0) {
						ParseArtists(file);
					}
					else if (i == 1) {
						ParseCds(file);
					}
					else {
						ParseTracks(file);
					}
					file.close();
					cout << "Done Reading file: " << fileName[i] << endl << endl;
				}
				else {
					cout << "Could not open file: " << fileName[i] << endl << endl;
				}
			}
		}
		
		// Display the menu for user interaction
		Menu();
	}
	else {
		cout << "Couldn't connect to database. " << endl << endl;
		exit(EXIT_FAILURE);
	}

	// Clean-up pointers from vectors
	for (int i = 0; i < cdVector.size(); i++) {
		delete cdVector[i];
		cout << "Removed " << i + 1 << " elements from cdVector, deleted pointer." << endl;
	}
	return 0;
}

/* Create an instance of Cd with the
 * default constructor, and return its' pointer */
Cd * CdFactory() {
	Cd *ptr = new Cd();
	return ptr;
}

/* Create an instance of Cd with the
 * parameter constructor, and return its' pointer */
Cd * CdFactory(const string data[]) {
	Cd *ptr = new Cd(atoi(data[0].c_str()), atoi(data[1].c_str()), data[2], data[3]);
	return ptr;
}

/* Create an instance of Cd with the
 * default constructor, and return its' pointer */
Artist * ArtistFactory() {
	Artist *ptr = new Artist();
	return ptr;
}

/* Create an instance of Cd with the
 * parameter constructor, and return its' pointer */
Artist * ArtistFactory(const string data[]) {
	Artist *ptr = new Artist(atoi(data[0].c_str()), data[1]);
	return ptr;
}

/* Create an instance of Cd with the
 * default constructor, and return its' pointer */
Track * TrackFactory() {
	Track *ptr = new Track();
	return ptr;
}

/* Create an instance of Cd with the
 * parameter constructor, and return its' pointer */
Track * TrackFactory(const string data[]) {
	Track *ptr = new Track(atoi(data[0].c_str()), atoi(data[1].c_str()), data[2]);
	return ptr;
}

/* Attempt to connect to the database. If successful,
returns true. Else, false is returned*/
bool ConnectDB () {
	if (sqlite3_open_v2(DB, &dbfile, SQLITE_OPEN_READWRITE, NULL ) == SQLITE_OK) {
		isOpen = true;
		return true;
	}
	return false;
}

/* If the connection to the database is open, it will
be closed.*/
bool DisconnectDB () {
	if (isOpen) {
		if (sqlite3_close(dbfile) == SQLITE_OK) {
			isOpen = false;
			return true;
		}
		else {
			return false;
		}
	}
	else {
		cout << "Cannot disconnect; program not connected to a database!" << endl;
	}
}

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

/* Display the menu for the user to interact with.
Options are: 1 = Update a row in the database
			 2 = Exit the program
Program will continue execution until user enters 2.
Only  options are to Update and Exit */
void Menu() {
	string data[6];
	string tableVal;
	int tableNum;
	string input;
	while (true) {	
		cout << "Select table to operate on:" << endl;
		
		cin.clear();
		DisplayTables();
		getline(cin, tableVal);
		tableNum = atoi(tableVal.c_str());
		
		if (tableNum == 4) {
			cout << "Disconnecting from the database..." << endl;
			if (DisconnectDB()) {
				cout << "Successfully disconnected!" << endl;
			}
			else {
				cout << "Error occurred while disconnecting from database." << endl;
			}
			exit(EXIT_SUCCESS);
		}
		cout << "Select operation to perform on table:" << endl;
		DisplayMenuOptions();
		getline(cin, input);
		
		switch(atoi(input.c_str())) {
			// Update a specific record on a table
			case 1:
				cout << endl << "Enter column to update: ";
				getline(cin, data[1]);
				
				cout << endl << "Enter the new value: ";
				getline(cin, data[2]);
				
				cout << endl << "Enter the column to filter on: ";
				getline(cin, data[3]);
				
				cout << endl << "Enter the value of the filter column: ";
				getline(cin, data[4]);
				
				if (tableNum == 1) {
					if(cdVector[0]->Update(data)) {
						cout << "Updated Successfully!" << endl;
					}
					else {
						cout << "Error while updating." << endl;
					}
				}
				else if (tableNum == 2) {
					
				}
				else {
					
				}
				
				break;
			
			// Select data from the relevant table
			case 2:			
				cout << endl << "Enter column to select or * for all: ";
				getline(cin, data[0]);
				
				if (tableNum == 1) {
					data[1] = "cd";
				}
				else if (tableNum == 2) {
					data[1] = "track";
				}
				else {
					data[1] = "artist";
				}
				
				if(Select(data)) {
					cout << "Selected Successfully!" << endl;
					cout << "Saved 'songDbOutput.txt' with query results" << endl;
				}
				else {
					cout << "Error while selecting." << endl;
				}
				break;
			
			// Delete all data from the table
			case 3:
				if (tableNum == 1) {
					if(cdVector[0]->Delete()) {
						cout << "Table Cleared Successfully!" << endl;
					}
					else {
						cout << "Error while clearing table." << endl;
					}
				}
				else if (tableNum == 2) {
					
				}
				else {
					
				}
				break;
		}
		// Reset input variables
		input = "";
		for (int i = 0; i <6; i++) {
			data[i] = "";
		}
	}
}

void DisplayTables() {
	cout << "1) cd\n2) track\n3) artist\n4) Exit Program" << endl;
}

/* Display all the available options to the user */
void DisplayMenuOptions() {
	cout << "1) Update Column\n2) Select and save to file\n3) Delete" << endl;
}

/* Attempt to open the file that is passed.
Will return false if failure while opening,
returns true if opened successfully */
bool OpenFileIn (fstream &file, string filename) {
	file.open(filename.c_str(), ios::in);
	if (file.fail()) {
		return false;
	}
	return true;
}

/* Attempt to open the file that is passed.
Will return false if failure while opening,
returns true if opened successfully */
bool OpenFileOut (fstream &file, string filename) {
	file.open(filename.c_str(), ios::app | ios::out);
	if (file.fail()) {
		return false;
	}
	return true;
}

void ParseCds(fstream &file) {
	string line;
	string delim   = "|";
	int startPos   = 0;
	int endPos     = 0;
	int numSuccess = 0;
	int numError   = 0;
	string cdData[4];

	// Get the first line of the file
	getline(file, line);
	endPos = line.find(delim);
	if (endPos != string::npos) {
		while (file) {
			// Go through each line multiple times to parse out the
			// required information
			for (int i = 0; i < 4; i++) {
				cdData[i] = line.substr(startPos, endPos);
				
				// Sanitize data when i == 1 || i == 2 due to multiple delimiters
				// being present in this section of data
				if (i == 1 || i == 2) {
					int tempStartPos = 0;
					int tempEndPos = 0;
					tempEndPos = cdData[i].find(delim, tempStartPos);
					cdData[i] = cdData[i].substr(tempStartPos, tempEndPos);
				}

				if (endPos == string::npos) {
					endPos = line.length() - 1;
				}
				
				startPos = endPos + 1;
				endPos = line.find(delim, startPos);
			}
			cdVector.push_back(CdFactory(cdData));
		
			// Insert the cd's into the cd table individually
			if (cdVector[0]->Insert(cdData)) {
				numSuccess++;
			}
			else {
				numError++;
			}
			
			// Get the next line and reset pos pointers
			getline(file, line);
			endPos = line.find(delim);
			startPos = 0;
		}
	}
	else {
		cout << "No delimiter was found on the cd file line!" << endl;
	}
	cout << "Successfully inserted " << numSuccess << " cd's!" << endl;
	cout << "Errors occurred while inserting cd's: " << numError << endl;
}

void ParseArtists(fstream &file) {
	string line;
	string delim   = "|";
	int startPos   = 0;
	int endPos     = 0;
	int numSuccess = 0;
	int numError   = 0;
	string artistData[3];

	// Get the first line of the file
	getline(file, line);
	endPos = line.find(delim);
	if (endPos != string::npos) {
		while (file) {
			// Go through each line multiple times to parse out the
			// required information
			artistData[0] = line.substr(startPos, endPos);	
			
			startPos = endPos + 1;
			endPos = line.find(delim, startPos);
			
			artistData[1] = line.substr(startPos, endPos);
			artistVector.push_back(ArtistFactory(artistData));
			
			// Insert the cd's into the cd table individually
			if (artistVector[0]->Insert(artistData)) {
				numSuccess++;
			}
			else {
				numError++;
			}
			
			// Get the next line and reset pos pointers
			getline(file, line);
			endPos = line.find(delim);
			startPos = 0;
		}
	}
	else {
		cout << "No delimiter was found on the cd file line!" << endl;
	}
	cout << "Successfully inserted " << numSuccess << " cd's!" << endl;
	cout << "Errors occurred while inserting cd's: " << numError << endl;
}

/* Parse tracks and their attributes from the cd file, then insert them 
individually into the database.*/
void ParseTracks(fstream &file) {
	string line;
	string delim   = "|";
	int startPos   = 0;
	int endPos     = 0;
	int numSuccess = 0;
	int numError   = 0;
	string trackData[4];

	// Get the first line of the file
	getline(file, line);
	endPos = line.find(delim);
	if (endPos != string::npos) {
		while (file) {
			// Go through each line multiple times to parse out the
			// required information
			for (int i = 0; i < 3; i++) {
				trackData[i] = line.substr(startPos, endPos);
				
				// Sanitize data when i == 1 || i == 2 due to multiple delimiters
				// being present in this section of data
				if (i == 1 || i == 2) {
					int tempStartPos = 0;
					int tempEndPos = 0;
					tempEndPos = trackData[i].find(delim, tempStartPos);
					trackData[i] = trackData[i].substr(tempStartPos, tempEndPos);
				}

				if (endPos == string::npos) {
					endPos = line.length() - 1;
				}
				
				startPos = endPos + 1;
				endPos = line.find(delim, startPos);
			}
			trackVector.push_back(TrackFactory(trackData));
			
			// Insert the track into the track table
			if (trackVector[0]->Insert(trackData)) {
				numSuccess++;
			}
			else {
				numError++;
			}
			// Get the next line and reset pos pointers
			getline(file, line);
			endPos = line.find(delim);
			startPos = 0;
		}
	}
	else {
		cout << "No delimiter found on the track file line!" << endl; 
	}
	cout << "Successfully inserted " << numSuccess << " tracks!" << endl;
	cout << "Errors occurred while inserting tracks: " << numError << endl;
}

bool Select(string data[]) {
	if (isOpen) {
		stringstream queryStrm;
		char *zErrMsg = 0;

		queryStrm << "select " << data[0] << " from " << data[1] << ";";
		string sqlStr = queryStrm.str();
		cout <<  setw(20) << left << "Column";
		cout << "Value" << endl;
		for (int i = 0; i < 41; i++) {
			cout << "-";
		}
		cout << endl;
 		int rc = sqlite3_exec(dbfile, sqlStr.c_str(), callback, 0, &zErrMsg);
		
		// Write select to file
		string filename = "songDbOutput.txt";
		remove(filename.c_str());
		sqlite3_exec(dbfile, sqlStr.c_str(), callbackFileOut, 0, &zErrMsg);
		
		if( rc != SQLITE_OK ){
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			sqlite3_free(zErrMsg);
		}
		else{
			return true;
		} 
	}
	else {
		return  false;
	}
}
