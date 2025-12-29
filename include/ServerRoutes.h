#ifndef SERVER_ROUTES_H
#define SERVER_ROUTES_H

#include "filemanager/filemanager.h"
#include "httplib/httplib.h"

// This function sets up all the web page routes
void setupRoutes(httplib::Server &server, FileManager &fileManager);

// This function initializes the users file with test data
void initializeFile(FileManager &fileManager);

#endif
