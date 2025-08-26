#include <iostream>
#include <string>
#include <vector>
using namespace std;

// --- Definición de la clase Libro (mejorada) ---
class Libro {
public:
    string titulo;
    string autor;
    int anioPublicacion;
    bool estaDisponible; // Nuevo atributo

    // Constructor básico
    Libro() {
        titulo = "";
        autor = "";
        anioPublicacion = 0;
        estaDisponible = true;
    }

    void mostrarDetallesCompletos() {
        cout << "Titulo: " << titulo << endl;
        cout << "Autor: " << autor << endl;
        cout << "Ano de Publicacion: " << anioPublicacion << endl;
        if (estaDisponible) {
            cout << "Disponible: Si" << endl;
        } else {
            cout << "Disponible: No" << endl;
        }
        cout << "---------------------------" << endl;
    }
};


// --- Definición de la clase Biblioteca ---
class Biblioteca {
private:
    vector<Libro> coleccion;

public:
    void agregarLibro(Libro nuevoLibro) {
        // evitar duplicados
        for (int i = 0; i < coleccion.size(); i++) {
            if (coleccion[i].titulo == nuevoLibro.titulo) {
                cout << "El libro ya existe en la biblioteca." << endl;
                return;
            }
        }
        coleccion.push_back(nuevoLibro);
        cout << "Libro agregado correctamente." << endl;
    }

    void mostrarInventario() {
        if (coleccion.size() == 0) {
            cout << "No hay libros en la biblioteca." << endl;
            return;
        }
        for (int i = 0; i < coleccion.size(); i++) {
            coleccion[i].mostrarDetallesCompletos();
        }
    }

    void mostrarDisponibles() {
        bool hay = false;
        for (int i = 0; i < coleccion.size(); i++) {
            if (coleccion[i].estaDisponible) {
                coleccion[i].mostrarDetallesCompletos();
                hay = true;
            }
        }
        if (!hay) {
            cout << "No hay libros disponibles para prestar." << endl;
        }
    }

    Libro* buscarLibro(string tituloBuscado) {
        for (int i = 0; i < coleccion.size(); i++) {
            if (coleccion[i].titulo == tituloBuscado) {
                return &coleccion[i];
            }
        }
        return NULL;
    }

    void prestarLibro(string tituloPrestamo) {
        Libro* libro = buscarLibro(tituloPrestamo);
        if (libro == NULL) {
            cout << "El libro no existe." << endl;
        } else if (!libro->estaDisponible) {
            cout << "El libro ya esta prestado." << endl;
        } else {
            libro->estaDisponible = false;
            cout << "Libro prestado exitosamente." << endl;
        }
    }

    void devolverLibro(string tituloDevolucion) {
        Libro* libro = buscarLibro(tituloDevolucion);
        if (libro == NULL) {
            cout << "El libro no existe." << endl;
        } else if (libro->estaDisponible) {
            cout << "El libro ya estaba disponible en la biblioteca." << endl;
        } else {
            libro->estaDisponible = true;
            cout << "Libro devuelto exitosamente." << endl;
        }
    }
};


// --- Función Principal con el Menú ---
int main() {
    Biblioteca miBiblioteca;
    int opcion = 0;

    // Agregar un libro de ejemplo
    Libro libroInicial;
    libroInicial.titulo = "El Hobbit";
    libroInicial.autor = "J.R.R. Tolkien";
    libroInicial.anioPublicacion = 1937;
    miBiblioteca.agregarLibro(libroInicial);

    while (opcion != 5) {
        cout << "\n--- BIBLIOTECA DIGITAL ---" << endl;
        cout << "1. Anadir libro" << endl;
        cout << "2. Mostrar inventario" << endl;
        cout << "3. Prestar libro" << endl;
        cout << "4. Devolver libro" << endl;
        cout << "5. Salir" << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        cin.ignore(); // limpiar buffer

        if (opcion == 1) {
            Libro nuevoLibro;
            cout << "Ingrese el titulo: ";
            getline(cin, nuevoLibro.titulo);
            cout << "Ingrese el autor: ";
            getline(cin, nuevoLibro.autor);
            cout << "Ingrese el ano de publicacion: ";
            cin >> nuevoLibro.anioPublicacion;
            cin.ignore();
            miBiblioteca.agregarLibro(nuevoLibro);
        }
        else if (opcion == 2) {
            miBiblioteca.mostrarInventario();
        }
        else if (opcion == 3) {
            cout << "\nLista de libros disponibles:" << endl;
            miBiblioteca.mostrarDisponibles();
            string titulo;
            cout << "Ingrese el titulo del libro a prestar: ";
            getline(cin, titulo);
            miBiblioteca.prestarLibro(titulo);
        }
        else if (opcion == 4) {
            string titulo;
            cout << "Ingrese el titulo del libro a devolver: ";
            getline(cin, titulo);
            miBiblioteca.devolverLibro(titulo);
        }
        else if (opcion == 5) {
            cout << "Saliendo del programa..." << endl;
        }
        else {
            cout << "Opcion invalida, intente de nuevo." << endl;
        }
    }

    return 0;
}
