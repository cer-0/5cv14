#include <stdio.h>
#include <ctype.h>
#include <string.h>

struct TS{
		char lexema[30];
		char TipTok[3];
		char TipDat[5];
		char reglas[10];
		int ind;
		struct TS *sig;
	};


int main(){
	int ID=0,NE=201,ND=301,NX=401;
	struct TS *ini = NULL, *fin = NULL, *aux = NULL;
	
	
	
	void almacenarTS(char Lex[30], char Tok[3], char Dat[5], char regla[10], int num){
		if(ini == NULL){
			ini = malloc(sizeof(struct TS));
			strcpy(ini->lexema,Lex);
			strcpy(ini->TipTok,Tok);
			strcpy(ini->TipDat,Dat);
			strcpy(ini->reglas,regla);
			ini->ind = num;
			ini->sig = NULL;
			fin = ini;
		}else{
			aux = malloc(sizeof(struct TS));
			strcpy(ini->lexema,Lex);
			strcpy(ini->TipTok,Tok);
			strcpy(ini->TipDat,Dat);
			strcpy(ini->reglas,regla);
			ini->ind = num;
			fin->sig = aux;
			fin = aux;
		}
	}
	
	void imprimirLista(){
		if (ini!= NULL){
			aux = ini;
			while(aux!= NULL){
				printf("\n");
				printf("\nLex: %s",aux->lexema);
				printf("\nToken: %s",aux->TipTok);
				printf("\nDato: %s",aux->TipDat);
				printf("\nReglas: %s",aux->reglas);
				printf("\nInd: %d",aux->ind);
				aux = aux->sig;
			}
		}
	}
	
	
	
	int caso = 0, indice = 0;
	char letra;
	char Palabra[20];
	int pos = 0;
	char PalRes[2][10] = {"float","int"};
	
	FILE *f;
	f = fopen("palabras.txt","r");
	
	
	while(fscanf(f, "%s", &Palabra) == 1){
		
		letra = Palabra[0];
		caso = 0;
		pos = 0;
		int longitudCad = strlen(Palabra);
		
		while (pos < longitudCad){
			switch(caso){
				
				case 0:
					if (isalpha(letra)){
						caso = 1;	
					}	
					
					if (isdigit(letra)){
						caso = 2;
					}
					
					if(letra == '+' || letra == '-'){
						caso = 8;
					}
					
					
					if (letra == '='){
						caso = 9;
					}
					
					
					if (letra == '>' || letra == '<'){
						caso = 11;
					}
					
					if (letra == '|'){
						caso = 12;
					}
					
					if (letra == '&'){
						caso = 14;
					}
					
					if (letra == '!'){
						caso = 16;
					}
					
					if (letra == '*' || letra == '/' || letra == '%' || letra == '^'){
						caso = 18;
					}
					
					break;
				
				case 1:
					if (isalpha(letra) || isdigit(letra)){
						caso = 1;
					}else{
						caso = 500;
					}
					break;
					
				case 2:
					if (isdigit(letra)){
						
						caso = 2;
					}else if(letra == '.'){
						caso = 3;
					}else{
						caso = 500;
					}
					
					
					break;
					
				case 3:
					if (isdigit(letra)){
						caso = 4;
					}else{
						caso = 500;
					}
					
					break;
					
				case 4:
					if (isdigit(letra)){
						caso = 4;
					}else if (letra == 'E' || letra == 'e'){
						caso = 5;
					}else{
						caso = 500;
					}
					
					break;
					
				case 5:
					if(letra == '+' || letra == '-'){
						caso = 6;
					}
					break;
					
				case 6:
					if (isdigit(letra)){
						caso = 7;
					}else{
						caso = 500;
					}
					break;
					
				case 7:
					if (isdigit(letra)){
						caso = 7;
					}else{
						caso = 500;
					}
					break;
					
				case 8:
					if (isdigit(letra)){
						caso = 2;
					}else{
						caso = 500;
					}
					break;
					
				case 9:
					if (letra == '='){
						caso = 10;
					}else{
						caso = 500;
					}
					break;
				
					
				case 11:
					if (letra == '='){
						caso = 10;
					}else{
						caso = 500;
					}
					break;
					
				case 12:
					if (letra == '|'){
						caso = 13;
					}else{
						caso = 500;
					}
					break;		
				
					
				case 14:
					if (letra == '&'){
						caso = 13;
					}else{
						caso = 500;
					}
					break;
				
				case 16:
					if (letra == '='){
						caso = 17;
					}else{
						caso = 500;
					}
					
				default:{
					caso = 500;	
					break;}
					
			}
				
		pos++;
		letra = Palabra[pos];
		}
		if (pos == longitudCad){
			
			int i = 0;
			while(i<2){
			
				if(strcmp(Palabra,PalRes[i])== 0){
					caso = 19;
				}
				i++;
			}
			
			
			switch (caso){
				case 1:
					almacenarTS(Palabra,"ID"," ","aritm.",ID);
					ID++;
					indice++;
					break;
				
				case 2:
					almacenarTS(Palabra,"NE"," ","aritm.",NE);
					NE++;
					indice++;
					break;
					
				case 4:
					almacenarTS(Palabra,"ND","","aritm.",ND);
					ND++;
					indice++;
					break;
					
				case 7:
					almacenarTS(Palabra,"NX","","aritm.",NX);
					NX++;
					indice++;
					break;
					
					
				case 8:
				case 9:
				case 18:
					
					switch (Palabra[0]){
						
						case '*':
							almacenarTS(Palabra,"OA","","aritm.",510);
							break;
						case '/':
							almacenarTS(Palabra,"OA","","aritm.",511);
							break;
						case '+':
							almacenarTS(Palabra,"OA","","aritm.",512);
							break;
						case '-':
							almacenarTS(Palabra,"OA","","aritm.",513);
							break;
						case '=':
							almacenarTS(Palabra,"Asig","","mov.",514);
							break;
					}
					
					indice++;
					break;

				case 19:
						almacenarTS("","",Palabra,"",0);
						break;
					break;
					
				default:
					printf("\nCadena invalida.");
			}
		
		}
		
	}
	
	fclose(f);
	
	imprimirLista();
	
}
