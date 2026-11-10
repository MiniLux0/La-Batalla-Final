# ⚔️ La Batalla Final por el Reino

Simulación interactiva de combate táctico por turnos desarrollada en **C++** como proyecto para el curso de **Fundamentos de Programación (CC112)** en la **Universidad Nacional de Ingeniería (UNI)**.

El proyecto demuestra el dominio práctico de la **gestión de memoria dinámica** (diferenciación entre memoria Stack y Heap), **aritmética de punteros estricta**, y redimensionamiento dinámico de arreglos en memoria libre sin fugas de memoria (*zero memory leaks*).

---

## 📋 Resumen del Proyecto

El Héroe Legendario ha llegado a las ruinas de una antigua fortaleza para enfrentarse al **Gran General Comandante** y a su **Ejército de Soldados** (Goblins, Esqueletos, Orcos y Zombis). El Comandante lidera desde el frente y sus tropas avanzan tácticamente, siendo capaz incluso de sacrificar a sus propios hombres para sobrevivir.

### 🧠 Arquitectura de Memoria y Punteros

| Entidad | Segmento de Memoria | Mecanismo de Asignación | Gestión de Liberación |
| :--- | :--- | :--- | :--- |
| **Héroe** | **Stack (Pila)** | Variable local en `main` | Automática (ámbito local) |
| **Comandante** | **Heap (Montículo)** | `new Enemigo` (`crearJefe`) | `delete jefe; jefe = nullptr;` |
| **Ejército** | **Heap (Montículo)** | `new Enemigo[cantidad]` (`crearEjercito`) | `delete[] ejercito; ejercito = nullptr;` |

- **Aritmética de Punteros Estricta**: Todo acceso a los miembros del arreglo del ejército se realiza exclusivamente mediante punteros:
  - `(ejercito + i)->vida`
  - `(ejercito + i)->nombre`
  - `*(tipos + (rand() % 4))`
  - *(Sin usar el operador de corchetes `[]` sobre el ejército)*.

---

## 🎯 Mecánicas y Reglas del Combate

### 1. Turno del Héroe
- **1. Ataque Individual**: Selecciona a un objetivo específico (el Comandante o un soldado) e inflige daño manual en HP.
- **2. Ataque de Área**: Aplica daño de área en HP a todos los enemigos activos simultáneamente.
- **3. Curación con Poción**: Consume una de sus 3 pociones para recuperar $+50$ HP (sin sobrepasar `vidaMax`).
- **4. Mecanismo de Protección**: Levanta una postura de guardia defensiva para ganar una jugada, recargar $+30$ HP y activar un escudo que bloquea por completo el siguiente daño recibido.

### 2. Turno del Bando Enemigo (3 Tipos de Ataque)
Cuando corresponde el turno al bando enemigo, el Comandante puede elegir una de las 3 estrategias:
1. **Ataque Independiente del Comandante (`turnoJefe`)**:
   - *Ataque directo* al Héroe.
   - *Sacrificio de un soldado*: Absorbe todos los HP restantes de un soldado vivo para curarse a sí mismo (el soldado sacrificado muere de inmediato).
   - *Mecanismo de protección*: Gana una jugada defensiva para recargar $+30$ HP y activar su escudo protector.
2. **El Comandante Ordena a un Soldado (`ordenarAtaqueSoldado`)**:
   - Da la orden a un soldado específico del ejército para que ataque al Héroe o active su guardia defensiva (+15 HP).
3. **El Comandante Ordena a Todo el Ejército (`turnoEjercito`)**:
   - Toda la tropa viva avanza en formación y realiza ataques secuenciales contra el Héroe.

### 3. Desaparición de Soldados Caídos (`eliminarSoldadosSinVida`)
Cada vez que un soldado llega a $0$ HP (por ataque individual, de área o por sacrificio del Comandante):
- Se notifica su baja en consola.
- Se reserva un nuevo bloque en el Heap con el tamaño exacto de soldados sobrevivientes (`new Enemigo[vivos]`).
- Se transfieren los datos mediante aritmética de punteros.
- Se libera el bloque anterior con `delete[] ejercito`.
- El soldado **desaparece tanto del listado en pantalla como físicamente de la memoria**.

---

## 🛠️ Compilación y Ejecución

El código no depende de librerías externas complejas, únicamente utiliza los encabezados estándar:
- `<iostream>`
- `<string>`
- `<cmath>`
- `<ctime>`

### Con GCC / MinGW:
```bash
g++ -Wall -Wextra -std=c++11 Batalla.cpp -o Batalla.exe
./Batalla.exe
```

### Con Clang:
```bash
clang++ -Wall -Wextra -std=c++11 Batalla.cpp -o Batalla.exe
./Batalla.exe
```

---

## 📂 Estructura del Repositorio

```text
La-Batalla-Final/
├── Batalla.cpp             # Código fuente C++ completo y documentado
├── Batalla.cpp.txt         # Versión en texto plano para entregas de laboratorio
├── Explicacion_Proyecto.md # Documento técnico completo para sustentación oral
├── README.md               # Documentación general del repositorio
└── .gitignore              # Exclusión de binarios y temporales
```

---

## 👨‍💻 Autor

- **MiniLux0** (Universidad Nacional de Ingeniería - UNI)
