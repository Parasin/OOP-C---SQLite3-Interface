#include "Track.h"

bool Track::ExecuteQuery(const string sqlStr) {
	if (sqlite3_prepare(dbfile, sqlStr.c_str(), -1, &stmt, 0) == SQLITE_OK) {
		sqlite3_step(stmt);
		sqlite3_finalize(stmt);
	}
	else {
		return false;
	}
	return true;
}

bool Track::Insert(const string data[]) {
	stringstream queryStrm;
	queryStrm << "insert into track(cd_id, track_id, title) values(" << data[0] << ", '" << data[1] << "', '" << data[2] <<"');";
	string sqlStr = queryStrm.str();
	return ExecuteQuery(sqlStr);
}

bool Track::Update(const string data[]) {
	stringstream queryStrm;
	queryStrm << "UPDATE track SET " << data[1] << "='" << data[2] << "' WHERE " << data[3] << "='" << data[4] << "';";
	string sqlStr = queryStrm.str();
	return ExecuteQuery(sqlStr);
}

bool Track::Delete() {
	stringstream queryStrm;
	queryStrm << "DELETE FROM track;";
	string sqlStr = queryStrm.str();
	return ExecuteQuery(sqlStr);
}

bool Track::CreateTable() {
	string sqlStr = "create table track ("\
	"cd_id INTEGER NOT NULL,"\
	"track_id INTEGER NOT NULL,"\
	"title VARCHAR(70),"\
	"PRIMARY KEY(cd_id, track_id));";
	return ExecuteQuery(sqlStr);
}