#include <iostream>
#include <string>
#include <cmath>
#include <ctime>

using namespace std;

// rand y srand sin requerir #include <cstdlib>
extern "C" int rand();
extern "C" void srand(unsigned int);

// ============================================================================
// ESTRUCTURAS DE DATOS (struct)
// ============================================================================

// El Heroe reside localmente en la Pila (Stack)
struct Heroe {
    string nombre;
    int vida;
    int vidaMax;
    int pociones;
    bool protegido; // Mecanismo de proteccion para ganar una jugada y recargar vida
};

// Los Enemigos se crean dinamicamente en el Monticulo (Heap)
struct Enemigo {
    string nombre;
    int vida;
    bool esJefe;
    bool protegido; // Mecanismo de proteccion para ganar una jugada y recargar vida
};

// ============================================================================
// GESTION DE MEMORIA Y CREACION DE ENTIDADES (HEAP)
// ============================================================================

// Crea el Comandante en el Heap mediante 'new Enemigo'
Enemigo* crearJefe(string nombre, int vida) {
    Enemigo* jefe = new Enemigo; // Asignacion dinamica simple en Heap
    jefe->nombre = nombre;
    jefe->vida = vida;
    jefe->esJefe = true;
    jefe->protegido = false;
    return jefe;
}

// Crea el Ejercito en el Heap mediante 'new Enemigo[cantidad]'
// Acceso estricto mediante aritmetica de punteros: *(tipos + indice) y (ejercito + i)
Enemigo* crearEjercito(int cantidad) {
    Enemigo* ejercito = new Enemigo[cantidad]; // Asignacion de arreglo dinamico en Heap
    string tipos[] = {"Goblin", "Esqueleto", "Orco", "Zombi"};

    for (int i = 0; i < cantidad; i++) {
        (ejercito + i)->nombre = *(tipos + (rand() % 4)); // Aritmetica de punteros estricta
        (ejercito + i)->vida = 25 + (rand() % 20);        // Vida aleatoria entre 25 y 44 HP
        (ejercito + i)->esJefe = false;
        (ejercito + i)->protegido = false;
    }
    return ejercito;
}

// ============================================================================
// FUNCIONES AUXILIARES Y ESTADO DEL JUEGO
// ============================================================================

// Cuenta la cantidad de soldados con vida > 0 usando aritmetica de punteros
int contarSoldadosVivos(const Enemigo* ejercito, int cantidadEjercito) {
    if (ejercito == nullptr || cantidadEjercito <= 0) return 0;
    int vivos = 0;
    for (int i = 0; i < cantidadEjercito; i++) {
        if ((ejercito + i)->vida > 0) {
            vivos++;
        }
    }
    return vivos;
}

// Muestra el estado actual del combate
void mostrarEstado(const Heroe& heroe, const Enemigo* jefe, const Enemigo* ejercito, int cantidadEjercito) {
    cout << "======================================================" << endl;
    cout << " HEROE: " << heroe.nombre << " | HP: " << heroe.vida << "/" << heroe.vidaMax 
         << " | Pociones: " << heroe.pociones;
    if (heroe.protegido) {
        cout << " [ESCUDO DEFENSIVO ACTIVO]";
    }
    cout << "\n------------------------------------------------------" << endl;
    cout << endl;

    if (jefe != nullptr && jefe->vida > 0) {
        cout << " [COMANDANTE] " << jefe->nombre << " | HP: " << jefe->vida;
        if (jefe->protegido) {
            cout << " [ESCUDO DEFENSIVO ACTIVO]";
        }
        cout << endl;
    } else {
        cout << " [COMANDANTE] " << (jefe != nullptr ? jefe->nombre : "Gran General") << " | [DERROTADO]" << endl;
    }

    cout << " --- EJERCITO DE SOLDADOS ---" << endl;
    int soldadosVivos = contarSoldadosVivos(ejercito, cantidadEjercito);
    for (int i = 0; i < cantidadEjercito; i++) {
        if ((ejercito + i)->vida > 0) {
            cout << "  [" << i + 1 << "] " << (ejercito + i)->nombre 
                 << " | HP: " << (ejercito + i)->vida;
            if ((ejercito + i)->protegido) {
                cout << " [ESCUDO ACTIVO]";
            }
            cout << endl;
        }
    }
    if (soldadosVivos == 0) {
        cout << "  (Todo el ejercito ha sido eliminado)" << endl;
    }
    cout << "======================================================" << endl;
}

