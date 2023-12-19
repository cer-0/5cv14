#include <map>
#include <string>

using namespace std;

FILE *Archivo;
char asig[1000];
map<string, int> precedencia;

void ConversionPosfija(TSimbolos *Ini){
	precedencia["*"] = 2;
	precedencia["/"] = 2;
	precedencia["-"] = 1;
	precedencia["+"] = 1;

	//Si es un ID, es una asignación
	if (!strcmp(AuxTabla->tipotoken,"ID"))
	{
		/*
		//Se agrega la variable y el operador de asignacion "=" a la notación
		push_pos(AuxTabla->lexema);
		AuxTabla = AuxTabla->liga;
		push_pos(AuxTabla->lexema);
		AuxTabla = AuxTabla->liga;
		*/
		AuxTabla = AuxTabla->liga->liga;
		//AQUI COMIENZA LA CONVERSION
		//Comienza a evaluar desde el primer operando mientras no sea ;
		while (strcmp(AuxTabla->lexema,";") != 0)
		{
			//Si es un número lo agrega a la notación
			if (!strcmp(AuxTabla->tipotoken,"NE") || !strcmp(AuxTabla->tipotoken,"ND"))
			{
				push_pos(AuxTabla);
			//Si es una variable lo agrega a la notación
			}else if (!strcmp(AuxTabla->tipotoken,"ID"))
			{
				push_pos(AuxTabla);
			//Si es un operador
			}else if (!strcmp(AuxTabla->tipotoken,"OA"))
			{
				//Si no hay operadores en la pila de operadores, hace push
				if (POperadores == NULL)
				{
					push_op(AuxTabla);
				}else
				{
					//Mientras haya operadores en la pila, si el operador en el tope es de mayor precedencia,
					//se saca de la pila y se introduce en la notación.
					//Si no, rompe el ciclo
					while (QOperadores!=NULL)
					{
						if ( (strcmp(QOperadores->lexema,"(") != 0) && (precedencia[QOperadores->lexema] > precedencia[AuxTabla->lexema]))
						{
							push_pos(QOperadores);
							pop_op();
						}else break;
					}
				
					push_op(AuxTabla);
				}
			//Si es un paréntesis
			}else if (!strcmp(AuxTabla->tipotoken,"PAR"))
			{
				if (!strcmp(AuxTabla->lexema,"("))
				{
					push_op(AuxTabla);
				}else if(!strcmp(AuxTabla->lexema,")"))
				{
					while (strcmp(QOperadores->lexema,"(")!= 0)
					{
						push_pos(QOperadores);
						pop_op();
					}
					pop_op();
				}
			}
			AuxTabla = AuxTabla->liga;
		}
			
		while (QOperadores != NULL)
		{
			push_pos(QOperadores);
			pop_op();
		}

		//Imprime la notacion posfija por consola
		AuxPosfija = PPosfija;
		while (AuxPosfija!=NULL)
		{
			printf("%s ",AuxPosfija->lexema);
			AuxPosfija = AuxPosfija->liga;
		}
		printf(";\n");
	}			
}

