/* Contiene AnalizadorLexico() e IdentificaTipo() */
char *palres[] = {"int", "float", "while"};
int i;

//1: ID
//3: Operador Relacional
//4: Asignacion
//5: Operador Logico
//6: Operador Aritmetico
//7: Entero
//8: Decimal
//9: Exponencial
int AnalizadorLexico( const char* lex )
{
	int t = strlen(lex), tiptok = 0;
	i = 0;

	switch( lex[i] )
	{
		case 'a'...'z':
		case 'A'...'Z':
			tiptok = 1;
			i++;
			while( i < t && isalnum( lex[i] ) )
				i++;
			break;
		case '<':
		case '>':
		case '=':
		case '!':
			if( lex[i] == '!' )
				tiptok = 5;
			else if( lex[i] == '=' )
				tiptok = 4;
			else
				tiptok = 3;
			i++;
			if( i+1 == t )
			{
				if( lex[i] == '=' )
					tiptok = 3;
				else
					i--;
				i++;
			}
			else if( i == t )
				break;
			else if( i < t)
				i--;
			break;
		case '*':
		case '/':
			i++;
			if(i == t )
				tiptok = 6;
			else
				i--;
			break;
		case '&':
			i++;
			if( i+1 == t && lex[i] == '&' )
			{
				tiptok = 5;
				i++;
			}
			else
				i--;
			break;
		case '|':
			i++;
			if( i < t && lex[i] == '|' )
			{
				tiptok = 5;
				i++;
			}
			else
				i--;
			break;
		case '0'...'9':
		case '+':
		case '-':
			if( lex[i] == '+' || lex[i] == '-' )
			{
				tiptok = 6;
				i++;
			}
			if( i < t && lex[i] >= '0' && lex[i] <= '9' )
			{
				i++;
				tiptok = 7;
				while( i < t && lex[i] >= '0' && lex[i] <= '9' )
					i++;
				if( i < t && lex[i] == '.' )
				{
					i++;
					if( i < t && lex[i] >= '0' && lex[i] <= '9' )
					{
						tiptok = 8;
						while( i < t && lex[i]>='0' && lex[i] <= '9' )
							i++;
					}
					else
						i--;
				}
				if( i < t && lex[i] == 'e' || lex[i] == 'E' )
				{
					i++;
					if( i < t && lex[i] == '+' || lex[i] == '-' )
						i++;
					if( i < t && lex[i] >= '0' && lex[i] <= '9' )
					{
						tiptok = 9;
						while( i < t && lex[i] >= '0' && lex[i] <= '9' )
							i++;
					}
					else
						i--;
				}
			}
			break;	
		case ';':
		case ',':
			tiptok = 10;
			i++;
			break;
		case '(':
		case ')':
			tiptok = 11;
			i++;
			break;
		case '{':
		case '}':
			tiptok = 12;
			i++;
			break;
		default:
			break;
	}
	return tiptok;
}

int IdentificaTipo( const char* cad )
{
	int tipo = AnalizadorLexico( cad );
	
	//2: Palabra Reservada
	int c = 0;
	if( tipo == 1 )
		while( c < 3 )
		{
			if( !(strcmp( cad, palres[c] ) ) )
			{
				tipo = 2;
				break;
			}
			c++;
		}
	
	switch( tipo )
	{
		case 1:
			return 1;
		case 2:
			return 2;
		case 3:
			return 3;
		case 4:
			return 4;
		case 5:
			return 5;
		case 6:
			return 6;
		case 7:
			return 7;
		case 8:
			return 8;
		case 9:
			return 9;
		case 10:
			return 10;
		case 11:
			return 11;
		case 12:
			return 12;
		default:
			return -1;
	}
}