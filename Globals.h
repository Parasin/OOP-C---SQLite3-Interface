#ifndef GLOBALS_H
#define GLOABLS_H

/* Global variables */
/* sqlite database pointer */
sqlite3 *dbfile;
sqlite3_stmt *stmt;
bool isOpen = false;
#endif