// Tarea: Los soldados sin vida deben desaparecer del listado del ejercito.
// Reasigna dinamicamente la memoria en el Heap creando un nuevo arreglo con
// solo los soldados sobrevivientes y liberando el anterior con delete[].
void eliminarSoldadosSinVida(Enemigo*& ejercito, int& cantidadEjercito) {
    if (ejercito == nullptr || cantidadEjercito <= 0) return;

    int vivos = contarSoldadosVivos(ejercito, cantidadEjercito);
    if (vivos == cantidadEjercito) {
        return; // Todos siguen vivos, no se requiere redimensionar
    }

    // Notificar bajas
    for (int i = 0; i < cantidadEjercito; i++) {
        if ((ejercito + i)->vida <= 0) {
            cout << "-> [BAJA] El soldado " << (ejercito + i)->nombre 
                 << " ha caido y desaparece del listado del ejercito." << endl;
        }
    }

    // Si todo el ejercito fue eliminado
    if (vivos == 0) {
        delete[] ejercito;
        ejercito = nullptr;
        cantidadEjercito = 0;
        return;
    }

    // Nuevo bloque en Heap para los soldados sobrevivientes
    Enemigo* nuevoEjercito = new Enemigo[vivos];
    int nuevoIdx = 0;
    for (int i = 0; i < cantidadEjercito; i++) {
        if ((ejercito + i)->vida > 0) {
            *(nuevoEjercito + nuevoIdx) = *(ejercito + i); // Copia mediante aritmetica de punteros
            nuevoIdx++;
        }
    }

    // Liberar memoria previa y reasignar puntero
    delete[] ejercito;
    ejercito = nuevoEjercito;
    cantidadEjercito = vivos;
}

// ============================================================================
// ACCIONES DEL HEROE
// ============================================================================

// Ataque individual especificado manualmente en HP
void ejecutarAtaqueIndividualHeroe(Enemigo* jefe, Enemigo* ejercito, int cantidadEjercito) {
    cout << endl;
    cout << "A quien quieres atacar?" << endl;
    if (jefe != nullptr && jefe->vida > 0) {
        cout << "0. " << jefe->nombre << " (Comandante)" << endl;
    }

    for (int i = 0; i < cantidadEjercito; i++) {
        if ((ejercito + i)->vida > 0) {
            cout << i + 1 << ". " << (ejercito + i)->nombre 
                 << " (HP: " << (ejercito + i)->vida << ")" << endl;
        }
    }
    cout << "Elige objetivo: ";
    int objetivo;
    if (!(cin >> objetivo)) return;

    int danio;
    cout << "Especifica la cantidad de HP de dano para el ataque: ";
    if (!(cin >> danio)) return;
    danio = abs(danio); // Funcion math (cmath)

    if (objetivo == 0 && jefe != nullptr && jefe->vida > 0) {
        if (jefe->protegido) {
            cout << "-> ¡El Comandante " << jefe->nombre << " bloqueo el ataque con su mecanismo de proteccion! No recibe dano." << endl;
            jefe->protegido = false;
        } else {
            jefe->vida = (int)fmax(0, jefe->vida - danio); // Funcion math (fmax)
            cout << "-> Causas " << danio << " HP de dano al Comandante " << jefe->nombre << "." << endl;
        }
    } else if (objetivo > 0 && objetivo <= cantidadEjercito && (ejercito + objetivo - 1)->vida > 0) {
        Enemigo* objetivoSoldado = ejercito + objetivo - 1;
        if (objetivoSoldado->protegido) {
            cout << "-> ¡" << objetivoSoldado->nombre << " bloqueo el ataque con su mecanismo de proteccion! No recibe dano." << endl;
            objetivoSoldado->protegido = false;
        } else {
            objetivoSoldado->vida = (int)fmax(0, objetivoSoldado->vida - danio); // Funcion math
            cout << "-> Causas " << danio << " HP de dano a " << objetivoSoldado->nombre << "." << endl;
        }
    } else {
        cout << "-> Objetivo invalido. Perdiste el turno." << endl;
    }
}

