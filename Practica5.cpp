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
#include "Compilador.h"

int main(void)	
{
	
	const char* archivo = "archivofuente.txt";
	const char* programa = "programafinal.txt";

	//En lugar de ingresar una cadena, se leen del archivo
	if( !LeeArchivo(archivo) )
	{
		if( !AnalizadorSintactico(PTabla, 0) )
		{
			ImprimeTabla();
			AnalizadorSemantico(PTabla, 0);
			/*
			if( !AnalizadorSemantico(PTabla, 0) )
				EscribeEnsamblador(programa);
			*/
		}
	}
	printf("Presione una tecla para continuar...");
	getchar();
	return 0;
}