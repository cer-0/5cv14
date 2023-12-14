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
								if(AuxTabla != NULL && !strcmp(AuxTabla->lexema, "("))
								{
									push("(");
									AuxTabla = AuxTabla->liga;
									ImprimeDerivacion();
									push("S");
									ImprimeDerivacion();
								}else{
								printf("Error Semantico. Solo pueden asignarse valores enteros a una variable entera\n");
								return -1;
								}
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
								}else if(AuxTabla != NULL && !strcmp(AuxTabla->lexema, ")"))
								{
									push(")");
									ImprimeDerivacion();
									AuxTabla = AuxTabla->liga;
									if(AuxTabla != NULL && !(strcmp(AuxTabla->tipotoken, "OA")|| !strcmp(AuxTabla->lexema, ";")))
									{
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
								}
							}
						} else if( !strcmp(tipo, "float") ) //Sino, si el tipo de la variable que guarda la asignacion es float
							{
							if(!strcmp(AuxTabla->tipdat, "float") && !strcmp(AuxTabla->tipdat, "int"))
							{
								printf("Error Semantico. No se reconoce el tipo\n");
								return -1;
							}else
							{
								if(AuxTabla != NULL && !strcmp(AuxTabla->lexema, "("))
								{
								push("(");
								AuxTabla = AuxTabla->liga;
								ImprimeDerivacion();
								push("S");
								ImprimeDerivacion();
								pop();
								}
							
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
							}else if(AuxTabla != NULL && !strcmp(AuxTabla->lexema, ")"))
								{
									push(")");
									ImprimeDerivacion();
									AuxTabla = AuxTabla->liga;	
									if(AuxTabla != NULL && !(strcmp(AuxTabla->tipotoken, "OA")|| !strcmp(AuxTabla->lexema, ";")))
									{
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
								}
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