// Ataque de area a todos los enemigos activos
void ejecutarAtaqueAreaHeroe(Enemigo* jefe, Enemigo* ejercito, int cantidadEjercito) {
    int danioArea;
    cout << "Especifica los HP de dano de area para TODOS los enemigos: ";
    if (!(cin >> danioArea)) return;
    danioArea = abs(danioArea); // Funcion math

    if (jefe != nullptr && jefe->vida > 0) {
        if (jefe->protegido) {
            cout << "-> ¡El Comandante " << jefe->nombre << " bloqueo el dano de area con su proteccion!" << endl;
            jefe->protegido = false;
        } else {
            jefe->vida = (int)fmax(0, jefe->vida - danioArea); // Funcion math
        }
    }

    for (int i = 0; i < cantidadEjercito; i++) {
        if ((ejercito + i)->vida > 0) {
            if ((ejercito + i)->protegido) {
                cout << "-> ¡" << (ejercito + i)->nombre << " [" << i + 1 << "] bloqueo el dano de area con su proteccion!" << endl;
                (ejercito + i)->protegido = false;
            } else {
                (ejercito + i)->vida = (int)fmax(0, (ejercito + i)->vida - danioArea); // Funcion math
            }
        }
    }
    cout << "-> Atacas en area aplicando " << danioArea << " HP de dano a cada enemigo activo." << endl;
}

// Curacion del Heroe consumiendo una pocion (+50 HP)
void ejecutarCuracionHeroe(Heroe& heroe) {
    if (heroe.pociones > 0) {
        heroe.vida = (int)fmin(heroe.vida + 50, heroe.vidaMax); // Funcion math (fmin)
        heroe.pociones--;
        cout << "-> Te curas +50 HP. Te quedan " << heroe.pociones << " pociones. (HP actual: " 
             << heroe.vida << "/" << heroe.vidaMax << ")" << endl;
    } else {
        cout << "-> ¡No tienes pociones restantes! Perdiste el turno." << endl;
    }
}

// Tarea: Mecanismo de proteccion del Heroe
// Gana una jugada defensiva para recargar vida y bloquear el siguiente golpe enemigo
void ejecutarProteccionHeroe(Heroe& heroe) {
    heroe.protegido = true;
    int recarga = 30;
    heroe.vida = (int)fmin(heroe.vida + recarga, heroe.vidaMax); // Funcion math
    cout << "-> [MECANISMO DE PROTECCION] El Heroe activa su escudo defensivo para ganar una jugada!" << endl;
    cout << "   Recarga +" << recarga << " HP (HP actual: " << heroe.vida << "/" << heroe.vidaMax 
         << ") y bloqueara el proximo dano enemigo." << endl;
}

// Gestion del turno completo del Heroe
void turnoHeroe(Heroe& heroe, Enemigo* jefe, Enemigo*& ejercito, int& cantidadEjercito) {
    cout << endl;
    cout << "OPCIONES DEL HEROE:" << endl;
    cout << "1. Ataque Individual (Elegir objetivo e ingresar HP de dano)" << endl;
    cout << "2. Ataque de Area (Ingresar HP de dano para TODOS los enemigos)" << endl;
    cout << "3. Usar Pocion Curativa (+50 HP)" << endl;
    cout << "4. Mecanismo de Proteccion (Gana una jugada, recarga +30 HP y activa escudo)" << endl;
    cout << "Elige accion (1-4): ";
    int opcion;
    if (!(cin >> opcion)) return;

    switch (opcion) {
        case 1:
            ejecutarAtaqueIndividualHeroe(jefe, ejercito, cantidadEjercito);
            break;
        case 2:
            ejecutarAtaqueAreaHeroe(jefe, ejercito, cantidadEjercito);
            break;
        case 3:
            ejecutarCuracionHeroe(heroe);
            break;
        case 4:
            ejecutarProteccionHeroe(heroe);
            break;
        default:
            cout << "-> Opcion invalida. Perdiste el turno." << endl;
            break;
    }

    // Los soldados sin vida desaparecen de inmediato tras el ataque del Heroe
    eliminarSoldadosSinVida(ejercito, cantidadEjercito);
}

// ============================================================================
// ACCIONES DE LOS ENEMIGOS
// ============================================================================

