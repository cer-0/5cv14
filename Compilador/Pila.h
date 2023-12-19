TSimbolos *PPila, *QPila, *AuxPila, *NuevoPila;
TSimbolos *PPosfija, *QPosfija, *AuxPosfija, *NuevoPosfija;
TSimbolos *POperadores, *QOperadores, *AuxOperadores, *AuxOperadores2, *NuevoOperadores;
TSimbolos *PPilaJerarquia, *QPilaJerarquia, *AuxPilaJerarquia, *NuevoPilaJerarquia;


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


void push_pos(TSimbolos *a)
{
	NuevoPosfija = (TSimbolos*)(malloc(sizeof(TSimbolos)));
	NuevoPosfija->liga = NULL;
	strcpy(NuevoPosfija->lexema, a->lexema);
	strcpy(NuevoPosfija->tipotoken, a->tipotoken);
	if(PPosfija == NULL)
	{
		PPosfija = NuevoPosfija;
		QPosfija = PPosfija;
	}
	else
	{
		QPosfija->liga = NuevoPosfija;
		QPosfija = QPosfija->liga;	
	}
}

void push_op(TSimbolos *a)
{
	NuevoOperadores = (TSimbolos*)(malloc(sizeof(TSimbolos)));
	NuevoOperadores->liga = NULL;
	strcpy(NuevoOperadores->lexema, a->lexema);
	strcpy(NuevoOperadores->tipotoken, a->tipotoken);
	if(POperadores == NULL)
	{
		POperadores = NuevoOperadores;
		QOperadores = POperadores;
	}
	else
	{
		QOperadores->liga = NuevoOperadores;
		QOperadores = QOperadores->liga;	
	}
}

void pop_op()
{
	AuxOperadores = POperadores;
	if(AuxOperadores != NULL)
	{
		if( POperadores->liga != NULL )
		{	
			while(AuxOperadores->liga->liga != NULL)
				AuxOperadores = AuxOperadores->liga;
			QOperadores = AuxOperadores;
			AuxOperadores = QOperadores->liga;
			QOperadores->liga = NULL;
			free(AuxOperadores);
		}
		else
		{
			free(AuxOperadores);
			POperadores = NULL;
			QOperadores = NULL;
			
		}	
	}
}

void push_jerarquia(const char *a)
{
	NuevoPilaJerarquia = (TSimbolos*)(malloc(sizeof(TSimbolos)));
	NuevoPilaJerarquia->liga = NULL;
	strcpy(NuevoPilaJerarquia->lexema, a);
	if(PPilaJerarquia == NULL)
	{
		PPilaJerarquia = NuevoPilaJerarquia;
		QPilaJerarquia = PPilaJerarquia;
	}
	else
	{
		QPilaJerarquia->liga = NuevoPilaJerarquia;
		QPilaJerarquia = QPilaJerarquia->liga;	
	}
}

void pop_jerarquia()
{
	AuxPilaJerarquia = PPilaJerarquia;
	if(AuxPilaJerarquia != NULL)
	{
		if( PPilaJerarquia->liga != NULL )
		{	
			while(AuxPilaJerarquia->liga->liga != NULL)
				AuxPilaJerarquia = AuxPilaJerarquia->liga;
			QPilaJerarquia = AuxPilaJerarquia;
			AuxPilaJerarquia = QPilaJerarquia->liga;
			QPilaJerarquia->liga = NULL;
			free(AuxPilaJerarquia);
		}
		else
		{
			free(AuxPilaJerarquia);
			PPilaJerarquia = NULL;
			QPilaJerarquia = NULL;
		}	
	}
}

void VaciaPosfija(void)
{
	AuxPosfija = PPosfija;
	while( AuxPosfija != NULL )
	{
		PPosfija = PPosfija->liga;
		free(AuxPosfija);
		AuxPosfija = PPosfija;
	}
	QPosfija = NULL;
}

void VaciaOperadores(void)
{
	AuxOperadores = POperadores;
	while( AuxOperadores != NULL )
	{
		POperadores = POperadores->liga;
		free(AuxOperadores);
		AuxOperadores = POperadores;
	}
	QOperadores = NULL;
}