/*Programadores:
	David
	Enrique
	Moises
	Labardini
  Compiladores 5CV14
  Profesor: Jesus Mora Jain
  Programa: Compilador
*/
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

struct TSimbolos
{
	//datos
	char lexema[100];
	char tipotoken[100];
	char tipdat[100];
	char regla[100];
	int ind;
	char derivacion[100];
	//liga
	TSimbolos *liga;
} *PTabla, *QTabla, *AuxTabla, *NuevoTabla;

TSimbolos *PPila, *QPila, *AuxPila, *NuevoPila;

int IdentificaTipo( const char* );
int AnalizadorLexico(const char* );
int LeeArchivo(const char*);
void EscribeEnsamblador(const char*);
void AgregaTablaSimbolos(const char*, int);
//void GuardaTablaSimbolos(const char*);
int AnalizadorSintactico(TSimbolos*, int);
int AnalizadorSemantico(TSimbolos*, int);
void push(const char* );
void pop();
void top();
void ImprimeDerivacion();
void VaciaPila(void);

const char *palres[] = {"int", "float", "while"};

int indID=0,indNE=201,indND=301,indNX=401;

int contadorllaves = 0;

int i;

void ImprimeTabla()
{
	AuxTabla = PTabla;
	printf("\t\t\t\tTABLA DE SIMBOLOS\n");
	printf("-----------------------------------------------------------------------------------------\n");
	printf("LEXEMA\t\tTIPO TOKEN\tTIPO DE DATO\tREGLA\t\tINDICE\n");
	printf("-----------------------------------------------------------------------------------------\n");
	while(AuxTabla != NULL)
	{
		printf( "%s\t\t%s\t\t%s\t\t%s\t\t%d\n", AuxTabla->lexema, AuxTabla->tipotoken, AuxTabla->tipdat, AuxTabla->regla, AuxTabla->ind );
		AuxTabla = AuxTabla->liga;
	}
	printf("-----------------------------------------------------------------------------------------\n");
}

int main(void)	
{
	
	const char* archivo = "archivofuente.txt";
	const char* programa = "programafinal.txt";

	//En lugar de ingresar una cadena, se leen del archivo
	if( !LeeArchivo(archivo) )
	{
		if( !AnalizadorSintactico(PTabla, 0) )
		{
			ImprimeTabla();
			AnalizadorSemantico(PTabla, 0);
			/*
			if( !AnalizadorSemantico(PTabla, 0) )
				EscribeEnsamblador(programa);
			*/
		}
	}
	printf("Presione una tecla para continuar...");
	getchar();
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
	int t = strlen(lex), tiptok = 0;
	i = 0;

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
			if( i < t && lex[i] == '=' )
			{
				tiptok = 3;
				i++;
			}
			break;
		case '*':
		case '/':
			tiptok = 6;
			i++;
			break;
		case '&':
			i++;
			if( i < t && lex[i] == '&' )
			{
				tiptok = 5;
				i++;
			}
			else
				tiptok = 0;
			break;
		case '|':
			i++;
			if( i < t && lex[i] == '|' )
			{
				tiptok = 5;
				i++;
			}
			else
				tiptok = 0;
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
						return 0;
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
						return 0;
				}
			}
			break;	
		case ';':
		case ',':
			tiptok = 10;
			i++;
			break;
		case '(':
		case ')':
			tiptok = 11;
			i++;
			break;
		case '{':
		case '}':
			tiptok = 12;
			i++;
			break;
		default:
			break;
	}
	return tiptok;
}

int IdentificaTipo( const char* cad )
{
	int tipo = AnalizadorLexico( cad );
	char seAgrega[1000];
	memset(seAgrega, 0, sizeof(seAgrega));
	strncpy(seAgrega, cad, i);

	//2: Palabra Reservada
	int c = 0;
	if( tipo == 1 )
		while( c < 3 )
		{
			if( !strcmp( seAgrega, palres[c] ) )
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
		case 11:
			return 11;
		case 12:
			return 12;
		default:
			return -1;
	}
}

int LeeArchivo(const char* arc)
{
	FILE *Archivo = fopen(arc, "r");
	char cadena[1000];
	if( Archivo != NULL ) //Si el archivo se abre correctamente
	{
		int fincar;
		while( fscanf(Archivo,"%s",cadena) != EOF )
		{
			fincar = 0;
			char analiza[1000];
			//Se utiliza una cadena auxiliar llamada analiza para obtener los tokens dentro de ella
			strcpy(analiza, cadena);
			//Mientras no se hayan analizado todos los tokens dentro de la cadena leida del archivo
			while( fincar < strlen(cadena) )
			{
				//Se identifica el tipo del token
				int tipo = IdentificaTipo(analiza);
				//fincar llega al ultimo indice analizado en la cadena
				fincar += i;
				//Se copian los caracteres del ultimo analisis lexico a una cadena auxiliar llamada agrega
				char agrega[1000];
				memset(agrega, 0, sizeof(agrega));
				strncpy(agrega, analiza, i);
				//Si el token es reconocible, se agrega a la tabla de simbolos
				if( tipo != -1 )
					AgregaTablaSimbolos(agrega, tipo);
				else	//Sino, ocurre un error lexico
				{
					printf("Error Lexico: No se reconoce el token.\n");
					return -1;
				}
				//fincar indica el ultimo indice analizado en la cadena analiza pasada como parametro
				if( fincar < strlen(cadena) )
					strcpy(analiza, cadena+fincar);	//Se obtiene el resto de la cadena para el siguiente analisis
			}
		}
		fclose(Archivo);
		return 0;
	}
	else
	{
		printf("Ocurrio un error al intentar abrir el archivo\n\n");
		return -1;
	}
}

