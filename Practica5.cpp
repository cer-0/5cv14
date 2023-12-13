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
#include "Bibliotecas/TSimbolos.h"
#include "Bibliotecas/Pila.h"
#include "Bibliotecas/AnalizadorLexico.h"
#include "Bibliotecas/AnalizadorSintactico.h"
#include "Bibliotecas/AnalizadorSemantico.h"
#include "Bibliotecas/EscribeEnsamblador.h"
#include "Bibliotecas/AgregaTablaSimbolos.h"
#include "Bibliotecas/ImprimeTabla.h"
#include "Bibliotecas/LeeArchivo.h"


int main(void)	
{
	
	const char* archivo = "archivofuente.txt";
	const char* programa = "programafinal.txt";

	//En lugar de ingresar una cadena, se leen del archivo
	if( !LeeArchivo(archivo) )
	{
		ImprimeTabla();
		/*
		ImprimeTabla();
		if( !AnalizadorSintactico(PTabla, 0) )
			if( !AnalizadorSintactico(PTabla, 0) )
				EscribeEnsamblador(programa);
		*/
	}
	printf("Presione una tecla para continuar...");
	getchar();
	return 0;
}