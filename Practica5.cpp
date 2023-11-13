/*Programador: David Emmanuel Nicolas Castañeda
  Compiladores 5CV14
  Profr: Jesus Mora Jain
  Practica 3: Reconocedor de tokens con tabla de simbolos
*/
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <windows.h>

struct TSimbolos
{
	//datos
	char lexema[1000];
	char tipotoken[1000];
	char derivacion[1000];
	//liga
	TSimbolos *liga;
} *PTabla, *QTabla, *Aux, *NuevoTabla;

TSimbolos *PPila, *QPila, *AuxPila, *NuevoPila;

int IdentificaTipo( const char* );
int AnalizadorLexico(const char* );
void LeeArchivo(const char*);
void AgregaTablaSimbolos(const char*, int);
//void GuardaTablaSimbolos(const char*);
void AnalizadorSintactico(void);
void push(const char* );
void pop();
void top();
void ImprimeDerivacion();

char *palres[] = {"for", "int", "while", "if", "float", 
				"printf", "do", "double", "char", "bool"};


int main(void)
{
	const char* archivo = "archivofuente.txt"; 
	
	//En lugar de ingresar una cadena, se leen del archivo
	LeeArchivo(archivo);
	//GuardaTablaSimbolos("TablaS.txt");
	AnalizadorSintactico();
	system("PAUSE");
	return 0;
}

//1: ID
//3: Operador Relacional
//4: Operador Logico
//5: Operador Aritmetico
//6: Entero
//7: Decimal
//8: Exponencial
int AnalizadorLexico( const char* lex )
{
	int i = 0, t = strlen(lex), tiptok;

	switch( lex[i] )
	{
		case 'a'...'z':
		case 'A'...'Z':
			tiptok = 1;
			i++;
			while( i < t && isalnum( lex[i] ) )
			{
				i++;
			}
			break;
		case '<':
		case '>':
		case '=':
		case '!':
			if( lex[i] == '!' )
				tiptok = 4;
			else
				tiptok = 3;
			i++;
			if( i < t )
			{
				if( lex[i] == '=' )
					tiptok = 3;
				else
					i--;
				i++;
			}
			break;
		case '*':
		case '/':
			i++;
			if(i == t )
				tiptok = 5;
			else
				i--;
			break;
		case '&':
			i++;
			if( i < t && lex[i] == '&' )
			{
				tiptok = 2;
			}
			else
				i--;
			break;
		case '|':
			i++;
			if( i < t && lex[i] == '|' )
			{
				tiptok = 2;
				i++;
			}
			else
				i--;
			break;
		case '0'...'9':
		case '+':
		case '-':
			if( lex[i] == '+' || lex[i] == '-' )
			{
				tiptok = 5;
				i++;
			}
			if( i < t && lex[i] >= '0' && lex[i] <= '9' )
			{
				i++;
				tiptok = 6;
				while( i < t && lex[i] >= '0' && lex[i] <= '9' )
					i++;
				if( i < t && lex[i] == '.' )
				{
					i++;
					if( i < t && lex[i] >= '0' && lex[i] <= '9' )
					{
						tiptok = 7;
						while( i < t && lex[i]>='0' && lex[i] <= '9' )
							i++;
					}
					else
						i--;
				}
				if( lex[i] == 'e' || lex[i] == 'E' )
				{
					i++;
					if( lex[i] == '+' || lex[i] == '-' )
						i++;
					if( i < t && lex[i] >= '0' && lex[i] <= '9' )
					{
						tiptok = 8;
						while( i < t && lex[i] >= '0' && lex[i] <= '9' )
							i++;
					}
					else
						i--;
				}
			}
			break;
		default:
			break;
	}
	if( i == t )
		return tiptok;
	return 0;
}

int IdentificaTipo( const char* cad )
{
	int tipo = AnalizadorLexico( cad );
	
	//2: Palabra Reservada
	int c = 0;
	if( tipo == 1 )
		while( c < (10) )
		{
			if( !(strcmp( cad, palres[c] ) ) )
			{
				tipo = 2;
				break;
			}
			c++;
		}
	
	switch( tipo )
	{
		case 1:
			return 1;
		case 2:
			return 2;
		case 3:
			return 3;
		case 4:
			return 4;
		case 5:
			return 5;
		case 6:
			return 6;
		case 7:
			return 7;
		case 8:
			return 8;
		default:
			return -1;
	}
}

void LeeArchivo(const char* arc)
{
	FILE *Archivo = fopen(arc, "r");
	char cadena[1000];
	if( Archivo != NULL ) //Si el archivo se abre correctamente
	{
		while( fscanf(Archivo,"%s",cadena) != EOF )
		{			
			int tipo = IdentificaTipo(cadena);		
			if(tipo != -1)
				AgregaTablaSimbolos(cadena, tipo);
		}
	}
	else
		printf("Ocurrio un error al intentar abrir el archivo\n\n");
}