void EscribeEnsamblador(const char* arc)
{
	FILE *Archivo = fopen(arc, "w");
	int tottemp = 0;	//El total de temporales que se tienen que generar
	if( Archivo != NULL ) //Si el archivo se abre correctamente
	{
		fprintf(Archivo, ".model small\n\n");
		fprintf(Archivo, ".stack\n\n");
		fprintf(Archivo, ".data\n");
		//Nota: La generacion de codigo objeto asume que el archivo fuente esta lexica y sintacticamente correcto

		AuxTabla = PTabla;
		//Primero, recorre todo el archivo fuente en busqueda de declaraciones
		//Para agregar las variables
		//O de asignaciones
		//Para agregar los temporales
		while( AuxTabla != NULL )
		{
			//Si se encuentra una palabra reservada
			if( !strcmp( AuxTabla->tipotoken, "PR" ) )
			{
				//Si la palabra reservada es un tipo de dato, procesa una declaracion
				if( !strcmp( AuxTabla->lexema, "int" ) || !strcmp( AuxTabla->lexema, "float" ) )
				{
					//Salta al identificador
					AuxTabla = AuxTabla->liga;
					//Si se encuentra un identificador
					if( !strcmp( AuxTabla->tipotoken, "ID" ) )
						fprintf(Archivo, "\t%s DW ?\n", AuxTabla->lexema);
					//Salta al punto y coma
					AuxTabla = AuxTabla->liga;
				}
			} else if( !strcmp( AuxTabla->tipotoken, "ID" ) ) {	//Si no, si se encuentra un identificador, es una asignacion
				int numtemp = 0; //Esta variable cuenta el numero total de temporales que se necesitan para esta asignacion
				//Salta a la asignacion
				AuxTabla = AuxTabla->liga;
				//Si el siguiente es una asignacion, cuenta un temporal
				if( !strcmp( AuxTabla->tipotoken, "AS" ) )
					numtemp++;
				//Salta al primer ID o NUM
				AuxTabla = AuxTabla->liga;
				while( strcmp( AuxTabla->derivacion, "SEP" ) )	//Mientras queden tokens por analizar
				{
					//Si los tokens siguientes, incluyendo el actual, tienen juntos la estructura:
					//( ID OA ID | ID OA NUM | NUM OA ID | NUM OA NUM )
					//Cuenta otro temporal
					if( AuxTabla != NULL )
						if( AuxTabla->liga != NULL )
							if( AuxTabla->liga->liga != NULL )
								if( ( !strcmp( AuxTabla->derivacion, "ID" ) || !strcmp( AuxTabla->derivacion, "NUM" ) ) && ( !strcmp( AuxTabla->liga->derivacion, "OA" ) ) && ( !strcmp( AuxTabla->liga->liga->derivacion, "ID" ) || !strcmp( AuxTabla->liga->liga->derivacion, "NUM" ) ) )
								{
									numtemp++;
									//Salta hasta el OA
									AuxTabla = AuxTabla->liga;
								}
					//Salta al siguiente caracter. Si antes salto al OA, se desplaza dos tokens
					AuxTabla = AuxTabla->liga;
				}
				//Actualiza los temporales necesarios totales
				if( numtemp > tottemp )
					tottemp = numtemp;
			}
			//Avanza a la siguiente instruccion
			AuxTabla = AuxTabla->liga;
		}
		//Genera el numero de temporales totales necesarios
		for( int c=0; c<tottemp; c++ )
			fprintf( Archivo, "\tT%d DW ?\n", c );
		fprintf( Archivo, "\n");
		fprintf( Archivo, ".code\n");
		fprintf( Archivo, "begin:\n");
		fprintf( Archivo, "\tmov ax, @data\n");
		fprintf( Archivo, "\tmov ds, ax\n\n");
		//Ahora, genera el codigo del programa
		AuxTabla = PTabla;
		while( AuxTabla != NULL )
		{
			if( !strcmp( AuxTabla->tipotoken, "ID" ) ) {	//Si se encuentra un identificador, es una asignacion
				//Se recuerda la variable a la que se le va a asignar un valor
				char asig[100];
				strcpy( asig, AuxTabla->lexema );
				int numtemp = -1; //Esta variable cuenta los temporales que se van utilizando
				//Salta a la asignacion
				AuxTabla = AuxTabla->liga;
				//Si el siguiente no es una asignacion, termina el programa
				if( strcmp( AuxTabla->tipotoken, "AS" ) )
					return;
				//Salta al primer ID o NUM
				AuxTabla = AuxTabla->liga;
				//Se recuerda el primer valor que aparece despues del signo =, por si solo existe ese en la instruccion
				char prim[100];
				strcpy( prim, AuxTabla->lexema );
				while( strcmp( AuxTabla->derivacion, "SEP" ) )	//Mientras queden tokens por analizar
				{
					//Si los tokens siguientes, incluyendo el actual, tienen juntos la estructura:
					//( ID OA ID | ID OA NUM | NUM OA ID | NUM OA NUM )
					//Cuenta otro temporal
					if( AuxTabla != NULL )
						if( AuxTabla->liga != NULL )
							if( AuxTabla->liga->liga != NULL )
								if( ( !strcmp( AuxTabla->derivacion, "ID" ) || !strcmp( AuxTabla->derivacion, "NUM" ) ) && ( !strcmp( AuxTabla->liga->derivacion, "OA" ) ) && ( !strcmp( AuxTabla->liga->liga->derivacion, "ID" ) || !strcmp( AuxTabla->liga->liga->derivacion, "NUM" ) ) )
								{
									//Se va a utilizar el siguiente temporal
									numtemp++;
									//Si el operador es una suma
									if( !strcmp( AuxTabla->liga->lexema, "+" ) )
									{
										//Genera el codigo ensamblador para la suma
										//Si ya se utilizo al menos un temporal
										if( numtemp > 0 )
											fprintf(Archivo, "\tmov ax, T%d\n", numtemp-1);
										else
											fprintf(Archivo, "\tmov ax, %s\n", AuxTabla->lexema);
										fprintf(Archivo, "\tadd ax, %s\n", AuxTabla->liga->liga->lexema);
										fprintf(Archivo, "\tmov T%d, ax\n\n", numtemp);
									} else if( !strcmp( AuxTabla->liga->lexema, "-" ) ) //Si el operador es una resta
									{
										//Genera el codigo ensamblador para la resta
										if( numtemp > 0 )
											fprintf(Archivo, "\tmov ax, T%d\n", numtemp-1);
										else
											fprintf(Archivo, "\tmov ax, %s\n", AuxTabla->lexema);
										fprintf(Archivo, "\tsub ax, %s\n", AuxTabla->liga->liga->lexema);
										fprintf(Archivo, "\tmov T%d, ax\n\n", numtemp);
									} else if( !strcmp( AuxTabla->liga->lexema, "*" ) ) //Si el operador es una multiplicacion
									{
										//Genera el codigo ensamblador para la multiplicacion
										if( numtemp > 0 )
											fprintf(Archivo, "\tmov ax, T%d\n", numtemp-1);
										else
											fprintf(Archivo, "\tmov ax, %s\n", AuxTabla->lexema);
										fprintf(Archivo, "\tmov bx, %s\n", AuxTabla->liga->liga->lexema);
										fprintf(Archivo, "\tmul bx\n");
										fprintf(Archivo, "\tmov T%d, ax\n\n", numtemp);
									} else if( !strcmp( AuxTabla->liga->lexema, "/" ) ) //Si el operador es una division
									{
										//Genera el codigo ensamblador para la division
										if( numtemp > 0 )
											fprintf(Archivo, "\tmov ax, T%d\n", numtemp-1);
										else
											fprintf(Archivo, "\tmov ax, %s\n", AuxTabla->lexema);
										fprintf(Archivo, "\tmov bx, %s\n", AuxTabla->liga->liga->lexema);
										fprintf(Archivo, "\tdiv bx\n");
										fprintf(Archivo, "\tmov T%d, ax\n\n", numtemp);
									}
									//Salta hasta el OA
									AuxTabla = AuxTabla->liga;
								}
					//Salta al siguiente caracter. Si antes salto al OA, se desplaza dos tokens
					AuxTabla = AuxTabla->liga;
				}
				//Genera el codigo ensamblador para la asignacion
				numtemp++;
				if( numtemp > 0 ) //Si se uso al menos un temporal, se asigna el ultimo temporal a la variable
				{
					fprintf(Archivo, "\tmov ax, T%d\n", numtemp-1);
					fprintf(Archivo, "\tmov T%d, ax\n", numtemp);
					fprintf(Archivo, "\tmov ax, T%d\n", numtemp);
					fprintf(Archivo, "\tmov %s, ax\n\n", asig);
				} else {	//Sino, se asigna el primer valor despues del token = 
					fprintf(Archivo, "\tmov ax, %s\n", prim);
					fprintf(Archivo, "\tmov T%d, ax\n", numtemp);
					fprintf(Archivo, "\tmov ax, T%d\n", numtemp);
					fprintf(Archivo, "\tmov %s, ax\n\n", asig);
				}
			}
			else
			{
				while( AuxTabla != NULL && strcmp( AuxTabla->tipotoken, "SEP" ) )
					AuxTabla = AuxTabla->liga;
			}
			//Avanza a la siguiente instruccion
			AuxTabla = AuxTabla->liga;
		}
		fprintf( Archivo, "\tEND begin\n");
		fclose(Archivo);		
	}
	else
		printf("Ocurrio un error al intentar abrir el archivo\n\n");
}

