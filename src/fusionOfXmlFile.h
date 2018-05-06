/*
 * fusionOfXmlFile.h
 *
 *  Created on: 17 févr. 2018
 *      Author: louis
 */

#ifndef FUSIONOFXMLFILE_H_
#define FUSIONOFXMLFILE_H_

#include "library.h"

#if defined(_WIN32)
#define XML_FILENAME_START "Passages\\"
#define FINAL_FILENAME_START "Archives\\"
#else
#define XML_FILENAME_START "Passages/"
#define FINAL_FILENAME_START "Archives/"
#define FINAL_FILENAME
#endif

#define ID "id"
#define EMAIL "email"
#define CHECK "check"
#define TIME "time"
#define FILENAME_END ".xml"

typedef struct User{
	char * id;
	char * time;
	char * statut;
}User;

void generateDaylyXmlFile();
void destroyFile();
void loadXmlFile(char *);
void createFinalXmlFile();
void checkProfileDirectoryExistFinal();
void checkProfileDirectoryExistSource();
void freeUser(User *);
void addUser(User *);
void addChild(xmlNodePtr, xmlNodePtr);
void addProp(User *, xmlNodePtr);
void mainLoop();
char * generateFileName();

User * initUser();
xmlNodePtr addNode();
xmlDocPtr openXmlFile(char *);

#endif /* FUSIONOFXMLFILE_H_ */
