#include "ZipManager.h"

#include <fstream>

ZipManager::ZipManager(const string& _path) : file_path (_path), archive (ZipFile::Open(_path))
{

}

bool ZipManager::is_open() const 
{
	return archive != nullptr;
};

const list<string> ZipManager::file_list() const 
{
	// Obtenemos el número de entradas totales
	size_t entries = archive->GetEntriesCount();

	list<string> entries_list;

	// Recorremos las entradas
	for (int i = 0; i < entries; i++)
	{
		auto entry = archive->GetEntry(int(i));

		string file_name = entry->GetFullName();

		entries_list.push_back(file_name);
	}

	return entries_list;
};

const string ZipManager::file_content(const string& file_name) const 
{
	ZipArchiveEntry::Ptr entry = archive->GetEntry(file_name);

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
};

bool ZipManager::update_content(const string& file_name, const string& content) 
{

	// Añadimos el contenido a un archivo temporal

	ofstream temporal_writer(file_name.c_str(), std::ios::trunc | std::ios::out);

	temporal_writer << content;

	temporal_writer.close();


	// Leemos el archivo temporal
	ifstream temporal_reader(file_name.c_str());

	// Leemos la entrada
	ZipArchiveEntry::Ptr entry = archive->GetEntry(file_name);

	// La creamos si no existe
	if (entry == nullptr)
	{
		entry = archive->CreateEntry(file_name);
	}

	entry->SetCompressionStream(temporal_reader);

	ZipFile::SaveAndClose(archive, file_path);

	return true;
};
