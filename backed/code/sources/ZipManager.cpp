#include "ZipManager.h"

ZipManager::ZipManager(const string& _path) : archive (ZipFile::Open("archive.zip"))
{

}

bool ZipManager::is_open() const {};

const list<string> ZipManager::file_list() const {};

const string ZipManager::file_content() const {};

bool ZipManager::update_content(const string& file_name, const string& content) {};
