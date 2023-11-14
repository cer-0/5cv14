/*Programadores:
	David
	Enrique
	Moises
	Labardini
  Compiladores 5CV14
  Profesor: Jesus Mora Jain
  Practica 5: Analizador sintactico
*/
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <windows.h>

struct TSimbolos
{
	//datos
	char lexema[1000];
	char tipotoken[3];
	char tipdat[3];
	char regla[10];
	int ind;
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

char *palres[] = {"int", "float"};

int indID=0,indNE=201,indND=301,indNX=401;

void ImprimeTabla()
{
	Aux = PTabla;
	while(Aux != NULL)
	{
		printf("%s ", Aux->lexema);
		Aux = Aux->liga;
	}
	printf("\n");
}

int main(void)	
{
	
	const char* archivo = "archivofuente.txt"; 
	
	//En lugar de ingresar una cadena, se leen del archivo
	LeeArchivo(archivo);
	ImprimeTabla();
	AnalizadorSintactico();
	system("PAUSE");
	return 0;
}

//1: ID
//3: Operador Relacional
//4: Asignacion
//5: Operador Logico
//6: Operador Aritmetico
//7: Entero
//8: Decimal
//9: Exponencial
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
				i++;
			break;
		case '<':
		case '>':
		case '=':
		case '!':
			if( lex[i] == '!' )
				tiptok = 5;
			else if( lex[i] == '=' )
				tiptok = 4;
			else
				tiptok = 3;
			i++;
			if( i+1 == t )
			{
				if( lex[i] == '=' )
					tiptok = 3;
				else
					i--;
				i++;
			}
			else if( i == t )
				break;
			else if( i < t)
				i--;
			break;
		case '*':
		case '/':
			i++;
			if(i == t )
				tiptok = 6;
			else
				i--;
			break;
		case '&':
			i++;
			if( i+1 == t && lex[i] == '&' )
			{
				tiptok = 5;
			}
			else
				i--;
			break;
		case '|':
			i++;
			if( i < t && lex[i] == '|' )
			{
				tiptok = 5;
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
				tiptok = 6;
				i++;
			}
			if( i < t && lex[i] >= '0' && lex[i] <= '9' )
			{
				i++;
				tiptok = 7;
				while( i < t && lex[i] >= '0' && lex[i] <= '9' )
					i++;
				if( i < t && lex[i] == '.' )
				{
					i++;
					if( i < t && lex[i] >= '0' && lex[i] <= '9' )
					{
						tiptok = 8;
						while( i < t && lex[i]>='0' && lex[i] <= '9' )
							i++;
					}
					else
						i--;
				}
				if( i < t && lex[i] == 'e' || lex[i] == 'E' )
				{
					i++;
					if( i < t && lex[i] == '+' || lex[i] == '-' )
						i++;
					if( i < t && lex[i] >= '0' && lex[i] <= '9' )
					{
						tiptok = 9;
						while( i < t && lex[i] >= '0' && lex[i] <= '9' )
							i++;
					}
					else
						i--;
				}
			}
			break;	
		case ';':
		case ',':
			tiptok = 10;
			i++;
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
		while( c < (2) )
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
		case 9:
			return 9;
		case 10:
			return 10;
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
			strcpy(NuevoTabla->tipotoken, "ID");
			NuevoTabla->ind = indID;
			indID++;
			break;
		case 2:
			strcpy(NuevoTabla->tipotoken, "PR");
			if (!strcmp (lex,"int")){
				NuevoTabla->ind = 517;
			}else{
				NuevoTabla->ind = 518;
			}
			break;
		case 3:
			strcpy(NuevoTabla->tipotoken, "OR");
			
			if (!strcmp(lex,">")){
				NuevoTabla->ind = 503;
			}else if (!strcmp(lex,"<")){
				NuevoTabla->ind = 504;
			}else if (!strcmp(lex,">=")){
				NuevoTabla->ind = 505;
			}else if (!strcmp(lex,"<=")){
				NuevoTabla->ind = 506;
			}else if (!strcmp(lex,"!=")){
				NuevoTabla->ind = 507;
			}else if (!strcmp(lex,"==")){
				NuevoTabla->ind = 508;
			}

