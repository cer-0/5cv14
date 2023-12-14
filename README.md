# 5CV14 - Práctica Compiladores

Repositorio que almacena el proyecto final de la materia de compiladores

## Gramáticas del compilador

Las siguientes son las gramáticas definidas en el compilador.

### Gramáticas del análisis sintáctico

Gramática de asignación:
```txt
S -> int V | float V ;
V -> ID, V | ID
```

Gramática de operaciones aritméticas:
```txt
S -> ID = V;
V -> ID + V | num + V | ID - V | num - V | num - V | ID * V | num * V |
     ID / V | num / v | ID | num
```

Gramática de while:
```txt
S -> while ( C ) { sentencia } | while ( C ) { S } |
     while ( C ) { S sentencias } | while ( C ) { sentencias S sentencias }
    
C  -> (C) | CR OL C | CR
CR -> id OR id | id OR num | num OR id
OL -> && | || 
OR -> < | > | <= | >= | == | !=
```

### Gramáticas del análisis semántico

```txt
T_INT => int
INT_OP => ADD | SUB | MUL

T_FLOAT => float
FLOAT_OP => ADD | SUB | MUL | DIV

G => T_INT = T_INT INT_OP T_INT | T_FLOAT = (T_INT | T_FLOAT) FLOAT_OP (T_INT | T_FLOAT)
```

## Ayuda

- [Guía sencilla de git](https://rogerdudler.github.io/git-guide/index.es.html)
- [Documentación de git](https://git-scm.com/doc)
- [Manual de uso de Github](https://docs.github.com/es/get-started/quickstart)
