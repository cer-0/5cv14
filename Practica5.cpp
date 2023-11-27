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
} *PTabla, *QTabla, *Aux, *NuevoTabla;

TSimbolos *PPila, *QPila, *AuxPila, *NuevoPila;

int IdentificaTipo( const char* );
int AnalizadorLexico(const char* );
int LeeArchivo(const char*);
void EscribeEnsamblador(const char*);
void AgregaTablaSimbolos(const char*, int);
//void GuardaTablaSimbolos(const char*);
int AnalizadorSintactico(TSimbolos*, int);
void push(const char* );
void pop();
void top();
void ImprimeDerivacion();
void VaciaPila(void);

char *palres[] = {"int", "float", "while"};

int indID=0,indNE=201,indND=301,indNX=401;

void ImprimeTabla()
{
	Aux = PTabla;
	printf("\t\t\t\tTABLA DE SIMBOLOS\n");
	printf("-----------------------------------------------------------------------------------------\n");
	printf("LEXEMA\t\tTIPO TOKEN\tTIPO DE DATO\tREGLA\t\tINDICE\n");
	printf("-----------------------------------------------------------------------------------------\n");
	while(Aux != NULL)
	{
		printf( "%s\t\t%s\t\t%s\t\t%s\t\t%d\n", Aux->lexema, Aux->tipotoken, Aux->tipdat, Aux->regla, Aux->ind );
		Aux = Aux->liga;
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
		ImprimeTabla();
		AnalizadorSintactico(PTabla, 0);
		/*if( !AnalizadorSintactico() )
			EscribeEnsamblador(programa);*/
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
				i++;
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
		while( c < 3 )
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
		while( fscanf(Archivo,"%s",cadena) != EOF )
		{
			//Si la cadena que extrajo tiene mas de un caracter, y el ultimo es ;
			if( strlen(cadena) > 1 && cadena[strlen(cadena) -1] == ';' )
			{
				//Crea una nueva cadena, que no contiene el ultimo caracter
				char cadsinpyc[1000];
				memset(cadsinpyc, 0, sizeof(cadsinpyc));
				strncpy( cadsinpyc, cadena, strlen(cadena)-1 );
				//Realiza dos analisis lexicos: uno para la cadena sin el ;, y otro para el ;
				int tipo = IdentificaTipo(cadsinpyc);
				if( tipo != -1 )		
					AgregaTablaSimbolos(cadsinpyc, tipo);
				else
				{
					printf("Error Lexico. No se reconoce: %s\n", cadsinpyc);
					return -1;
				}
				tipo = IdentificaTipo(";");		
				AgregaTablaSimbolos(";", tipo);
			} else if( strlen(cadena) > 1 && cadena[strlen(cadena) -1] == ',' ) {
				//Crea una nueva cadena, que no contiene el ultimo caracter
				char cadsinpyc[1000];
				memset(cadsinpyc, 0, sizeof(cadsinpyc));
				strncpy( cadsinpyc, cadena, strlen(cadena)-1 );
				//Realiza dos analisis lexicos: uno para la cadena sin el ;, y otro para el ;
				int tipo = IdentificaTipo(cadsinpyc);
				if( tipo != -1 )		
					AgregaTablaSimbolos(cadsinpyc, tipo);
				else
				{
					printf("Error Lexico. No se reconoce: %s\n", cadsinpyc);
					return -1;
				}
				tipo = IdentificaTipo(",");		
				AgregaTablaSimbolos(",", tipo);
			} else if( strlen(cadena) > 1 && ( cadena[strlen(cadena) -1] == '(' || cadena[strlen(cadena) -1] == ')' ) ) { //Sino, si la cadena que extrajo tiene mas de un caracter, y el ultimo es (
				//Crea una nueva cadena, que no contiene el ultimo caracter
				char cadsinpar[1000];
				memset(cadsinpar, 0, sizeof(cadsinpar));
				strncpy( cadsinpar, cadena, strlen(cadena)-1 );
				//Realiza dos analisis lexicos: uno para la cadena sin el ;, y otro para el ;
				int tipo = IdentificaTipo(cadsinpar);
				if( tipo != -1 )		
					AgregaTablaSimbolos(cadsinpar, tipo);
				else
				{
					printf("Error Lexico. No se reconoce: %s\n", cadsinpar);
					return -1;
				}
				if( cadena[ strlen(cadena)-1 ] == '(' )
				{
					tipo = IdentificaTipo( "(" );		
					AgregaTablaSimbolos("(", tipo);
				}
				else
				{
					tipo = IdentificaTipo( ")" );		
					AgregaTablaSimbolos(")", tipo);
				}
			} else {	//Sino, simplemente somete la cadena entera al Analisis Lexico
				int tipo = IdentificaTipo(cadena);
				if( tipo != -1 )		
					AgregaTablaSimbolos(cadena, tipo);
				else
				{
					printf("Error Lexico. No se reconoce: %s\n", cadena);
					return -1;
				}
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

		Aux = PTabla;
		//Primero, recorre todo el archivo fuente en busqueda de declaraciones
		//Para agregar las variables
		//O de asignaciones
		//Para agregar los temporales
		while( Aux != NULL )
		{
			//Si se encuentra una palabra reservada
			if( !strcmp( Aux->tipotoken, "PR" ) )
			{
				//Si la palabra reservada es un tipo de dato, procesa una declaracion
				if( !strcmp( Aux->lexema, "int" ) || !strcmp( Aux->lexema, "float" ) )
				{
					//Salta al identificador
					Aux = Aux->liga;
					//Si se encuentra un identificador
					if( !strcmp( Aux->tipotoken, "ID" ) )
						fprintf(Archivo, "\t%s DW ?\n", Aux->lexema);
					//Salta al punto y coma
					Aux = Aux->liga;
				}
			} else if( !strcmp( Aux->tipotoken, "ID" ) ) {	//Si no, si se encuentra un identificador, es una asignacion
				int numtemp = 0; //Esta variable cuenta el numero total de temporales que se necesitan para esta asignacion
				//Salta a la asignacion
				Aux = Aux->liga;
				//Si el siguiente es una asignacion, cuenta un temporal
				if( !strcmp( Aux->tipotoken, "AS" ) )
					numtemp++;
				//Salta al primer ID o NUM
				Aux = Aux->liga;
				while( strcmp( Aux->derivacion, "SEP" ) )	//Mientras queden tokens por analizar
				{
					//Si los tokens siguientes, incluyendo el actual, tienen juntos la estructura:
					//( ID OA ID | ID OA NUM | NUM OA ID | NUM OA NUM )
					//Cuenta otro temporal
					if( Aux != NULL )
						if( Aux->liga != NULL )
							if( Aux->liga->liga != NULL )
								if( ( !strcmp( Aux->derivacion, "ID" ) || !strcmp( Aux->derivacion, "NUM" ) ) && ( !strcmp( Aux->liga->derivacion, "OA" ) ) && ( !strcmp( Aux->liga->liga->derivacion, "ID" ) || !strcmp( Aux->liga->liga->derivacion, "NUM" ) ) )
								{
									numtemp++;
									//Salta hasta el OA
									Aux = Aux->liga;
								}
					//Salta al siguiente caracter. Si antes salto al OA, se desplaza dos tokens
					Aux = Aux->liga;
				}
				//Actualiza los temporales necesarios totales
				if( numtemp > tottemp )
					tottemp = numtemp;
			}
			//Avanza a la siguiente instruccion
			Aux = Aux->liga;
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
		Aux = PTabla;
		while( Aux != NULL )
		{
			if( !strcmp( Aux->tipotoken, "ID" ) ) {	//Si se encuentra un identificador, es una asignacion
				//Se recuerda la variable a la que se le va a asignar un valor
				char asig[100];
				strcpy( asig, Aux->lexema );
				int numtemp = -1; //Esta variable cuenta los temporales que se van utilizando
				//Salta a la asignacion
				Aux = Aux->liga;
				//Si el siguiente no es una asignacion, termina el programa
				if( strcmp( Aux->tipotoken, "AS" ) )
					return;
				//Salta al primer ID o NUM
				Aux = Aux->liga;
				//Se recuerda el primer valor que aparece despues del signo =, por si solo existe ese en la instruccion
				char prim[100];
				strcpy( prim, Aux->lexema );
				while( strcmp( Aux->derivacion, "SEP" ) )	//Mientras queden tokens por analizar
				{
					//Si los tokens siguientes, incluyendo el actual, tienen juntos la estructura:
					//( ID OA ID | ID OA NUM | NUM OA ID | NUM OA NUM )
					//Cuenta otro temporal
					if( Aux != NULL )
						if( Aux->liga != NULL )
							if( Aux->liga->liga != NULL )
								if( ( !strcmp( Aux->derivacion, "ID" ) || !strcmp( Aux->derivacion, "NUM" ) ) && ( !strcmp( Aux->liga->derivacion, "OA" ) ) && ( !strcmp( Aux->liga->liga->derivacion, "ID" ) || !strcmp( Aux->liga->liga->derivacion, "NUM" ) ) )
								{
									//Se va a utilizar el siguiente temporal
									numtemp++;
									//Si el operador es una suma
									if( !strcmp( Aux->liga->lexema, "+" ) )
									{
										//Genera el codigo ensamblador para la suma
										//Si ya se utilizo al menos un temporal
										if( numtemp > 0 )
											fprintf(Archivo, "\tmov ax, T%d\n", numtemp-1);
										else
											fprintf(Archivo, "\tmov ax, %s\n", Aux->lexema);
										fprintf(Archivo, "\tadd ax, %s\n", Aux->liga->liga->lexema);
										fprintf(Archivo, "\tmov T%d, ax\n\n", numtemp);
									} else if( !strcmp( Aux->liga->lexema, "-" ) ) //Si el operador es una resta
									{
										//Genera el codigo ensamblador para la resta
										if( numtemp > 0 )
											fprintf(Archivo, "\tmov ax, T%d\n", numtemp-1);
										else
											fprintf(Archivo, "\tmov ax, %s\n", Aux->lexema);
										fprintf(Archivo, "\tsub ax, %s\n", Aux->liga->liga->lexema);
										fprintf(Archivo, "\tmov T%d, ax\n\n", numtemp);
									} else if( !strcmp( Aux->liga->lexema, "*" ) ) //Si el operador es una multiplicacion
									{
										//Genera el codigo ensamblador para la multiplicacion
										if( numtemp > 0 )
											fprintf(Archivo, "\tmov ax, T%d\n", numtemp-1);
										else
											fprintf(Archivo, "\tmov ax, %s\n", Aux->lexema);
										fprintf(Archivo, "\tmov bx, %s\n", Aux->liga->liga->lexema);
										fprintf(Archivo, "\tmul bx\n");
										fprintf(Archivo, "\tmov T%d, ax\n\n", numtemp);
									} else if( !strcmp( Aux->liga->lexema, "/" ) ) //Si el operador es una division
									{
										//Genera el codigo ensamblador para la division
										if( numtemp > 0 )
											fprintf(Archivo, "\tmov ax, T%d\n", numtemp-1);
										else
											fprintf(Archivo, "\tmov ax, %s\n", Aux->lexema);
										fprintf(Archivo, "\tmov bx, %s\n", Aux->liga->liga->lexema);
										fprintf(Archivo, "\tdiv bx\n");
										fprintf(Archivo, "\tmov T%d, ax\n\n", numtemp);
									}
									//Salta hasta el OA
									Aux = Aux->liga;
								}
					//Salta al siguiente caracter. Si antes salto al OA, se desplaza dos tokens
					Aux = Aux->liga;
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
				while( Aux != NULL && strcmp( Aux->tipotoken, "SEP" ) )
					Aux = Aux->liga;
			}
			//Avanza a la siguiente instruccion
			Aux = Aux->liga;
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
	Aux = Ini;
	push("G");
	//Mientras haya simbolos por analizar
	while( Aux != NULL )
	{
		ImprimeDerivacion();
		//Si hay un simbolo no terminal al frente de la pila, y es G
		if( !strcmp(QPila->derivacion, "G") )
		{
			//Se elimina de la pila
			pop();
			//Si el primer token es una palabra reservada
			if( !(strcmp(Aux->tipotoken, "PR")) )
			{
				strcpy(Aux->derivacion, "PR");
				//Si el token es un int o float, se trata de una declaracion
				if( !(strcmp(Aux->lexema, "int")) || !(strcmp(Aux->lexema, "float")) )
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
					//Mientras los siguientes sean no terminales V
					while ( Aux!= NULL && !strcmp(QPila->derivacion, "V")){
						pop();
						if( Aux != NULL && !strcmp(Aux->tipotoken, "ID") )
						{
							strcpy(Aux->derivacion, "ID");
							push("ID");
							strcpy(Aux->tipdat,tipodato);
							Aux = Aux->liga;
							if ( Aux != NULL && !strcmp(Aux->lexema, ",") ){
								strcpy(Aux->derivacion, "SEP");
								push(",");
								push(" ");
								push("V");
								ImprimeDerivacion();
								Aux = Aux->liga;
							}else if( Aux != NULL && !strcmp(Aux->lexema, ";") )
							{//Sino, si es un ; se termina la instrucción
								strcpy(Aux->derivacion, "SEP");
								push(";");
								ImprimeDerivacion();
								Aux = Aux->liga;
								if( Aux != NULL && !strcmp(Aux->lexema, "}") )
								{
									push("}");
									ImprimeDerivacion();
									Aux = Aux->liga;
									return 0;
								}
							}else{ //Si el siguiente no es , o ; , es un error sintactico
								printf("Error Sintactico: Se esperaba una , o un ;\n");
								return -1;
							}
						}else{
							printf("Error Sintactico: Se esperaba un ID\n");
							return -1;
						}
					}
				} else if( !strcmp(Aux->lexema, "while") ) {
					//GRAMATICA DEL WHILE VA ACA
					int numpar = 0;		//Cuenta el numero de pares de parentesis dentro de la condicion (incluyendo los principales)
					strcpy(Aux->derivacion, "while");
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
					Aux = Aux->liga;
					if( !strcmp(Aux->lexema, "(") )
					{
						numpar++;
						Aux = Aux->liga;
						//Mientras tenga que analizar una expresion condicional
						while( !strcmp(QPila->derivacion, "C") )
						{
							pop();
							if( !strcmp(Aux->lexema, "(") )
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
								Aux = Aux->liga;
							}
							else if( Aux != NULL )
									if( Aux->liga != NULL )
										if( Aux->liga->liga != NULL )
										{
											if( ( !strcmp( Aux->tipotoken, "ID" ) || !strcmp( Aux->tipotoken, "NE" ) || !strcmp( Aux->tipotoken, "ND" ) || !strcmp( Aux->tipotoken, "NX" ) ) && !strcmp( Aux->liga->tipotoken, "OR" ) && ( !strcmp( Aux->liga->liga->tipotoken, "ID" ) || !strcmp( Aux->liga->liga->tipotoken, "NE" ) || !strcmp( Aux->liga->liga->tipotoken, "ND" ) || !strcmp( Aux->liga->liga->tipotoken, "NX" ) ) )
											{
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
												if( Aux->liga->liga->liga != NULL && !strcmp( Aux->liga->liga->liga->tipotoken, "OL" ) )
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
													push(Aux->tipotoken);
													push(" ");
													push("OR");
													push(" ");
													push(Aux->liga->liga->tipotoken);
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
													Aux = Aux->liga;
													push(Aux->lexema);
													push(" ");
													push(Aux->liga->tipotoken);
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
													Aux = Aux->liga;
													Aux = Aux->liga;
													//Se deriva OL
													if( !strcmp( Aux->lexema, "&&" ) || !strcmp( Aux->lexema, "||" ) )
													{
														pop();
														push(" ");
														push(Aux->lexema);
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
													Aux = Aux->liga;
												} else if( Aux->liga->liga->liga != NULL && !strcmp( Aux->liga->liga->liga->lexema, ")" ) ) {
													TSimbolos *Act = Aux->liga->liga->liga;
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
														push(Aux->tipotoken);
														push(" ");
														push("OR");
														push(" ");
														push(Aux->liga->liga->tipotoken);
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
														Aux = Aux->liga;
														push(Aux->lexema);
														push(" ");
														push(Aux->liga->tipotoken);
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
														Aux = Aux->liga;
														Aux = Aux->liga;
														while( !strcmp(Aux->lexema, ")") )
															Aux = Aux->liga;
														//Se deriva OL
														if( !strcmp( Aux->lexema, "&&" ) || !strcmp( Aux->lexema, "||" ) )
														{
															pop();
															push(Aux->lexema);
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
															Aux = Aux->liga;
														} else {
															printf("Error Sintactico. Se esperaba un operador logico\n");
															return -1;
														}
													} else if( numpar == 0 )
													{
														pop();
														//Si son los parentesis principales, se deriva la expresion, se termina la comdicion del while
														//Se deriva CR => (ID|NUM) OR (ID|NUM)
														push(Aux->tipotoken);
														push(" ");
														push("OR");
														push(" ");
														push(Aux->liga->liga->tipotoken);
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
														Aux = Aux->liga;
														push(Aux->lexema);
														push(" ");
														push(Aux->liga->tipotoken);
														for( int it=0; it<totpar; it++ )
															push(")");
														push("{");
														push("G");
														push("}");
														ImprimeDerivacion();
														while( strcmp( QPila->derivacion, "{" ) )
															pop();
														while( strcmp( Aux->lexema, "{" ) )
															Aux = Aux->liga;
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
							AnalizadorSintactico(Aux->liga, numcopia+1);
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
			} else if( Aux != NULL && !strcmp(Aux->tipotoken, "ID") ) { //Si empieza con ID
				strcpy(Aux->derivacion, "ID");
				push("ID");
				Aux = Aux->liga;
				//Si el siguiente es un =, ocurre una asignacion
				if( Aux != NULL && !strcmp(Aux->tipotoken, "AS") )
				{
					strcpy(Aux->derivacion, "AS");
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
							{
								strcpy(Aux->derivacion, "ID");
								push("ID");
							}
							else
							{
								strcpy(Aux->derivacion, "NUM");
								push(Aux->tipotoken);
							}
							Aux = Aux->liga;
							if( Aux != NULL && !strcmp(Aux->lexema, ";") )
							{
								strcpy(Aux->derivacion, "SEP");
								Aux = Aux->liga;
								push(";");
								ImprimeDerivacion();
								if( Aux != NULL && !strcmp(Aux->lexema, "}") )
								{
									push("}");
									ImprimeDerivacion();
									Aux = Aux->liga;
									return 0;
								}
								break;
							}else if( Aux != NULL && !strcmp(Aux->tipotoken, "OA") ){
								strcpy( Aux->derivacion, "OA" );
								push(" ");
								push(Aux->lexema);
								Aux = Aux->liga;
								push(" ");
								push("S");
								ImprimeDerivacion();
							}else {
								printf("Error Sintactico: Se esperaba un Operador Aritmetico o un ;\n");
								return -1;
							}
						}
					}
				}
			} else {
				Aux = Aux->liga;
			}
		}
		if( Aux != NULL && !strcmp(Aux->lexema, "}") )
		{
			push("}");
			ImprimeDerivacion();
			Aux = Aux->liga;
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
		if( Aux != NULL )
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