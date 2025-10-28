#ifndef EMERGENCIA_HEAP_H
#define EMERGENCIA_HEAP_H

#include <iostream>
#include <string>
#include <algorithm> // Para std::swap y std::min

using namespace std;

// === Integrante 1: Modelado de datos (estructura) ===
struct Incidente {
    int prioridad;      // Mayor número = Mayor prioridad (Max Heap)
    string descripcion;
    string ubicacion;

    Incidente(int p, string d, string u) : prioridad(p), descripcion(d), ubicacion(u) {}
    
    void mostrarDetalles() const {
        cout << "  Prioridad: " << prioridad 
             << " | Descripcion: " << descripcion 
             << " | Ubicacion: " << ubicacion << endl;
    }
};

struct Nodo {
    Incidente* incidente;
    Nodo* izquierda;
    Nodo* derecha;
    int npl; // Null Path Length (longitud del camino nulo)

    Nodo(Incidente* inc) : incidente(inc), izquierda(nullptr), derecha(nullptr), npl(0) {}
    ~Nodo() { delete incidente; } // Limpieza de memoria
};

class LeftistHeap {
private:
    Nodo* raiz;

    // Función auxiliar de apoyo: obtiene el NPL de un nodo (0 si es nullptr)
    int getNPL(Nodo* h) const {
        return h ? h->npl : -1;
    }
    
    // Función auxiliar para intercambiar hijos y mantener la propiedad de Leftist Heap
    void swapChildren(Nodo* h) {
        if (!h) return;
        swap(h->izquierda, h->derecha);
        // Actualiza el NPL: 1 + min(NPL(izquierda), NPL(derecha))
        h->npl = 1 + min(getNPL(h->izquierda), getNPL(h->derecha));
    }
