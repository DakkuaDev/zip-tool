#pragma once



#include <string>
#include <list>

using namespace std;

class ZipManager
{
private:

	string file_path;

public:

	// Default
	ZipManager(const string& path);
	~ZipManager() = default;
	

public:


	/*Devolvemos la lista con sus archivos*/
	const list<string> file_list() const;

	/*Devolvemos el contenido del archivo*/
	const string file_content(const string& file_name);

	/*Actualizamos el contenido del archivo*/
	bool update_content(const string& file_name, const string& content);


};

