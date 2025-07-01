# Kotlin Compiler

## Estructura del Proyecto

El proyecto esta separado en 3 partes:

- **compiler**: Es el compilador programado en c++
- **lambda**: La api utilizada para desplegar el compilador a la nube
- **frontend**: El frontend utilizado para renderizar el compilador en la web

## Estructura de compiler

compiler se conforma de la siguiente manera:

- **Inputs**: Carpeta con todos los inputs
- **Outputs**: Carpeta con todos los outputs
- **src**: Carpeta con el codigo fuente

## Estructura de src

## Clases/Structs

### Regla Gramatical

```ebnf
ClassDec ::= "class" Identifier "(" [ ClassParamList ] ")" "{" [ VarDecList ] "}"
```

### Clases utilizadas

- **ClassDec**: Representa una declaración de clase (nombre, parámetros, campos)
- **ClassDecList**: Lista de todas las clases del programa
- **ParamList / Param**: Lista y definición de parámetros del constructor
- **VarDecList / VarDec**: Lista y definición de variables miembro
- **ClassAccessor**: Expresión para acceso a miembros (atributos) de una instancia de clase

### Parseo de clases

- El parser reconoce la estructura de una clase usando `parseClassDec()`:
  1. Detecta la palabra clave `class` y el identificador.
  2. Parsea los parámetros del constructor con `parseClassParamList()`.
  3. Parsea los campos con `parseVarDecList()`.
  4. Construye el nodo `ClassDec` y lo agrega a la lista de clases (`ClassDecList`).
- Los parámetros pueden ser `var` o `val` y tienen tipo.
- Los campos se declaran dentro de las llaves `{ ... }`.

### Acceso a atributos (member access)

- El acceso a atributos se representa en la gramática como:
  ```ebnf
  Primary ::= ... | Identifier "." Identifier | ...
  ```
- En el parser, esto se reconoce en `parsePrimary()`:
  - Si tras un identificador hay un punto (`.`) y otro identificador, se construye un nodo `ClassAccessor`.
- En el AST, `ClassAccessor` almacena el nombre del objeto y el nombre del atributo.
- En la generación de código, el visitor calcula el offset del atributo dentro de la clase y genera el código para acceder a ese campo en memoria.

### Manejo de memoria y referencias

- En este compilador, **todas las variables de tipo clase almacenan una referencia (puntero) al objeto, no el objeto en sí**. Esto es equivalente al manejo de referencias implícitas en Kotlin y otros lenguajes modernos.
- Cuando se instancia una clase (por ejemplo, `val obj = MiClase(...)`):
  1. Se reserva espacio en el stack para el objeto (`subq $<size>, %rsp`).
  2. Se obtiene la dirección base del objeto (`leaq 0(%rsp), %rdi`).
  3. Se llama al constructor, que inicializa los campos en esa dirección.
  4. **La variable local `obj` almacena la dirección (referencia) del objeto**. Todas las operaciones posteriores sobre `obj` usan esa referencia.
- Cuando se asigna una variable de tipo clase a otra (`val b = a`), **ambas variables apuntan al mismo objeto** (referencia compartida).
- El acceso a atributos (`obj.atributo`) siempre se realiza a través de la referencia almacenada en la variable.
- No se realiza copia profunda del objeto al asignar o pasar como argumento, solo se copia la referencia.
- La información de offsets y tamaños de cada clase se almacena en la estructura `clases` (`unordered_map<string, ClassInfo>`).

### Operaciones implementadas

- Declaración de clases con parámetros y campos
- Inicialización de campos en el constructor
- Acceso a atributos de instancia (`objeto.atributo`)
- Llamada a constructores
- Soporte para múltiples clases y objetos
- **Manejo de referencias implícitas para objetos (punteros a memoria)**

### Estructura y diseño de los visitors

- **Visitor base (`Visitor`)**: Define métodos `visit` para cada tipo de nodo, incluyendo `ClassDec`, `ClassDecList` y `ClassAccessor`.
- **GenCodeVisitor**: Implementa la generación de código para clases, constructores y acceso a atributos. Usa mapas para offsets y tipos de campos. Métodos relevantes: `visit(ClassDec*)`, `visit(ClassAccessor*)`.
- **PrintVisitor**: Permite imprimir la estructura del AST, útil para depuración.

### Resumen del flujo

1. El parser construye el AST de las clases y los accesos a atributos.
2. El visitor de generación de código recorre el AST y produce el código ensamblador para clases, constructores y acceso a miembros.
3. El visitor de impresión permite visualizar la estructura de clases y accesos para depuración.

---

## Referencias y Punteros

### ¿Por qué las variables de objetos son referencias (punteros)?

- En este compilador, siguiendo el modelo de Kotlin y Java, **todas las variables de tipo clase son referencias**. Esto significa que la variable almacena la dirección de memoria donde está el objeto, no una copia del objeto.
- Esto permite que múltiples variables puedan referirse al mismo objeto y que las operaciones sobre una variable afecten al mismo objeto compartido.
- Es eficiente: copiar una referencia es mucho más rápido que copiar todo el contenido de un objeto grande.

### ¿Cómo funcionan las referencias en la implementación?

- Cuando se crea un objeto, se reserva espacio en memoria (en el stack) y se obtiene su dirección.
- Esa dirección se almacena en la variable que representa el objeto.
- Al asignar una variable de tipo clase a otra, solo se copia la dirección (referencia), no el contenido del objeto.
- El acceso a atributos y métodos siempre se realiza a través de la referencia.

### Paso de referencias a funciones

- Cuando se pasa un objeto como argumento a una función, **se pasa la referencia (puntero) al objeto**.
- Dentro de la función, el parámetro recibe la dirección del objeto original, por lo que cualquier cambio sobre el objeto afecta al mismo objeto fuera de la función.
- En el codegen, esto se implementa pasando la dirección del objeto en uno de los registros de argumentos (por ejemplo, `%rdi`, `%rsi`, etc.).
- No se realiza copia profunda del objeto al pasar como argumento.

### Ejemplo ilustrativo

```kotlin
val a = MiClase(1)
val b = a // b y a apuntan al mismo objeto
fun f(x: MiClase) {
    x.valor = 42 // modifica el objeto original
}
f(a)
```

En este ejemplo, tanto `a` como `b` son referencias al mismo objeto. La función `f` recibe la referencia y puede modificar el objeto original.

---