// Tipo de Ataque 1: El comandante ataca de manera independiente
// Puede realizar ataque directo, sacrificar un soldado para curarse, o activar su proteccion
void turnoJefe(Enemigo* jefe, Enemigo* ejercito, int cantidadEjercito, Heroe& heroe) {
    if (jefe == nullptr || jefe->vida <= 0) return;

    cout << endl;
    cout << "------------------------------------------------------" << endl;
    cout << " TURNO DEL COMANDANTE (" << jefe->nombre << " - HP: " << jefe->vida << ")" << endl;
    cout << "------------------------------------------------------" << endl;
    cout << "1. Ataque directo al Heroe" << endl;
    cout << "2. Sacrificar un soldado para curarse HP" << endl;
    cout << "3. Activar mecanismo de proteccion para ganar jugada y recargar vida" << endl;
    cout << "Elige accion del Comandante (1-3): ";
    int accion;
    if (!(cin >> accion)) return;

    if (accion == 1) {
        int danio;
        cout << "Ingresa la cantidad de HP de ataque del Comandante al Heroe: ";
        if (!(cin >> danio)) return;
        danio = abs(danio); // Funcion math

        if (heroe.protegido) {
            cout << "-> ¡El Heroe bloqueo el ataque del Comandante con su mecanismo de proteccion! No recibe dano." << endl;
            heroe.protegido = false;
        } else {
            heroe.vida = (int)fmax(0, heroe.vida - danio); // Funcion math
            cout << "-> " << jefe->nombre << " causa " << danio << " HP de dano al Heroe." << endl;
        }
    } 
    else if (accion == 2) {
        int soldadoVivo = -1;
        for (int i = 0; i < cantidadEjercito; i++) {
            if ((ejercito + i)->vida > 0) {
                soldadoVivo = i;
                break;
            }
        }

        if (soldadoVivo != -1) {
            int hpAbsorbido = (ejercito + soldadoVivo)->vida;
            cout << "-> ¡" << jefe->nombre << " sacrifica a " << (ejercito + soldadoVivo)->nombre 
                 << " y absorbe sus " << hpAbsorbido << " HP!" << endl;
            jefe->vida += hpAbsorbido;
            (ejercito + soldadoVivo)->vida = 0; // El soldado muere debido al sacrificio
        } else {
            cout << "No hay soldados vivos para sacrificar. Realiza ataque directo al Heroe:" << endl;
            int danio;
            cout << "Ingresa HP de ataque directo: ";
            if (!(cin >> danio)) return;
            danio = abs(danio);

            if (heroe.protegido) {
                cout << "-> ¡El Heroe bloqueo el ataque del Comandante con su mecanismo de proteccion! No recibe dano." << endl;
                heroe.protegido = false;
            } else {
                heroe.vida = (int)fmax(0, heroe.vida - danio);
                cout << "-> " << jefe->nombre << " causa " << danio << " HP de dano al Heroe." << endl;
            }
        }
    } 
    else if (accion == 3) {
        jefe->protegido = true;
        int recarga = 30;
        jefe->vida += recarga;
        cout << "-> [MECANISMO DE PROTECCION] ¡" << jefe->nombre 
             << " activa su escudo protector, gana una jugada defensiva y recarga +" << recarga 
             << " HP! (HP actual: " << jefe->vida << ")" << endl;
    } 
    else {
        cout << "-> Accion invalida del Comandante. Pierde la accion." << endl;
    }
}

