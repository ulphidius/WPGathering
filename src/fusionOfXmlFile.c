/*
 * fusionOfXmlFile.c
 *
 *  Created on: 17 févr. 2018
 *      Author: louis
 */
#include "fusionOfXmlFile.h"

// Génération du fichier journalier
void generateDaylyXmlFile(){
	DIR* rep = NULL; // Création varaible dossier
	struct dirent* file = NULL; /* Déclaration d'un pointeur vers la structure dirent. */
	char * filename = NULL;

	rep = opendir(XML_FILENAME_START);
	// Vérification de l'ouverture
	if (rep == NULL){
		printf("Erreur à l'ouverture du dossier ! \n");
		return;
	}
	// Lecture des fichiers des du dossier
	while ((file = readdir(rep)) != NULL){
		if(!strcmp(file->d_name, ".") || !strcmp(file->d_name, "..")) continue;
		filename = malloc(strlen(XML_FILENAME_START) + strlen(file->d_name) + 1);
		if(filename == NULL){
			printf("Erreur allocation ! \n");
			return;
		}
		// Pour le nom de fichier
		strcpy(filename, XML_FILENAME_START);
		strcat(filename, file->d_name);

		// Ce que doit faire mon programme
		loadXmlFile(filename);
	}

	// Fermeture du dossier
	if (closedir(rep) == -1){
		printf("erreur fermeture du dossier ! \n");
		return;
	}
	free(filename);
	return;
}

// Vider le dossier ou les fichiers transitoires sont
void destroyFile(){
	DIR* rep = NULL; // Création varaible dossier
	struct dirent* file = NULL; /* Déclaration d'un pointeur vers la structure dirent. */
	char * filename = NULL;

	rep = opendir(XML_FILENAME_START);
	// Vérification de l'ouverture
	if (rep == NULL){
		printf("Erreur à l'ouverture du dossier ! \n");
		return;
	}
	// Lecture des fichiers des du dossier
	while ((file = readdir(rep)) != NULL){
		if(!strcmp(file->d_name, ".") || !strcmp(file->d_name, "..")) continue;
		filename = malloc(strlen(XML_FILENAME_START) + strlen(file->d_name) + 1);
		if(filename == NULL){
			printf("Erreur allocation ! \n");
			return;
		}
		// Pour le nom de fichier
		strcpy(filename, XML_FILENAME_START);
		strcat(filename, file->d_name);

		// Suppression de chaque fichier
		remove(filename);
	}

	// Fermeture du dossier
	if (closedir(rep) == -1){
		printf("erreur fermeture du dossier ! \n");
		return;
	}
	free(filename);
	return;
}

// Chargement d'un fichier
void loadXmlFile(char * filename){
	xmlDocPtr doc = NULL;
	xmlNode * root_node = NULL;
	xmlNode * cur_node = NULL;
	User * user = NULL;

	user = initUser();
	// Lecture des deux fichier
	doc = openXmlFile(filename);

	root_node = xmlDocGetRootElement(doc);
	cur_node = root_node;
	cur_node = cur_node->children;

	// Boucle de lecture
	while(cur_node != NULL){
		if(!strcmp((char*)cur_node->name, "User")){
			user->id = (char *)xmlGetProp(cur_node, (xmlChar*)ID);
			user->time = (char *)xmlGetProp(cur_node, (xmlChar*)TIME);
			user->statut = (char *)xmlGetProp(cur_node, (xmlChar*)CHECK);

			addUser(user);
		}
		cur_node = cur_node->next;
	}

	xmlFreeDoc(doc);
	xmlCleanupParser();
	xmlMemoryDump();

	freeUser(user);
	free(user);
}

// Création du fichier journalier
void createFinalXmlFile(){
	xmlDocPtr doc = NULL;
	xmlNodePtr root_node = NULL;
	char * filename = NULL;

	doc = xmlNewDoc(BAD_CAST "1.0");
	if(doc == NULL){
		printf("Erreur création du fichier xml ! \n");
		return;
	}

	root_node = xmlNewNode(NULL, BAD_CAST "Passage");
	if(root_node == NULL){
		printf("Erreur création du node racine ! \n");
		xmlFreeDoc(doc);
		return;
	}

	xmlDocSetRootElement(doc, root_node);

	filename = generateFileName();
	xmlSaveFormatFileEnc(filename, doc, "UTF-8", 1);

	xmlFreeDoc(doc);
	free(filename);
	xmlCleanupParser();
	xmlMemoryDump();
}

// Génération du nom du fichier
char * generateFileName(){
	time_t secondes;
	struct tm instant;
	char * filename = NULL;
	char * day = NULL;
	char * month = NULL;
	char * year = NULL;

	filename = malloc(250 * 1);
	day = malloc(3 * 1);
	month = malloc(3 * 1);
	year = malloc(5 * 1);

	checkProfileDirectoryExistFinal();
	time(&secondes);
	instant=*localtime(&secondes);

	sprintf(day, "%d", instant.tm_mday);
	sprintf(month, "%d", (instant.tm_mon+1));
	sprintf(year, "%d", ((instant.tm_year+1900) - 2000));

	strcpy(filename, FINAL_FILENAME_START);
	if(strlen(day) == 1){
		strcat(filename, "0");
	}
	strcat(filename, day);
	if(strlen(month) == 1){
		strcat(filename, "0");
	}
	strcat(filename, month);
	strcat(filename, year);
	strcat(filename, FILENAME_END);

	free(day);
	free(month);
	free(year);

	return filename;
}

