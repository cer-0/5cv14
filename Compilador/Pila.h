TSimbolos *PPila, *QPila, *AuxPila, *NuevoPila;
TSimbolos *PPosfija, *QPosfija, *AuxPosfija, *NuevoPosfija;
TSimbolos *POperadores, *QOperadores, *AuxOperadores, *AuxOperadores2, *NuevoOperadores;


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


void push_pos(const char *a)
{
	NuevoPosfija = (TSimbolos*)(malloc(sizeof(TSimbolos)));
	NuevoPosfija->liga = NULL;
	strcpy(NuevoPosfija->lexema, a);
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

void push_op(const char *a)
{
	NuevoOperadores = (TSimbolos*)(malloc(sizeof(TSimbolos)));
	NuevoOperadores->liga = NULL;
	strcpy(NuevoOperadores->lexema, a);
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

