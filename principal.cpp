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
// === Integrante 1: Entrada de incidentes (Insertar) ===
public:
    void insertar(Incidente* inc) {
        Nodo* nuevo_nodo = new Nodo(inc);
        raiz = merge(raiz, nuevo_nodo); 
        cout << "\n Incidente insertado con Prioridad: " << inc->prioridad << endl;
    }

// === Integrante 2: Atención de incidentes ===
    
    // Atender el incidente de mayor prioridad (Extraer Máx)
    Incidente* extraer_max_prioridad() {
        if (!raiz) {
            cout << "Error: No hay incidentes para atender." << endl;
            return nullptr;
        }

        Nodo* nodo_atendido = raiz;
        Incidente* inc_atendido = nodo_atendido->incidente;
        
        // 1. Fusionar los subárboles izquierdo y derecho para formar la nueva raíz
        raiz = merge(raiz->izquierda, raiz->derecha); 
        
        // 2. Separar el Incidente del Nodo antes de liberar el Nodo (evita doble delete)
        nodo_atendido->incidente = nullptr; 
        delete nodo_atendido; // El destructor del Nodo ya no elimina el Incidente
        
        cout << "\n INCIDENTE ATENDIDO (Mayor Prioridad):" << endl;
        inc_atendido->mostrarDetalles();
        return inc_atendido;
    }

    // Visualizar el incidente más urgente sin eliminarlo (Peek Máx)
    Incidente* visualizar_urgente() const {
        if (!raiz) {
            cout << "La zona está libre. No hay incidentes urgentes." << endl;
            return nullptr;
        }
        cout << "\n INCIDENTE MÁS URGENTE (sin atender):" << endl;
        raiz->incidente->mostrarDetalles();
        return raiz->incidente;
    }
// === Integrante 3: Simulación de Fusión de Zonas ===

    // Operación de clase para fusionar dos Heaps (Zonas)
    static LeftistHeap fusionar_zonas(LeftistHeap& zona1, LeftistHeap& zona2) {
        LeftistHeap nueva_zona;
        
        // Realiza la fusión usando el método 'merge' e invalida las raíces originales
        nueva_zona.raiz = zona1.merge(zona1.raiz, zona2.raiz); 
        
        // Es crucial que las zonas originales queden "vacías" de sus nodos
        // para que la nueva zona se encargue de la memoria.
        zona1.raiz = nullptr; 
        zona2.raiz = nullptr;
        
        return nueva_zona;
    }
    
    // Función auxiliar para simulación
    bool estaVacio() const { return raiz == nullptr; }
};

#endif // EMERGENCIA_HEAP_H





