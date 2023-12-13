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