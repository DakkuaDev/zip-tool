#pragma once

#include "ZipLib/ZipFile.h"
#include <string>
#include <list>


using namespace std;

class ZipManager
{
private:

	ZipArchive::Ptr archive;

public:

	// Default
	ZipManager(const string& path);
	~ZipManager();
	

public:

	/*Verificamos si el archivo se puede abrir o no*/
	bool is_open() const;

	/*Devolvemos la lista con sus archivos*/
	const list<string> file_list() const;

	/*Devolvemos el contenido del archivo*/
	const string file_content() const;

	/*Actualizamos el contenido del archivo*/
	bool update_content(const string& file_name, const string& content);


};

