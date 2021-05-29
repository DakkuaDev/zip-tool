// Backed-Test.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include <iostream>
#include "ZipManager.h"

using namespace std;

int main()
{
    ZipManager test("example.zip");

    test.update_content("hola.txt", "Hello World!");

    string content = test.file_content("hola.txt");

    list<string> contents = test.file_list();

    for (list<string>::iterator c = contents.begin(); c != contents.end(); c++ )
    {
        cout << *c << endl;
    }

    cout << content;
    
}

