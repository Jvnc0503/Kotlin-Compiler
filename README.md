# Kotlin Compiler

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
- En el AST, `ClassAccessor` almacena el nombre de la variable/objeto y el nombre del atributo.
- En la generación de código, el visitor calcula el offset del atributo dentro de la clase y genera el código para acceder a ese campo en memoria.

### Manejo de memoria y creación de objetos
- Cuando se instancia una clase (por ejemplo, `val obj = MiClase(...)`), el generador de código:
  1. Calcula el tamaño total del objeto sumando los offsets de los parámetros y campos.
  2. Reserva espacio en el stack con `subq $<size>, %rsp`.
  3. Obtiene la dirección base del objeto con `leaq 0(%rsp), %rdi` (el puntero al objeto se pasa en `%rdi`).
  4. Copia los argumentos del constructor a los registros correspondientes (`%rsi`, `%rdx`, ...).
  5. Llama al constructor de la clase (`call MiClase$ctor`).
  6. Al finalizar, la dirección del objeto queda en `%rdi` y se mueve a `%rax` para su uso posterior.
  7. Si el objeto es una variable local, **la variable almacena la dirección del objeto, no el objeto en sí**. Es decir, la variable contiene un puntero al área de memoria donde está el objeto.

- Para acceder a un atributo:
  1. Se recupera la dirección del objeto desde la pila (`movq <offset>(%rbp), %rax`).
  2. Se suma el offset del atributo dentro del objeto y se accede a su valor (`movq <offset>(%rax), %rax`).

- La información de offsets y tamaños de cada clase se almacena en la estructura `clases` (`unordered_map<string, ClassInfo>`).

### Operaciones implementadas
- Declaración de clases con parámetros y campos
- Inicialización de campos en el constructor
- Acceso a atributos de instancia (`objeto.atributo`)
- Llamada a constructores
- Soporte para múltiples clases y objetos

### Estructura y diseño de los visitors
- **Visitor base (`Visitor`)**: Define métodos `visit` para cada tipo de nodo, incluyendo `ClassDec`, `ClassDecList` y `ClassAccessor`.
- **GenCodeVisitor**: Implementa la generación de código para clases, constructores y acceso a atributos. Usa mapas para offsets y tipos de campos. Métodos relevantes: `visit(ClassDec*)`, `visit(ClassAccessor*)`.
- **PrintVisitor**: Permite imprimir la estructura del AST, útil para depuración.

### Resumen del flujo
1. El parser construye el AST de las clases y los accesos a atributos.
2. El visitor de generación de código recorre el AST y produce el código ensamblador para clases, constructores y acceso a miembros.
3. El visitor de impresión permite visualizar la estructura de clases y accesos para depuración.
