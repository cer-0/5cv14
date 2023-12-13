int AnalizadorSemantico( TSimbolos *Ini, int numcopia )
{
	AuxTabla = Ini;
	push("G");
	//Mientras haya simbolos por analizar
	while( AuxTabla != NULL )
	{
		
		//Si hay un simbolo no terminal al frente de la pila, y es G
		if( !strcmp(QPila->derivacion, "G") )
		{
			/*
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
					while ( AuxTabla!= NULL && !strcmp(QPila->derivacion, "V")){
						pop();
						if( AuxTabla != NULL && !strcmp(AuxTabla->tipotoken, "ID") )
						{
							strcpy(AuxTabla->derivacion, "ID");
							push("ID");
							strcpy(AuxTabla->tipdat,tipodato);
							AuxTabla = AuxTabla->liga;
							if ( AuxTabla != NULL && !strcmp(AuxTabla->lexema, ",") ){
								strcpy(AuxTabla->derivacion, "SEP");
								push(",");
								push(" ");
								push("V");
								ImprimeDerivacion();
								AuxTabla = AuxTabla->liga;
							}else if( AuxTabla != NULL && !strcmp(AuxTabla->lexema, ";") )
							{//Sino, si es un ; se termina la instrucción
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
							printf("Error Sintactico: Se esperaba un ID\n");
							return -1;
						}
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
			} else */ if( AuxTabla != NULL && !strcmp(AuxTabla->tipotoken, "ID") ) { //Si empieza con ID
				ImprimeDerivacion();
				//Se elimina G de la pila
				pop();
				char tipo[1000]; //Contiene el tipo de dato que se va a analizar en todos los tokens
				//Busca el ID en la tabla de simbolos y guarda su tipo de dato
				TSimbolos *Aux2 = PTabla;
				while( Aux2 != NULL && strcmp(Aux2->tipotoken, "ID") && strcmp(Aux2->lexema, AuxTabla->lexema) )
					Aux2 = Aux2->liga;
				strcpy(tipo, Aux2->tipdat);
				push(tipo);
				push(" ");
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
						if( AuxTabla != NULL && ( !strcmp(AuxTabla->tipdat , tipo) ) )
						{
							push(AuxTabla->tipdat);
							AuxTabla = AuxTabla->liga;
							if( AuxTabla != NULL && !strcmp(AuxTabla->lexema, ";") )
							{
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
								push(" ");
								push(AuxTabla->lexema);
								AuxTabla = AuxTabla->liga;
								push(" ");
								push("S");
								ImprimeDerivacion();
							}
						} else {
							printf("Error Semantico. Se esperaba una operacion entre tipos compatibles.\n");
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