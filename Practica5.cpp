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
void EscribeArchivo(const char*);
void AgregaTablaSimbolos(const char*, int);
//void GuardaTablaSimbolos(const char*);
void AnalizadorSintactico(void);
void push(const char* );
void pop();
void top();
void ImprimeDerivacion();

char *palres[] = {"int", "float"};

int indID=0,indNE=201,indND=301,indNX=401;

int main(void)	
{
	
	const char* archivo = "archivofuente.txt";
	const char* programa = "programa.txt";

	//En lugar de ingresar una cadena, se leen del archivo
	LeeArchivo(archivo);
	AnalizadorSintactico();
	printf("BIEN\n");
	EscribeArchivo(programa);
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
		fclose(Archivo);
	}
	else
		printf("Ocurrio un error al intentar abrir el archivo\n\n");
}

void EscribeArchivo(const char* arc)
{
	FILE *Archivo = fopen("programa.txt", "w");
	if( Archivo != NULL ) //Si el archivo se abre correctamente
	{
		fprintf(Archivo, ".MODEL SMALL\n");
		fprintf(Archivo, ".STACK\n");
		fprintf(Archivo, ".DATA\n");
		Aux = PPila;
		while( Aux != NULL )
		{
			//Si se encuentra una palabra reservada
			//Tiene que declarar una variable
			if( !strcmp( Aux->tipotoken, "PR" ) )
			{
				Aux = Aux->liga;
				fprintf(Archivo, "%s DW ??\n", Aux->lexema);
				//Se salta el punto y coma
				Aux = Aux->liga;
				//Avanza a la siguiente declaracion
				Aux = Aux->liga;
			}
		}
		fclose(Archivo);		
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
	//Mientras haya simbolos por analizar
	while( Aux != NULL )
	{
		//Si hay un simbolo no terminal al frente de la pila, y es G
		if( !strcmp(QPila->derivacion, "G") )
		{
			//Se elimina de la pila
			pop();
			//Si el primer token es una palabra reservada, entonces es una declaracion
			if( !(strcmp(Aux->tipotoken, "PR")))
			{
				//Se declara una variable que guarda el tipo de dato declarado
				char tipodato [5];
				strcpy(tipodato,Aux->lexema);
				//Se ingresa la palabra reservada a la pila
				push(Aux->lexema);	
				push(" ");
				push("V");
				ImprimeDerivacion();
				//Se avanza al siguiente caracter
				Aux = Aux->liga;
				//Mientras los siguientes no sean no terminales V
				while ( Aux!= NULL && !strcmp(QPila->derivacion, "V")){
					pop();
					if( Aux != NULL && !strcmp(Aux->tipotoken, "ID") )
					{
						push("ID");
						strcpy(Aux->tipdat,tipodato);
						Aux = Aux->liga;
						if ( Aux != NULL && !strcmp(Aux->lexema, ",") ){
							push(" ");
							push(",");
							push(" ");
							push("V");
							ImprimeDerivacion();
							Aux = Aux->liga;
						}else if( Aux != NULL && !strcmp(Aux->lexema, ";") )
						{//Sino, si es un ; se termina la instrucción
							push(";");
							ImprimeDerivacion();
							Aux = Aux->liga;
							break;
						}else{
							printf("Error Sintactico\n");
							return;
						}
					}else{
							printf("Error Sintactico\n");
							return;
						}

				}
			}
			//Si empieza con ID
			if( !strcmp(Aux->tipotoken, "ID") )
			{
				push("ID");
				Aux = Aux->liga;
				//Si el siguiente es un =, ocurre una asignacion
				if( Aux != NULL && !strcmp(Aux->tipotoken, "AS") )
				{
					push(" = ");
					push("S");
					ImprimeDerivacion();
					Aux = Aux->liga;
					//mientras los siguientes sean no terminales S
					while( Aux != NULL && !strcmp(QPila->derivacion, "S") )
					{
						pop();
						if( Aux != NULL && ( !strcmp(Aux->tipotoken, "NE") || !strcmp(Aux->tipotoken, "ND") || !strcmp(Aux->tipotoken, "NX") || !strcmp(Aux->tipotoken, "ID") ) )
						{
							if( !strcmp(Aux->tipotoken, "ID") )
								push("ID");
							else
								push("num");
							Aux = Aux->liga;
							if( Aux != NULL && !strcmp(Aux->lexema, ";") )
							{
								Aux = Aux->liga;
								push(";");
								ImprimeDerivacion();
								break;
							}else if( Aux != NULL && !strcmp(Aux->tipotoken, "OA") ){
								push(" ");
								push(Aux->lexema);
								Aux = Aux->liga;
								push(" ");
								push("S");
								ImprimeDerivacion();
							}else {
								printf("Error Sintactico\n");
								return;
							}
						}
					}
				}
			}
		}
		if( strcmp(QPila->derivacion, ";") )
		{//Si el ultimo caracter no fue ; es error sintactico
			printf("Error Sintactico\n");
			return;
		}
		Aux = Aux->liga;
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
