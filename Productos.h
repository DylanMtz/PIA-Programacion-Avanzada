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

char fotoProducto1[MAX_PATH] = "";
char fotoProducto2[MAX_PATH] = "";

/// <summary>
/// Si el nombre coincide con algun producto, entonces aProducto sera diferente de NULL,
/// pero si no existe ese nombre ent
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

/// <summary>
/// funcion que crea un nuevo producto, le asigna sus valores, y le asigna NULL a los enlaces
/// </summary>
/// <param name="nombreDelProducto"></param>
/// <param name="cantidadEnInventario"></param>
/// <param name="codigoDelProducto"></param>
/// <param name="marcaDelProducto"></param>
/// <param name="descripcionDelProducto"></param>
/// <param name="precio"></param>
/// <returns>Retorna un nuevo producto</returns>
productos* crearProducto(
	string nombreDelProducto,
	string cantidadEnInventario,
	string codigoDelProducto,
	string marcaDelProducto,
	string descripcionDelProducto,
	string precio
) {

	productos* nuevoProducto = new productos;
	nuevoProducto->nombreProducto = nombreDelProducto;
	nuevoProducto->cantidadProducto = cantidadEnInventario;
	nuevoProducto->codigoProducto = codigoDelProducto;
	nuevoProducto->marcaProducto = marcaDelProducto;
	nuevoProducto->precioProducto = precio;
	nuevoProducto->IDProducto = 0;
	for (int i = 0; i < MAX_PATH; i++) {
		oProducto->descripcionProducto[i] = desc[i];
		oProducto->fotoP1[i] = fotoProducto1[i];
		oProducto->fotoP2[i] = fotoProducto2[i];
	}
	nuevoProducto->nextProducto = NULL;
	nuevoProducto->prevProducto = NULL;
	return nuevoProducto;
}


void insertarProductoEnLista(productos* nuevoProducto)
{
	aProducto = oProducto;
	while (aProducto->nextProducto != NULL) {
		aProducto = aProducto->nextProducto;
	}

	aProducto->nextProducto = nuevoProducto;
	nuevoProducto->prevProducto = aProducto;
	aProducto = nuevoProducto;
	aProducto = oProducto;
}

void saveProducto(productos* pOrigen) {
	lectorEscritor.open("Productos.bin", ios::out | ios::trunc | ios::binary);
	if (lectorEscritor.is_open()) {
		pOrigen = oProducto;
		while (pOrigen != NULL) {
			lectorEscritor.write(reinterpret_cast<char*>(pOrigen), sizeof(productos));
			pOrigen = pOrigen->nextProducto;
		}
		lectorEscritor.close();
		MessageBox(NULL, "La lista de productos se ha actualizado exitosamente", "GUARDAR", MB_ICONASTERISK);
		return;
	}
	else {
		MessageBox(NULL, "No se pudo abrir el archivo", "NO GUARDAR", MB_ICONASTERISK);
		return;
	}
}

#endif // !_PRODUCTOS_H_
