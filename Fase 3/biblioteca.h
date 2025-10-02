#pragma once
#include <string>
#include <vector>
#include <optional>

// ========================
// Modelos (Tablas 3FN)
// ========================
struct Autor {
    int id;
    std::string nombre;
    std::string nacionalidad;
};

struct Libro {
    int id;
    std::string titulo;
    std::string isbn;
    int anio_publicacion;
    int id_autor; // FK -> Autor.id
};

struct Estudiante {
    int id;
    std::string nombre;
    std::string grado;
};

struct Prestamo {
    int id;
    int id_libro;       // FK -> Libro.id
    int id_estudiante;  // FK -> Estudiante.id
    std::string fecha_prestamo;   // yyyy-mm-dd
    std::string fecha_devolucion; // "" si activo
};

// ========================
// "Repositorio" en memoria
// ========================
class BibliotecaDB {
public:
    // Datos en memoria
    std::vector<Autor> autores;
    std::vector<Libro> libros;
    std::vector<Estudiante> estudiantes;
    std::vector<Prestamo> prestamos;

    // CRUD básicos
    bool agregarAutor(const Autor& a);
    bool agregarLibro(const Libro& l);
    bool agregarEstudiante(const Estudiante& e);
    bool agregarPrestamo(const Prestamo& p); // valida: libro disponible, FKs, id único
    bool devolverPrestamo(int id_prestamo, const std::string& fecha_devolucion);

    // Búsquedas simples
    std::optional<Autor> getAutor(int id) const;
    std::optional<Libro> getLibro(int id) const;
    std::optional<Estudiante> getEstudiante(int id) const;
    std::optional<Prestamo> getPrestamo(int id) const;

    // Utilidades "join" (simuladas con bucles)
    std::string nombreAutorDeLibro(int id_libro) const;
    std::string nombreEstudiante(int id_estudiante) const;

    // Persistencia CSV simple (campos separados por coma; evitar comas en textos)
    bool guardarTodo(const std::string& carpeta) const;
    bool cargarTodo(const std::string& carpeta);

private:
    // Helpers de búsqueda por id (índices)
    int idxAutor(int id) const;
    int idxLibro(int id) const;
    int idxEstudiante(int id) const;
    int idxPrestamo(int id) const;

    // Validaciones
    bool existePrestamoActivoParaLibro(int id_libro) const;
};
