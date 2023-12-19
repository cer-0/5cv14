
#include <map>
#include <string>



using namespace std;

map<string, int> precedencia;

void ConversionPosfija(TSimbolos *Ini){


	precedencia["*"] = 2;
	precedencia["/"] = 2;
	precedencia["-"] = 1;
	precedencia["+"] = 1;

	AuxTabla = Ini;
	while (AuxTabla!=NULL)
	{	
		//Si es un ID, es una asignación
		if (!strcmp(AuxTabla->tipotoken,"ID"))
		{
			//Se agrega la variable y el operador de asignacion "=" a la notación
			push_pos(AuxTabla->lexema);
			AuxTabla = AuxTabla->liga;
			push_pos(AuxTabla->lexema);
			AuxTabla = AuxTabla->liga;
			//AQUI COMIENZA LA CONVERSION
			//Comienza a evaluar desde el primer operando mientras no sea ;
			while (strcmp(AuxTabla->lexema,";")!= 0)
			{
				//Si es un número lo agrega a la notación
				if (!strcmp(AuxTabla->tipotoken,"NE") || !strcmp(AuxTabla->tipotoken,"ND"))
				{
					push_pos(AuxTabla->lexema);
				//Si es una variable lo agrega a la notación
				}else if (!strcmp(AuxTabla->tipotoken,"ID"))
				{
					push_pos(AuxTabla->lexema);
				//Si es un operador
				}else if (!strcmp(AuxTabla->tipotoken,"OA"))
				{
					//Si no hay operadores en la pila de operadores, hace push
					if (POperadores == NULL)
					{
						push_op(AuxTabla->lexema);
					}else
					{
						//Mientras haya operadores en la pila, si el operador en el tope es de mayor precedencia,
						//se saca de la pila y se introduce en la notación.
						//Si no, rompe el ciclo
						while (QOperadores!=NULL)
						{
							if ( (strcmp(QOperadores->lexema,"(") != 0) && (precedencia[QOperadores->lexema] > precedencia[AuxTabla->lexema]))
							{
								push_pos(QOperadores->lexema);
								pop_op();
							}else break;
						}
					
						push_op(AuxTabla->lexema);
					}
				//Si es un paréntesis
				}else if (!strcmp(AuxTabla->tipotoken,"PAR"))
				{
					if (!strcmp(AuxTabla->lexema,"("))
					{
						push_op(AuxTabla->lexema);
					}else if(!strcmp(AuxTabla->lexema,")"))
					{
						while (strcmp(QOperadores->lexema,"(")!= 0)
						{
							push_pos(QOperadores->lexema);
							pop_op();
						}
						pop_op();
					}
				}
				AuxTabla = AuxTabla->liga;
			}
			
			while (QOperadores != NULL)
			{
				push_pos(QOperadores->lexema);
				pop_op();
			}

			push_pos(";");


			
		//Si no, recorre la tabla hasta encontrar un ";"
		}else{

			while (strcmp(AuxTabla->lexema,";")!= 0)
			{
				AuxTabla = AuxTabla->liga;
			}
		}
		
		AuxTabla = AuxTabla->liga;

	}

	AuxPosfija = PPosfija;
	while (AuxPosfija!=NULL)
	{
		if (!strcmp(AuxPosfija->lexema,";"))
		{
			printf("%s\n",AuxPosfija->lexema);
		}else
		{
			printf("%s ",AuxPosfija->lexema);
		}
		
		AuxPosfija = AuxPosfija->liga;
		
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
				char asig[1000];
				strcpy( asig, AuxTabla->lexema );
				int numtemp = -1; //Esta variable cuenta los temporales que se van utilizando
				//Salta a la asignacion
				AuxTabla = AuxTabla->liga;
				//Si el siguiente no es una asignacion, termina el programa
				if( AuxTabla != NULL && strcmp( AuxTabla->tipotoken, "AS" ) )
					return;
				//Salta al primer ID o NUM
				AuxTabla = AuxTabla->liga;
				//Se recuerda el primer valor que aparece despues del signo =, por si solo existe ese en la instruccion
				char prim[100];
				if( AuxTabla != NULL )
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