// Tipo de Ataque 2: El comandante ordenar a uno de sus soldados
void ordenarAtaqueSoldado(Enemigo* ejercito, int cantidadEjercito, Heroe& heroe) {
    int vivos = contarSoldadosVivos(ejercito, cantidadEjercito);
    if (vivos == 0) {
        cout << "-> No hay soldados vivos disponibles para recibir ordenes." << endl;
        return;
    }

    cout << endl;
    cout << "El Comandante selecciona a un soldado para atacar:" << endl;
    for (int i = 0; i < cantidadEjercito; i++) {
        if ((ejercito + i)->vida > 0) {
            cout << "  " << i + 1 << ". " << (ejercito + i)->nombre 
                 << " (HP: " << (ejercito + i)->vida << ")" << endl;
        }
    }
    cout << "Elige que soldado realizara el ataque (1-" << cantidadEjercito << "): ";
    int soldadoIndex;
    if (!(cin >> soldadoIndex)) return;

    if (soldadoIndex >= 1 && soldadoIndex <= cantidadEjercito && (ejercito + soldadoIndex - 1)->vida > 0) {
        Enemigo* atacante = ejercito + soldadoIndex - 1;

        cout << "Accion para el soldado " << atacante->nombre << ":" << endl;
        cout << "1. Atacar al Heroe" << endl;
        cout << "2. Activar mecanismo de proteccion (ganar jugada defensiva y recargar +15 HP)" << endl;
        cout << "Elige accion (1-2): ";
        int accionSoldado;
        if (!(cin >> accionSoldado)) return;

        if (accionSoldado == 1) {
            int danioSoldado;
            cout << "Ingresa los HP de ataque de " << atacante->nombre << " hacia el Heroe: ";
            if (!(cin >> danioSoldado)) return;
            danioSoldado = abs(danioSoldado);

            if (heroe.protegido) {
                cout << "-> ¡El Heroe bloqueo el ataque de " << atacante->nombre << " con su escudo protector! No recibe dano." << endl;
                heroe.protegido = false;
            } else {
                heroe.vida = (int)fmax(0, heroe.vida - danioSoldado);
                cout << "-> " << atacante->nombre << " acata la orden y resta " << danioSoldado << " HP al Heroe." << endl;
            }
        } else if (accionSoldado == 2) {
            atacante->protegido = true;
            atacante->vida += 15;
            cout << "-> ¡" << atacante->nombre << " activa su mecanismo de proteccion y recarga +15 HP! (HP actual: " 
                 << atacante->vida << ")" << endl;
        } else {
            cout << "-> Accion invalida del soldado. Pierde su jugada." << endl;
        }
    } else {
        cout << "-> Seleccion de soldado invalida. Se perdio la orden." << endl;
    }
}

// Tipo de Ataque 3: El comandante ordenar a todo el ejercito
void turnoEjercito(Enemigo* ejercito, int cantidadEjercito, Heroe& heroe) {
    int vivos = contarSoldadosVivos(ejercito, cantidadEjercito);
    if (vivos == 0) {
        cout << "  (No quedan soldados vivos en el ejercito)" << endl;
        return;
    }

    cout << endl;
    cout << "-> ¡El Comandante ordena a TODO el ejercito avanzar en formacion y atacar!" << endl;
    for (int i = 0; i < cantidadEjercito; i++) {
        if ((ejercito + i)->vida > 0 && heroe.vida > 0) {
            int danioSoldado;
            cout << "Ingresa los HP de ataque de " << (ejercito + i)->nombre 
                 << " [" << i + 1 << "] hacia el Heroe: ";
            if (!(cin >> danioSoldado)) return;
            danioSoldado = abs(danioSoldado);

            if (heroe.protegido) {
                cout << "-> ¡El Heroe bloqueo el ataque de " << (ejercito + i)->nombre << " con su escudo protector!" << endl;
                heroe.protegido = false;
            } else {
                heroe.vida = (int)fmax(0, heroe.vida - danioSoldado);
                cout << "-> " << (ejercito + i)->nombre << " resta " << danioSoldado << " HP al Heroe." << endl;
            }
        }
    }
}

