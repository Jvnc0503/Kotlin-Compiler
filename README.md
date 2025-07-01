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
- En el AST, `ClassAccessor` almacena el objeto y el nombre del atributo.
- En la generación de código, el visitor calcula el offset del atributo dentro de la clase y genera el código para acceder a ese campo en memoria.

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
