/*
* @Author: Daniel Guerra Gallardo
* @Description: Aplicaci�n con Interfaz Gr�fica de Usuario que permite tratar archivos Zip como documentos: A�adir, Quitar, Listar, Modificar y Guardar
* Adem�s contiene compatibilidad con scripting con Lua
* @Title: Middleware. Pr�ctica Final
* @Date: 05/2021
*/

#include "ZipLib/ZipFile.h"
#include "ZipManager.h"

#include <fstream>


ZipManager::ZipManager(const string& _path) : file_path(_path) {}

const list<string> ZipManager::file_list() const 
{

	ZipArchive::Ptr myArchive;
	myArchive = ZipFile::Open(file_path);

	// Obtenemos el n�mero de entradas totales
	size_t entries = myArchive->GetEntriesCount();

	list<string> entries_list;

	// Recorremos las entradas
	for (int i = 0; i < entries; i++)
	{
		auto entry = myArchive->GetEntry(int(i));

		string file_name = entry->GetFullName();

		entries_list.push_back(file_name);
	}

	return entries_list;
}

const string ZipManager::file_content(const string& file_name) 
{

	ZipArchive::Ptr myArchive;
	myArchive = ZipFile::Open(file_path);


	ZipArchiveEntry::Ptr entry = myArchive->GetEntry(file_name);

	std::istream* decompressStream = entry->GetDecompressionStream();


	// Leemos el archivo y lo devolvemos

	stringstream buffer;

	do
	{
		// Leo la linea
		string line;
		getline(*decompressStream, line);

		// Concateno al buffer
		buffer << line;

	} 	while (!decompressStream->eof()); // Mientras no llegamos al final del archivo

	return buffer.str();
}

bool ZipManager::update_content(const string& file_name, const string& content) 
{
	ZipArchive::Ptr myArchive;
	myArchive = ZipFile::Open(file_path);
	// A�adimos el contenido a un archivo temporal

	ofstream temporal_writer(file_name.c_str(), std::ios::trunc | std::ios::out);

	temporal_writer << content;

	temporal_writer.close();


	// Leemos el archivo temporal
	ifstream temporal_reader(file_name.c_str(), std::ios::binary);

	// Leemos la entrada
	ZipArchiveEntry::Ptr entry = myArchive->GetEntry(file_name);

	// La creamos si no existe
	if (entry == nullptr)
	{
		entry = myArchive->CreateEntry(file_name);
	}

	entry->SetCompressionStream(temporal_reader);

	ZipFile::Save(myArchive, file_path);

	//ZipFile::Save(archive, file_path);

	temporal_reader.close();

	return true;
}

void ZipManager::delete_file(const string& file_name) { ZipFile::RemoveEntry(file_path.c_str(), file_name.c_str()); }

void ZipManager::create_file(const string& file_name) { ZipFile::AddFile(file_path.c_str(), file_name.c_str()); }
