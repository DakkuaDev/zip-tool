/*
* @Author: Daniel Guerra Gallardo
* @Description: Aplicación con Interfaz Gráfica de Usuario que permite tratar archivos Zip como documentos: Añadir, Quitar, Listar, Modificar y Guardar
* Además contiene compatibilidad con scripting con Lua
* @Title: Middleware. Práctica Final
* @Date: 05/2021
*/

#include "ZipLib/ZipFile.h"
#include "ZipManager.h"
#include "DataModel.h"

#include <fstream>

ZipManager::ZipManager(string& _archive) 
{
	archive = make_unique<Item>(_archive);
	model   = make_unique<DataModel>();
}


const list<string> ZipManager::file_list() const 
{
	// Creo un item de nuevo registro
	
	ZipArchive::Ptr myArchive;
	myArchive = ZipFile::Open(archive->get_archive_name());

	// Obtenemos el número de entradas totales
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
	myArchive = ZipFile::Open(archive->get_archive_name());


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
	myArchive = ZipFile::Open(archive->get_archive_name());
	// Añadimos el contenido a un archivo temporal

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

	ZipFile::Save(myArchive, archive->get_archive_name());

	//ZipFile::Save(archive_path, archive_path);

	temporal_reader.close();

	return true;
}

void ZipManager::delete_file(const string& file_name) { ZipFile::RemoveEntry(archive->get_archive_name().c_str(), file_name.c_str()); }

void ZipManager::create_file(const string& file_name) { ZipFile::AddFile(archive->get_archive_name().c_str(), file_name.c_str()); }