void AgregaTablaSimbolos(const char* lex, int tiptok)
{
	NuevoTabla = (TSimbolos *)malloc(sizeof(TSimbolos));
	strcpy( NuevoTabla->lexema, lex );
	NuevoTabla->liga = NULL;
	switch( tiptok )
	{
		case 1:
			strcpy(NuevoTabla->tipotoken, "ID");
			strcpy( NuevoTabla->tipdat, QTabla->lexema );
			strcpy( NuevoTabla->regla, "D(D|A)*" );
			NuevoTabla->ind = indID;
			indID++;
			break;
		case 2:
			strcpy(NuevoTabla->tipotoken, "PR");
			if ( !strcmp(lex, "int" ) ){
				NuevoTabla->ind = 517;
				strcpy(NuevoTabla->tipdat, "int");
				strcpy(NuevoTabla->regla, "int ID");
			}else if( !strcmp( lex, "float" ) ){
				NuevoTabla->ind = 518;
				strcpy(NuevoTabla->tipdat, "float");
				strcpy(NuevoTabla->regla, "float ID");
			}else if( !strcmp( lex, "while" ) ){
				NuevoTabla->ind = 519;
				strcpy(NuevoTabla->tipdat, "NULL");
				strcpy(NuevoTabla->regla, "while(C){S}");
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
			strcpy( NuevoTabla->tipdat, "NULL" );
			strcpy( NuevoTabla->regla, "ID = ID|num" );
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
			strcpy( NuevoTabla->tipdat, "int" );
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
					strcpy( NuevoTabla->tipdat, "NULL" );
					strcpy( NuevoTabla->regla, "S;" );
					break;
			}
			break;
		case 11:
			strcpy(NuevoTabla->tipotoken, "PAR");
			switch( lex[0] )
			{
				case '(':
					NuevoTabla->ind = 701;
					strcpy(NuevoTabla->tipdat, "NULL");
					strcpy(NuevoTabla->regla, "(S)");
					break;
				case ')':
					NuevoTabla->ind = 702;
					strcpy(NuevoTabla->tipdat, "NULL");
					strcpy(NuevoTabla->regla, "(S)");
					break;
			}
			break;
		case 12:
			strcpy(NuevoTabla->tipotoken, "LLA");
			switch( lex[0] )
			{
				case '{':
					NuevoTabla->ind = 801;
					strcpy(NuevoTabla->tipdat, "NULL");
					strcpy(NuevoTabla->regla, "{S}");
					break;
				case '}':
					NuevoTabla->ind = 802;
					strcpy(NuevoTabla->tipdat, "NULL");
					strcpy(NuevoTabla->regla, "{S}");
					break;
			}
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

int AnalizadorSintactico( TSimbolos *Ini, int numcopia )
{
	AuxTabla = Ini;
	push("G");
	//Mientras haya simbolos por analizar
	while( AuxTabla != NULL )
	{
		ImprimeDerivacion();
		//Si hay un simbolo no terminal al frente de la pila, y es G
		if( !strcmp(QPila->derivacion, "G") )
		{
			//Se elimina de la pila
			pop();
			//Si el primer token es una palabra reservada
			if( !(strcmp(AuxTabla->tipotoken, "PR")) )
			{
				strcpy(AuxTabla->derivacion, "PR");
				//Si el token es un int o float, se trata de una declaracion
				if( !(strcmp(AuxTabla->lexema, "int")) || !(strcmp(AuxTabla->lexema, "float")) )
				{
					//Se declara una variable que guarda el tipo de dato declarado
					char tipodato [5];
					strcpy(tipodato,AuxTabla->lexema);
					//Se ingresa la palabra reservada a la pila
					push(AuxTabla->lexema);	
					push(" ");
					push("V");
					ImprimeDerivacion();
					//Se avanza al siguiente caracter
					AuxTabla = AuxTabla->liga;
					//Mientras los siguientes sean no terminales V
					if( AuxTabla != NULL && !strcmp(QPila->derivacion, "V") ) //Se asegura de que exista al menos un ID
					{
						while ( AuxTabla != NULL && !strcmp(QPila->derivacion, "V")){
							pop();
							if( AuxTabla != NULL && !strcmp(AuxTabla->tipotoken, "ID") )
							{
								strcpy(AuxTabla->derivacion, "ID");
								push("ID");
								//Localiza alguna declaracion anterior de esta variable
								//Si la encuentra, se muestra un error sintactico
								TSimbolos *Aux2 = PTabla;
								while( Aux2 != AuxTabla && (strcmp( Aux2->tipotoken, "ID" ) || strcmp( Aux2->lexema, AuxTabla->lexema )) )
									Aux2 = Aux2->liga;
								if( Aux2 != AuxTabla )
								{
									printf("Error Sintactico. La variable %s ya esta declarada.\n", AuxTabla->lexema);
									return -1;
								}
								strcpy(AuxTabla->tipdat,tipodato);
								AuxTabla = AuxTabla->liga;
								if ( AuxTabla != NULL && !strcmp(AuxTabla->lexema, ",") ){
									strcpy(AuxTabla->derivacion, "SEP");
									push(",");
									push(" ");
									push("V");
									ImprimeDerivacion();
									AuxTabla = AuxTabla->liga;
								}else if( AuxTabla != NULL && !strcmp(AuxTabla->lexema, ";") ) {//Sino, si es un ; se termina la instrucción
									strcpy(AuxTabla->derivacion, "SEP");
									push(";");
									ImprimeDerivacion();
									AuxTabla = AuxTabla->liga;
									if( AuxTabla != NULL && !strcmp(AuxTabla->lexema, "}") )
									{
										contadorllaves--;
										if( contadorllaves < 0 )
										{
											printf("Error Sintactico: Hay mas llaves de cierre que de apertura\n");
											return -1;
										}
										push("}");
										ImprimeDerivacion();
										AuxTabla = AuxTabla->liga;
										return 0;
									}
								}else{ //Si el siguiente no es , o ; , es un error sintactico
									printf("Error Sintactico: Se esperaba una , o un ;\n");
									return -1;
								}
							}else{
								printf("Error Sintactico: Se esperaba un identificador\n");
								return -1;
							}
						}
					} else {
						printf("Error Sintactico: Se esperaba un identificador");
					}
				} else if( !strcmp(AuxTabla->lexema, "while") ) {
					//GRAMATICA DEL WHILE VA ACA
					int numpar = 0;		//Cuenta el numero de pares de parentesis dentro de la condicion (incluyendo los principales)
					strcpy(AuxTabla->derivacion, "while");
					push("while");
					push("(");
					push("C");
					push(")");
					push("{");
					push("G");
					push("}");
					ImprimeDerivacion();
					while( strcmp(QPila->derivacion, "C") )
						pop();
					AuxTabla = AuxTabla->liga;
					if( !strcmp(AuxTabla->lexema, "(") )
					{
						numpar++;
						AuxTabla = AuxTabla->liga;
						//Mientras tenga que analizar una expresion condicional
						while( !strcmp(QPila->derivacion, "C") )
						{
							pop();
							if( !strcmp(AuxTabla->lexema, "(") )
							{
								numpar++;
								push("(");
								push("C");
								for( int it=0; it<numpar; it++ )
									push(")");
								push("{");
								push("G");
								push("}");
								ImprimeDerivacion();
								while( strcmp(QPila->derivacion, "C") )
									pop();
								AuxTabla = AuxTabla->liga;
							}
							else if( AuxTabla != NULL )
									if( AuxTabla->liga != NULL )
										if( AuxTabla->liga->liga != NULL )
										{
											if( ( !strcmp( AuxTabla->tipotoken, "ID" ) || !strcmp( AuxTabla->tipotoken, "NE" ) || !strcmp( AuxTabla->tipotoken, "ND" ) || !strcmp( AuxTabla->tipotoken, "NX" ) ) && !strcmp( AuxTabla->liga->tipotoken, "OR" ) && ( !strcmp( AuxTabla->liga->liga->tipotoken, "ID" ) || !strcmp( AuxTabla->liga->liga->tipotoken, "NE" ) || !strcmp( AuxTabla->liga->liga->tipotoken, "ND" ) || !strcmp( AuxTabla->liga->liga->tipotoken, "NX" ) ) )
											{
												if( !strcmp( AuxTabla->tipotoken, "ID" ) )
												{
													//Localiza la declaracion de la variable e identifica su tipo
													//Si no la encuentra, se muestra un error sintactico
													TSimbolos *Aux2 = PTabla;
													while( Aux2 != AuxTabla && (strcmp( Aux2->tipotoken, "ID" ) || strcmp( Aux2->lexema, AuxTabla->lexema )) )
														Aux2 = Aux2->liga;
													if( Aux2 == AuxTabla )
													{
														printf("Error Sintactico. La variable %s no esta declarada.\n", AuxTabla->lexema);
														return -1;
													}
													else
													{
														strcpy( AuxTabla->tipdat, Aux2->tipdat );
													}
												}
												if( !strcmp( AuxTabla->liga->liga->tipotoken, "ID" ) )
												{
													//Localiza la declaracion de la variable e identifica su tipo
													//Si no la encuentra, se muestra un error sintactico
													TSimbolos *Aux2 = PTabla;
													while( Aux2 != AuxTabla->liga->liga && (strcmp( Aux2->tipotoken, "ID" ) || strcmp( Aux2->lexema, AuxTabla->liga->liga->lexema )) )
														Aux2 = Aux2->liga;
													if( Aux2 == AuxTabla->liga->liga )
													{
														printf("Error Sintactico. La variable %s no esta declarada.\n", AuxTabla->lexema);
														return -1;
													}
													else
													{
														strcpy( AuxTabla->liga->liga->tipdat, Aux2->tipdat );
													}
												}

												push("CR");
												for( int it=0; it<numpar; it++ )
													push(")");
												push("{");
												push("G");
												push("}");
												ImprimeDerivacion();
												while( strcmp( QPila->derivacion, "CR" ) )
													pop();
												//Si despues, de la expresion condicional, aparece un operador logico
												if( AuxTabla->liga->liga->liga != NULL && !strcmp( AuxTabla->liga->liga->liga->tipotoken, "OL" ) )
												{
													//Se indica que C deriva en esta gramatica
													push(" ");
													push("OL");
													push(" ");
													push("C");
													for( int it=0; it<numpar; it++ )
														push(")");
													push("{");
													push("G");
													push("}");
													ImprimeDerivacion();
													while( strcmp( QPila->derivacion, "CR" ) )
														pop();
													pop();
													//Se deriva CR => (ID|NUM) OR (ID|NUM)
													push(AuxTabla->tipotoken);
													push(" ");
													push("OR");
													push(" ");
													push(AuxTabla->liga->liga->tipotoken);
													push(" ");
													push("OL");
													push(" ");
													push("C");
													for( int it=0; it<numpar; it++ )
														push(")");
													push("{");
													push("G");
													push("}");
													ImprimeDerivacion();
													while( strcmp( QPila->derivacion, "OR" ) )
														pop();
													pop();
													//Se deriva OR => && | ||
													AuxTabla = AuxTabla->liga;
													push(AuxTabla->lexema);
													push(" ");
													push(AuxTabla->liga->tipotoken);
													push(" ");
													push("OL");
													push(" ");
													push("C");
													for( int it=0; it<numpar; it++ )
														push(")");
													push("{");
													push("G");
													push("}");
													ImprimeDerivacion();
													while( strcmp( QPila->derivacion, "OL" ) )
														pop();
													pop();
													AuxTabla = AuxTabla->liga;
													AuxTabla = AuxTabla->liga;
													//Se deriva OL
													if( !strcmp( AuxTabla->lexema, "&&" ) || !strcmp( AuxTabla->lexema, "||" ) )
													{
														pop();
														push(" ");
														push(AuxTabla->lexema);
														push(" ");
														push("C");
														for( int it=0; it<numpar; it++ )
															push(")");
														push("{");
														push("G");
														push("}");
														ImprimeDerivacion();
														while( strcmp(QPila->derivacion, "C") )
															pop();
													} else {
														printf("Error Sintactico. Se esperaba un operador logico");
														return -1;
													}
													AuxTabla = AuxTabla->liga;
												} else if( AuxTabla->liga->liga->liga != NULL && !strcmp( AuxTabla->liga->liga->liga->lexema, ")" ) ) {
													TSimbolos *Act = AuxTabla->liga->liga->liga;
													numpar--;
													int totpar = 1;
													Act = Act->liga;
													while( !strcmp(Act->lexema, ")") )
													{
														numpar--;
														totpar++;
														Act = Act->liga;
													}
													if( numpar > 0 )
													{
														//Si no son los parentesis principales, se espera un operador logico
														//Se deriva CR => (ID|NUM) OR (ID|NUM)
														pop();
														push(AuxTabla->tipotoken);
														push(" ");
														push("OR");
														push(" ");
														push(AuxTabla->liga->liga->tipotoken);
														for( int it=0; it<totpar; it++ )
															push(")");
														push(" ");
														push("OL");
														push(" ");
														push("C");
														push(")");
														push("{");
														push("G");
														push("}");
														ImprimeDerivacion();
														while( strcmp( QPila->derivacion, "OR" ) )
															pop();
														pop();
														//Se deriva OR => && | ||
														AuxTabla = AuxTabla->liga;
														push(AuxTabla->lexema);
														push(" ");
														push(AuxTabla->liga->tipotoken);
														for( int it=0; it<totpar; it++ )
															push(")");
														push(" ");
														push("OL");
														push(" ");
														push("C");
														push(")");
														push("{");
														push("G");
														push("}");
														ImprimeDerivacion();
														while( strcmp(QPila->derivacion, "OL") )
															pop();
														AuxTabla = AuxTabla->liga;
														AuxTabla = AuxTabla->liga;
														while( !strcmp(AuxTabla->lexema, ")") )
															AuxTabla = AuxTabla->liga;
														//Se deriva OL
														if( !strcmp( AuxTabla->lexema, "&&" ) || !strcmp( AuxTabla->lexema, "||" ) )
														{
															pop();
															push(AuxTabla->lexema);
															push(" ");
															push("C");
															for( int it=0; it<totpar; it++ )
																push(")");
															push("{");
															push("G");
															push("}");
															ImprimeDerivacion();
															while( strcmp(QPila->derivacion, "C") )
																pop();
															AuxTabla = AuxTabla->liga;
														} else {
															printf("Error Sintactico. Se esperaba un operador logico\n");
															return -1;
														}
													} else if( numpar == 0 )
													{
														pop();
														//Si son los parentesis principales, se deriva la expresion, se termina la comdicion del while
														//Se deriva CR => (ID|NUM) OR (ID|NUM)
														push(AuxTabla->tipotoken);
														push(" ");
														push("OR");
														push(" ");
														push(AuxTabla->liga->liga->tipotoken);
														for( int it=0; it<totpar; it++ )
															push(")");
														push("{");
														push("G");
														push("}");
														ImprimeDerivacion();
														while( strcmp( QPila->derivacion, "OR" ) )
															pop();
														pop();
														//Se deriva OR => && | ||
														AuxTabla = AuxTabla->liga;
														push(AuxTabla->lexema);
														push(" ");
														push(AuxTabla->liga->tipotoken);
														for( int it=0; it<totpar; it++ )
															push(")");
														push("{");
														push("G");
														push("}");
														ImprimeDerivacion();
														while( strcmp( QPila->derivacion, "{" ) )
															pop();
														while( AuxTabla != NULL && strcmp( AuxTabla->lexema, "{" ) )
															AuxTabla = AuxTabla->liga;
														if( AuxTabla != NULL && !strcmp( AuxTabla->lexema, "{" ) )
															contadorllaves++;
														else
														{
															printf("Error Sintactico: Se esperabaw una llave de apertura");
															return -1;
														}
													} else {
														printf( "Error Sintactico. Existen mas parentesis de cierre que de apertura\n" );
														return -1;
													}
												} else {
													printf("Error Sintactico: Se esperaba un parentesis de cierre o una expresion logica\n");
													return -1;
												}
											} else {
												printf("Error Sintactico. Se esperaba una expresion condicional");
												return -1;					
											}
										} else {
											printf("Error. Se esperaba una expresion condicional\n");
											return -1;
										}
						}
						
						if( numpar == 0 && !strcmp( QPila->derivacion, "{" ) )
						{
							//AHORA ANALIZA LAS INSTRUCCIONES DEL BLOQUE WHILE
							AnalizadorSintactico(AuxTabla->liga, numcopia+1);
						} else if( numpar != 0 ) {
							printf("Error. No existe la misma cantidad de parentesis de apertura que de cierre");
							return -1;
						}
						 else if( strcmp( QPila->derivacion, "{" ) ) {
							printf("Error Sintactico. Se esperaba una llave de apertura");
							return -1;
						}

					} else {
						printf("Error Sintactico. Se esperaba un paréntesis de apertura");
						return -1;
					}
				}
			} else if( AuxTabla != NULL && !strcmp(AuxTabla->tipotoken, "ID") ) { //Si empieza con ID
				strcpy(AuxTabla->derivacion, "ID");
				push("ID");
				//Localiza la declaracion de la variable e identifica su tipo
				//Si no la encuentra, se muestra un error sintactico
				TSimbolos *Aux2 = PTabla;
				while( Aux2 != AuxTabla && (strcmp( Aux2->tipotoken, "ID" ) || strcmp( Aux2->lexema, AuxTabla->lexema )) )
					Aux2 = Aux2->liga;
				if( Aux2 == AuxTabla )
				{
					printf("Error Sintactico. La variable %s no esta declarada.\n", AuxTabla->lexema);
					return -1;
				}
				else
				{
					strcpy( AuxTabla->tipdat, Aux2->tipdat );
				}
				AuxTabla = AuxTabla->liga;
				//Si el siguiente es un =, ocurre una asignacion
				if( AuxTabla != NULL && !strcmp(AuxTabla->tipotoken, "AS") )
				{
					strcpy(AuxTabla->derivacion, "AS");
					push(" = ");
					push("S");
					ImprimeDerivacion();
					AuxTabla = AuxTabla->liga;
					//mientras los siguientes sean no terminales S
					while( AuxTabla != NULL && !strcmp(QPila->derivacion, "S") )
					{
						pop();
						if( AuxTabla != NULL && ( !strcmp(AuxTabla->tipotoken, "NE") || !strcmp(AuxTabla->tipotoken, "ND") || !strcmp(AuxTabla->tipotoken, "NX") || !strcmp(AuxTabla->tipotoken, "ID") ) )
						{
							if( !strcmp(AuxTabla->tipotoken, "ID") )
							{
								strcpy(AuxTabla->derivacion, "ID");
								push("ID");
								//Localiza la declaracion de la variable e identifica su tipo
								//Si no la encuentra, se muestra un error sintactico
								TSimbolos *Aux2 = PTabla;
								while( Aux2 != AuxTabla && (strcmp( Aux2->tipotoken, "ID" ) || strcmp( Aux2->lexema, AuxTabla->lexema )) )
									Aux2 = Aux2->liga;
								if( Aux2 == AuxTabla )
								{
									printf("Error Sintactico. La variable %s no esta declarada.\n", AuxTabla->lexema);
									return -1;
								}
								else
								{
									strcpy( AuxTabla->tipdat, Aux2->tipdat );
								}
							}
							else
							{
								strcpy(AuxTabla->derivacion, "NUM");
								push(AuxTabla->tipotoken);
							}
							AuxTabla = AuxTabla->liga;
							if( AuxTabla != NULL && !strcmp(AuxTabla->lexema, ";") )
							{
								strcpy(AuxTabla->derivacion, "SEP");
								AuxTabla = AuxTabla->liga;
								push(";");
								ImprimeDerivacion();
								if( AuxTabla != NULL && !strcmp(AuxTabla->lexema, "}") )
								{
									contadorllaves--;
									if( contadorllaves < 0 )
									{
										printf("Error Sintactico: Hay mas llaves de cierre que de apertura\n");
										return -1;
									}
									push("}");
									ImprimeDerivacion();
									AuxTabla = AuxTabla->liga;
									return 0;
								}
								break;
							}else if( AuxTabla != NULL && !strcmp(AuxTabla->tipotoken, "OA") ){
								strcpy( AuxTabla->derivacion, "OA" );
								push(" ");
								push(AuxTabla->lexema);
								AuxTabla = AuxTabla->liga;
								push(" ");
								push("S");
								ImprimeDerivacion();
							}else {
								printf("Error Sintactico: Se esperaba un Operador Aritmetico o un ;\n");
								return -1;
							}
						}
					}
				} else {
					printf("Error Sintactico: Se esperaba una asignacion.\n");
				}
			} else {
				printf("Error Sintactico: No se reconoce la sintaxis.\n");
				return -1;
			}
		}
		if( AuxTabla != NULL && !strcmp(AuxTabla->lexema, "}") )
		{
			push("}");
			ImprimeDerivacion();
			AuxTabla = AuxTabla->liga;
			return 0;
		}

		if( QPila != NULL && strcmp(QPila->derivacion, ";") && strcmp(QPila->derivacion, "}") )
		{//Si el ultimo caracter no fue ; es error sintactico
			printf("Error Sintactico: Se esperaba un ; o una llave de cierre\n");
			return -1;
		}
		if( numcopia == 0 )
		{
			VaciaPila();
			printf("\n");
		}
		if( AuxTabla != NULL )
				push("G");
	}
	if( numcopia == 0 && contadorllaves != 0 )
	{
		printf( "Error Sintactico: La cantidad de llaves de apertura no es la misma que las de cierre\n" );
	}
	return 0;
}

int AnalizadorSemantico( TSimbolos *Ini, int numcopia )
{
	AuxTabla = Ini;
	push("G");
	//Mientras haya simbolos por analizar
	while( AuxTabla != NULL )
	{
		//El analisis semantico asume que el codigo esta sintacticamente correcto
		//De modo que omite las comprobaciones de tokens consecutivos
		//Si hay un simbolo no terminal al frente de la pila, y es G
		if( !strcmp(QPila->derivacion, "G") )
		{
			//Si empieza con ID, y le sigue un =, ocurre una asignacion
			if( AuxTabla != NULL && !strcmp(AuxTabla->tipotoken, "ID") && AuxTabla->liga != NULL && !strcmp(AuxTabla->liga->tipotoken, "AS") ) {
				ImprimeDerivacion();
				//Se elimina G de la pila
				pop();
				char tipo[1000]; //Contiene el tipo de dato que se va a analizar en todos los tokens
				memset(tipo, 0, sizeof(tipo));
				strcpy(tipo, AuxTabla->tipdat);
				push(tipo);
				push(" ");
				//Como el analisis sintactico se ejecuta antes se asume que esta sintacticamente correcto
				//De modo que se salta el token = sin comprobacion
				AuxTabla = AuxTabla->liga;
				//Si el siguiente es un =, ocurre una asignacion
				if( AuxTabla != NULL && !strcmp(AuxTabla->tipotoken, "AS") )
				{
					push(" = ");
					push(" ");
					push("S");
					ImprimeDerivacion();
					AuxTabla = AuxTabla->liga;
					//mientras los siguientes sean no terminales S
					while( AuxTabla != NULL && !strcmp(QPila->derivacion, "S") )
					{
						pop();
						//Si el tipo de la variable que guarda la asignacion es int
						if( !strcmp(tipo, "int") )
						{
							if( strcmp(AuxTabla->tipdat, "int") )
							{
								printf("Error Semantico. Solo pueden asignarse valores enteros a una variable entera\n");
								return -1;
							} else if ( !strcmp(AuxTabla->liga->lexema, "/") ) {
								printf("Error Semantico. No se puede asignar el resultado de una division a una variable entera\n");
								return -1;
							} else {
								push(AuxTabla->tipdat);
								AuxTabla = AuxTabla->liga;
								if( AuxTabla != NULL && !strcmp(AuxTabla->lexema, ";") )
								{
									AuxTabla = AuxTabla->liga;
									push(";");
									ImprimeDerivacion();
									break;
								}else if( AuxTabla != NULL && !strcmp(AuxTabla->tipotoken, "OA") ){
									push(" ");
									push(AuxTabla->lexema);
									AuxTabla = AuxTabla->liga;
									push(" ");
									push("S");
									ImprimeDerivacion();
								}	
							}
						} else if( !strcmp(tipo, "float") ) { //Sino, si el tipo de la variable que guarda la asignacion es float
							push(AuxTabla->tipdat);
							AuxTabla = AuxTabla->liga;
							if( AuxTabla != NULL && !strcmp(AuxTabla->lexema, ";") )
							{
								AuxTabla = AuxTabla->liga;
								push(";");
								ImprimeDerivacion();
								break;
							}else if( AuxTabla != NULL && !strcmp(AuxTabla->tipotoken, "OA") ){
								push(" ");
								push(AuxTabla->lexema);
								AuxTabla = AuxTabla->liga;
								push(" ");
								push("S");
								ImprimeDerivacion();
							}
						} else {
							printf("Error Semantico. No se reconoce el tipo\n");
							return -1;
						}
					}
				}
			} else {
				AuxTabla = AuxTabla->liga;
			}
		}
		VaciaPila();

		if( AuxTabla != NULL )
				push("G");
	}
	return 0;
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
			QPila->liga = NULL;
			free(AuxPila);
		}
		else
		{
			free(AuxPila);
			PPila = NULL;
			QPila = NULL;
		}	
	}
}

void top()
{
	printf("%s\n", QPila->derivacion);
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

void VaciaPila()
{
	while( PPila != NULL )
		pop();
}