/*
* @Author: Daniel Guerra Gallardo
* @Description: Aplicación con Interfaz Gráfica de Usuario que permite tratar archivos Zip como documentos: Añadir, Quitar, Listar, Modificar y Guardar
* Además contiene compatibilidad con scripting con Lua
* @Title: Middleware. Práctica Final
* @Date: 05/2021
*/

#pragma once

#include "api.hpp"

#include <string>
#include <vector>
#include <memory>

using namespace std;

class Item;
class Entry;

class TEST_API DataModel
{

public:
	DataModel() {};
	~DataModel() = default;

private:

	string archive_path;
	vector< shared_ptr< Item > > items;
	vector< shared_ptr< Entry> > entries;

public:

	inline string& get_archive_path() { return archive_path; }
	inline vector< shared_ptr< Item > > get_items() { return items; }

	inline void set_archive_path(const string& _path) { archive_path = _path; }
	inline void set_item(shared_ptr< Item > _new_item) { items.push_back(_new_item); }
	inline void set_entry(shared_ptr< Entry > _new_entry) { entries.push_back(_new_entry); }

};

class TEST_API Item
{
public:
	Item(const string& _name) : name(_name) {}
	~Item() = default;

private:

	string name;

	enum Compression
	{
		STORE,
		DEFLATE
	};

public:
	inline string& get_archive_name() { return name; }

};

class TEST_API Entry
{
public:

	Entry(string& _name) : name(_name) {}
	~Entry() = default;

private:
	string name;

public:
	inline string& get_entry_name() { return name; }
	
};


