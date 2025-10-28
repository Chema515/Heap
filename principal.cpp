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
// === Integrante 3: Función de zonas (Merge) ===
public:
    // Función fundamental: Fusión de dos Leftist Heaps
    Nodo* merge(Nodo* h1, Nodo* h2) {
        if (!h1) return h2;
        if (!h2) return h1;

        // 1. Condición de Max Heap: La raíz de mayor prioridad es la nueva raíz.
        if (h1->incidente->prioridad < h2->incidente->prioridad) {
            swap(h1, h2); // Asegura que h1 es el nodo con mayor prioridad
        }

        // 2. Fusión recursiva: El hijo derecho de la nueva raíz (h1) se fusiona con el otro heap (h2).
        h1->derecha = merge(h1->derecha, h2);

        // 3. Condición de Leftist Heap: Verifica y ajusta la propiedad NPL(izq) >= NPL(der).
        if (getNPL(h1->izquierda) < getNPL(h1->derecha)) {
            swapChildren(h1); // Intercambia si no cumple y actualiza el NPL
        } else {
            // Si ya cumple, solo actualiza el NPL (el NPL es min(izq, der) + 1)
            h1->npl = 1 + getNPL(h1->derecha);
        }

        return h1;
    }

// Constructor y Destructor
public:
    LeftistHeap() : raiz(nullptr) {}

    // Destructor: Libera recursivamente toda la memoria
    ~LeftistHeap() {
        destruir(raiz);
    }
    void destruir(Nodo* n) {
        if (!n) return;
        destruir(n->izquierda);
        destruir(n->derecha);
        delete n;
    }

