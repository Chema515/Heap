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

#include <limits> // Para limpiar el buffer

// === Integrante 4: Interfaz de simulación ===

// Función auxiliar para leer enteros y limpiar el buffer de entrada
int leerOpcion() {
    int opcion;
    while (!(cin >> opcion)) {
        cout << "Entrada inválida. Por favor, ingrese un número: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Limpia el buffer
    return opcion;
}

void menu_zona(LeftistHeap& zona, const string& nombre_zona) {
    int opcion;
    do {
        cout << "\n=== GESTIÓN DE INCIDENTES: " << nombre_zona << " ===" << endl;
        cout << "1. Insertar nuevo Incidente" << endl;
        cout << "2. Atender incidente de mayor prioridad" << endl;
        cout << "3. Visualizar incidente más urgente" << endl;
        cout << "4. Regresar al Menú Principal" << endl;
        cout << "Seleccione una opción: ";

        opcion = leerOpcion();

        switch (opcion) {
            case 1: { // Insertar Incidente
                int p;
                string d, u;
                cout << "\n[NUEVO INCIDENTE]" << endl;
                cout << "Prioridad (1-100, 100 es max): "; p = leerOpcion();
                cout << "Descripción: "; getline(cin, d);
                cout << "Ubicación: "; getline(cin, u);
                
                // Creación dinámica del incidente (será liberado por el destructor del Nodo)
                Incidente* nuevo_inc = new Incidente(p, d, u); 
                zona.insertar(nuevo_inc);
                break;
            }
            case 2: // Atender Incidente
                zona.extraer_max_prioridad();
                break;
            case 3: // Visualizar Incidente
                zona.visualizar_urgente();
                break;
            case 4:
                cout << "Regresando al menú principal..." << endl;
                break;
            default:
                cout << "Opción no válida. Intente de nuevo." << endl;
        }
    } while (opcion != 4);
}

void menu_principal() {
    // Inicialización de las dos zonas de atención (dos Leftist Heaps)
    LeftistHeap zonaA;
    LeftistHeap zonaB;
    LeftistHeap zonaFusionada; // Para almacenar el resultado de la fusión
    
    int opcion;

    // Simulación de carga inicial para probar las estructuras
    zonaA.insertar(new Incidente(50, "Corte de luz sector norte", "ZN-1A"));
    zonaB.insertar(new Incidente(90, "Incendio reportado", "ZB-2C"));
    zonaA.insertar(new Incidente(75, "Accidente de tráfico menor", "ZN-3D"));
    zonaB.insertar(new Incidente(60, "Fuga de gas leve", "ZB-4E"));

    do {
        cout << "\n==========================================" << endl;
        cout << "   SISTEMA DE GESTIÓN DE EMERGENCIAS (Leftist Heap) " << endl;
        cout << "==========================================" << endl;
        cout << "1. Gestionar Zona A" << endl;
        cout << "2. Gestionar Zona B" << endl;
        cout << "3. Fusionar Zonas A y B" << endl;
        
        if (!zonaFusionada.estaVacio()) {
             cout << "4. Atender/Visualizar Incidente en ZONA FUSIONADA" << endl;
        }

        cout << "0. Salir" << endl;
        cout << "Seleccione una opción: ";

        opcion = leerOpcion();

        switch (opcion) {
            case 1: 
                menu_zona(zonaA, "ZONA A");
                break;
            case 2: 
                menu_zona(zonaB, "ZONA B");
                break;
            case 3: { // Fusión de Zonas (Integrante 3)
                if (zonaFusionada.estaVacio()) {
                     cout << "\n*** FUSIONANDO ZONAS A y B... ***" << endl;
                     zonaFusionada = LeftistHeap::fusionar_zonas(zonaA, zonaB);
                     cout << "Zonas A y B fusionadas en ZONA FUSIONADA." << endl;
                     cout << "Zona A y Zona B están ahora vacías." << endl;
                } else {
                    cout << "Ya existe una Zona Fusionada. Atiéndala antes de fusionar nuevamente." << endl;
                }
                break;
            }
            case 4: 
                if (!zonaFusionada.estaVacio()) {
                    menu_zona(zonaFusionada, "ZONA FUSIONADA");
                } else {
                    cout << "Opción no válida." << endl;
                }
                break;
            case 0:
                cout << "Saliendo del sistema. ¡Adiós!" << endl;
                break;
            default:
                cout << "Opción no válida. Intente de nuevo." << endl;
        }
    } while (opcion != 0);
}

int main() {
    menu_principal();
    return 0;
}
