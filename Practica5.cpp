/*Programadores:
	David
	Enrique
	Moises
	Labardini
  Compiladores 5CV14
  Profesor: Jesus Mora Jain
  Programa: Compilador
*/
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include "Compilador.h"

int main(void)	
{

	const char* archivo = "archivofuente.txt";
	const char* programa = "programafinal.txt";

	//En lugar de ingresar una cadena, se leen del archivo
	printf("\n\t\t**INICIANDO ANALISIS LEXICO**\n");
	if( !LeeArchivo(archivo) )
	{
		printf("\n\n\t\t**LEXICAMENTE CORRECTO**\n\n");
		printf("\n\t\t**INICIANDO ANALISIS SINTACTICO**\n");
		if( !AnalizadorSintactico(PTabla, 0) )
		{
			//ImprimeTabla();
			printf("\n\n\t\t**SINTACTICAMENTE CORRECTO**\n\n");
			printf("\n\t\t**INICIANDO ANALISIS SEMANTICO**\n");
			if( !AnalizadorSemantico(PTabla, 0) )
			{
				printf("\n\n\t\t**SEMANTICAMENTE CORRECTO**\n\n");
				printf("\n\t\t**CONVIRTIENDO A NOTACION RPN Y ESCRIBIENDO CODIGO OBJETO**\n");
				EscribeEnsamblador(programa);
			}
		}
	}
	printf("\nPresione una tecla para continuar...");
	getchar();
	return 0;
}
