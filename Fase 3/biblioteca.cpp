#include "biblioteca.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

// Utilidad CSV mínima: split por coma (no se soportan comas dentro de textos)
static std::vector<std::string> splitCSV(const std::string& s) {
    std::vector<std::string> out;
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, ',')) out.push_back(item);
    return out;
}

static std::string trim(const std::string& s) {
    size_t b = s.find_first_not_of(" \t\r\n");
    size_t e = s.find_last_not_of(" \t\r\n");
    if (b == std::string::npos) return "";
    return s.substr(b, e - b + 1);
}

// ================= CRUD =================
bool BibliotecaDB::agregarAutor(const Autor& a) {
    if (idxAutor(a.id) != -1) return false; // id duplicado
    autores.push_back(a);
    return true;
}

bool BibliotecaDB::agregarLibro(const Libro& l) {
    if (idxLibro(l.id) != -1) return false;
    if (idxAutor(l.id_autor) == -1) return false; // FK autor debe existir
    libros.push_back(l);
    return true;
}

bool BibliotecaDB::agregarEstudiante(const Estudiante& e) {
    if (idxEstudiante(e.id) != -1) return false;
    estudiantes.push_back(e);
    return true;
}

bool BibliotecaDB::agregarPrestamo(const Prestamo& p) {
    if (idxPrestamo(p.id) != -1) return false; // id duplicado
    if (idxLibro(p.id_libro) == -1) return false; // FK libro
    if (idxEstudiante(p.id_estudiante) == -1) return false; // FK estudiante
    if (existePrestamoActivoParaLibro(p.id_libro)) return false; // no disponible
    prestamos.push_back(p);
    return true;
}

bool BibliotecaDB::devolverPrestamo(int id_prestamo, const std::string& fecha_devolucion) {
    int i = idxPrestamo(id_prestamo);
    if (i == -1) return false;
    prestamos[i].fecha_devolucion = fecha_devolucion;
    return true;
}

// ================= BÚSQUEDAS =================
std::optional<Autor> BibliotecaDB::getAutor(int id) const {
    int i = idxAutor(id);
    if (i == -1) return std::nullopt;
    return autores[i];
}
std::optional<Libro> BibliotecaDB::getLibro(int id) const {
    int i = idxLibro(id);
    if (i == -1) return std::nullopt;
    return libros[i];
}
std::optional<Estudiante> BibliotecaDB::getEstudiante(int id) const {
    int i = idxEstudiante(id);
    if (i == -1) return std::nullopt;
    return estudiantes[i];
}
std::optional<Prestamo> BibliotecaDB::getPrestamo(int id) const {
    int i = idxPrestamo(id);
    if (i == -1) return std::nullopt;
    return prestamos[i];
}

// ================= JOINS (simulados) =================
std::string BibliotecaDB::nombreAutorDeLibro(int id_libro) const {
    int iL = idxLibro(id_libro);
    if (iL == -1) return "(libro no existe)";
    int idA = libros[iL].id_autor;
    int iA = idxAutor(idA);
    return iA == -1 ? "(autor no existe)" : autores[iA].nombre;
}

std::string BibliotecaDB::nombreEstudiante(int id_estudiante) const {
    int i = idxEstudiante(id_estudiante);
    return i == -1 ? "(estudiante no existe)" : estudiantes[i].nombre;
}

// ================= PERSISTENCIA =================
bool BibliotecaDB::guardarTodo(const std::string& carpeta) const {
    try {
        std::ofstream fa(carpeta + "/autores.txt"); // id,nombre,nacionalidad
        for (auto &x : autores)
            fa << x.id << "," << x.nombre << "," << x.nacionalidad << "\n";

        std::ofstream fl(carpeta + "/libros.txt"); // id,titulo,isbn,anio,id_autor
        for (auto &x : libros)
            fl << x.id << "," << x.titulo << "," << x.isbn << "," << x.anio_publicacion << "," << x.id_autor << "\n";

        std::ofstream fe(carpeta + "/estudiantes.txt"); // id,nombre,grado
        for (auto &x : estudiantes)
            fe << x.id << "," << x.nombre << "," << x.grado << "\n";

        std::ofstream fp(carpeta + "/prestamos.txt"); // id,id_libro,id_estudiante,fecha_prestamo,fecha_devolucion
        for (auto &x : prestamos)
            fp << x.id << "," << x.id_libro << "," << x.id_estudiante << "," << x.fecha_prestamo << "," << x.fecha_devolucion << "\n";

        return true;
    } catch (...) { return false; }
}

bool BibliotecaDB::cargarTodo(const std::string& carpeta) {
    try {
        autores.clear(); libros.clear(); estudiantes.clear(); prestamos.clear();

        // Autores
        std::ifstream fa(carpeta + "/autores.txt");
        std::string line;
        while (std::getline(fa, line)) {
            auto v = splitCSV(line);
            if (v.size() < 3) continue;
            Autor a{ std::stoi(v[0]), trim(v[1]), trim(v[2]) };
            autores.push_back(a);
        }

        // Libros
        std::ifstream fl(carpeta + "/libros.txt");
        while (std::getline(fl, line)) {
            auto v = splitCSV(line);
            if (v.size() < 5) continue;
            Libro l{ std::stoi(v[0]), trim(v[1]), trim(v[2]), std::stoi(v[3]), std::stoi(v[4]) };
            libros.push_back(l);
        }

        // Estudiantes
        std::ifstream fe(carpeta + "/estudiantes.txt");
        while (std::getline(fe, line)) {
            auto v = splitCSV(line);
            if (v.size() < 3) continue;
            Estudiante e{ std::stoi(v[0]), trim(v[1]), trim(v[2]) };
            estudiantes.push_back(e);
        }

        // Prestamos
        std::ifstream fp(carpeta + "/prestamos.txt");
        while (std::getline(fp, line)) {
            auto v = splitCSV(line);
            if (v.size() < 5) continue;
            Prestamo p{ std::stoi(v[0]), std::stoi(v[1]), std::stoi(v[2]), trim(v[3]), trim(v[4]) };
            prestamos.push_back(p);
        }

        return true;
    } catch (...) { return false; }
}

// ================= HELPERS =================
int BibliotecaDB::idxAutor(int id) const {
    for (size_t i = 0; i < autores.size(); ++i) if (autores[i].id == id) return (int)i;
    return -1;
}
int BibliotecaDB::idxLibro(int id) const {
    for (size_t i = 0; i < libros.size(); ++i) if (libros[i].id == id) return (int)i;
    return -1;
}
int BibliotecaDB::idxEstudiante(int id) const {
    for (size_t i = 0; i < estudiantes.size(); ++i) if (estudiantes[i].id == id) return (int)i;
    return -1;
}
int BibliotecaDB::idxPrestamo(int id) const {
    for (size_t i = 0; i < prestamos.size(); ++i) if (prestamos[i].id == id) return (int)i;
    return -1;
}

bool BibliotecaDB::existePrestamoActivoParaLibro(int id_libro) const {
    for (auto &p : prestamos) {
        if (p.id_libro == id_libro && trim(p.fecha_devolucion).empty())
            return true;
    }
    return false;
}
