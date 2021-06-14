/*
* @Author: Daniel Guerra Gallardo
* @Description: Aplicación con Interfaz Gráfica de Usuario que permite tratar archivos Zip como documentos: Añadir, Quitar, Listar, Modificar y Guardar
* Además contiene compatibilidad con scripting con Lua
* @Title: Middleware. Práctica Final
* @Date: 05/2021
*/

#pragma once

#include "api.hpp"
#include "DataModel.h";

#include <string>
#include <list>
#include <memory>
#include <vector>

using namespace std;

class TEST_API ZipManager
{
	class Entry;
private:

	unique_ptr<DataModel> model = nullptr;
	unique_ptr<Item> archive = nullptr;

public:

	ZipManager() {};
	ZipManager(string& path);
	~ZipManager() = default;

public:


	/*Devolvemos la lista con sus archivos*/
	const list<string> file_list() const;

	/*Devolvemos el contenido del archivo*/
	const string file_content(const string& file_name);

	/*Actualizamos el contenido del archivo*/
	bool update_content(const string& file_name, const string& content);

	/* Borramos un archivo del Zip */
	void delete_file(const string& file_name);

	/* Añadimos un archivo al Zip */
	void create_file(const string& file_name);

	inline void set_file_path(const string& _path) { model.get()->set_archive_path(_path); }
	//void push_entry(string& file_name) {model.get()->set_entry(make_shared<Entry>(file_name)); }
	//void push_item(string& file_name) { model.get()->set_item(make_shared<Item>(file_name)); }


};


