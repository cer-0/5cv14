void ImprimeTabla()
{
	AuxTabla = PTabla;
	printf("\t\t\t\tTABLA DE SIMBOLOS\n");
	printf("-----------------------------------------------------------------------------------------\n");
	printf("LEXEMA\t\tTIPO TOKEN\tTIPO DE DATO\tREGLA\t\tINDICE\n");
	printf("-----------------------------------------------------------------------------------------\n");
	while(AuxTabla != NULL)
	{
		printf( "%s\t\t%s\t\t%s\t\t%s\t\t%d\n", AuxTabla->lexema, AuxTabla->tipotoken, AuxTabla->tipdat, AuxTabla->regla, AuxTabla->ind );
		AuxTabla = AuxTabla->liga;
	}
	printf("-----------------------------------------------------------------------------------------\n");
}