			break;
		case 4:
			strcpy(NuevoTabla->tipotoken, "AS");
			NuevoTabla->ind = 800; //Indice de la asignacion
			break;
		case 5:
			strcpy(NuevoTabla->tipotoken, "OL");
			if (!strcmp(lex,"&&")){
				NuevoTabla->ind = 501;
			}else if (!strcmp(lex,"||")){
				NuevoTabla->ind = 502;
			}
			break;
		case 6:
			strcpy(NuevoTabla->tipotoken, "OA");
			strcpy(NuevoTabla->regla, "Aritm.");
			switch (lex[0]){
				case '*':
					NuevoTabla->ind = 510;
					break;
				case '/':
					NuevoTabla->ind = 511;
					break;
				case '+':
					NuevoTabla->ind = 512;
					break;
				case '-':
					NuevoTabla->ind = 513;
					break;
				case '=':
					NuevoTabla->ind = 514;
					break;
			}
			break;
		case 7:
			strcpy(NuevoTabla->tipotoken, "NE");
			strcpy(NuevoTabla->regla, "Aritm.");
			NuevoTabla->ind = indNE;
			indNE++;
			break;
		case 8:
			strcpy(NuevoTabla->tipotoken, "ND");
			strcpy(NuevoTabla->regla, "Aritm.");
			NuevoTabla->ind = indND;
			indND++;
			break;
		case 9:
			strcpy(NuevoTabla->tipotoken, "NX");
			strcpy(NuevoTabla->regla, "Aritm.");
			NuevoTabla->ind = indNX;
			indNX++;
			break;
		
		case 10:
			strcpy(NuevoTabla->tipotoken, "SEP");
			switch (lex[0]){
				case ',':
					NuevoTabla->ind = 515;
					break;
				case ';':
					NuevoTabla->ind = 516;
					break;
			}
			break;
		default:
			printf("Error Lexico %s\n", lex);
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

void AnalizadorSintactico(void)
{
	Aux = PTabla;
	push("G");
	ImprimeDerivacion();
	//Si hay simbolos por analizar
	if( Aux != NULL )
	{
		//Si al frente de la pila hay un simbolo no terminal G
		if( !strcmp(QPila->derivacion, "G") )
		{
			//Se elimina de la pila
			pop();
			//Si el token actual es una palabra reservada
			if( !(strcmp(Aux->tipotoken, "PR")))
			{
				//Si hay un simbolo no terminal al frente de la pila, lo elimina
				pop();
				//Se ingresa el nuevo token a la pila
				push(Aux->lexema);
				//Se avanza al siguiente caracter
				Aux = Aux->liga;
				//Se inserta un simbolo no terminal V
				push(" ");
				push("V");
				//Se imprime la derivacion
				ImprimeDerivacion();
				//Si quedan simbolos por analizar
				if( Aux != NULL )
				{
					while(Aux != NULL)
					{
						//Si hay un simbolo no terminal al frente de la pila, y es V
						if( !strcmp(QPila->derivacion, "V") )
						{
							//Si el token actual es un identificador
							if (!strcmp(Aux->tipotoken, "ID"))
							{
								//Si hay un simbolo no terminal al frente de la pila, lo elimina
								pop();
								//Se inserta un identificador en la pila
								push("ID");
								ImprimeDerivacion();
								//Si el siguiente caracter es =, ocurre una asignacion
								if( Aux->liga != NULL && !strcmp(Aux->liga->tipotoken, "AS") )
								{
									Aux = Aux->liga;
									//Entonces se agrega la asignacion a la pila
									push(" ");
									push("=");
									if( Aux->liga != NULL )
									{
										Aux = Aux->liga;
										push(" ");
										push("S");
									}else{
										printf("Error Sintactico\n");
										return;
									}
								}
							}else{	//Si no, ocurre un error sintactico
								printf("Error Sintactico\n");
								return;
							}
						}else if( !strcmp(Aux->lexema, ",") ){ //Si se encuentra una coma
							//Se inserta una coma y un simbolo no terminal V
							push(",");
							push(" ");
							push("V");
							//Se imprime la derivacion
							ImprimeDerivacion();
						}else if( !strcmp(Aux->lexema, ";") ){	//Si se encuentra un punto y coma
							push(";");							//Es el fin de la declaracion
							ImprimeDerivacion();				//Se imprime la derivacion
						}else{	//Si no, ocurre un error sintactico
							printf("Error Sintactico\n");
							return;
						}
						//Se avanza al siguiente caracter
						Aux = Aux->liga;
					}
				}else{ //Despues de la palabra reservada, se espera un identificador
				printf("Error Sintactico\n"); //Si no lo hay, es un error sintactico
				return;
				}	
			}else{ //Si no es ningun token reconocible, es un error sintactico
				printf("Error Sintactico\n");
				return;
			}
		}
	}
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
		printf("%s", AuxPila->derivacion);
		AuxPila = AuxPila->liga;
	}
	printf("\n");
}