// Initilisation de la structure user
User * initUser(){
	User * user = NULL;
	user = malloc(sizeof(User) * 1);

	if(user == NULL){
		printf("Erreur allocation de la structure ! \n");
		return NULL;
	}

	user->id = malloc(50 * 1);
	if(user->id == NULL){
		printf("Erreur allocation de id ! \n");
		free(user);
		return NULL;
	}
	user->statut = malloc(4 * 1);
	if(user->statut == NULL){
		printf("Erreur allocation de statut ! \n");
		free(user->id);
		free(user);
		return NULL;
	}
	user->time = malloc(6 * 1);
	if(user->time == NULL){
		printf("Erreur allocation de time ! \n");
		free(user->id);
		free(user->statut);
		free(user);
		return NULL;
	}

	return user;
}

// Libération de User
void freeUser(User * user){
	if(user->id != NULL)
		free(user->id);
	if(user->statut != NULL)
		free(user->statut);
	if(user->time != NULL)
		free(user->time);
}

// Fonction de verification de l'existance du dossier
void checkProfileDirectoryExistFinal(){
	DIR* dir = opendir(FINAL_FILENAME_START);

	if (dir){
		// Si oui
		closedir(dir);

	}else if (ENOENT == errno){
		// Sinon
#if defined(_WIN32)
		_mkdir(FINAL_FILENAME_START);
#else
		mkdir(FINAL_FILENAME_START, 0700);
#endif

	}else{
		// Erreur
		printf("Erreur à l'ouverture/creation du dossier Passage ! \n");
		exit(EXIT_FAILURE);

	}
}

// Fonction de verification de l'existance du dossier
void checkProfileDirectoryExistSource(){
	DIR* dir = opendir(XML_FILENAME_START);

	if (dir){
		// Si oui
		closedir(dir);

	}else if (ENOENT == errno){
		// Sinon
#if defined(_WIN32)
		_mkdir(XML_FILENAME_START);
#else
		mkdir(XML_FILENAME_START, 0700);
#endif

	}else{
		// Erreur
		printf("Erreur à l'ouverture/creation du dossier Passage ! \n");
		exit(EXIT_FAILURE);

	}
}

// Ouverture du fichier xml
xmlDocPtr openXmlFile(char * filename){
	xmlDocPtr doc = NULL;
	doc = xmlReadFile(filename, "UTF-8", 1);
	if(doc == NULL){
		printf("Erreur ouverture du fichier ! \n");
		return NULL;
	}
	return doc;
}

// Obtention du noeud racine
xmlNodePtr getRootNode(xmlDocPtr doc){
	xmlNodePtr root_node = NULL;
	root_node = xmlDocGetRootElement(doc);
	if(root_node == NULL){
		printf("Erreur à l'obtention du noeud racine ! \n");
		return NULL;
	}
	return root_node;
}

// Ajout d'un node
xmlNodePtr addNode(){
	xmlNodePtr node = NULL;
	node = xmlNewNode(NULL, BAD_CAST "User");
	if(node == NULL){
		printf("Erreur à la création du node ! \n");
		return NULL;
	}
	return node;
}

// Ajout des données
void addProp(User * user, xmlNodePtr node){
	if(xmlNewProp(node, BAD_CAST ID, BAD_CAST user->id) == NULL ||
	   xmlNewProp(node, BAD_CAST TIME, BAD_CAST user->time) == NULL ||
	   xmlNewProp(node, BAD_CAST CHECK, BAD_CAST user->statut) == NULL){
		printf("Erreur à l'ajout des propriétés ! \n");
		return;
	}
}

// Transformation noeud classique en noeud enfant
void addChild(xmlNodePtr root_node, xmlNodePtr node){
	if(xmlAddChild(root_node, node) == NULL){
		printf("Erreur à l'ajout de l'enfant");
		return;
	}
}

// Ajout d'un passage
void addUser(User * user){
	xmlDocPtr doc = NULL;
	xmlNodePtr root_node = NULL;
	xmlNodePtr node = NULL;
	char * filename = NULL;

	filename = generateFileName();
	doc = openXmlFile(filename);
	root_node = getRootNode(doc);
	node = addNode();

	addProp(user, node);

	addChild(root_node, node);

	xmlSaveFormatFileEnc(filename, doc, "UTF-8", 1);

	free(filename);
	xmlFreeDoc(doc);
	xmlCleanupParser();
	xmlMemoryDump();
}

// Boucle principal
void mainLoop(){
	// Génération des dossiers
	checkProfileDirectoryExistFinal();
	checkProfileDirectoryExistSource();

	// Création du fichier jounalier
	createFinalXmlFile();

	// Lancement de la sécance principal
	generateDaylyXmlFile();

	// Supression des fichiers dans l'archive
	destroyFile();
}
