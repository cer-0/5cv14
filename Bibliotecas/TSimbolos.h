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
} *PTabla, *QTabla, *AuxTabla, *NuevoTabla;