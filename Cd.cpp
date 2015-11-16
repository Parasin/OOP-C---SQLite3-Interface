#include "Cd.h"
#include"Globals.h"
bool Cd::ExecuteQuery(string sqlStr) {
	if (sqlite3_prepare(dbfile, sqlStr.c_str(), -1, &stmt, 0) == SQLITE_OK) {
		sqlite3_step(stmt);
		sqlite3_finalize(stmt);
	}
	else {
		return false;
	}
	return true;
}

bool Cd::Insert(string data[]) {
	stringstream queryStrm;
	queryStrm << "insert into cd(id, title, artist_id, catalogue) values(" << data[0] << ", '" << data[1] << "', " << data[2] << ", '" << data[3] << "');";
	string sqlStr = queryStrm.str();
	return ExecuteQuery(sqlStr);
}

bool Cd::Update(string data[]) {
	stringstream queryStrm;
	queryStrm << "UPDATE cd SET " << data[1] << "='" << data[2] << "' WHERE " << data[3] << "='" << data[4] << "';";
	string sqlStr = queryStrm.str();
	return ExecuteQuery(sqlStr);
}

bool Cd::Delete() {
	stringstream queryStrm;
	queryStrm << "DELETE FROM cd;";
	string sqlStr = queryStrm.str();
	return ExecuteQuery(sqlStr);
}

bool Cd::CreateTable() {
	string sqlStr = "create table cd (" \
		"id INTEGER AUTO_INCREMENT NOT NULL PRIMARY KEY," \
		"title VARCHAR(70) NOT NULL," \
		"artist_id INTEGER NOT NULL," \
		"catalogue VARCHAR(30) NOT NULL);";
	return ExecuteQuery(sqlStr);
}