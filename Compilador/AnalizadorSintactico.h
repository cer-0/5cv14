int contadorllaves = 0;

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