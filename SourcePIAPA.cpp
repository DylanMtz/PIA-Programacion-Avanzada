#include <Windows.h>
#include <fstream>
#include <string>
#include "resource.h"
#include <ctime>

#define MI_TIMER 16
time_t tiempoActual;
tm* infoTiempo;
long long int fechaHoy;

#define CM_INFO 100
#define CM_PRODUCTS 101
#define CM_ENV 102
#define CM_EXIT 103
using namespace std;



struct User {
	int IDUser;
	string username;
	string password;
	string fechaCreacion;
	User* next;
	User* prev;
}*oUser, * aUser, * userAccess;

struct InfoVendedor {
	int IDInfoVendedor;
	int IDUser;
	string nombreVendedor;
	string aliasDeLaEmpresa;
	char imagen[MAX_PATH] = "";
	InfoVendedor* nextInfoVendedor;
	InfoVendedor* prevInfoVendedor;
}*oInfoVendedor, * aInfoVendedor;

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

productos* buscadorDeProductos(int IP) {
	productos* aProd = oProducto;
	for (int i = 0; i < IP; i++) {
		aProd = aProd->nextProducto;
	}
	return aProd;
}

struct Envios {
	tm* fecha;
	string fechastr;
	string status;
	int IDEnvio;
	int IDUser;
	int productoAEnviarID;
	string productoAEnviar;
	string precio;
	string cantidad, calle, colonia, ciudad, estado, mensaje;
	Envios* nextEnvio;
	Envios* prevEnvio;
	string obtenerNombreEnvio() {
		string envio = "";
		envio.append("Envio de ");
		envio.append(productoAEnviar);
		return envio;
	}
}*oEnvios, * aEnvios;

bool isFull = false;
bool exitProgramModal = false;
bool exitProgramAltaEnvios = false;
bool exitProgramAltaEnviosNew = false;
bool isNumber = false;
bool exitProgram = false;
bool exitProgramRegister = false;
bool exitProgramInfo = false;
bool exitProgramEnvios = false;
bool exitProgramAlta = false;
bool exitProgramProductos = false;
bool exitProgramModify = false;
bool exitProgramResumen = false;
bool exitProgramMostrarP = false;
bool exitProgramModificaP = false;
bool leerFecha = false;
HINSTANCE hGInstance;
HWND hTxtRegisterUserName;
HWND hTxtRegisterPassword;
HWND hTxTRegisterNombrePropio;
HWND hTxTRegisterApellidos;
HWND hTxTCorreo;
HWND hTxTFechaNacimiento;
HWND hTxtFechaEnvio;
HWND hTxtConfirmarContrasena;
HWND hTxtLoginUserName;
HWND hTxtLoginPassword;
HWND hTxtNombreCompleto;
HWND hTxtEmpresa;
HWND hTxtNombreProducto;
HWND hTxtCantidadEnInventario;
HWND hTxtCodigoProducto;
HWND hTxtMarcaProducto;
HWND hTxtDescripcionProducto;
HWND hTxtPrecio;
HWND hTxtCantidadEnvio;
HWND hTxtCalle;
HWND hTxtColonia;
HWND hTxtCiudad;
HWND hTxtEstado;
HWND hTxtMensaje;
fstream lectorEscritor;
HMENU hMenu;
HWND hlTiempo;
HWND hTxtChangeProdNombre;
HWND hTxtChangeProdCantidad;
HWND hTxtChangeProdCodigo;
HWND hTxtChangeProdMarca;
HWND hTxtChangeProdPrecio;
HWND hTxtChangeProdDescripcion;
HWND hTxtChangeEnvioProd;
HWND hTxtChangeEnvioCant;
HWND hTxtChangeEnvioPrec;
HWND hTxtChangeEnvioCalle;
HWND hTxtChangeEnvioColonia;
HWND hTxtChangeEnvioCiudad;
HWND hTxtChangeEnvioEstado;
HWND hTxtChangeEnvioMensaje;
HWND hTxtChangeEnvioFecha;
HWND hTxtFiltrar;
HWND hTxtFiltrarFecha;

char fotoVendedor[MAX_PATH] = "";
char fotoProducto1[MAX_PATH] = "";
char fotoProducto2[MAX_PATH] = "";
char fotoProducto1Nueva[MAX_PATH] = "";
char fotoProducto2Nueva[MAX_PATH] = "";
char desc[MAX_PATH] = "";
int GLOBAL_USER_ID = 1;
int GLOBAL_PRODUCTO_ID = 1;
int GLOBAL_ENVIO_ID = 1;

void freeMemoryUser();
void freeMemoryInfoVendedor();
void freeMemoryProductos();
void freeMemoryEnvios();
void loadUser();
void save(User*);
void saveInfoVendedor(InfoVendedor*);
void saveProducto(productos*);
void saveEnvios(Envios*);
void loadInfoVendedor();
void loadProducto();
void loadEnvios();
void getGlobalId(); //GlobalId.txt obtener 1 registro del numero a guardar
void saveGlobalId();//GlobalId.txt guardar el numero en el archivo binario
string getText(HWND);
void actualizarProducto(HWND hwnd);
void eliminarProducto();
tm* calcularFecha(string fechaEnv);
string calcularEstadoDeEnvio(tm* ltm);

