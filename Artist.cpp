#include "Artist.h"
bool Artist::ExecuteQuery(const string sqlStr) {
	if (sqlite3_prepare(dbfile, sqlStr.c_str(), -1, &stmt, 0) == SQLITE_OK) {
		sqlite3_step(stmt);
		sqlite3_finalize(stmt);
	}
	else {
		return false;
	}
	return true;
}

bool Artist::Insert(string data[]) {
	stringstream queryStrm;
	queryStrm << "insert into artist(id, name) values(" << data[0] << ", '" << data[1] << "');";
	string sqlStr = queryStrm.str();
	return ExecuteQuery(sqlStr);
}

bool Artist::Update(string data[]) {
	stringstream queryStrm;
	queryStrm << "UPDATE artist SET " << data[1] << "='" << data[2] << "' WHERE " << data[3] << "='" << data[4] << "';";
	string sqlStr = queryStrm.str();
	return ExecuteQuery(sqlStr);
}

bool Artist::Delete() {
	stringstream queryStrm;
	queryStrm << "DELETE FROM artist;";
	string sqlStr = queryStrm.str();
	return ExecuteQuery(sqlStr);
}

bool Artist::CreateTable() {
	string sqlStr = "create table artist (" \
		"id INTEGER AUTO_INCREMENT NOT NULL PRIMARY KEY," \
		"name VARCHAR(70) NOT NULL);";
	return ExecuteQuery(sqlStr);
}