int indID = 0, indNE = 201, indND = 301, indNX = 401;

void AgregaTablaSimbolos(const char *lex, int tiptok)
{
	NuevoTabla = (TSimbolos *)malloc(sizeof(TSimbolos));
	strcpy(NuevoTabla->lexema, lex);
	NuevoTabla->liga = NULL;
	switch (tiptok)
	{
	case 1:
		strcpy(NuevoTabla->tipotoken, "ID");
		if (QTabla != NULL)
			strcpy(NuevoTabla->tipdat, QTabla->lexema);
		strcpy(NuevoTabla->regla, "D(D|A)*");
		NuevoTabla->ind = indID;
		indID++;
		break;
	case 2:
		strcpy(NuevoTabla->tipotoken, "PR");
		if (!strcmp(lex, "int"))
		{
			NuevoTabla->ind = 517;
			strcpy(NuevoTabla->tipdat, "int");
			strcpy(NuevoTabla->regla, "int ID");
		}
		else if (!strcmp(lex, "float"))
		{
			NuevoTabla->ind = 518;
			strcpy(NuevoTabla->tipdat, "float");
			strcpy(NuevoTabla->regla, "float ID");
		}
		else if (!strcmp(lex, "while"))
		{
			NuevoTabla->ind = 519;
			strcpy(NuevoTabla->tipdat, "NULL");
			strcpy(NuevoTabla->regla, "while(C){S}");
		}
		break;
	case 3:
		strcpy(NuevoTabla->tipotoken, "OR");
		if (!strcmp(lex, ">"))
		{
			NuevoTabla->ind = 503;
		}
		else if (!strcmp(lex, "<"))
		{
			NuevoTabla->ind = 504;
		}
		else if (!strcmp(lex, ">="))
		{
			NuevoTabla->ind = 505;
		}
		else if (!strcmp(lex, "<="))
		{
			NuevoTabla->ind = 506;
		}
		else if (!strcmp(lex, "!="))
		{
			NuevoTabla->ind = 507;
		}
		else if (!strcmp(lex, "=="))
		{
			NuevoTabla->ind = 508;
		}

		break;
	case 4:
		strcpy(NuevoTabla->tipotoken, "AS");
		NuevoTabla->ind = 800; // Indice de la asignacion
		strcpy(NuevoTabla->tipdat, "NULL");
		strcpy(NuevoTabla->regla, "ID = ID|num");
		break;
	case 5:
		strcpy(NuevoTabla->tipotoken, "OL");
		if (!strcmp(lex, "&&"))
		{
			NuevoTabla->ind = 501;
		}
		else if (!strcmp(lex, "||"))
		{
			NuevoTabla->ind = 502;
		}
		break;
	case 6:
		strcpy(NuevoTabla->tipotoken, "OA");
		strcpy(NuevoTabla->regla, "Aritm.");
		switch (lex[0])
		{
		case '*':
			NuevoTabla->ind = 510;
			break;
		case '/':
			NuevoTabla->ind = 511;
			break;
		case '+':
			NuevoTabla->ind = 512;
			break;
		case '-':
			NuevoTabla->ind = 513;
			break;
		case '=':
			NuevoTabla->ind = 514;
			break;
		}
		break;
	case 7:
		strcpy(NuevoTabla->tipotoken, "NE");
		strcpy(NuevoTabla->tipdat, "int");
		strcpy(NuevoTabla->regla, "Aritm.");
		NuevoTabla->ind = indNE;
		indNE++;
		break;
	case 8:
		strcpy(NuevoTabla->tipotoken, "ND");
		strcpy(NuevoTabla->tipdat, "float");
		strcpy(NuevoTabla->regla, "Aritm.");
		NuevoTabla->ind = indND;
		indND++;
		break;
	case 9:
		strcpy(NuevoTabla->tipotoken, "NX");
		strcpy(NuevoTabla->regla, "Aritm.");
		NuevoTabla->ind = indNX;
		indNX++;
		break;

	case 10:
		strcpy(NuevoTabla->tipotoken, "SEP");
		switch (lex[0])
		{
		case ',':
			NuevoTabla->ind = 515;
			break;
		case ';':
			NuevoTabla->ind = 516;
			strcpy(NuevoTabla->tipdat, "NULL");
			strcpy(NuevoTabla->regla, "S;");
			break;
		}
		break;
	case 11:
		strcpy(NuevoTabla->tipotoken, "PAR");
		switch (lex[0])
		{
		case '(':
			NuevoTabla->ind = 701;
			strcpy(NuevoTabla->tipdat, "NULL");
			strcpy(NuevoTabla->regla, "(S)");
			break;
		case ')':
			NuevoTabla->ind = 702;
			strcpy(NuevoTabla->tipdat, "NULL");
			strcpy(NuevoTabla->regla, "(S)");
			break;
		}
		break;
	case 12:
		strcpy(NuevoTabla->tipotoken, "LLA");
		switch (lex[0])
		{
		case '{':
			NuevoTabla->ind = 801;
			strcpy(NuevoTabla->tipdat, "NULL");
			strcpy(NuevoTabla->regla, "{S}");
			break;
		case '}':
			NuevoTabla->ind = 802;
			strcpy(NuevoTabla->tipdat, "NULL");
			strcpy(NuevoTabla->regla, "{S}");
			break;
		}
		break;
	}

	if (PTabla == NULL)
	{
		PTabla = NuevoTabla;
		QTabla = PTabla;
	}
	else
	{
		QTabla->liga = NuevoTabla;
		QTabla = NuevoTabla;
	}
}