BOOL CALLBACK fLogin(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK fRegister(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK fInfoVendedor(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK fProductos(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK fMostrarProductos(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK fModificarProductos(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK fAltaEnvio(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK fEnvios(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK fModify(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK fResumenEnvio(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK fModal(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK faltaEnviosNew(HWND, UINT, WPARAM, LPARAM);

bool validarNombre(string nombre) {
	for (int i = 0; i < nombre.length(); i++) {
		if ((nombre[i] >= 33 && nombre[i] <= 64) || (nombre[i] >= 91 && nombre[i] <= 96) || (nombre[i] >= 123 && nombre[i] <= 126)) {
			MessageBox(NULL, "Tu nombre tiene caracteres especiales y/o numeros", "NO ALTA", MB_ICONASTERISK);
			return TRUE;
		}
	}
	return FALSE;
}

bool validarContrasena(string contrasena) {
	bool contieneMayusculas = FALSE;
	bool contieneMinusculas = FALSE;
	bool contieneNumero = FALSE;
	bool contieneSigno = FALSE;

	if (contrasena.length() < 8) {
		MessageBox(NULL, "La contraseña debe de tener por lo menos 8 caracteres o mas", "NO ALTA", MB_ICONASTERISK);
		return TRUE;
	}

	for (int i = 0; i < contrasena.length(); i++) {
		if (contrasena[i] >= 65 && contrasena[i] <= 90)
			contieneMayusculas = TRUE;
		if (contrasena[i] >= 97 && contrasena[i] <= 122)
			contieneMinusculas = TRUE;
		if (contrasena[i] >= 48 && contrasena[i] <= 57)
			contieneNumero = TRUE;
		if ((contrasena[i] >= 33 && contrasena[i] <= 47) || (contrasena[i] >= 58 && contrasena[i] <= 64) || (contrasena[i] >= 91 && contrasena[i] <= 96) || (contrasena[i] >= 123 && contrasena[i] <= 126))
			contieneSigno = TRUE;
	}

	if (contieneMayusculas && contieneMinusculas && contieneNumero && contieneSigno)
		return FALSE;
	else {
		MessageBox(NULL, "La contraseña debe de tener por lo menos una mayuscula, una minuscula, un signo y un numero", "NO ALTA", MB_ICONASTERISK);
		return TRUE;
	}
}

int contadorEnvios = 1;

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, PSTR cmdLine, int cShow) {
	getGlobalId();
	oUser = aUser = NULL;
	hGInstance = hInst;
	HWND hwnd = CreateDialog(hInst, MAKEINTRESOURCE(IDD_LOGIN), NULL, fLogin);
	SetTimer(hwnd, MI_TIMER, 10000, (TIMERPROC)NULL);
	ShowWindow(hwnd, SW_SHOW);
	loadUser();
	loadInfoVendedor();
	loadProducto();
	loadEnvios();
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

BOOL CALLBACK fLogin(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	switch (msg) {
	case WM_INITDIALOG: {
		hTxtLoginUserName = GetDlgItem(hwnd, TXT_NAME);
		hTxtLoginPassword = GetDlgItem(hwnd, TXT_PASSWORD);
	}break;
	case WM_COMMAND: {
		switch (LOWORD(wparam)) {
		case BTN_EXITLOGIN: {
			PostQuitMessage(16);
			break;
		}
		case BTN_REGISTER: {
			//Abre la ventana de registro
			HWND hRegister = CreateDialog(hGInstance, MAKEINTRESOURCE(IDD_REGISTER), NULL, fRegister);
			ShowWindow(hRegister, SW_SHOW);
			exitProgram = false;
			DestroyWindow(hwnd);
		}break;
		case BTN_ENTER: {
			string userName = getText(hTxtLoginUserName);
			string password = getText(hTxtLoginPassword);

			if (userName.compare("") == 0 || password.compare("") == 0) {
				MessageBox(NULL, "Falta información. Por favor, llene los campos correspondientes", "NO LOGIN", MB_ICONASTERISK);
				break;
			}

			if (oUser != NULL) {
				bool found = true;
				while (aUser != NULL) {
					if (aUser->username.compare(userName) == 0 && aUser->password.compare(password) == 0) {
						break;
					}
					if (aUser->next == NULL)
						found = false;
					aUser = aUser->next;
				}

				if (found == true) {

					/*if (isFull) {
						HWND hEnvios = CreateDialog(hGInstance, MAKEINTRESOURCE(IDD_ENVIOS), NULL, fEnvios);
						ShowWindow(hEnvios, SW_SHOW);
						exitProgram = false;
						DestroyWindow(hwnd);
						break;
					}*/
					//	else {
					userAccess = aUser;
					bool existeUsuario = false;
					while (aInfoVendedor != NULL) {
						isFull = true;
						if (aInfoVendedor->IDUser == userAccess->IDUser) {
							existeUsuario = true;
							if (aInfoVendedor->nombreVendedor.length() == 0 || aInfoVendedor->aliasDeLaEmpresa.length() == 0) {
								HWND hPrincipal = CreateDialog(hGInstance, MAKEINTRESOURCE(IDD_INFOVENDEDOR), NULL, fInfoVendedor);
								ShowWindow(hPrincipal, SW_SHOW);
								exitProgram = false;
								DestroyWindow(hwnd);
								break;
							}
							else {
								HWND hEnvios = CreateDialog(hGInstance, MAKEINTRESOURCE(IDD_ENVIOS), NULL, fEnvios);
								ShowWindow(hEnvios, SW_SHOW);
								exitProgramEnvios = false;
								DestroyWindow(hwnd);
								break;
							}
						}
						aInfoVendedor = aInfoVendedor->nextInfoVendedor;
					}
					if (!existeUsuario) {
						HWND hPrincipal = CreateDialog(hGInstance, MAKEINTRESOURCE(IDD_INFOVENDEDOR), NULL, fInfoVendedor);
						ShowWindow(hPrincipal, SW_SHOW);
						exitProgram = false;
						DestroyWindow(hwnd);
						break;
					}
				}
				else {
					MessageBox(NULL, "Ningun usuario coincide con tus credenciales", "NO LOGIN", MB_ICONASTERISK);
					aUser = oUser;
				}
			}
			else {
				MessageBox(NULL, "NO hay usuarios dados de alta", "NO LOGIN", MB_ICONASTERISK);
				break;
			}

		}break;
		}
	}break;
	case WM_CLOSE:
		exitProgram = true;
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		if (exitProgram) {
			//freeMemoryUser();
			PostQuitMessage(16);
		}
		break;
	}
	return FALSE;
}

BOOL CALLBACK fRegister(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	switch (msg) {
	case WM_INITDIALOG: {
		hTxtRegisterUserName = GetDlgItem(hwnd, TXT_RNAME);
		hTxtRegisterPassword = GetDlgItem(hwnd, TXT_RPASSWORD);
		hTxtConfirmarContrasena = GetDlgItem(hwnd, TXT_CONFIRMPASSWORD);
		hTxTRegisterNombrePropio = GetDlgItem(hwnd, TXT_RNAMEP);
		hTxTRegisterApellidos = GetDlgItem(hwnd, TXT_APELLIDOS);
		hTxTCorreo = GetDlgItem(hwnd, TXT_CORREO);
		hTxTFechaNacimiento = GetDlgItem(hwnd, IDC_DTPR);
	}break;
	case WM_COMMAND: {
		switch (LOWORD(wparam)) {
		case BTN_RETURN: {
			HWND hLogin = CreateDialog(hGInstance, MAKEINTRESOURCE(IDD_LOGIN), NULL, fLogin);
			ShowWindow(hLogin, SW_SHOW);
			exitProgramRegister = false;
			DestroyWindow(hwnd);
		}break;
		case BTN_REGISTER2: {
			string userName = getText(hTxtRegisterUserName);
			string password = getText(hTxtRegisterPassword);
			string confPassword = getText(hTxtConfirmarContrasena);
			string nombrePropio = getText(hTxTRegisterNombrePropio);
			string apellidos = getText(hTxTRegisterApellidos);
			string correo = getText(hTxTCorreo);
			string fechaNacimiento = getText(hTxTFechaNacimiento);

			/*if (userName.compare("") == 0 || password.compare("") == 0 || nombrePropio.compare("") == 0 || apellidos.compare("") == 0 || correo.compare("") == 0 || fechaNacimiento.compare("") == 0) {
				MessageBox(NULL, "Falto llenar la informacion", "NO ALTA", MB_ICONASTERISK);
				break;
			}

			if(strstr(correo.c_str(),"@")==NULL){ //Pendiente de buscar una validacion mas robusta
				MessageBox(NULL, "Correo no valido", "NO ALTA", MB_ICONASTERISK);
				break;
			}

			tm* ltm = new tm;
			char* pch;
			char* dup = _strdup(fechaNacimiento.c_str());
			pch = strtok(dup, "/");
			ltm->tm_mon = atoi(strtok(NULL, "/"));
			ltm->tm_year = atoi(strtok(NULL, "/"));
			ltm->tm_mday = atoi(pch);

			time(&tiempoActual);
			infoTiempo = localtime(&tiempoActual);


			char day[5];
			char month[5];
			char year[5];


			++infoTiempo->tm_mon;
			infoTiempo->tm_year += 1900;


			_itoa(infoTiempo->tm_mday, day, 10);
			_itoa(infoTiempo->tm_mon, month, 10);
			_itoa(infoTiempo->tm_year, year, 10);

			string dia = day;
			if (dia.length() < 2) {
				dia.clear();
				dia.append("0");
				dia.append(day);
			}

			string mes = month;
			if (mes.length() < 2) {
				mes.clear();
				mes.append("0");
				mes.append(month);
			}

			string aFecha = year;

			if (ltm->tm_year >= infoTiempo->tm_year && ltm->tm_mon >= infoTiempo->tm_mon && ltm->tm_mday >= infoTiempo->tm_mday) {
				MessageBox(NULL, "Fecha de nacimiento no valida", "NO ALTA", MB_ICONASTERISK);
				break;
			}

			if (validarNombre(nombrePropio)==TRUE || validarNombre(apellidos)==TRUE)
				break;

			if (validarContrasena(password)==TRUE)
				break;

			if (password != confPassword) {
				MessageBox(NULL, "Las contraseñas no coinciden", "NO ALTA", MB_ICONASTERISK);
				break;
			}*/

			if (oUser != NULL) {
				bool found = true;
				while (aUser->username.compare(userName) != 0) {
					if (aUser->next == NULL) {
						found = false;
						break;
					}
					aUser = aUser->next;
				}
				if (found == true) {
					MessageBox(NULL, "Un usuario ya tiene este Username", "NO ALTA", MB_ICONASTERISK);
					aUser = oUser;
					break;
				}
				else
					aUser = oUser;
			}

			if (oUser == NULL) {
				oUser = new User;
				oUser->IDUser = GLOBAL_USER_ID;
				oUser->username.append(userName);
				oUser->password.append(password);
				//	oUser->fechaCreacion = aFecha + "/" + mes + "/" + dia;
				oUser->next = NULL;
				oUser->prev = NULL;
				aUser = oUser;
				GLOBAL_USER_ID++;
			}
			else {
				aUser = oUser;
				while (aUser->next != NULL)
					aUser = aUser->next;
				aUser->next = new User;
				aUser->next->prev = aUser;
				aUser = aUser->next;
				aUser->IDUser = GLOBAL_USER_ID;
				aUser->username.append(userName);
				aUser->password.append(password);
				//aUser->fechaCreacion = aFecha + "/" + mes + "/" + dia;
				aUser->next = NULL;
				//aUser = oUser;
				GLOBAL_USER_ID++;
			}
			userAccess = aUser;
			aUser = oUser;
			save(aUser);
			saveGlobalId();
			//save(aUser); aqui puede haber cambios
			HWND hLogin = CreateDialog(hGInstance, MAKEINTRESOURCE(IDD_LOGIN), NULL, fLogin);
			ShowWindow(hLogin, SW_SHOW);
			exitProgramRegister = false;
			DestroyWindow(hwnd);
		}break;
		}
	}break;
	case WM_CLOSE:
		exitProgramRegister = true;
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		if (exitProgramRegister) {
			//freeMemoryUser();
			PostQuitMessage(117);
		}
		break;
	}
	return FALSE;
}

BOOL CALLBACK fInfoVendedor(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	switch (msg) {
	case WM_INITDIALOG: {
		hMenu = LoadMenu(hGInstance, MAKEINTRESOURCE(IDR_MENU1));
		SetMenu(hwnd, hMenu);
		hTxtNombreCompleto = GetDlgItem(hwnd, TXT_COMPLETENAME);
		hTxtEmpresa = GetDlgItem(hwnd, TXT_ALIAS);
		while (aInfoVendedor != NULL) {
			isFull = true;
			if (aInfoVendedor->IDUser == userAccess->IDUser) {
				SetWindowText(hTxtNombreCompleto, aInfoVendedor->nombreVendedor.c_str());
				SetWindowText(hTxtEmpresa, aInfoVendedor->aliasDeLaEmpresa.c_str());
				for (int i = 0; i < MAX_PATH; i++) {
					fotoVendedor[i] = aInfoVendedor->imagen[i];
				}
				HBITMAP hbFotoInfo = (HBITMAP)LoadImage(NULL, fotoVendedor, IMAGE_BITMAP, 600, 400, LR_LOADFROMFILE);
				HWND hPbFotoInfo = GetDlgItem(hwnd, PB_FOTO);
				SendMessage(hPbFotoInfo, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbFotoInfo);
			}
			aInfoVendedor = aInfoVendedor->nextInfoVendedor;
		}aInfoVendedor = oInfoVendedor;
	}break;
	case WM_COMMAND: {
		switch (LOWORD(wparam)) {
		case BTN_IMGIV: {
			OPENFILENAME ofn;
			ZeroMemory(&ofn, sizeof(OPENFILENAME));

			ofn.hwndOwner = hwnd;
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.lpstrFile = fotoVendedor;
			ofn.nMaxFile = MAX_PATH;
			ofn.lpstrDefExt = "txt";
			ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR;
			ofn.lpstrFilter = "Imagenes bmp\0*.bmp";

			if (GetOpenFileName(&ofn)) {
				HBITMAP hbFotoInfo = (HBITMAP)LoadImage(NULL, fotoVendedor, IMAGE_BITMAP, 600, 400, LR_LOADFROMFILE);
				HWND hPbFotoInfo = GetDlgItem(hwnd, PB_FOTO);
				SendMessage(hPbFotoInfo, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbFotoInfo);
			}
			else {
				MessageBox(hwnd, "No ha seleccionado ninguna imagen", "NO CARGAR", MB_ICONASTERISK);
				break;
			}
		}break;
		case BTN_CONFIRMIV: {
			string nombreCompleto = getText(hTxtNombreCompleto);
			string aliasEmpresa = getText(hTxtEmpresa);

			//Para validar que no haya numeros
			int taker = nombreCompleto.size();
			for (int i = 0; i < taker; i++) {
				if (isalpha(nombreCompleto[i])) { //Recorre la cadena en busca de algún caracter que sea un numero
					isNumber = true;
				}
				else {
					string spaceBar;
					spaceBar = nombreCompleto[i];
					if (spaceBar.compare(" ") == 0) {
						isNumber = true;
					}
					else {
						isNumber = false;
						MessageBox(NULL, "Su nombre no puede contener numeros", "Informacion del vendedor", MB_ICONASTERISK);

					}
				}
			}

			//Para convertir a maýusculas
			int aliasMayus = aliasEmpresa.size();
			for (int j = 0; j < aliasMayus; j++) {
				aliasEmpresa[j] = toupper(aliasEmpresa[j]);
			}

			if (nombreCompleto.compare("") == 0 || aliasEmpresa.compare("") == 0) {
				MessageBox(NULL, "Falto llenar la informacion", "NO ALTA", MB_ICONASTERISK);
				break;
			}

			string fotoVend(fotoVendedor);
			if (isNumber = true && nombreCompleto.compare("") == 1 && aliasEmpresa.compare("") == 1 && fotoVend.compare("") == 1) {
				if (oInfoVendedor == NULL) {
					oInfoVendedor = new InfoVendedor;
					oInfoVendedor->nombreVendedor = nombreCompleto;
					oInfoVendedor->aliasDeLaEmpresa = aliasEmpresa;
					for (int i = 0; i < MAX_PATH; i++) {
						oInfoVendedor->imagen[i] = fotoVendedor[i];
					}
					oInfoVendedor->IDUser = userAccess->IDUser;
					oInfoVendedor->nextInfoVendedor = NULL;
					oInfoVendedor->prevInfoVendedor = NULL;
					aInfoVendedor = oInfoVendedor;
				}
				else {
					bool existeVendedor = false;
					do {
						if (aInfoVendedor->IDUser == userAccess->IDUser) {
							existeVendedor = true;
							break;
						}
						else {
							aInfoVendedor = aInfoVendedor->nextInfoVendedor;
						}
					} while (aInfoVendedor != NULL);

					aInfoVendedor = oInfoVendedor;
					if (existeVendedor == false) {
						while (aInfoVendedor->nextInfoVendedor != NULL) {
							aInfoVendedor = aInfoVendedor->nextInfoVendedor;
						}
						aInfoVendedor->nextInfoVendedor = new InfoVendedor;
						aInfoVendedor->nextInfoVendedor->prevInfoVendedor = aInfoVendedor;
						aInfoVendedor = aInfoVendedor->nextInfoVendedor;
						aInfoVendedor->nombreVendedor = nombreCompleto;
						aInfoVendedor->aliasDeLaEmpresa = aliasEmpresa;
						for (int i = 0; i < MAX_PATH; i++) {
							aInfoVendedor->imagen[i] = fotoVendedor[i];
						}
						aInfoVendedor->IDUser = userAccess->IDUser;
						aInfoVendedor->nextInfoVendedor = NULL;
						aInfoVendedor = oInfoVendedor;
					}
					else {
						do {
							if (aInfoVendedor->IDUser == userAccess->IDUser) {
								break;
							}
							else {
								aInfoVendedor = aInfoVendedor->nextInfoVendedor;
							}
						} while (aInfoVendedor != NULL);
						aInfoVendedor->nombreVendedor = nombreCompleto;
						aInfoVendedor->aliasDeLaEmpresa = aliasEmpresa;
						for (int i = 0; i < MAX_PATH; i++) {
							aInfoVendedor->imagen[i] = fotoVendedor[i];
						}
					}

				}

				aInfoVendedor = oInfoVendedor;
				saveInfoVendedor(aInfoVendedor);

				HWND hEnvios = CreateDialog(hGInstance, MAKEINTRESOURCE(IDD_ENVIOS), NULL, fEnvios);
				ShowWindow(hEnvios, SW_SHOW);
				exitProgramInfo = false;
				DestroyWindow(hwnd);
			}
		}break;

		case ID_SALIR: {
			int m_result = DialogBox(hGInstance, MAKEINTRESOURCE(IDD_MODAL), hwnd, fModal);
		}break;
		case ID_ENVIOS: {
			HWND hEnvios = CreateDialog(hGInstance, MAKEINTRESOURCE(IDD_ENVIOS), NULL, fEnvios);
			ShowWindow(hEnvios, SW_SHOW);
			exitProgramEnvios = false;
			DestroyWindow(hwnd);
		}break;
		case ID_PRODUCTOS_VER: {
			HWND hProducts = CreateDialog(hGInstance, MAKEINTRESOURCE(IDD_PRODUCTOS), NULL, fProductos);
			ShowWindow(hProducts, SW_SHOW);
			exitProgramInfo = false;
			DestroyWindow(hwnd);
		}break;
		case ID_WATCHINFO: MessageBox(NULL, "Usted ya se encuentra en esta ventana", "Menu", MB_ICONQUESTION);
			break;
		}
	}break;
	case WM_CLOSE:
		exitProgramInfo = true;
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		if (exitProgramInfo) {
			PostQuitMessage(117);
		}
		break;
	}
	return FALSE;
}

BOOL CALLBACK fModal(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	switch (msg) {
	case WM_COMMAND: {
		switch (LOWORD(wparam)) {
		case BTN_CANCEL: {
			EndDialog(hwnd, 16);
			break;
		}
		case BTN_EXIT: {
			userAccess = NULL;
			freeMemoryUser();
			freeMemoryInfoVendedor();
			freeMemoryProductos();
			freeMemoryEnvios();
			PostQuitMessage(16);
		}break;
		}
	}break;
	case WM_CLOSE:
		KillTimer(hwnd, MI_TIMER);
		EndDialog(hwnd, 16);
		break;
	case WM_DESTROY:
		break;
	}
	return FALSE;
}

BOOL CALLBACK fProductos(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	switch (msg) {
	case WM_INITDIALOG: {
		hMenu = LoadMenu(hGInstance, MAKEINTRESOURCE(IDR_MENU1));
		SetMenu(hwnd, hMenu);
		hTxtNombreProducto = GetDlgItem(hwnd, TXT_NDP);
		hTxtCantidadEnInventario = GetDlgItem(hwnd, TXT_CEEI);
		hTxtCodigoProducto = GetDlgItem(hwnd, TXT_CDP);
		hTxtMarcaProducto = GetDlgItem(hwnd, TXT_MARCA);
		hTxtDescripcionProducto = GetDlgItem(hwnd, TXT_DESCRIPCION);
		hTxtPrecio = GetDlgItem(hwnd, TXT_PRECIO);
	}break;
	case WM_COMMAND: {
		switch (LOWORD(wparam)) {
		case BTN_MOSTRARPRODUCTOS: {
			HWND hShowProd = CreateDialog(hGInstance, MAKEINTRESOURCE(IDD_PRODUCTLIST), NULL, fMostrarProductos);
			ShowWindow(hShowProd, SW_SHOW);
			exitProgramMostrarP = false;
			DestroyWindow(hwnd);
		}break;
		case BTN_CARGARP2: {
			OPENFILENAME ofn;
			ZeroMemory(&ofn, sizeof(OPENFILENAME));

			ofn.hwndOwner = hwnd;
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.lpstrFile = fotoProducto2;
			ofn.nMaxFile = MAX_PATH;
			ofn.lpstrDefExt = "txt";
			ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR;
			ofn.lpstrFilter = "Imagenes bmp\0*.bmp";

			if (GetOpenFileName(&ofn)) {
				HBITMAP hbFotoInfo = (HBITMAP)LoadImage(NULL, fotoProducto2, IMAGE_BITMAP, 600, 400, LR_LOADFROMFILE);
				HWND hPbFotoInfo = GetDlgItem(hwnd, PB_FOTOP2);
				SendMessage(hPbFotoInfo, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbFotoInfo);
			}
			else {
				MessageBox(hwnd, "No ha seleccionado ninguna imagen", "NO CARGAR", MB_ICONASTERISK);
				break;
			}
		}break;
		case BTN_CARGARP1: {
			OPENFILENAME ofn;
			ZeroMemory(&ofn, sizeof(OPENFILENAME));

			ofn.hwndOwner = hwnd;
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.lpstrFile = fotoProducto1;
			ofn.nMaxFile = MAX_PATH;
			ofn.lpstrDefExt = "txt";
			ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR;
			ofn.lpstrFilter = "Imagenes bmp\0*.bmp";

			if (GetOpenFileName(&ofn)) {
				HBITMAP hbFotoInfo = (HBITMAP)LoadImage(NULL, fotoProducto1, IMAGE_BITMAP, 600, 400, LR_LOADFROMFILE);
				HWND hPbFotoInfo = GetDlgItem(hwnd, PB_FOTOP1);
				SendMessage(hPbFotoInfo, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbFotoInfo);

			}
			else {
				MessageBox(hwnd, "No ha seleccionado ninguna imagen", "NO CARGAR", MB_ICONASTERISK);
				break;
			}
		}break;
		case BTN_CONFIRMARPRODUCTO: {
			string nombreDelProducto = getText(hTxtNombreProducto);
			string cantidadEnInventario = getText(hTxtCantidadEnInventario);
			string codigoDelProducto = getText(hTxtCodigoProducto);
			string marcaDelProducto = getText(hTxtMarcaProducto);
			string descripcionDelProducto = getText(hTxtDescripcionProducto);
			string precio = getText(hTxtPrecio);


			strcpy(desc, descripcionDelProducto.c_str());

			string foto1(fotoProducto1);
			string foto2(fotoProducto2);

			if (nombreDelProducto.compare("") == 0 || cantidadEnInventario.compare("") == 0 || codigoDelProducto.compare("") == 0 || marcaDelProducto.compare("") == 0 || descripcionDelProducto.compare("") == 0 || foto1.compare("") == 0 || foto2.compare("") == 0 || precio.compare("") == 0) {
				MessageBox(NULL, "Falto llenar la informacion", "NO ALTA", MB_ICONASTERISK);
				break;
			}

			/*	if (oProducto != NULL) {
					bool found = true;
					while (aProducto->nombreProducto.compare(nombreDelProducto) != 0) {
						if (aProducto->nextProducto == NULL && aProducto->IDUser == userAccess->IDUser) {
							found = false;
							break;
						}
						aProducto = aProducto->nextProducto;
					}
					if (found == true) {
						MessageBox(NULL, "Ya hay un producto con ese nombre", "NO ALTA", MB_ICONASTERISK);
						aProducto = oProducto;
						break;
					}
					else
						aProducto = oProducto;
				}*/


			if (nombreDelProducto.compare("") == 1 && cantidadEnInventario.compare("") == 1 && codigoDelProducto.compare("") == 1 && descripcionDelProducto.compare("") == 1 && marcaDelProducto.compare("") == 1 && foto1.compare("") == 1 && foto2.compare("") == 1 && precio.compare("") == 1) {
				if (oProducto == NULL) {
					oProducto = new productos;
					oProducto->IDProducto = GLOBAL_PRODUCTO_ID++;
					oProducto->nombreProducto = nombreDelProducto;
					oProducto->cantidadProducto = cantidadEnInventario;
					oProducto->codigoProducto = codigoDelProducto;
					oProducto->marcaProducto = marcaDelProducto;
					for (int i = 0; i < MAX_PATH; i++) {
						oProducto->descripcionProducto[i] = desc[i];
						oProducto->fotoP1[i] = fotoProducto1[i];
						oProducto->fotoP2[i] = fotoProducto2[i];
					}
					oProducto->precioProducto = precio;
					oProducto->IDUser = userAccess->IDUser;
					oProducto->nextProducto = NULL;
					oProducto->prevProducto = NULL;
					aProducto = oProducto;
				}
				else {
					int productosDuplicados = 0;
					aProducto = oProducto;
					do {
						if (nombreDelProducto == aProducto->nombreProducto && aProducto->IDUser == userAccess->IDUser) {
							productosDuplicados++;
						}
						//if (aProducto->nextProducto!=NULL) {
						aProducto = aProducto->nextProducto;
						//}
					} while (aProducto != NULL);

					if (productosDuplicados >= 1) {
						MessageBox(NULL, "Usted ya tiene registrado un producto con ese nombre", "NO ALTA", MB_ICONASTERISK);
						break;
					}

					else {
						aProducto = oProducto;
						while (aProducto->nextProducto != NULL) {
							aProducto = aProducto->nextProducto;
						}
						aProducto->nextProducto = new productos;
						aProducto->IDProducto = GLOBAL_PRODUCTO_ID++;
						aProducto->nextProducto->prevProducto = aProducto;
						aProducto = aProducto->nextProducto;
						aProducto->nombreProducto = nombreDelProducto;
						aProducto->cantidadProducto = cantidadEnInventario;
						aProducto->codigoProducto = codigoDelProducto;
						aProducto->marcaProducto = marcaDelProducto;
						//aProducto->IDProducto = aProducto->prevProducto->IDProducto + 1;
						for (int i = 0; i < MAX_PATH; i++) {
							aProducto->descripcionProducto[i] = desc[i];
							aProducto->fotoP1[i] = fotoProducto1[i];
							aProducto->fotoP2[i] = fotoProducto2[i];
						}
						aProducto->precioProducto = precio;
						aProducto->IDUser = userAccess->IDUser;
						aProducto->nextProducto = NULL;
						aProducto = oProducto;
					}
				}
				aProducto = oProducto;
				saveProducto(aProducto);
				HWND hEnvios = CreateDialog(hGInstance, MAKEINTRESOURCE(IDD_ENVIOS), NULL, fEnvios);
				ShowWindow(hEnvios, SW_SHOW);
				exitProgramInfo = false;
				DestroyWindow(hwnd);
			}
		}break;
		case ID_SALIR: {
			int m_result = DialogBox(hGInstance, MAKEINTRESOURCE(IDD_MODAL), hwnd, fModal);
		}break;
		case ID_ENVIOS: {
			HWND hEnvios = CreateDialog(hGInstance, MAKEINTRESOURCE(IDD_ENVIOS), NULL, fEnvios);
			ShowWindow(hEnvios, SW_SHOW);
			exitProgramEnvios = false;
			DestroyWindow(hwnd);
		}break;
		case ID_PRODUCTOS_VER:
			MessageBox(NULL, "Usted ya se encuentra en esta ventana", "Menu", MB_ICONQUESTION); break;
		case ID_WATCHINFO: {
			HWND hInfo = CreateDialog(hGInstance, MAKEINTRESOURCE(IDD_INFOVENDEDOR), NULL, fInfoVendedor);
			ShowWindow(hInfo, SW_SHOW);
			exitProgramInfo = false;
			DestroyWindow(hwnd);
		}
		}
	}break;
	case WM_CLOSE:
		exitProgramProductos = true;
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		if (exitProgramProductos) {
			PostQuitMessage(117);
		}
		break;
	}
	return FALSE;
}

BOOL CALLBACK fMostrarProductos(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	switch (msg) {
	case WM_INITDIALOG: {
		hMenu = LoadMenu(hGInstance, MAKEINTRESOURCE(IDR_MENU1));
		SetMenu(hwnd, hMenu);

		HWND hLbProductos = GetDlgItem(hwnd, IDC_LIST1);
		char cProducto[50];
		int selIndex = (int)SendMessage(hLbProductos, LB_GETCURSEL, NULL, NULL);
		SendMessage(hLbProductos, LB_GETTEXT, (WPARAM)selIndex, (LPARAM)cProducto);

		aProducto = oProducto;

		while (aProducto != NULL) {
			if (aProducto->IDUser == userAccess->IDUser) {
				int indexProducto = SendMessage(hLbProductos, LB_ADDSTRING, 0, (LPARAM)aProducto->nombreProducto.c_str());
				SendMessage(hLbProductos, LB_SETITEMDATA, indexProducto, aProducto->IDProducto);
			}
			aProducto = aProducto->nextProducto;
		}aProducto = oProducto;

	}break;
	case WM_COMMAND: {
		HWND hLblCantidad = GetDlgItem(hwnd, LBL_CANT);
		//HWND hLbCodigo = GetDlgItem(hwnd, IDC_LIST2);
		//HWND hLbPrecio = GetDlgItem(hwnd, IDC_LIST5);
		//HWND hLbMarca = GetDlgItem(hwnd, IDC_LIST3);
		//char cCantidad[50];
		//char cCodigo[50];
		//char cPrecio[50];
		//char cMarca[50];
		//int selIndexCant = (int)SendMessage(hLbCantidad, LB_GETCURSEL, NULL, NULL);
		//int selIndexCode = (int)SendMessage(hLbCodigo, LB_GETCURSEL, NULL, NULL);
		//int selIndexPrecio = (int)SendMessage(hLbPrecio, LB_GETCURSEL, NULL, NULL);
		//int selIndexMarca = (int)SendMessage(hLbMarca, LB_GETCURSEL, NULL, NULL);
		//SendMessage(hLbCantidad, LB_GETTEXT, (WPARAM)selIndexCant, (LPARAM)cCantidad);
		//SendMessage(hLbCantidad, LB_SETCURSEL, (WPARAM)selIndexCant, (LPARAM)aProducto->marcaProducto.c_str());
		//SendMessage(hLbCodigo, LB_GETTEXT, (WPARAM)selIndexCode, (LPARAM)cCodigo);
		//SendMessage(hLbCodigo, LB_ADDSTRING, 0, (LPARAM)aProducto->codigoProducto.c_str());
		//SendMessage(hLbPrecio, LB_GETTEXT, (WPARAM)selIndexPrecio, (LPARAM)cPrecio);
		//SendMessage(hLbPrecio, LB_ADDSTRING, 0, (LPARAM)aProducto->precioProducto.c_str());
		//SendMessage(hLbMarca, LB_GETTEXT, (WPARAM)selIndexMarca, (LPARAM)cMarca);
		//SendMessage(hLbMarca, LB_ADDSTRING, 0, (LPARAM)aProducto->marcaProducto.c_str());
		switch (LOWORD(wparam)) {
		case BTN_FILTRAR: {
			hTxtFiltrar = GetDlgItem(hwnd, EB_FILTRAR);
			string filtrar = getText(hTxtFiltrar);
			HWND hLbProductos = GetDlgItem(hwnd, IDC_LIST1);
			SendMessage(hLbProductos, LB_RESETCONTENT, 0, 0);
			aProducto = oProducto;

			int productosEncontrados = 0;

			while (aProducto != NULL) {
				string productoMin = "";
				string filtrarMin = "";
				for (int i = 0; i < aProducto->nombreProducto.length(); i++) {
					productoMin.append(1, tolower(aProducto->nombreProducto[i]));
				}
				for (int i = 0; i < filtrar.length(); i++) {
					filtrarMin.append(1, tolower(filtrar[i]));
				}
				if (aProducto->IDUser == userAccess->IDUser && strstr(productoMin.c_str(), filtrarMin.c_str())) {
					int indexProducto = SendMessage(hLbProductos, LB_ADDSTRING, 0, (LPARAM)aProducto->nombreProducto.c_str());
					SendMessage(hLbProductos, LB_SETITEMDATA, indexProducto, aProducto->IDProducto);
					productosEncontrados++;
				}
				aProducto = aProducto->nextProducto;
			}aProducto = oProducto;

			if (productosEncontrados == 0) {
				MessageBox(NULL, "El producto que intentas filtrar no existe en esta lista", "SIN PRODUCTOS", MB_OK);
				while (aProducto != NULL) {
					if (aProducto->IDUser == userAccess->IDUser) {
						int indexProducto = SendMessage(hLbProductos, LB_ADDSTRING, 0, (LPARAM)aProducto->nombreProducto.c_str());
						SendMessage(hLbProductos, LB_SETITEMDATA, indexProducto, aProducto->IDProducto);
						productosEncontrados++;
					}
					aProducto = aProducto->nextProducto;
				}aProducto = oProducto;
			}

		}break;
		case IDC_LIST1: {
			if (HIWORD(wparam) == LBN_SELCHANGE) {

				if (oProducto == NULL)
					break;


				int index = SendDlgItemMessage(hwnd, IDC_LIST1, LB_GETCURSEL, NULL, NULL);
				int idProducto = SendDlgItemMessage(hwnd, IDC_LIST1, LB_GETITEMDATA, index, NULL);

				aProducto = oProducto;

				while (aProducto != NULL) {
					if (aProducto->IDProducto == idProducto)
						break;
					aProducto = aProducto->nextProducto;
				}

				if (aProducto == NULL)
					break;

				productos* aShow = aProducto;

				/*ShowWindow(GetDlgItem(hwnd, LBL_CANT), SW_SHOWDEFAULT);
				ShowWindow(GetDlgItem(hwnd, LBL_COD), SW_SHOWDEFAULT);
				ShowWindow(GetDlgItem(hwnd, LBL_PREC), SW_SHOWDEFAULT);
				ShowWindow(GetDlgItem(hwnd, LBL_MARC), SW_SHOWDEFAULT);
				ShowWindow(GetDlgItem(hwnd, LBL_DESC), SW_SHOWDEFAULT);*/

				SendDlgItemMessage(hwnd, LBL_CANT, WM_SETTEXT, NULL, (LPARAM)aShow->cantidadProducto.c_str());
				SendDlgItemMessage(hwnd, LBL_COD, WM_SETTEXT, NULL, (LPARAM)aShow->codigoProducto.c_str());
				SendDlgItemMessage(hwnd, LBL_PREC, WM_SETTEXT, NULL, (LPARAM)aShow->precioProducto.c_str());
				SendDlgItemMessage(hwnd, LBL_MARC, WM_SETTEXT, NULL, (LPARAM)aShow->marcaProducto.c_str());
				SendDlgItemMessage(hwnd, LBL_DESC, WM_SETTEXT, NULL, (LPARAM)aShow->descripcionProducto);
				HBITMAP hbFotoInfo = (HBITMAP)LoadImage(NULL, aShow->fotoP1, IMAGE_BITMAP, 600, 400, LR_LOADFROMFILE);
				HWND hPbFotoInfo = GetDlgItem(hwnd, PB_SHOWFOTOP1);
				SendMessage(hPbFotoInfo, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbFotoInfo);
				HBITMAP hbFotoInfo2 = (HBITMAP)LoadImage(NULL, aShow->fotoP2, IMAGE_BITMAP, 600, 400, LR_LOADFROMFILE);
				HWND hPbFotoInfo2 = GetDlgItem(hwnd, PB_SHOWFOTOP2);
				SendMessage(hPbFotoInfo2, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbFotoInfo2);
			}
		}break;
		case BTN_BORRARPRODUCTO: {

			if (aProducto->IDUser != userAccess->IDUser) {
				MessageBox(NULL, "No puedes borrar un producto que no es tuyo", "ATENCION", MB_OK);
				break;
			}

			SendDlgItemMessage(hwnd, LBL_CANT, WM_SETTEXT, NULL, (LPARAM)"");
			SendDlgItemMessage(hwnd, LBL_COD, WM_SETTEXT, NULL, (LPARAM)"");
			SendDlgItemMessage(hwnd, LBL_PREC, WM_SETTEXT, NULL, (LPARAM)"");
			SendDlgItemMessage(hwnd, LBL_MARC, WM_SETTEXT, NULL, (LPARAM)"");
			SendDlgItemMessage(hwnd, LBL_DESC, WM_SETTEXT, NULL, (LPARAM)"");
			HWND hLbProductos = GetDlgItem(hwnd, IDC_LIST1);
			char cProducto[50];
			int selIndex = (int)SendMessage(hLbProductos, LB_GETCURSEL, NULL, NULL);
			SendMessage(hLbProductos, LB_GETTEXT, (WPARAM)selIndex, (LPARAM)cProducto);

			if (oProducto == NULL) {
				MessageBox(NULL, "No hay productos en la lista", "SIN PRODUCTOS", MB_OK);
				break;
			}

			eliminarProducto();
			SendMessage(hLbProductos, LB_DELETESTRING, (WPARAM)selIndex, 0);
			HWND hPbFotoInfo2 = GetDlgItem(hwnd, PB_FOTOP2);
			HWND hPbFotoInfo1 = GetDlgItem(hwnd, PB_FOTOP1);
			SendMessage(hPbFotoInfo2, STM_SETIMAGE, IMAGE_BITMAP, NULL);
			SendMessage(hPbFotoInfo1, STM_SETIMAGE, IMAGE_BITMAP, NULL);
			saveProducto(aProducto);

		}break;

		case BTN_MODIFICAPRODUCTO: {
			/*HWND hLbProductos = GetDlgItem(hwnd, IDC_LIST1);
			char cProducto[50];
			int selIndex = (int)SendMessage(hLbProductos, LB_GETCURSEL, NULL, NULL);
			SendMessage(hLbProductos, LB_GETTEXT, (WPARAM)selIndex, (LPARAM)cProducto);
			if (oProducto == NULL) {
				MessageBox(NULL, "No hay productos en la lista", "SIN PRODUCTOS", MB_OK);
				break;
			}
			if (aProducto->nextProducto == NULL && aProducto->prevProducto == NULL) {
				delete aProducto;
				SendMessage(hLbProductos, LB_DELETESTRING, (WPARAM)selIndex, 0);
				oProducto = aProducto = NULL;
			}
			else if (aProducto->nextProducto == NULL) {
				aProducto->prevProducto->nextProducto = NULL;
				delete aProducto;
				SendMessage(hLbProductos, LB_DELETESTRING, (WPARAM)selIndex, 0);
				aProducto = oProducto;
			}
			else if (aProducto->prevProducto == NULL) {
				oProducto = oProducto->nextProducto;
				oProducto->prevProducto = NULL;
				delete aProducto;
				SendMessage(hLbProductos, LB_DELETESTRING, (WPARAM)selIndex, 0);
				aProducto = oProducto;
			}
			else {
				aProducto->prevProducto->nextProducto = aProducto->nextProducto;
				aProducto->nextProducto->prevProducto = aProducto->prevProducto;
				delete aProducto;
				SendMessage(hLbProductos, LB_DELETESTRING, (WPARAM)selIndex, 0);
				aProducto = oProducto;
			}*/

			if (aProducto->IDUser != userAccess->IDUser) {
				MessageBox(NULL, "No puedes editar un producto que no es tuyo", "ATENCION", MB_OK);
				break;
			}

			HWND hModificaProductos = CreateDialog(hGInstance, MAKEINTRESOURCE(IDD_MODIFYPROD), NULL, fModificarProductos);
			ShowWindow(hModificaProductos, SW_SHOW);
			exitProgramModificaP = false;
			DestroyWindow(hwnd);
		}break;
		case ID_SALIR: {
			int m_result = DialogBox(hGInstance, MAKEINTRESOURCE(IDD_MODAL), hwnd, fModal);
		}break;
		case ID_ENVIOS: {
			HWND hEnvios = CreateDialog(hGInstance, MAKEINTRESOURCE(IDD_ENVIOS), NULL, fEnvios);
			ShowWindow(hEnvios, SW_SHOW);
			exitProgramEnvios = false;
			DestroyWindow(hwnd);
		}break;
		case ID_PRODUCTOS_VER: {
			HWND hProducts = CreateDialog(hGInstance, MAKEINTRESOURCE(IDD_PRODUCTOS), NULL, fProductos);
			ShowWindow(hProducts, SW_SHOW);
			exitProgramInfo = false;
			DestroyWindow(hwnd);
		}break;
		case ID_WATCHINFO: {
			HWND hInfo = CreateDialog(hGInstance, MAKEINTRESOURCE(IDD_INFOVENDEDOR), NULL, fInfoVendedor);
			ShowWindow(hInfo, SW_SHOW);
			exitProgramInfo = false;
			DestroyWindow(hwnd);
		}break;
		}
	}break;
	case WM_CLOSE:
		exitProgram = true;
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		if (exitProgram) {
			//freeMemoryUser();
			PostQuitMessage(16);
		}
		break;
	}
	return FALSE;
}

BOOL CALLBACK fModificarProductos(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	switch (msg) {
	case WM_INITDIALOG: {
		hMenu = LoadMenu(hGInstance, MAKEINTRESOURCE(IDR_MENU1));
		SetMenu(hwnd, hMenu);
		hTxtChangeProdNombre = GetDlgItem(hwnd, TXT_NDPM);
		hTxtChangeProdCantidad = GetDlgItem(hwnd, TXT_CEEIM);
		hTxtChangeProdCodigo = GetDlgItem(hwnd, TXT_CDPM);
		hTxtChangeProdMarca = GetDlgItem(hwnd, TXT_MARCAM);
		hTxtChangeProdDescripcion = GetDlgItem(hwnd, TXT_DESCRIPCIONM);
		hTxtChangeProdPrecio = GetDlgItem(hwnd, TXT_PRECIOM);
		//while (aProducto != NULL) {
			//isFull = true;
			//if (aProducto->IDUser == userAccess->IDUser) {
		SetWindowText(hTxtChangeProdNombre, aProducto->nombreProducto.c_str());
		SetWindowText(hTxtChangeProdCantidad, aProducto->cantidadProducto.c_str());
		SetWindowText(hTxtChangeProdCodigo, aProducto->codigoProducto.c_str());
		SetWindowText(hTxtChangeProdMarca, aProducto->marcaProducto.c_str());
		SetWindowText(hTxtChangeProdPrecio, aProducto->precioProducto.c_str());
		SetWindowText(hTxtChangeProdDescripcion, aProducto->descripcionProducto);
		/*for (int i = 0; i < MAX_PATH; i++) {
			desc[i] = aProducto->descripcionProducto[i];
			fotoProducto1[i] = aProducto->fotoP1[i];
			fotoProducto2[i] = aProducto->fotoP2[i];
		}*/
		HBITMAP hbFotoP1 = (HBITMAP)LoadImage(NULL, aProducto->fotoP1, IMAGE_BITMAP, 600, 400, LR_LOADFROMFILE);
		HWND hPbFotoP1 = GetDlgItem(hwnd, PB_FOTOP1M);
		SendMessage(hPbFotoP1, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbFotoP1);
		HBITMAP hbFotoP2 = (HBITMAP)LoadImage(NULL, aProducto->fotoP2, IMAGE_BITMAP, 600, 400, LR_LOADFROMFILE);
		HWND hPbFotoP2 = GetDlgItem(hwnd, PB_FOTOP2M);
		SendMessage(hPbFotoP2, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbFotoP2);
		//	}
			//aProducto = aProducto->nextProducto;
		//}aProducto = oProducto;
	}break;
	case WM_COMMAND: {
		switch (LOWORD(wparam)) {
		case BTN_CONFIRMARCAMBIO: {

			string nombreDelProducto = getText(hTxtChangeProdNombre);
			int productosDuplicados = 0;
			productos* auxProducto = aProducto;
			aProducto = oProducto;
			do {
				if (nombreDelProducto == aProducto->nombreProducto && aProducto->IDUser == userAccess->IDUser) {
					productosDuplicados++;
				}
				//if (aProducto->nextProducto!=NULL) {
				aProducto = aProducto->nextProducto;
				//}
			} while (aProducto != NULL);

			if (productosDuplicados >= 1) {
				MessageBox(NULL, "Usted ya tiene registrado un producto con ese nombre", "NO ALTA", MB_ICONASTERISK);
				break;
			}
			aProducto = auxProducto;
			aProducto->nombreProducto = nombreDelProducto;
			aProducto->cantidadProducto = getText(hTxtChangeProdCantidad);
			aProducto->codigoProducto = getText(hTxtChangeProdCodigo);
			aProducto->marcaProducto = getText(hTxtChangeProdMarca);
			string descripcion = getText(hTxtChangeProdDescripcion);
			strcpy(aProducto->descripcionProducto, descripcion.c_str());
			for (int i = 0; i < MAX_PATH; i++) { //jugar con la linea 977-985
				//aProducto->descripcionProducto[i] = descripcion[i];
				if (fotoProducto1[0] != 0 && fotoProducto1 != "")
					aProducto->fotoP1[i] = fotoProducto1[i];
				if (fotoProducto2[0] != 0 && fotoProducto2 != "")
					aProducto->fotoP2[i] = fotoProducto2[i];
			}
			fotoProducto1[0] = 0;
			fotoProducto2[0] = 0;
			aProducto->precioProducto = getText(hTxtChangeProdPrecio);
			aProducto = oProducto;
			saveProducto(aProducto);
			HWND hShowProd = CreateDialog(hGInstance, MAKEINTRESOURCE(IDD_PRODUCTLIST), NULL, fMostrarProductos);
			ShowWindow(hShowProd, SW_SHOW);
			exitProgramMostrarP = false;
			DestroyWindow(hwnd);
		}break;
		case BTN_CARGARP1M: {
			OPENFILENAME ofn;
			ZeroMemory(&ofn, sizeof(OPENFILENAME));

			ofn.hwndOwner = hwnd;
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.lpstrFile = fotoProducto1;
			ofn.nMaxFile = MAX_PATH;
			ofn.lpstrDefExt = "txt";
			ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR;
			ofn.lpstrFilter = "Imagenes bmp\0*.bmp";

			if (GetOpenFileName(&ofn)) {
				HBITMAP hbFotoPP1 = (HBITMAP)LoadImage(NULL, fotoProducto1, IMAGE_BITMAP, 600, 400, LR_LOADFROMFILE);
				HWND hPbFotoPP1 = GetDlgItem(hwnd, PB_FOTOP1M);
				SendMessage(hPbFotoPP1, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbFotoPP1);
			}
			else {
				MessageBox(hwnd, "No ha seleccionado ninguna imagen", "NO CARGAR", MB_ICONASTERISK);
				break;
			}
		}break;
		case BTN_CARGARP2M: {
			OPENFILENAME ofn;
			ZeroMemory(&ofn, sizeof(OPENFILENAME));

			ofn.hwndOwner = hwnd;
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.lpstrFile = fotoProducto2;
			ofn.nMaxFile = MAX_PATH;
			ofn.lpstrDefExt = "txt";
			ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR;
			ofn.lpstrFilter = "Imagenes bmp\0*.bmp";

			if (GetOpenFileName(&ofn)) {
				HBITMAP hbFotoPP2 = (HBITMAP)LoadImage(NULL, fotoProducto2, IMAGE_BITMAP, 600, 400, LR_LOADFROMFILE);
				HWND hPbFotoPP2 = GetDlgItem(hwnd, PB_FOTOP2M);
				SendMessage(hPbFotoPP2, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbFotoPP2);
			}
			else {
				MessageBox(hwnd, "No ha seleccionado ninguna imagen", "NO CARGAR", MB_ICONASTERISK);
				break;
			}
		}break;
		case ID_SALIR: {
			int m_result = DialogBox(hGInstance, MAKEINTRESOURCE(IDD_MODAL), hwnd, fModal);
		}break;
		case ID_ENVIOS: {
			HWND hEnvios = CreateDialog(hGInstance, MAKEINTRESOURCE(IDD_ENVIOS), NULL, fEnvios);
			ShowWindow(hEnvios, SW_SHOW);
			exitProgramEnvios = false;
			DestroyWindow(hwnd);
		}break;
		case ID_PRODUCTOS_VER: {
			HWND hProducts = CreateDialog(hGInstance, MAKEINTRESOURCE(IDD_PRODUCTOS), NULL, fProductos);
			ShowWindow(hProducts, SW_SHOW);
			exitProgramInfo = false;
			DestroyWindow(hwnd);
		}break;
		case ID_WATCHINFO: {
			HWND hInfo = CreateDialog(hGInstance, MAKEINTRESOURCE(IDD_INFOVENDEDOR), NULL, fInfoVendedor);
			ShowWindow(hInfo, SW_SHOW);
			exitProgramInfo = false;
			DestroyWindow(hwnd);
		}break;
		}
	}break;
	case WM_CLOSE:
		exitProgram = true;
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		if (exitProgram) {
			//freeMemoryUser();
			PostQuitMessage(16);
		}
		break;
	}
	return FALSE;
}

BOOL CALLBACK fEnvios(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	switch (msg) {
	case WM_INITDIALOG: {
		hMenu = LoadMenu(hGInstance, MAKEINTRESOURCE(IDR_MENU1));
		SetMenu(hwnd, hMenu);

		HWND hLbEnviosResumen = GetDlgItem(hwnd, LB_ENVIOSDELETEMODIFY);
		char cEnvios[50];
		int selIndex = (int)SendMessage(hLbEnviosResumen, LB_GETCURSEL, NULL, NULL);
		SendMessage(hLbEnviosResumen, LB_GETTEXT, (WPARAM)selIndex, (LPARAM)cEnvios);

		aEnvios = oEnvios;


		while (aEnvios != NULL) {
			if (aEnvios->IDUser == userAccess->IDUser) {

				int indexEnvio = SendMessage(hLbEnviosResumen, LB_ADDSTRING, 0, (LPARAM)aEnvios->obtenerNombreEnvio().c_str());
				SendMessage(hLbEnviosResumen, LB_SETITEMDATA, indexEnvio, aEnvios->IDEnvio);
				contadorEnvios++;
			}
			aEnvios = aEnvios->nextEnvio;
		}aEnvios = oEnvios;

	}break;
	case WM_COMMAND: {
		switch (LOWORD(wparam)) {
		case LB_ENVIOSDELETEMODIFY: {
			if (HIWORD(wparam) == LBN_SELCHANGE) {

				if (oEnvios == NULL)
					break;


				int index = SendDlgItemMessage(hwnd, LB_ENVIOSDELETEMODIFY, LB_GETCURSEL, NULL, NULL);
				int idEnvio = SendDlgItemMessage(hwnd, LB_ENVIOSDELETEMODIFY, LB_GETITEMDATA, index, NULL);

				aEnvios = oEnvios;

				while (aEnvios != NULL) {
					if (aEnvios->IDEnvio == idEnvio)
						break;
					aEnvios = aEnvios->nextEnvio;
				}

				if (aEnvios == NULL)
					break;

				Envios* aShow = aEnvios;
			}
		}break;
		case BTN_DELETEENV: {

			if (contadorEnvios == 1) {
				MessageBox(NULL, "No hay ningun envio para borrar", "Menu", MB_ICONQUESTION);
				break;
			}

			if (aEnvios == NULL) {
				break;
			}

			if (oEnvios == NULL) {
				MessageBox(NULL, "No hay envios en la lista", "SIN PRODUCTOS", MB_OK);
				break;
			}

			if (aEnvios->status == "Enviado") {
				MessageBox(NULL, "No puedes eliminar un envio ya enviado", "ADVERTENCIA", MB_ICONQUESTION);
				break;
			}

			if (MessageBox(NULL, "¿Esta seguro que desea eliminar este envio?", "ADVERTENCIA", MB_OKCANCEL) == IDCANCEL) {
				break;
			}

			HWND hLbEnviosResumen = GetDlgItem(hwnd, LB_ENVIOSDELETEMODIFY);
			int index = SendDlgItemMessage(hwnd, LB_ENVIOSDELETEMODIFY, LB_GETCURSEL, NULL, NULL);
			SendMessage(hLbEnviosResumen, LB_DELETESTRING, (WPARAM)index, 0);

			if (aEnvios->nextEnvio == NULL && aEnvios->prevEnvio == NULL) {
				delete aEnvios;
				oEnvios = aEnvios = NULL;
			}

			else if (aEnvios->nextEnvio == NULL) {
				aEnvios->prevEnvio->nextEnvio = NULL;
				delete aEnvios;
				aEnvios = oEnvios;
			}
			else if (aEnvios->prevEnvio == NULL) {
				oEnvios = oEnvios->nextEnvio;
				oEnvios->prevEnvio = NULL;
				delete aEnvios;
				aEnvios = oEnvios;
			}
			else {
				aEnvios->prevEnvio->nextEnvio = aEnvios->nextEnvio;
				aEnvios->nextEnvio->prevEnvio = aEnvios->prevEnvio;
				delete aEnvios;
				aEnvios = oEnvios;
			}
			saveEnvios(aEnvios);

		}break;
		case BTN_MODIFYENV: {
			if (contadorEnvios == 1) {
				MessageBox(NULL, "No hay ningun envio para modificar", "Menu", MB_ICONQUESTION);
				break;
			}
			contadorEnvios = 1;
			HWND hModEnv = CreateDialog(hGInstance, MAKEINTRESOURCE(IDD_MODIFY), NULL, fModify);
			ShowWindow(hModEnv, SW_SHOW);
			exitProgramModify = false;
			DestroyWindow(hwnd);
		}break;
		case BTN_MOSTRARENVIO: {

			if (contadorEnvios == 1) {
				MessageBox(NULL, "No hay ningun envio para mostrar", "Menu", MB_ICONQUESTION);
				break;
			}
			contadorEnvios = 1;
			HWND hResEnv = CreateDialog(hGInstance, MAKEINTRESOURCE(IDD_RESUMENENVIO), NULL, fResumenEnvio);
			ShowWindow(hResEnv, SW_SHOW);
			exitProgramResumen = false;
			DestroyWindow(hwnd);
		}break;
		case BTN_NUEVOENVIO: {
			contadorEnvios = 1;
			/*HWND hResEnv = CreateDialog(hGInstance, MAKEINTRESOURCE(IDD_RESUMENENVIO), NULL, fResumenEnvio);
			ShowWindow(hResEnv, SW_SHOW);
			exitProgramResumen = false;
			DestroyWindow(hwnd);*/
			/*	HWND hAltaEnv = CreateDialog(hGInstance, MAKEINTRESOURCE(IDD_ALTA), NULL, fAltaEnvio);
				ShowWindow(hAltaEnv, SW_SHOW);
				exitProgramAltaEnvios = false;
				DestroyWindow(hwnd);*/
			HWND hResEnv = CreateDialog(hGInstance, MAKEINTRESOURCE(IDD_ALTA), NULL, faltaEnviosNew);
			ShowWindow(hResEnv, SW_SHOW);
			exitProgramResumen = false;
			DestroyWindow(hwnd);
		}break;
		case ID_SALIR: {
			int m_result = DialogBox(hGInstance, MAKEINTRESOURCE(IDD_MODAL), hwnd, fModal);
		}break;
		case ID_ENVIOS: MessageBox(NULL, "Usted ya se encuentra en esta ventana", "Menu", MB_ICONQUESTION); break;
		case ID_PRODUCTOS_VER: {
			contadorEnvios = 1;
			HWND hProducts = CreateDialog(hGInstance, MAKEINTRESOURCE(IDD_PRODUCTOS), NULL, fProductos);
			ShowWindow(hProducts, SW_SHOW);
			exitProgramInfo = false;
			DestroyWindow(hwnd);
		}break;
		case ID_WATCHINFO: {
			contadorEnvios = 1;
			HWND hInfo = CreateDialog(hGInstance, MAKEINTRESOURCE(IDD_INFOVENDEDOR), NULL, fInfoVendedor);
			ShowWindow(hInfo, SW_SHOW);
			exitProgramInfo = false;
			DestroyWindow(hwnd);
		}
		}
	}break;
	case WM_CLOSE:
		contadorEnvios = 1;
		exitProgramEnvios = true;
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		contadorEnvios = 1;
		if (exitProgramEnvios) {
			PostQuitMessage(117);
		}
		break;
	}

	return FALSE;
}

BOOL CALLBACK fResumenEnvio(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	switch (msg) {
	case WM_INITDIALOG: {
		hMenu = LoadMenu(hGInstance, MAKEINTRESOURCE(IDR_MENU1));
		SetMenu(hwnd, hMenu);

		HWND hLbEnvios = GetDlgItem(hwnd, IDC_LIST11);
		char cEnvios[50];
		int selIndex = (int)SendMessage(hLbEnvios, LB_GETCURSEL, NULL, NULL);
		SendMessage(hLbEnvios, LB_GETTEXT, (WPARAM)selIndex, (LPARAM)cEnvios);
		hTxtChangeEnvioFecha = GetDlgItem(hwnd, LBL_ENVFECHAM);

		aEnvios = oEnvios;

		int contador = 1;
		while (aEnvios != NULL) {
			if (aEnvios->IDUser == userAccess->IDUser) {
				int indexEnvio = SendMessage(hLbEnvios, LB_ADDSTRING, 0, (LPARAM)aEnvios->obtenerNombreEnvio().c_str());
				SendMessage(hLbEnvios, LB_SETITEMDATA, indexEnvio, aEnvios->IDEnvio);
				contador++;
			}
			aEnvios = aEnvios->nextEnvio;
		}aEnvios = oEnvios;

	}break;
	case WM_COMMAND: {
		switch (LOWORD(wparam)) {
		case BTN_FILTRARFECHA: {
			hTxtFiltrarFecha = GetDlgItem(hwnd, DTP_FILTRAR);
			string filtrarFecha = getText(hTxtFiltrarFecha);
			HWND hLbEnvios = GetDlgItem(hwnd, IDC_LIST11);
			SendMessage(hLbEnvios, LB_RESETCONTENT, 0, 0);
			aEnvios = oEnvios;

			int enviosEncontrados = 0;

			while (aEnvios != NULL) {
				if (aEnvios->IDUser == userAccess->IDUser && aEnvios->fechastr==filtrarFecha) {
					int indexEnvios = SendMessage(hLbEnvios, LB_ADDSTRING, 0, (LPARAM)aEnvios->obtenerNombreEnvio().c_str());
					SendMessage(hLbEnvios, LB_SETITEMDATA, indexEnvios, aEnvios->IDEnvio);
					enviosEncontrados++;
				}
				aEnvios = aEnvios->nextEnvio;
			}aEnvios = oEnvios;

			if (enviosEncontrados == 0) {
				MessageBox(NULL, "El envio que intentas filtrar no existe en esta lista", "SIN ENVIOS", MB_OK);
				while (aEnvios != NULL) {
					if (aEnvios->IDUser == userAccess->IDUser) {
						int indexEnvios = SendMessage(hLbEnvios, LB_ADDSTRING, 0, (LPARAM)aEnvios->obtenerNombreEnvio().c_str());
						SendMessage(hLbEnvios, LB_SETITEMDATA, indexEnvios, aEnvios->IDEnvio);
						enviosEncontrados++;
					}
					aEnvios = aEnvios->nextEnvio;
				}aEnvios = oEnvios;
			}
		}break;
		case IDC_LIST11: {
			if (HIWORD(wparam) == LBN_SELCHANGE) {

				if (oEnvios == NULL)
					break;


				int index = SendDlgItemMessage(hwnd, IDC_LIST11, LB_GETCURSEL, NULL, NULL);
				int idEnvio = SendDlgItemMessage(hwnd, IDC_LIST11, LB_GETITEMDATA, index, NULL);

				aEnvios = oEnvios;

				while (aEnvios != NULL) {
					if (aEnvios->IDEnvio == idEnvio)
						break;
					aEnvios = aEnvios->nextEnvio;
				}

				if (aEnvios == NULL)
					break;

				Envios* aShow = aEnvios;

				/*ShowWindow(GetDlgItem(hwnd, LBL_CANT), SW_SHOWDEFAULT);
				ShowWindow(GetDlgItem(hwnd, LBL_COD), SW_SHOWDEFAULT);
				ShowWindow(GetDlgItem(hwnd, LBL_PREC), SW_SHOWDEFAULT);
				ShowWindow(GetDlgItem(hwnd, LBL_MARC), SW_SHOWDEFAULT);
				ShowWindow(GetDlgItem(hwnd, LBL_DESC), SW_SHOWDEFAULT);*/

				SendDlgItemMessage(hwnd, LBL_PROD1, WM_SETTEXT, NULL, (LPARAM)aShow->productoAEnviar.c_str());
				SendDlgItemMessage(hwnd, LBL_CANT1, WM_SETTEXT, NULL, (LPARAM)aShow->cantidad.c_str());
				SendDlgItemMessage(hwnd, LBL_PREC1, WM_SETTEXT, NULL, (LPARAM)aShow->precio.c_str());
				SendDlgItemMessage(hwnd, LBL_ST1, WM_SETTEXT, NULL, (LPARAM)aShow->calle.c_str());
				SendDlgItemMessage(hwnd, LBL_COL1, WM_SETTEXT, NULL, (LPARAM)aShow->colonia.c_str());
				SendDlgItemMessage(hwnd, LBL_CIUDAD1, WM_SETTEXT, NULL, (LPARAM)aShow->ciudad.c_str());
				SendDlgItemMessage(hwnd, LBL_EDO1, WM_SETTEXT, NULL, (LPARAM)aShow->estado.c_str());
				SendDlgItemMessage(hwnd, LBL_MSJ1, WM_SETTEXT, NULL, (LPARAM)aShow->mensaje.c_str());
				SendDlgItemMessage(hwnd, LBL_ENVFECHA, WM_SETTEXT, NULL, (LPARAM)aShow->fechastr.c_str());
				SendDlgItemMessage(hwnd, LBL_STATUSENV, WM_SETTEXT, NULL, (LPARAM)aShow->status.c_str());
				/*HBITMAP hbFotoInfo = (HBITMAP)LoadImage(NULL, aShow->fotoP1, IMAGE_BITMAP, 600, 400, LR_LOADFROMFILE);
				HWND hPbFotoInfo = GetDlgItem(hwnd, PB_SHOWFOTOP1);
				SendMessage(hPbFotoInfo, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbFotoInfo);
				HBITMAP hbFotoInfo2 = (HBITMAP)LoadImage(NULL, aShow->fotoP2, IMAGE_BITMAP, 600, 400, LR_LOADFROMFILE);
				HWND hPbFotoInfo2 = GetDlgItem(hwnd, PB_SHOWFOTOP2);
				SendMessage(hPbFotoInfo2, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbFotoInfo2);*/
			}

		}break;
		case ID_SALIR: {
			int m_result = DialogBox(hGInstance, MAKEINTRESOURCE(IDD_MODAL), hwnd, fModal);
		}break;
		case ID_ENVIOS: {
			HWND hEnvios = CreateDialog(hGInstance, MAKEINTRESOURCE(IDD_ENVIOS), NULL, fEnvios);
			ShowWindow(hEnvios, SW_SHOW);
			exitProgramEnvios = false;
			DestroyWindow(hwnd);
		}break;
		case ID_PRODUCTOS_VER: {
			HWND hProducts = CreateDialog(hGInstance, MAKEINTRESOURCE(IDD_PRODUCTOS), NULL, fProductos);
			ShowWindow(hProducts, SW_SHOW);
			exitProgramInfo = false;
			DestroyWindow(hwnd);
		}break;
		case ID_WATCHINFO: {
			HWND hInfo = CreateDialog(hGInstance, MAKEINTRESOURCE(IDD_INFOVENDEDOR), NULL, fInfoVendedor);
			ShowWindow(hInfo, SW_SHOW);
			exitProgramInfo = false;
			DestroyWindow(hwnd);
		}break;
		}
	}break;
	case WM_CLOSE:
		exitProgram = true;
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		if (exitProgram) {
			//freeMemoryUser();
			PostQuitMessage(16);
		}
		break;
	}
	return FALSE;
}

BOOL CALLBACK fModify(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	switch (msg) {
	case WM_INITDIALOG: {
		hMenu = LoadMenu(hGInstance, MAKEINTRESOURCE(IDR_MENU1));
		SetMenu(hwnd, hMenu);

		hTxtChangeEnvioProd = GetDlgItem(hwnd, LBL_ENVPROD);
		hTxtChangeEnvioCant = GetDlgItem(hwnd, LBL_ENVCANT);
		hTxtChangeEnvioPrec = GetDlgItem(hwnd, LBL_ENVMONTO);
		hTxtChangeEnvioCalle = GetDlgItem(hwnd, TXT_ENVCALLEM);
		hTxtChangeEnvioColonia = GetDlgItem(hwnd, TXT_ENVCOLM);
		hTxtChangeEnvioCiudad = GetDlgItem(hwnd, TXT_ENVCIUM);
		hTxtChangeEnvioEstado = GetDlgItem(hwnd, TXT_ENVESTM);
		hTxtChangeEnvioMensaje = GetDlgItem(hwnd, LBL_ENVMSJM);
		hTxtChangeEnvioFecha = GetDlgItem(hwnd, LBL_ENVFECHAM);

		SetWindowText(hTxtChangeEnvioProd, aEnvios->productoAEnviar.c_str());
		SetWindowText(hTxtChangeEnvioCant, aEnvios->cantidad.c_str());
		SetWindowText(hTxtChangeEnvioPrec, aEnvios->precio.c_str());
		SetWindowText(hTxtChangeEnvioCalle, aEnvios->calle.c_str());
		SetWindowText(hTxtChangeEnvioColonia, aEnvios->colonia.c_str());
		SetWindowText(hTxtChangeEnvioCiudad, aEnvios->ciudad.c_str());
		SetWindowText(hTxtChangeEnvioEstado, aEnvios->estado.c_str());
		SetWindowText(hTxtChangeEnvioMensaje, aEnvios->mensaje.c_str());
		SetWindowText(hTxtChangeEnvioFecha, aEnvios->fechastr.c_str());

	}break;
	case WM_COMMAND: {
		switch (LOWORD(wparam)) {
		case BTN_CONFENVM: {
			aEnvios->calle = getText(hTxtChangeEnvioCalle);
			aEnvios->colonia = getText(hTxtChangeEnvioColonia);
			aEnvios->ciudad = getText(hTxtChangeEnvioCiudad);
			aEnvios->estado = getText(hTxtChangeEnvioEstado);
			saveEnvios(aEnvios);
			aEnvios = oEnvios;

			HWND hEnvios = CreateDialog(hGInstance, MAKEINTRESOURCE(IDD_ENVIOS), NULL, fEnvios);
			ShowWindow(hEnvios, SW_SHOW);
			exitProgramEnvios = false;
			DestroyWindow(hwnd);
		}break;
		case ID_SALIR: {
			int m_result = DialogBox(hGInstance, MAKEINTRESOURCE(IDD_MODAL), hwnd, fModal);
		}break;
		case ID_ENVIOS: {
			HWND hEnvios = CreateDialog(hGInstance, MAKEINTRESOURCE(IDD_ENVIOS), NULL, fEnvios);
			ShowWindow(hEnvios, SW_SHOW);
			exitProgramEnvios = false;
			DestroyWindow(hwnd);
		}break;
		case ID_PRODUCTOS_VER: {
			HWND hProducts = CreateDialog(hGInstance, MAKEINTRESOURCE(IDD_PRODUCTOS), NULL, fProductos);
			ShowWindow(hProducts, SW_SHOW);
			exitProgramInfo = false;
			DestroyWindow(hwnd);
		}break;
		case ID_WATCHINFO: {
			HWND hInfo = CreateDialog(hGInstance, MAKEINTRESOURCE(IDD_INFOVENDEDOR), NULL, fInfoVendedor);
			ShowWindow(hInfo, SW_SHOW);
			exitProgramInfo = false;
			DestroyWindow(hwnd);
		}break;
		}
	}break;
	case WM_CLOSE:
		exitProgram = true;
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		if (exitProgram) {
			//freeMemoryUser();
			PostQuitMessage(16);
		}
		break;
	}
	return FALSE;
}

BOOL CALLBACK faltaEnviosNew(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	switch (msg) {
	case WM_INITDIALOG: {
		hMenu = LoadMenu(hGInstance, MAKEINTRESOURCE(IDR_MENU1));
		SetMenu(hwnd, hMenu);

		hTxtCantidadEnvio = GetDlgItem(hwnd, TXT_CANTIDAD);
		hTxtPrecio = GetDlgItem(hwnd, LBL_PAGAR);
		hTxtCalle = GetDlgItem(hwnd, TXT_ST);
		hTxtColonia = GetDlgItem(hwnd, TXT_COLONIA);
		hTxtCiudad = GetDlgItem(hwnd, TXT_CIUDAD);
		hTxtEstado = GetDlgItem(hwnd, TXT_ESTADO);
		hTxtMensaje = GetDlgItem(hwnd, TXT_MENSAJE);
		hTxtFechaEnvio = GetDlgItem(hwnd, DTP_FECHA);

		HWND hCbProductos = GetDlgItem(hwnd, CB_PRODUCTS);
		HWND hLblPrecio = GetDlgItem(hwnd, LBL_PAGAR);

		aProducto = oProducto;

		while (aProducto != NULL) {
			if (aProducto->IDUser == userAccess->IDUser) {
				SendMessage(hCbProductos, CB_ADDSTRING, 100, (LPARAM)aProducto->nombreProducto.c_str());
			}
			aProducto = aProducto->nextProducto;
		}
	}break;
	case WM_COMMAND: {
		switch (HIWORD(wparam))
		{
		case CBN_SELCHANGE:
		{
			HWND hCbProductos = GetDlgItem(hwnd, CB_PRODUCTS);
			TCHAR strText[255] = { 0 };
			int idx_row = SendMessage(hCbProductos, CB_GETCURSEL, 0, 0);
			SendMessage(hCbProductos, CB_GETLBTEXT, idx_row, (LPARAM)strText);

			aProducto = oProducto;

			while (aProducto != NULL) {
				if (aProducto->nombreProducto == strText)
					break;
				aProducto = aProducto->nextProducto;
			}

			if (aProducto == NULL)
				break;

			productos* aShow = aProducto;



			/*	HWND const control{ (HWND)lParam };
				int const index{ ::SendMessage(control, CB_GETCURSEL, nullptr, nullptr) };
				if (index == CB_ERR)
				{
					// Handle error
					return 0;
				}
				// [optional] Retrieve text of selected item
				int const len{ (int)::SendMessage(control, CB_GETLBTEXTLEN, (WPARAM)index, nullptr) };
				if (len == CB_ERR)
				{
					// Handle error
					return 0;
				}
				std::vector<wchar_t> buffer(len + 1);
				::SendMessageW(control, CB_GETLBTEXT, (WPARAM)index, (LPARAM)buffer.data());

				return 0;*/
		}

		break;
		}
		switch (LOWORD(wparam)) {
		case BTN_CALCULARPRECIO: {
			string cantidadEnvio = getText(hTxtCantidadEnvio);

			float fCantidadEnvio = atof(cantidadEnvio.c_str());
			float fCantidadProducto = atof(aProducto->cantidadProducto.c_str());
			if (fCantidadEnvio > fCantidadProducto) {
				MessageBox(NULL, "No existe suficiente producto en el inventario", "NO ALTA", MB_ICONASTERISK);
			}
			else {
				float monto = atof(aProducto->precioProducto.c_str()) * atof(cantidadEnvio.c_str());
				string montostr = to_string(monto);
				HWND hLblPrecio = GetDlgItem(hwnd, LBL_PAGAR);
				SendDlgItemMessage(hwnd, LBL_PAGAR, WM_SETTEXT, NULL, (LPARAM)montostr.c_str());
			}
		}break;
		case BTN_CANCELN: {
			MessageBox(NULL, "Falto llenar la informacion", "NO ALTA", MB_ICONASTERISK);
		}break;
		case CB_PRODUCTS: {
			int x = 10;
			x++;
		}break;
		case IDC_BUTTON6: {
			string cantidadEnvio = getText(hTxtCantidadEnvio);
			string precioEnvio = getText(hTxtPrecio);
			string calle = getText(hTxtCalle);
			string colonia = getText(hTxtColonia);
			string ciudad = getText(hTxtCiudad);
			string estado = getText(hTxtEstado);
			string mensaje = getText(hTxtMensaje);
			string fechaEnv = getText(hTxtFechaEnvio);


			tm* ltm = calcularFecha(fechaEnv);

			//	string aFecha = year;

			if (ltm->tm_year < infoTiempo->tm_year || (ltm->tm_year == infoTiempo->tm_year && ltm->tm_mon < infoTiempo->tm_mon) || (ltm->tm_year == infoTiempo->tm_year && ltm->tm_mon == infoTiempo->tm_mon && ltm->tm_mday < infoTiempo->tm_mday)) {
				MessageBox(NULL, "Fecha de envio no valida", "NO ALTA", MB_ICONASTERISK);
				break;
			}

			/*if (ltm->tm_year == infoTiempo->tm_year && ltm->tm_mon < infoTiempo->tm_mon) {
				MessageBox(NULL, "Fecha de envio no valida", "NO ALTA", MB_ICONASTERISK);
				break;
			}

			if ( ltm->tm_year==infoTiempo->tm_year && ltm->tm_mon==infoTiempo->tm_mon && ltm->tm_mday<=infoTiempo->tm_mday) {
				MessageBox(NULL, "Fecha de envio no valida", "NO ALTA", MB_ICONASTERISK);
				break;
			}*/

			if (cantidadEnvio.compare("") == 0 || calle.compare("") == 0 || colonia.compare("") == 0 || ciudad.compare("") == 0 || estado.compare("") == 0 || mensaje.compare("") == 0) {
				MessageBox(NULL, "Falto llenar la informacion", "NO ALTA", MB_ICONASTERISK);
				break;
			}

			int productosActuales = atoi(aProducto->cantidadProducto.c_str());
			int productosAEnviar = atoi(cantidadEnvio.c_str());
			int productosRestantes = productosActuales - productosAEnviar;



			string estadoEnvio = calcularEstadoDeEnvio(ltm);


			if (productosActuales < productosAEnviar) {
				MessageBox(NULL, "No cuenta con los suficientes productos en el inventario", "NO ALTA", MB_ICONASTERISK);
				break;
			}
			else {
				if (oEnvios == NULL) {
					oEnvios = new Envios;
					oEnvios->productoAEnviarID = aProducto->IDProducto;
					oEnvios->productoAEnviar = aProducto->nombreProducto;
					oEnvios->IDEnvio = GLOBAL_ENVIO_ID;
					GLOBAL_ENVIO_ID++;
					oEnvios->cantidad = cantidadEnvio;
					oEnvios->precio = precioEnvio;
					oEnvios->calle = calle;
					oEnvios->colonia = colonia;
					oEnvios->ciudad = ciudad;
					oEnvios->estado = estado;
					oEnvios->mensaje = mensaje;
					oEnvios->status = estadoEnvio;
					oEnvios->fecha = ltm;
					oEnvios->IDUser = userAccess->IDUser;
					oEnvios->nextEnvio = NULL;
					oEnvios->prevEnvio = NULL;
					oEnvios->fechastr = fechaEnv;
					aEnvios = oEnvios;
					aProducto->cantidadProducto = to_string(productosRestantes);
				}
				else {
					aEnvios = oEnvios;
					while (aEnvios->nextEnvio != NULL) {
						aEnvios = aEnvios->nextEnvio;
					}
					aEnvios->nextEnvio = new Envios;
					aEnvios->nextEnvio->prevEnvio = aEnvios;
					aEnvios = aEnvios->nextEnvio;
					aEnvios->productoAEnviarID = aProducto->IDProducto;
					aEnvios->productoAEnviar = aProducto->nombreProducto;
					aEnvios->IDEnvio = GLOBAL_ENVIO_ID;
					GLOBAL_ENVIO_ID++;
					aEnvios->cantidad = cantidadEnvio;
					aEnvios->precio = precioEnvio;
					aEnvios->calle = calle;
					aEnvios->colonia = colonia;
					aEnvios->ciudad = ciudad;
					aEnvios->estado = estado;
					aEnvios->status = estadoEnvio;
					aEnvios->mensaje = mensaje;
					aEnvios->fecha = ltm;
					aEnvios->fechastr = fechaEnv;
					aEnvios->IDUser = userAccess->IDUser;
					aEnvios->nextEnvio = NULL;
					//	aEnvios = oEnvios;
					aProducto->cantidadProducto = to_string(productosRestantes);
				}
				if (aProducto->cantidadProducto == "0") {
					eliminarProducto();
				}
				saveProducto(aProducto);
				saveEnvios(aEnvios);
				aEnvios = oEnvios;
				HWND hEnvios = CreateDialog(hGInstance, MAKEINTRESOURCE(IDD_ENVIOS), NULL, fEnvios);
				ShowWindow(hEnvios, SW_SHOW);
				exitProgramEnvios = false;
				DestroyWindow(hwnd);
			}
		}break;
		case ID_SALIR: {
			int m_result = DialogBox(hGInstance, MAKEINTRESOURCE(IDD_MODAL), hwnd, fModal);
		}break;
		case ID_ENVIOS: {
			HWND hEnvios = CreateDialog(hGInstance, MAKEINTRESOURCE(IDD_ENVIOS), NULL, fEnvios);
			ShowWindow(hEnvios, SW_SHOW);
			exitProgramEnvios = false;
			DestroyWindow(hwnd);
		}break;
		case ID_PRODUCTOS_VER: {
			HWND hProducts = CreateDialog(hGInstance, MAKEINTRESOURCE(IDD_PRODUCTOS), NULL, fProductos);
			ShowWindow(hProducts, SW_SHOW);
			exitProgramInfo = false;
			DestroyWindow(hwnd);
		}break;
		case ID_WATCHINFO: {
			HWND hInfo = CreateDialog(hGInstance, MAKEINTRESOURCE(IDD_INFOVENDEDOR), NULL, fInfoVendedor);
			ShowWindow(hInfo, SW_SHOW);
			exitProgramInfo = false;
			DestroyWindow(hwnd);
		}break;
		}
	}break;
	case WM_CLOSE:
		exitProgram = true;
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		if (exitProgram) {
			//freeMemoryUser();
			PostQuitMessage(16);
		}
		break;
	}
	return FALSE;
}

BOOL CALLBACK fAltaEnvio(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	switch (msg) {
		/*case WM_TIMER: {

		time(&tiempoActual);
		infoTiempo = localtime(&tiempoActual);

		char day[5];
		char month[5];
		char year[5];

		_itoa(infoTiempo->tm_mday, day, 10);
		_itoa(++infoTiempo->tm_mon, month, 10);
		_itoa(infoTiempo->tm_year + 1900, year, 10);

		string dia = day;
		if (dia.length() < 2) {
			dia.clear();
			dia.append("0");
			dia.append(day);
		}

		string mes = month;
		if (mes.length() < 2) {
			mes.clear();
			mes.append("0");
			mes.append(month);
		}

		string aFecha = year;
		string vFechaActual = aFecha + mes + dia;
		fechaHoy = atoll(vFechaActual.c_str());


	}break;*/
	case WM_INITDIALOG: {
		hMenu = LoadMenu(hGInstance, MAKEINTRESOURCE(IDR_MENU1));
		SetMenu(hwnd, hMenu);

		hTxtCantidadEnvio = GetDlgItem(hwnd, TXT_CANTIDAD);
		hTxtCalle = GetDlgItem(hwnd, TXT_ST);
		hTxtColonia = GetDlgItem(hwnd, TXT_COLONIA);
		hTxtCiudad = GetDlgItem(hwnd, TXT_CIUDAD);
		hTxtEstado = GetDlgItem(hwnd, TXT_ESTADO);
		hTxtMensaje = GetDlgItem(hwnd, TXT_MENSAJE);

		HWND hCbProductos = GetDlgItem(hwnd, CB_PRODUCTS);

		while (aProducto != NULL) {
			SendMessage(hCbProductos, CB_ADDSTRING, 0, (LPARAM)aProducto->nombreProducto.c_str());
			aProducto = aProducto->nextProducto;
		}aProducto = oProducto;

	}break;

	case WM_COMMAND: {
		switch (wparam) {
		case IDC_BUTTON6: {
			string cantidadEnvio = getText(hTxtCantidadEnvio);
			string calle = getText(hTxtCalle);
			string colonia = getText(hTxtColonia);
			string ciudad = getText(hTxtCiudad);
			string estado = getText(hTxtEstado);
			string mensaje = getText(hTxtMensaje);

			if (cantidadEnvio.compare("") == 0 || calle.compare("") == 0 || colonia.compare("") == 0 || ciudad.compare("") == 0 || estado.compare("") == 0 || mensaje.compare("") == 0) {
				MessageBox(NULL, "Falto llenar la informacion", "NO ALTA", MB_ICONASTERISK);
				break;
			}

			if (cantidadEnvio.compare("") == 1 || calle.compare("") == 1 || colonia.compare("") == 1 || ciudad.compare("") == 1 || estado.compare("") == 1 || mensaje.compare("") == 1) {
				if (oEnvios == NULL) {
					oEnvios = new Envios;
					oEnvios->cantidad = cantidadEnvio;
					oEnvios->calle = calle;
					oEnvios->colonia = colonia;
					oEnvios->ciudad = ciudad;
					oEnvios->estado = estado;
					oEnvios->mensaje = mensaje;
					//oEnvios->fecha = fechaDeEnvio;
					//oProducto->IDUser = userAccess->IDUser;
					oEnvios->nextEnvio = NULL;
					oEnvios->prevEnvio = NULL;
					aEnvios = oEnvios;
				}
				else {
					while (aEnvios->nextEnvio != NULL) {
						aEnvios = aEnvios->nextEnvio;
					}
					aEnvios->nextEnvio = new Envios;
					aEnvios->nextEnvio->prevEnvio = aEnvios;
					aEnvios = aEnvios->nextEnvio;
					aEnvios->cantidad = cantidadEnvio;
					aEnvios->calle = calle;
					aEnvios->colonia = colonia;
					aEnvios->ciudad = ciudad;
					aEnvios->estado = estado;
					aEnvios->mensaje = mensaje;
					//aEnvios->fecha = fechaDeEnvio;
					//	aProducto->IDUser = userAccess->IDUser;
					aEnvios->nextEnvio = NULL;
					aEnvios = oEnvios;
				}
				saveEnvios(aEnvios);
				aEnvios = oEnvios;
				HWND hEnvios = CreateDialog(hGInstance, MAKEINTRESOURCE(IDD_ENVIOS), NULL, fEnvios);
				ShowWindow(hEnvios, SW_SHOW);
				exitProgramEnvios = false;
				DestroyWindow(hwnd);
			}
		}break;
		case ID_SALIR: {
			int m_result = DialogBox(hGInstance, MAKEINTRESOURCE(IDD_MODAL), hwnd, fModal);
		}break;
		case ID_ENVIOS: {
			HWND hEnvios = CreateDialog(hGInstance, MAKEINTRESOURCE(IDD_ENVIOS), NULL, fEnvios);
			ShowWindow(hEnvios, SW_SHOW);
			exitProgramEnvios = false;
			DestroyWindow(hwnd);
		}break;
		case ID_PRODUCTOS_VER: {
			HWND hProducts = CreateDialog(hGInstance, MAKEINTRESOURCE(IDD_PRODUCTOS), NULL, fProductos);
			ShowWindow(hProducts, SW_SHOW);
			exitProgramInfo = false;
			DestroyWindow(hwnd);
		}break;
		case ID_WATCHINFO: {
			HWND hInfo = CreateDialog(hGInstance, MAKEINTRESOURCE(IDD_INFOVENDEDOR), NULL, fInfoVendedor);
			ShowWindow(hInfo, SW_SHOW);
			exitProgramInfo = false;
			DestroyWindow(hwnd);
		}break;
		}
		/*if (LOWORD(wparam) == BTN_CHECA && HIWORD(wparam) == BN_CLICKED) {
			HWND hDtpTiempo = GetDlgItem(hwnd, DTP_FECHA);
			string fechaDtp = getText(hDtpTiempo);
			string diaDTP = fechaDtp.substr(0, 2);
			string mesDTP = fechaDtp.substr(3, 2);
			string anioDTP = fechaDtp.substr(6, 4);
			string fechaSeleccionada = anioDTP + mesDTP + diaDTP;
			//int fechaDeEnvio = NULL;
			long long int fechaDeEnvio = atoll(fechaSeleccionada.c_str());

			time(&tiempoActual);
			infoTiempo = localtime(&tiempoActual);

			char day[5];
			char month[5];
			char year[5];

			_itoa(infoTiempo->tm_mday, day, 10);
			_itoa(++infoTiempo->tm_mon, month, 10);
			_itoa(infoTiempo->tm_year + 1900, year, 10);

			string dia = day;
			if (dia.length() < 2) {
				dia.clear();
				dia.append("0");
				dia.append(day);
			}

			string mes = month;
			if (mes.length() < 2) {
				mes.clear();
				mes.append("0");
				mes.append(month);
			}

			string aFecha = year;
			string vFechaActual = aFecha + mes + dia;
			fechaHoy = atoll(vFechaActual.c_str());
			leerFecha = true;

			if (fechaDeEnvio > fechaHoy) {
				MessageBox(NULL, "Envio en proceso", "r", MB_ICONASTERISK);
			}
			else if (fechaDeEnvio == fechaHoy) {
				MessageBox(NULL, "Ya llegó", "r", MB_ICONASTERISK);
			}
			else if (fechaDeEnvio < fechaHoy) {
				MessageBox(NULL, "ya paso", "r", MB_ICONASTERISK);
			}

		}break;*/
	case WM_CLOSE:
		//KillTimer(hwnd, MI_TIMER);
		exitProgramAltaEnvios = true;
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		if (exitProgramAltaEnvios) {
			PostQuitMessage(117);
		}
		break;
	}
				   return FALSE;
	}
}

string getText(HWND hwnd) {
	int length = GetWindowTextLength(hwnd);
	if (length < 1)
		return "";
	char cText[50];
	GetWindowText(hwnd, cText, ++length);
	string sText(cText);
	return sText;
}

void save(User* pOrigen) {
	pOrigen = oUser;
	lectorEscritor.open("Usuario.bin", ios::out | ios::trunc | ios::binary);
	if (lectorEscritor.is_open()) {
		while (pOrigen != NULL) {
			lectorEscritor.write(reinterpret_cast<char*>(pOrigen), sizeof(User));
			pOrigen = pOrigen->next;
		}
		lectorEscritor.close();
		MessageBox(NULL, "Guardado exitoso", "GUARDAR", MB_ICONASTERISK);
		return;
	}
	else {
		MessageBox(NULL, "No se pudo abrir el archivo", "NO GUARDAR", MB_ICONASTERISK);
		return;
	}
}

void saveEnvios(Envios* pOrigen) {
	pOrigen = oEnvios;
	lectorEscritor.open("Envios.bin", ios::out | ios::trunc | ios::binary);
	if (lectorEscritor.is_open()) {
		while (pOrigen != NULL) {
			lectorEscritor.write(reinterpret_cast<char*>(pOrigen), sizeof(Envios));
			pOrigen = pOrigen->nextEnvio;
		}
		lectorEscritor.close();
		MessageBox(NULL, "Guardado de envio exitoso", "GUARDAR", MB_ICONASTERISK);
		return;
	}
	else {
		MessageBox(NULL, "No se pudo abrir el archivo", "NO GUARDAR", MB_ICONASTERISK);
		return;
	}
}

void saveInfoVendedor(InfoVendedor* pOrigen) {
	pOrigen = oInfoVendedor;
	lectorEscritor.open("Informacion del Vendedor.bin", ios::out | ios::trunc | ios::binary);
	if (lectorEscritor.is_open()) {
		while (pOrigen != NULL) {
			lectorEscritor.write(reinterpret_cast<char*>(pOrigen), sizeof(InfoVendedor));
			pOrigen = pOrigen->nextInfoVendedor;
		}
		lectorEscritor.close();
		MessageBox(NULL, "Su informacion como vendedor ha sido guardada exitosamente", "GUARDAR", MB_ICONASTERISK);
		return;
	}
	else {
		MessageBox(NULL, "No se pudo abrir el archivo", "NO GUARDAR", MB_ICONASTERISK);
		return;
	}
}

void loadUser() {
	lectorEscritor.open("Usuario.bin", ios::binary | ios::in | ios::ate);
	if (lectorEscritor.is_open()) {
		int totalCharactersUsers = lectorEscritor.tellg();
		if (totalCharactersUsers < 1) {
			MessageBox(NULL, "Archivo Vacio", "NO CARGA", MB_ICONASTERISK);
			lectorEscritor.close();
			return;
		}
		for (int i = 0; i < totalCharactersUsers / sizeof(User); i++) {
			if (oUser == NULL) {
				User* temp = new User;
				oUser = new User;
				lectorEscritor.seekg(i * sizeof(User));
				lectorEscritor.read(reinterpret_cast<char*>(temp), sizeof(User));
				oUser->username = temp->username;
				oUser->password = temp->password;
				oUser->IDUser = temp->IDUser;
				oUser->fechaCreacion = temp->fechaCreacion;
				oUser->next = NULL;
				oUser->prev = NULL;
				aUser = oUser;
				delete reinterpret_cast<char*>(temp);
				continue;
			}
			else {
				while (aUser->next != NULL) {
					aUser = aUser->next;
				}
				User* temp = new User;
				aUser->next = new User;
				lectorEscritor.seekg(i * sizeof(User));
				lectorEscritor.read(reinterpret_cast<char*>(temp), sizeof(User));
				aUser->next->prev = aUser;
				aUser = aUser->next;
				aUser->username = temp->username;
				aUser->password = temp->password;
				aUser->IDUser = temp->IDUser;
				aUser->fechaCreacion = temp->fechaCreacion;
				aUser->next = NULL;
				aUser = oUser;
				delete reinterpret_cast<char*>(temp);
				continue;
			}
		}
		lectorEscritor.close();
		MessageBox(NULL, "Carga exitosa", "CARGAR", MB_ICONASTERISK);
		return;
	}
	else {
		MessageBox(NULL, "No hay archivos cargados todavia", "NO CARGAR", MB_ICONASTERISK);
		return;
	}
}

void loadInfoVendedor() {
	lectorEscritor.open("Informacion del Vendedor.bin", ios::binary | ios::in | ios::ate);
	if (lectorEscritor.is_open()) {
		int totalCharactersUsers = lectorEscritor.tellg();
		if (totalCharactersUsers < 1) {
			MessageBox(NULL, "Archivo Vacio", "NO CARGA", MB_ICONASTERISK);
			lectorEscritor.close();
			return;
		}
		for (int i = 0; i < totalCharactersUsers / sizeof(InfoVendedor); i++) {
			if (oInfoVendedor == NULL) {
				InfoVendedor* temp = new InfoVendedor;
				oInfoVendedor = new InfoVendedor;
				lectorEscritor.seekg(i * sizeof(InfoVendedor));
				lectorEscritor.read(reinterpret_cast<char*>(temp), sizeof(InfoVendedor));
				oInfoVendedor->nombreVendedor = temp->nombreVendedor;
				oInfoVendedor->IDUser = temp->IDUser;
				oInfoVendedor->IDInfoVendedor = temp->IDInfoVendedor;
				oInfoVendedor->aliasDeLaEmpresa = temp->aliasDeLaEmpresa;
				for (int i = 0; i < MAX_PATH; i++) {
					oInfoVendedor->imagen[i] = temp->imagen[i];
				}
				oInfoVendedor->nextInfoVendedor = NULL;
				oInfoVendedor->prevInfoVendedor = NULL;
				aInfoVendedor = oInfoVendedor;
				delete reinterpret_cast<char*>(temp);
				continue;
			}
			else {
				while (aInfoVendedor->nextInfoVendedor != NULL) {
					aInfoVendedor = aInfoVendedor->nextInfoVendedor;
				}
				InfoVendedor* temp = new InfoVendedor;
				aInfoVendedor->nextInfoVendedor = new InfoVendedor;
				lectorEscritor.seekg(i * sizeof(InfoVendedor));
				lectorEscritor.read(reinterpret_cast<char*>(temp), sizeof(InfoVendedor));
				aInfoVendedor->nextInfoVendedor->prevInfoVendedor = aInfoVendedor;
				aInfoVendedor = aInfoVendedor->nextInfoVendedor;
				aInfoVendedor->nombreVendedor = temp->nombreVendedor;
				aInfoVendedor->IDUser = temp->IDUser;
				aInfoVendedor->IDInfoVendedor = temp->IDInfoVendedor;
				aInfoVendedor->aliasDeLaEmpresa = temp->aliasDeLaEmpresa;
				for (int i = 0; i < MAX_PATH; i++) {
					aInfoVendedor->imagen[i] = temp->imagen[i];
				}
				aInfoVendedor->nextInfoVendedor = NULL;
				aInfoVendedor = oInfoVendedor;
				delete reinterpret_cast<char*>(temp);
				continue;
			}
		}
		lectorEscritor.close();
		MessageBox(NULL, "Carga exitosa", "CARGAR", MB_ICONASTERISK);
		//isFull = true;
		return;
	}
	else {
		MessageBox(NULL, "La informacion de vendedor no ha sido cargada todavia", "NO CARGA", MB_ICONASTERISK);
		//isFull = false;
		return;
	}
}

void freeMemoryUser() {
	if (oUser != NULL) { //Revisar el origen es null ? NO, liberamos la memoria
		while (aUser != NULL) { //Mientras aux NO sea NULL ? avanzamos
			User* temp = aUser; //Guardamos en un temporal aux
			aUser = aUser->next; //Avanzamos
			delete temp; //Borramos el temporal
		}
	}
	else //Origen si fue null, no hacemos nada
		return;
	aUser = oUser = NULL;
}

void freeMemoryInfoVendedor() {
	if (oInfoVendedor != NULL) { //Revisar el origen es null ? NO, liberamos la memoria
		while (aInfoVendedor != NULL) { //Mientras aux NO sea NULL ? avanzamos
			InfoVendedor* temp = aInfoVendedor; //Guardamos en un temporal aux
			aInfoVendedor = aInfoVendedor->nextInfoVendedor; //Avanzamos
			delete temp; //Borramos el temporal
		}
	}
	else //Origen si fue null, no hacemos nada
		return;
	aInfoVendedor = oInfoVendedor = NULL;
}

void freeMemoryProductos() {
	if (oProducto != NULL) { //Revisar el origen es null ? NO, liberamos la memoria
		while (aProducto != NULL) { //Mientras aux NO sea NULL ? avanzamos
			productos* temp = aProducto; //Guardamos en un temporal aux
			aProducto = aProducto->nextProducto; //Avanzamos
			delete temp; //Borramos el temporal
		}
	}
	else //Origen si fue null, no hacemos nada
		return;
	aProducto = oProducto = NULL;
}

void freeMemoryEnvios() {
	if (oEnvios != NULL) { //Revisar el origen es null ? NO, liberamos la memoria
		while (aEnvios != NULL) { //Mientras aux NO sea NULL ? avanzamos
			Envios* temp = aEnvios; //Guardamos en un temporal aux
			aEnvios = aEnvios->nextEnvio; //Avanzamos
			delete temp; //Borramos el temporal
		}
	}
	else //Origen si fue null, no hacemos nada
		return;
	aEnvios = oEnvios = NULL;
}

void getGlobalId() {
	lectorEscritor.open("GlobalId.txt", ios::in);
	if (lectorEscritor.is_open()) {
		lectorEscritor >> GLOBAL_USER_ID;
		lectorEscritor.close();
		MessageBox(NULL, "Carga con exito de ID!", "CARGA ID", MB_ICONASTERISK);
		return;
	}
	else {
		MessageBox(NULL, "No se pudo abrir el archivo", "NO CARGA ID", MB_ICONERROR);
		return;
	}
}

void saveProducto(productos* pOrigen) {
	pOrigen = oProducto;
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

void loadEnvios() {
	lectorEscritor.open("Envios.bin", ios::binary | ios::in | ios::ate);
	if (lectorEscritor.is_open()) {
		int totalCharactersUsers = lectorEscritor.tellg();
		if (totalCharactersUsers < 1) {
			MessageBox(NULL, "Archivo Vacio", "NO CARGA", MB_ICONASTERISK);
			lectorEscritor.close();
			return;
		}
		for (int i = 0; i < totalCharactersUsers / sizeof(Envios); i++) {
			GLOBAL_ENVIO_ID++;
			if (oEnvios == NULL) {
				Envios* temp = new Envios;
				oEnvios = new Envios;
				lectorEscritor.seekg(i * sizeof(Envios));
				lectorEscritor.read(reinterpret_cast<char*>(temp), sizeof(Envios));
				oEnvios->productoAEnviarID = temp->productoAEnviarID;
				oEnvios->IDEnvio = temp->IDEnvio;
				oEnvios->productoAEnviar = temp->productoAEnviar;
				oEnvios->cantidad = temp->cantidad;
				oEnvios->precio = temp->precio;
				oEnvios->calle = temp->calle;
				oEnvios->colonia = temp->colonia;
				oEnvios->ciudad = temp->ciudad;
				oEnvios->estado = temp->estado;
				oEnvios->mensaje = temp->mensaje;
				oEnvios->fecha = temp->fecha;
				oEnvios->fechastr = temp->fechastr;
				tm* ltm = calcularFecha(oEnvios->fechastr);
				string estadoEnvio = calcularEstadoDeEnvio(ltm);
				oEnvios->status = estadoEnvio;
				GLOBAL_ENVIO_ID = oEnvios->IDEnvio + 1;
				oEnvios->IDUser = temp->IDUser;
				oEnvios->nextEnvio = NULL;
				oEnvios->prevEnvio = NULL;
				aEnvios = oEnvios;
				delete reinterpret_cast<char*>(temp);
				continue;
			}
			else {
				while (aEnvios->nextEnvio != NULL) {
					aEnvios = aEnvios->nextEnvio;
				}
				Envios* temp = new Envios;
				aEnvios->nextEnvio = new Envios;
				lectorEscritor.seekg(i * sizeof(Envios));
				lectorEscritor.read(reinterpret_cast<char*>(temp), sizeof(Envios));
				aEnvios->nextEnvio->prevEnvio = aEnvios;
				aEnvios = aEnvios->nextEnvio;
				aEnvios->productoAEnviarID = temp->productoAEnviarID;
				aEnvios->productoAEnviar = temp->productoAEnviar;
				aEnvios->IDEnvio = temp->IDEnvio;
				GLOBAL_ENVIO_ID = aEnvios->IDEnvio + 1;
				aEnvios->IDUser = temp->IDUser;
				aEnvios->cantidad = temp->cantidad;
				aEnvios->precio = temp->precio;
				aEnvios->calle = temp->calle;
				aEnvios->colonia = temp->colonia;
				aEnvios->ciudad = temp->ciudad;
				aEnvios->estado = temp->estado;
				aEnvios->mensaje = temp->mensaje;
				aEnvios->fecha = temp->fecha;
				aEnvios->fechastr = temp->fechastr;
				tm* ltm = calcularFecha(aEnvios->fechastr);
				string estadoEnvio = calcularEstadoDeEnvio(ltm);
				aEnvios->status = estadoEnvio;
				//aEnvios->status = temp->status;
				aEnvios->nextEnvio = NULL;
				aEnvios = oEnvios;
				delete reinterpret_cast<char*>(temp);
				continue;
			}
		}
		lectorEscritor.close();
		MessageBox(NULL, "Carga exitosa", "CARGAR", MB_ICONASTERISK);
		//isFull = true;
		return;
	}
	else {
		MessageBox(NULL, "Los envios no han sido cargados todavia", "NO CARGA", MB_ICONASTERISK);
		//isFull = false;
		return;
	}
}

void loadProducto() {
	lectorEscritor.open("Productos.bin", ios::binary | ios::in | ios::ate);
	if (lectorEscritor.is_open()) {
		int totalCharactersUsers = lectorEscritor.tellg();
		if (totalCharactersUsers < 1) {
			MessageBox(NULL, "Archivo Vacio", "NO CARGA", MB_ICONASTERISK);
			lectorEscritor.close();
			return;
		}
		for (int i = 0; i < totalCharactersUsers / sizeof(productos); i++) {
			if (oProducto == NULL) {
				productos* temp = new productos;
				oProducto = new productos;
				lectorEscritor.seekg(i * sizeof(productos));
				lectorEscritor.read(reinterpret_cast<char*>(temp), sizeof(productos));
				oProducto->nombreProducto = temp->nombreProducto;
				oProducto->cantidadProducto = temp->cantidadProducto;
				oProducto->codigoProducto = temp->codigoProducto;
				oProducto->marcaProducto = temp->marcaProducto;
				oProducto->precioProducto = temp->precioProducto;
				oProducto->IDProducto = temp->IDProducto;
				GLOBAL_PRODUCTO_ID = oProducto->IDProducto + 1;
				oProducto->IDUser = temp->IDUser;
				for (int i = 0; i < MAX_PATH; i++) {
					oProducto->descripcionProducto[i] = temp->descripcionProducto[i];
				}
				for (int i = 0; i < MAX_PATH; i++) {
					oProducto->fotoP1[i] = temp->fotoP1[i];
				}
				for (int i = 0; i < MAX_PATH; i++) {
					oProducto->fotoP2[i] = temp->fotoP2[i];
				}
				oProducto->nextProducto = NULL;
				oProducto->prevProducto = NULL;
				aProducto = oProducto;
				delete reinterpret_cast<char*>(temp);
				continue;
			}
			else {
				while (aProducto->nextProducto != NULL) {
					aProducto = aProducto->nextProducto;
				}
				productos* temp = new productos;
				aProducto->nextProducto = new productos;
				lectorEscritor.seekg(i * sizeof(productos));
				lectorEscritor.read(reinterpret_cast<char*>(temp), sizeof(productos));
				aProducto->nextProducto->prevProducto = aProducto;
				aProducto = aProducto->nextProducto;
				aProducto->nombreProducto = temp->nombreProducto;
				aProducto->cantidadProducto = temp->cantidadProducto;
				aProducto->codigoProducto = temp->codigoProducto;
				aProducto->marcaProducto = temp->marcaProducto;
				aProducto->precioProducto = temp->precioProducto;
				aProducto->IDProducto = temp->IDProducto;
				GLOBAL_PRODUCTO_ID = aProducto->IDProducto + 1;
				aProducto->IDUser = temp->IDUser;
				for (int i = 0; i < MAX_PATH; i++) {
					aProducto->descripcionProducto[i] = temp->descripcionProducto[i];
				}
				for (int i = 0; i < MAX_PATH; i++) {
					aProducto->fotoP1[i] = temp->fotoP1[i];
				}
				for (int i = 0; i < MAX_PATH; i++) {
					aProducto->fotoP2[i] = temp->fotoP2[i];
				}
				aProducto->nextProducto = NULL;
				aProducto = oProducto;
				delete reinterpret_cast<char*>(temp);
				continue;
			}
		}
		lectorEscritor.close();
		MessageBox(NULL, "Carga exitosa", "CARGAR", MB_ICONASTERISK);
		//isFull = true;
		return;
	}
	else {
		MessageBox(NULL, "Los productos no han sido cargados todavia", "NO CARGA", MB_ICONASTERISK);
		//isFull = false;
		return;
	}
}

void saveGlobalId() {
	lectorEscritor.open("GlobalId.txt", ios::out | ios::trunc);
	if (lectorEscritor.is_open()) {
		lectorEscritor << GLOBAL_USER_ID;
		lectorEscritor.close();
		MessageBox(NULL, "Guardado con exito!", "GUARDAR", MB_ICONASTERISK);
		return;
	}
	else {
		MessageBox(NULL, "No se pudo abrir el archivo", "NO GUARDAR", MB_ICONERROR);
		return;
	}
}

void eliminarProducto() {
	if (aProducto->nextProducto == NULL && aProducto->prevProducto == NULL) {
		delete aProducto;
		oProducto = aProducto = NULL;
	}
	else if (aProducto->nextProducto == NULL) {
		aProducto->prevProducto->nextProducto = NULL;
		delete aProducto;
		aProducto = oProducto;
	}
	else if (aProducto->prevProducto == NULL) {
		oProducto = oProducto->nextProducto;
		oProducto->prevProducto = NULL;
		delete aProducto;
		aProducto = oProducto;
	}
	else {
		aProducto->prevProducto->nextProducto = aProducto->nextProducto;
		aProducto->nextProducto->prevProducto = aProducto->prevProducto;
		delete aProducto;
		aProducto = oProducto;
	}
}

tm* calcularFecha(string fechaEnv) {
	tm* ltm = new tm;
	char* pch;
	char* dup = _strdup(fechaEnv.c_str());
	pch = strtok(dup, "/");
	ltm->tm_mday = atoi(strtok(NULL, "/"));
	ltm->tm_year = atoi(strtok(NULL, "/"));
	ltm->tm_mon = atoi(pch);
	ltm->tm_hour = 1;
	ltm->tm_min = 1;
	ltm->tm_sec = 1;
	ltm->tm_wday = 0;
	ltm->tm_isdst = 0;
	ltm->tm_yday = 0;

	time(&tiempoActual);
	infoTiempo = localtime(&tiempoActual);


	char day[5];
	char month[5];
	char year[5];


	++infoTiempo->tm_mon;
	infoTiempo->tm_year += 1900;


	_itoa(infoTiempo->tm_mday, day, 10);
	_itoa(infoTiempo->tm_mon, month, 10);
	_itoa(infoTiempo->tm_year, year, 10);

	string dia = day;
	if (dia.length() < 2) {
		dia.clear();
		dia.append("0");
		dia.append(day);
	}

	string mes = month;
	if (mes.length() < 2) {
		mes.clear();
		mes.append("0");
		mes.append(month);
	}
	return ltm;
}

string calcularEstadoDeEnvio(tm* ltm) {
	string estadoEnvio = "Pendiente de envio";
	if (ltm->tm_year == infoTiempo->tm_year && ltm->tm_mon == infoTiempo->tm_mon && ltm->tm_mday == infoTiempo->tm_mday) {
		estadoEnvio = "Enviado";
	}
	return estadoEnvio;

}