void PosfijaAEnsamblador(void)
{
	char ultimoToken[1000];
	char Op1[1000];
	char Op2[1000];
	char OpArit[1000];
	bool especial = true;
	AuxPosfija = PPosfija;
	int numtemp = -1;
	if( AuxPosfija != NULL && AuxPosfija->liga != NULL )
	{
		while( AuxPosfija != NULL )
		{
			//if( AuxPosfija->liga != NULL && AuxPosfija->liga->liga != NULL )
			//printf("AuxPosfija vale: %s %s %s\n", AuxPosfija->tipotoken, AuxPosfija->liga->tipotoken, AuxPosfija->liga->liga->tipotoken);
			bool operacion = false;
			//Si la estructura es Operando-Operando-Operador
			if( strcmp(AuxPosfija->tipotoken, "OA") && AuxPosfija->liga != NULL && AuxPosfija->liga->liga != NULL && strcmp(AuxPosfija->liga->tipotoken, "OA") && !strcmp(AuxPosfija->liga->liga->tipotoken, "OA") )
			{
				//Se guarda el ultimo token en la pila de jerarquias
				if( AuxPosfija != PPosfija && !especial )
					push_jerarquia(ultimoToken);
				else
					especial = false;
				//Se anota una operacion que va a utilizar un nuevo temporal
				strcpy( Op1, AuxPosfija->lexema );
				strcpy( Op2, AuxPosfija->liga->lexema);
				strcpy( OpArit, AuxPosfija->liga->liga->lexema);
				//Se indica el temporal que se va a utilizar
				numtemp++;
				char ult[1000];
				sprintf(ult, "T%d\0", numtemp);
				strcpy(ultimoToken, ult);
				operacion = true;
				AuxPosfija = AuxPosfija->liga->liga->liga;
		  	//Sino, si la estructura es Operando-Operador
			} else if( strcmp(AuxPosfija->tipotoken, "OA") && AuxPosfija->liga != NULL && !strcmp(AuxPosfija->liga->tipotoken, "OA") )
			{
				char Temp[1000];
		  		sprintf(Temp, "T%d\0", numtemp);
		  		//Se anota una operacion que va a utilizar un nuevo temporal
		  		strcpy( Op1, Temp);
		  		strcpy( Op2, AuxPosfija->lexema);
		  		strcpy( OpArit, AuxPosfija->liga->lexema);
		 		numtemp++;
				char ult[1000];
				sprintf(ult, "T%d\0", numtemp);
				strcpy(ultimoToken, ult);
		  		operacion = true;
				AuxPosfija = AuxPosfija->liga->liga;
		 		//Sino, si la estructura es Operador
			} else if( !strcmp(AuxPosfija->tipotoken, "OA") )
			{
				char Temp[1000];
				sprintf(Temp, "T%d\0", numtemp);
				//Se anota una operacion que va a utilizar un nuevo temporal
		  		strcpy( Op1, QPilaJerarquia->lexema);
				pop_jerarquia();
		  		strcpy( Op2, Temp);
		  		strcpy( OpArit, AuxPosfija->lexema);
		 		numtemp++;
				char ult[1000];
				sprintf(ult, "T%d\0", numtemp);
				strcpy(ultimoToken, ult);
		  		operacion = true;
				AuxPosfija = AuxPosfija->liga;
		  	//Sino, ocurre un caso especial
			} else {
				push_jerarquia(AuxPosfija->lexema);
				char ult[1000];
				sprintf(ult, "T%d\0", numtemp);
				strcpy(ultimoToken, ult);
				especial = true;
				operacion = false;
				AuxPosfija = AuxPosfija->liga;
			}

			//Si se va a realizar una operacion
			if( operacion )
			{
				if( !strcmp(OpArit, "+") )
				{
					fprintf(Archivo, "\tmov ax, %s\n", Op1);
					fprintf(Archivo, "\tmov bx, %s\n", Op2);
					fprintf(Archivo, "\tadd ax, bx\n");
					fprintf(Archivo, "\tmov T%d, ax\n\n", numtemp);
				} else if( !strcmp(OpArit, "-") )
				{
					fprintf(Archivo, "\tmov ax, %s\n", Op1);
					fprintf(Archivo, "\tmov bx, %s\n", Op2);
					fprintf(Archivo, "\tsub ax, bx\n");
					fprintf(Archivo, "\tmov T%d, ax\n\n", numtemp);
				} else if( !strcmp(OpArit, "*") )
				{
					fprintf(Archivo, "\tmov ax, %s\n", Op1);
					fprintf(Archivo, "\tmov bx, %s\n", Op2);
					fprintf(Archivo, "\tmul bx\n");
					fprintf(Archivo, "\tmov T%d, ax\n\n", numtemp);
				} else if( !strcmp(OpArit, "/") )
				{
					fprintf(Archivo, "\tmov ax, %s\n", Op1);
					fprintf(Archivo, "\tmov bx, %s\n", Op2);
					fprintf(Archivo, "\tdiv bx\n");
					fprintf(Archivo, "\t mov dx, 0\n");
					fprintf(Archivo, "\tmov T%d, ax\n", numtemp);
				}
			}
		}
		fprintf(Archivo, "\tmov ax, T%d\n", numtemp);
		fprintf(Archivo, "\tmov T%d, ax\n", ++numtemp);
		fprintf(Archivo, "\tmov ax, T%d\n", numtemp);
		fprintf(Archivo, "\tmov %s, ax\n\n", asig);
		numtemp++;
	} else {
		fprintf(Archivo, "\tmov ax, %s\n", AuxPosfija->lexema);
		fprintf(Archivo, "\tmov T0, ax\n");
		fprintf(Archivo, "\tmov ax, T0\n");
		fprintf(Archivo, "\tmov %s, ax\n\n", asig);
	}
}

void EscribeEnsamblador(const char* arc)
{
	Archivo = fopen(arc, "w");
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
					//Salta al primer identificador
					AuxTabla = AuxTabla->liga;
					while ( AuxTabla != NULL && !strcmp(AuxTabla->tipotoken, "ID") ) {
						fprintf(Archivo, "\t%s DW ?\n", AuxTabla->lexema);
						if( AuxTabla->liga != NULL && !strcmp(AuxTabla->liga->lexema, ",") )
						{
							AuxTabla = AuxTabla->liga;
							AuxTabla = AuxTabla->liga;
						}
						else
							break;
					}
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
					if( !strcmp(AuxTabla->tipotoken, "OA") )
						numtemp++;
					//Salta al siguiente caracter
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
			if( !strcmp( AuxTabla->tipotoken, "ID") && AuxTabla->liga != NULL && !strcmp(AuxTabla->liga->tipotoken, "AS") ) {	//Si se encuentra un identificador, es una asignacion
				strcpy(asig, AuxTabla->lexema);
				ConversionPosfija(AuxTabla);
				PosfijaAEnsamblador();
				VaciaPosfija();
				VaciaOperadores();
			}
			else
			{
				while( AuxTabla != NULL && strcmp( AuxTabla->lexema, ";" ) )
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