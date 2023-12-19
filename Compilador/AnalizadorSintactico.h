int contadorllaves = 0, contadorparentesis = 0;

int AnalizadorSintactico(TSimbolos *Ini, int numcopia)
{
	AuxTabla = Ini;
	push("G");
	// Mientras haya simbolos por analizar
	while (AuxTabla != NULL)
	{
		ImprimeDerivacion();
		// Si hay un simbolo no terminal al frente de la pila, y es G
		if (!strcmp(QPila->derivacion, "G"))
		{
			// Se elimina de la pila
			pop();
			// Si el primer token es una palabra reservada
			if (!(strcmp(AuxTabla->tipotoken, "PR")))
			{
				strcpy(AuxTabla->derivacion, "PR");
				// Si el token es un int o float, se trata de una declaracion
				if (!(strcmp(AuxTabla->lexema, "int")) || !(strcmp(AuxTabla->lexema, "float")))
				{
					// Se declara una variable que guarda el tipo de dato declarado
					char tipodato[5];
					strcpy(tipodato, AuxTabla->lexema);
					// Se ingresa la palabra reservada a la pila
					push(AuxTabla->lexema);
					push(" ");
					push("V");
					ImprimeDerivacion();
					// Se avanza al siguiente caracter
					AuxTabla = AuxTabla->liga;
					// Mientras los siguientes sean no terminales V
					if (AuxTabla != NULL && !strcmp(QPila->derivacion, "V")) // Se asegura de que exista al menos un ID
					{
						while (AuxTabla != NULL && !strcmp(QPila->derivacion, "V"))
						{
							pop();
							if (AuxTabla != NULL && !strcmp(AuxTabla->tipotoken, "ID"))
							{
								strcpy(AuxTabla->derivacion, "ID");
								push("ID");
								// Localiza alguna declaracion anterior de esta variable
								// Si la encuentra, se muestra un error sintactico
								TSimbolos *Aux2 = PTabla;
								while (Aux2 != AuxTabla && (strcmp(Aux2->tipotoken, "ID") || strcmp(Aux2->lexema, AuxTabla->lexema)))
									Aux2 = Aux2->liga;
								if (Aux2 != AuxTabla)
								{
									printf("Error Sintactico. La variable %s ya esta declarada.\n", AuxTabla->lexema);
									return -1;
								}
								strcpy(AuxTabla->tipdat, tipodato);
								AuxTabla = AuxTabla->liga;
								if (AuxTabla != NULL && !strcmp(AuxTabla->lexema, ","))
								{
									strcpy(AuxTabla->derivacion, "SEP");
									push(",");
									push(" ");
									push("V");
									ImprimeDerivacion();
									AuxTabla = AuxTabla->liga;
								}
								else if (AuxTabla != NULL && !strcmp(AuxTabla->lexema, ";"))
								{ // Sino, si es un ; se termina la instrucción
									strcpy(AuxTabla->derivacion, "SEP");
									push(";");
									ImprimeDerivacion();
									AuxTabla = AuxTabla->liga;
									if (AuxTabla != NULL && !strcmp(AuxTabla->lexema, "}"))
									{
										contadorllaves--;
										if (contadorllaves < 0)
										{
											printf("Error Sintactico: Hay mas llaves de cierre que de apertura\n");
											return -1;
										}
										push("}");
										ImprimeDerivacion();
										AuxTabla = AuxTabla->liga;
										return 0;
									}
								}
								else
								{ // Si el siguiente no es , o ; , es un error sintactico
									printf("Error Sintactico: Se esperaba una , o un ;\n");
									return -1;
								}
							}
							else
							{
								printf("Error Sintactico: Se esperaba un identificador\n");
								return -1;
							}
						}
					}
					else
					{
						printf("Error Sintactico: Se esperaba un identificador");
					}
				}
			}
			else if (AuxTabla != NULL && !strcmp(AuxTabla->tipotoken, "ID"))
			{ // Si empieza con ID
				strcpy(AuxTabla->derivacion, "ID");
				push("ID");
				// Localiza la declaracion de la variable e identifica su tipo
				// Si no la encuentra, se muestra un error sintactico
				TSimbolos *Aux2 = PTabla;
				while (Aux2 != AuxTabla && (strcmp(Aux2->tipotoken, "ID") || strcmp(Aux2->lexema, AuxTabla->lexema)))
					Aux2 = Aux2->liga;
				if (Aux2 == AuxTabla)
				{
					printf("Error Sintactico. La variable %s no esta declarada.\n", AuxTabla->lexema);
					return -1;
				}
				else
				{
					strcpy(AuxTabla->tipdat, Aux2->tipdat);
				}
				AuxTabla = AuxTabla->liga;
				// Si el siguiente es un =, ocurre una asignacion
				if (AuxTabla != NULL && !strcmp(AuxTabla->tipotoken, "AS"))
				{
					strcpy(AuxTabla->derivacion, "AS");
					push(" = ");
					push("S");
					ImprimeDerivacion();
					AuxTabla = AuxTabla->liga;
					// mientras los siguientes sean no terminales S
					while (AuxTabla != NULL && !strcmp(QPila->derivacion, "S"))
					{
						pop();
						if (AuxTabla != NULL && (!strcmp(AuxTabla->tipotoken, "NE") || !strcmp(AuxTabla->tipotoken, "ND") || !strcmp(AuxTabla->tipotoken, "NX") || !strcmp(AuxTabla->tipotoken, "ID")))
						{
							if (!strcmp(AuxTabla->tipotoken, "ID"))
							{
								strcpy(AuxTabla->derivacion, "ID");
								push("ID");
								// Localiza la declaracion de la variable e identifica su tipo
								// Si no la encuentra, se muestra un error sintactico
								TSimbolos *Aux2 = PTabla;
								while (Aux2 != AuxTabla && (strcmp(Aux2->tipotoken, "ID") || strcmp(Aux2->lexema, AuxTabla->lexema)))
									Aux2 = Aux2->liga;
								if (Aux2 == AuxTabla)
								{
									printf("Error Sintactico. La variable %s no esta declarada.\n", AuxTabla->lexema);
									return -1;
								}
								else
								{
									strcpy(AuxTabla->tipdat, Aux2->tipdat);
								}
							}
							else
							{
								strcpy(AuxTabla->derivacion, "NUM");
								push(AuxTabla->tipotoken);
							}
							AuxTabla = AuxTabla->liga;
							if (AuxTabla != NULL && !strcmp(AuxTabla->lexema, ";"))
							{
								strcpy(AuxTabla->derivacion, "SEP");
								AuxTabla = AuxTabla->liga;
								push(";");
								ImprimeDerivacion();
								if (AuxTabla != NULL && !strcmp(AuxTabla->lexema, "}"))
								{
									contadorllaves--;
									if (contadorllaves < 0)
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
							}
							else if (AuxTabla != NULL && !strcmp(AuxTabla->tipotoken, "OA"))
							{
								strcpy(AuxTabla->derivacion, "OA");
								push(" ");
								push(AuxTabla->lexema);
								AuxTabla = AuxTabla->liga;
								push(" ");
								push("S");
								ImprimeDerivacion();
							}
							else if (AuxTabla->lexema, ")")
							{
								push(")");
								contadorparentesis--;
								ImprimeDerivacion();
								AuxTabla = AuxTabla->liga;
								if (AuxTabla != NULL && (strcmp(AuxTabla->tipotoken, "OA") || strcmp(AuxTabla->lexema, ";")))
								{
									if (AuxTabla != NULL && !strcmp(AuxTabla->tipotoken, "OA"))
									{
										strcpy(AuxTabla->derivacion, "OA");
										push(" ");
										push(AuxTabla->lexema);
										AuxTabla = AuxTabla->liga;
										push(" ");
										push("S");
										ImprimeDerivacion();
									}
									else
									{
										strcpy(AuxTabla->derivacion, "SEP");
										AuxTabla = AuxTabla->liga;
										push(";");
										ImprimeDerivacion();
										if (AuxTabla != NULL && !strcmp(AuxTabla->lexema, "}"))
										{
											contadorllaves--;
											if (contadorllaves < 0)
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
									}
								}
							}
							else
							{
								printf("Error Sintactico: Se esperaba un Operador Aritmetico o un ;\n");
								return -1;
							}
						}
						else if (AuxTabla != NULL && !strcmp(AuxTabla->lexema, "("))
						{
							push("(");
							contadorparentesis++;
							ImprimeDerivacion();
							AuxTabla = AuxTabla->liga;
							push("S");
							ImprimeDerivacion();
						}
					}
				}
				else
				{
					printf("Error Sintactico: Se esperaba una asignacion.\n");
				}
			}
			else
			{
				printf("Error Sintactico: No se reconoce la sintaxis.\n");
				return -1;
			}
		}

		if (AuxTabla != NULL && !strcmp(AuxTabla->lexema, "}"))
		{
			push("}");
			ImprimeDerivacion();
			AuxTabla = AuxTabla->liga;
			return 0;
		}

		if (QPila != NULL && strcmp(QPila->derivacion, ";") && strcmp(QPila->derivacion, "}") && strcmp(QPila->derivacion, ")"))
		{ // Si el ultimo caracter no fue ; es error sintactico
			printf("Error Sintactico: Se esperaba un ; o una llave de cierre\n");
			return -1;
		}
		if (numcopia == 0)
		{
			VaciaPila();
			printf("\n");
		}
		if (AuxTabla != NULL)
			push("G");
	}
	if (numcopia == 0 && contadorllaves != 0)
	{
		printf("Error Sintactico: La cantidad de llaves de apertura no es la misma que las de cierre\n");
	}
	if (numcopia == 0 && contadorparentesis != 0)
	{
		printf("Error Sintactico: La cantidad de parentsis de apertura no es la misma que las de cierre\n");
		return -1;
	}
	return 0;
}
