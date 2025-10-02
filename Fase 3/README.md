# Biblioteca Escolar — Fase 3 (C++)

Implementación mínima funcional basada en el modelo relacional de la **Fase 2 (3FN)**.

## Estructura
```
phase3_cpp/
├─ biblioteca.h        # Modelos (structs) + clase BibliotecaDB (interfaz)
├─ biblioteca.cpp      # Implementación de BibliotecaDB (CRUD, joins, persistencia)
├─ main.cpp            # Menú interactivo y flujo de la app
└─ data/               # Archivos CSV simples: *.txt
   ├─ autores.txt
   ├─ libros.txt
   ├─ estudiantes.txt
   └─ prestamos.txt
```

## Compilación
Requiere **g++** (C++17):
```bash
g++ -std=c++17 -O2 main.cpp biblioteca.cpp -o biblioteca
```
> Ejecutar dentro de `phase3_cpp/`.

## Ejecución
```bash
./biblioteca    # Linux / macOS
biblioteca.exe  # Windows (MinGW)
```

La aplicación intentará **cargar** al inicio los datos desde `./data`. Si no existen, el directorio se crea tras **guardar** por primera vez.

## Menú (CRUD + joins simulados)
1. Agregar Autor  
2. Listar Autores  
3. Agregar Libro  
4. Listar Libros (con Autor) ← *join Autor–Libro*  
5. Agregar Estudiante  
6. Listar Estudiantes  
7. Registrar Préstamo (prestar libro) ← *valida disponibilidad y FKs*  
8. Devolver Préstamo  
9. Listar Préstamos activos ← *join Libro–Estudiante*  
10. Buscar Préstamos por Estudiante ← *join Libro–Estudiante*  
11. Guardar todo  
12. Cargar desde disco  
0. Salir

## Persistencia (CSV simple)
Se guardan/leen cuatro archivos:
- `data/autores.txt` → `id,nombre,nacionalidad`
- `data/libros.txt` → `id,titulo,isbn,anio,id_autor`
- `data/estudiantes.txt` → `id,nombre,grado`
- `data/prestamos.txt` → `id,id_libro,id_estudiante,fecha_prestamo,fecha_devolucion`

> Nota: Se usa un **CSV simple** (separador coma). Evita comas dentro de los textos (p. ej., en `titulo`) para simplificar el parsing.

## Cobertura de CRUD (>=80%)
- Create, Read, List para las 4 tablas.  
- Update parcial en **Préstamo** (devolución).  
- Validaciones de negocio básicas: **ID duplicado**, **FK existentes** y **libro no disponible** si ya tiene préstamo activo.

## Justificación técnica
- Se simulan *joins* con bucles buscando por `id` en los vectores.  
- El modelo respeta 3FN (no se repiten datos de Autor en Libro, etc.).  
- Estructuras simples (`struct`) y `std::vector` para mantener el diseño pedido.

## Créditos
Alumno/a: David Alejandro Aguilar Aguilar · Fecha: 2025-10-02
