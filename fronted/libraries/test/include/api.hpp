/*
* @Author: Daniel Guerra Gallardo
* @Description: Libreria de enlace dinámico para la creación de una herramienta
* @Title: Middleware. Práctica Final
* @Date: 15/04/2021
*/

#pragma once

#include <ciso646>

#if defined(USE_THIS_LIBRARY_AS_STATIC)

#define	TEST_API

#else

#if defined(THIS_IS_THE_LIBRARY)
#define TEST_API __declspec(dllexport)

#else
#define TEST_API __declspec(dllimport)

#endif

#endif
