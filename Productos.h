#pragma once
#ifndef _PRODUCTOS_H_
#define _PRODUCTOS_H_

#include <string>

using namespace std;

struct productos {
	int IDProducto;
	int IDUser;
	string nombreProducto, cantidadProducto, codigoProducto, marcaProducto, precioProducto;
	char descripcionProducto[MAX_PATH] = "";
	char fotoP1[MAX_PATH] = "";
	char fotoP2[MAX_PATH] = "";
	productos* nextProducto;
	productos* prevProducto;
}*oProducto, * aProducto;


/// <summary>
/// Si el nombre coincide con algun producto, entonces aProducto sera diferente de NULL,
/// pero si no existe ese nombre entonces aProducto contendra NULL
/// </summary>
/// <param name="nombreDelProducto"></param>
void buscarProductoPorNombre(string nombreDelProducto) {
	aProducto = oProducto;
	while (aProducto->nombreProducto.compare(nombreDelProducto) != 0) {
		if (aProducto->nextProducto == NULL) {
			break;
		}
		aProducto = aProducto->nextProducto;
	}
}

#endif // !_PRODUCTOS_H_
