#ifndef ENV
#define ENV

#include <iostream>
#include <list>
#include <string>
#include <unordered_map>
#include <vector>

#include "declaration.h"

using namespace std;

class Environment {
   private:
    vector<unordered_map<string, int>> levels;                      // Almacena valores de variables
    vector<unordered_map<string, Declaration>> declaration_levels;  // Almacena tipos de variables

    // Busca el nivel en el que está una variable
    int search_rib(string var) {
        int idx = levels.size() - 1;
        while (idx >= 0) {
            if (levels[idx].find(var) != levels[idx].end()) {
                return idx;
            }
            idx--;
        }
        return -1;
    }

   public:
    Environment() {}

    void clear() {
        levels.clear();
        declaration_levels.clear();
    }

    // Añadir un nuevo nivel
    void add_level() {
        unordered_map<string, int> l;
        unordered_map<string, Declaration> t;  // Mapa para tipos
        levels.push_back(l);
        declaration_levels.push_back(t);
    }

    // Añadir una variable con su valor y tipo
    void add_var(string var, int value, Declaration dec) {
        if (levels.size() == 0) {
            cout << "Environment sin niveles: no se pueden agregar variables" << endl;
            exit(0);
        }
        levels.back()[var] = value;
        declaration_levels.back()[var] = dec;
    }

    // Añadir una variable sin valor inicial
    void add_var(string var, Declaration dec) {
        levels.back()[var] = 0;  // Valor por defecto
        declaration_levels.back()[var] = dec;
    }

    // Remover un nivel
    bool remove_level() {
        if (levels.size() > 0) {
            levels.pop_back();
            declaration_levels.pop_back();
            return true;
        }
        return false;
    }

    // Actualizar el valor de una variable
    bool update(string x, int value) {
        int idx = search_rib(x);
        if (idx < 0)
            return false;
        levels[idx][x] = value;
        return true;
    }

    // Verificar si una variable está declarada
    bool check(string x) {
        int idx = search_rib(x);
        return (idx >= 0);
    }

    // Obtener el valor de una variable
    int lookup(string x) {
        int idx = search_rib(x);
        if (idx < 0) {
            cout << "Variable no declarada: " << x << endl;
            exit(0);
        }
        return levels[idx][x];
    }

    // Obtener el tipo de una variable
    Declaration lookup_type(string x) {
        int idx = search_rib(x);
        if (idx < 0) {
            cout << "Variable no declarada: " << x << endl;
            exit(0);
        }
        return declaration_levels[idx][x];
    }

    // Verificar el tipo de una variable antes de asignar un valor
    bool typecheck(string var, string expected_type) {
        string actual_type = lookup_type(var).type;
        if (actual_type != expected_type) {
            cout << "Error de tipo: se esperaba " << expected_type << " pero se encontró " << actual_type << " para la variable " << var << endl;
            return false;
        }
        return true;
    }
};

#endif