void AgregaTablaSimbolos(const char* lex, int tiptok)
{
	NuevoTabla = (TSimbolos *)malloc(sizeof(TSimbolos));
	strcpy(NuevoTabla->lexema, lex);
	NuevoTabla->liga = NULL;
	switch( tiptok )
	{
		case 1:
			strcpy(NuevoTabla->tipotoken, "Identificador");
			break;
		case 2:
			strcpy(NuevoTabla->tipotoken, "Palabra Reservada");
			break;
		case 3:
			strcpy(NuevoTabla->tipotoken, "Operador Relacional");
			break;
		case 4:
			strcpy(NuevoTabla->tipotoken, "Operador Logico");
			break;
		case 5:
			strcpy(NuevoTabla->tipotoken, "Operador Aritmetico");
			break;
		case 6:
			strcpy(NuevoTabla->tipotoken, "Entero");
			break;
		case 7:
			strcpy(NuevoTabla->tipotoken, "Decimal");
			break;
		case 8:
			strcpy(NuevoTabla->tipotoken, "Exponencial");
			break;
	}
	
	if( PTabla == NULL)
	{
		PTabla = NuevoTabla;
		QTabla = PTabla;
	}
	else
	{
		QTabla->liga = NuevoTabla;
		QTabla = NuevoTabla;
	}		
}
/*
void GuardaTablaSimbolos(const char* arc)
{
	FILE *Archivo2 = fopen(arc, "w");
	char cadena[1000];
	if( Archivo2 != NULL ) //Si el archivo se abre correctamente
	{
		Aux = PTabla;
		while(Aux != NULL)
		{
			fprintf(Archivo2, "%s %s\n", Aux->lexema, Aux->tipotoken);
			Aux = Aux->liga;
		}
	}
	else
		printf("Ocurrio un error al intentar abrir el archivo\n\n");
	fclose(Archivo2);
}
*/

void AnalizadorSintactico(void)
{
	Aux = PTabla;
	printf("=> S\n");
	while( Aux != NULL )
	{
		//Si el token actual es un numero
		if( !(strcmp(Aux->tipotoken, "Entero")) || !(strcmp(Aux->tipotoken, "Decimal")) || !(strcmp(Aux->tipotoken, "Exponencial")) )
		{
			pop();
			push("num");
			Aux = Aux->liga;
			if(Aux != NULL)
			{
				if( !strcmp(Aux->lexema, "*") )
				{
					push(" ");
					push("*");
					push(" ");
					if( Aux->liga != NULL )
						push("S");
				}
				else if( !strcmp(Aux->lexema, "/") )
				{
					push(" ");
					push("/");
					push(" ");
					if( Aux->liga != NULL )
					push("S");
				}
				else
				{
					pop();
					push("E");
				}
				if(!strcmp(QPila->derivacion, "E"))
				{
					ImprimeDerivacion();
					pop();
					push("num");
					if( !strcmp(Aux->lexema, "+") )
					{
						push(" ");
						push("+");
						push(" ");
						if( Aux->liga != NULL )
							push("S");
					}
					else if( !strcmp(Aux->lexema, "-") )
					{
						push(" ");
						push("-");
						push(" ");
						if( Aux->liga != NULL )
							push("S");
					}
					else
					{
						printf("Error Sintactico\n");
						return;	
					}	
				}
				Aux = Aux->liga;	
			}
			ImprimeDerivacion();
		}
		else
		{
			printf("Error Sintactico\n");
			return;
		}
	}
	if( strcmp(QPila->derivacion, "num") )
		printf("Error Sintactico\n");
}

void push(const char* a)
{
	NuevoPila = (TSimbolos*)(malloc(sizeof(TSimbolos)));
	NuevoPila->liga = NULL;
	strcpy(NuevoPila->derivacion, a);
	if(PPila == NULL)
	{
		PPila = NuevoPila;
		QPila = PPila;
	}
	else
	{
		QPila->liga = NuevoPila;
		QPila = QPila->liga;	
	}
}

void pop()
{
	AuxPila = PPila;
	if(AuxPila != NULL)
	{
		if( PPila->liga != NULL )
		{	
			while(AuxPila->liga->liga != NULL)
				AuxPila = AuxPila->liga;
			QPila = AuxPila;
			AuxPila = QPila->liga;
			free(AuxPila);
		}
		else
		{
			free(AuxPila);
			PPila = NULL;
		}	
	}
}

void top()
{
	printf("%s", QPila->derivacion);
}

void ImprimeDerivacion()
{
	printf("=> ");
	AuxPila = PPila;
	while(AuxPila != NULL)
	{
		printf("%s ", AuxPila->derivacion);
		AuxPila = AuxPila->liga;
	}
	printf("\n");
}
