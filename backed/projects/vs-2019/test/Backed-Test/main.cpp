// Backed-Test.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include <iostream>
#include "ZipManager.h"

int main()
{
    ZipManager test("example.zip");

    test.update_content("hola.txt", "Hello World!");
    
}