// Turno de los Enemigos: Controla los 3 tipos de ataque requeridos por la tarea
void turnoEnemigos(Enemigo* jefe, Enemigo*& ejercito, int& cantidadEjercito, Heroe& heroe) {
    int soldadosVivos = contarSoldadosVivos(ejercito, cantidadEjercito);
    bool jefeVivo = (jefe != nullptr && jefe->vida > 0);

    if (!jefeVivo && soldadosVivos == 0) return;

    cout << endl;
    cout << "======================================================" << endl;
    cout << "               TURNO DE LOS ENEMIGOS                  " << endl;
    cout << "======================================================" << endl;

    if (jefeVivo && soldadosVivos > 0) {
        cout << "Estrategia de ataque enemigo dirigida por el Comandante:" << endl;
        cout << "1. El comandante ataca de manera independiente" << endl;
        cout << "2. El comandante ordenar a uno de sus soldados" << endl;
        cout << "3. El comandante ordenar a todo el ejercito" << endl;
        cout << "Elige tipo de ataque (1-3): ";
        int tipoAtaque;
        if (!(cin >> tipoAtaque)) return;

        switch (tipoAtaque) {
            case 1:
                turnoJefe(jefe, ejercito, cantidadEjercito, heroe);
                break;
            case 2:
                ordenarAtaqueSoldado(ejercito, cantidadEjercito, heroe);
                break;
            case 3:
                turnoEjercito(ejercito, cantidadEjercito, heroe);
                break;
            default:
                cout << "-> Tipo de ataque invalido. Los enemigos pierden la iniciativa." << endl;
                break;
        }
    } else if (jefeVivo && soldadosVivos == 0) {
        cout << "El Comandante lucha solo ya que todo su ejercito ha sido eliminado." << endl;
        turnoJefe(jefe, ejercito, cantidadEjercito, heroe);
    } else if (!jefeVivo && soldadosVivos > 0) {
        cout << "El Comandante ha caido, pero las tropas restantes contraatacan:" << endl;
        cout << "1. Atacar con un soldado individual" << endl;
        cout << "2. Atacar con todo el ejercito restante" << endl;
        cout << "Elige accion (1-2): ";
        int opt;
        if (!(cin >> opt)) return;
        if (opt == 1) {
            ordenarAtaqueSoldado(ejercito, cantidadEjercito, heroe);
        } else {
            turnoEjercito(ejercito, cantidadEjercito, heroe);
        }
    }

    // Si hubo sacrificio u otra baja durante el turno enemigo, limpiar el arreglo dinamico
    eliminarSoldadosSinVida(ejercito, cantidadEjercito);
}

// ============================================================================
// FUNCION PRINCIPAL (MAIN)
// ============================================================================

int main() {
    srand(time(0));

    // El Heroe reside localmente en la Pila (Stack)
    Heroe heroe = {"Heroe Legendario", 150, 150, 3, false};

    // Objetos dinamicos reservados en el Monticulo (Heap)
    int cantidadEjercito = 6;
    Enemigo* jefe = crearJefe("Gran General Comandante", 200);
    Enemigo* ejercito = crearEjercito(cantidadEjercito); // Puntero a arreglo dinamico (new[])

    cout << "======================================================" << endl;
    cout << "   COMBATE: HEROE VS COMANDANTE Y SU EJERCITO        " << endl;
    cout << "======================================================" << endl;

    while (cin && heroe.vida > 0 && ((jefe != nullptr && jefe->vida > 0) || contarSoldadosVivos(ejercito, cantidadEjercito) > 0)) {

        // Condicion de Victoria
        if ((jefe == nullptr || jefe->vida <= 0) && contarSoldadosVivos(ejercito, cantidadEjercito) == 0) {
            break;
        }

        mostrarEstado(heroe, jefe, ejercito, cantidadEjercito);

        // 1. Ejecutar turno del Heroe
        turnoHeroe(heroe, jefe, ejercito, cantidadEjercito);

        // Verificar si los enemigos fueron derrotados tras el ataque del Heroe
        if ((jefe == nullptr || jefe->vida <= 0) && contarSoldadosVivos(ejercito, cantidadEjercito) == 0) {
            break;
        }

        // 2. Ejecutar turno de los Enemigos
        turnoEnemigos(jefe, ejercito, cantidadEjercito, heroe);

        // Verificar si el Heroe fue derrotado
        if (heroe.vida <= 0) {
            break;
        }
    }

    // Resultado final del combate
    cout << endl;
    cout << "======================================================" << endl;
    if (heroe.vida > 0 && (jefe == nullptr || jefe->vida <= 0) && contarSoldadosVivos(ejercito, cantidadEjercito) == 0) {
        cout << " ¡VICTORIA! El Heroe ha derrotado al Jefe y a todo su ejercito." << endl;
    } else if (heroe.vida <= 0) {
        cout << " ¡HAS SIDO DERROTADO! El Heroe se quedo sin HP." << endl;
    } else {
        cout << " Fin de la simulacion." << endl;
    }
    cout << "======================================================" << endl;

    // Liberacion explicita de memoria en el Heap asignando punteros a nullptr
    if (jefe != nullptr) {
        delete jefe;
        jefe = nullptr;
    }

    if (ejercito != nullptr) {
        delete[] ejercito;
        ejercito = nullptr;
    }

    cout << endl << "[SISTEMA] Memoria liberada correctamente (delete / delete[] asignados a nullptr)." << endl;
    cout << "[SISTEMA] Programa finalizado sin fugas de memoria." << endl;

    return 0;
}
