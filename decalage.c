#include "decalage.h"
#include <string.h>

typedef int boolean; // Ce type ne doit prendre que les valeurs 0 ou 1

boolean cleValable(CLE k)
{
	if(k < 0)
		return 0;
	else return 1;
}

boolean estDansAlphabet(char caractere)
{
	if(caractere >= 'a' && caractere <= 'z')
		return 1;
	else return 0;
}

CERTIFICATION chiffrement_message(char* message, CLE k)
{
	if(!cleValable(k))
		return 1;
		
	char caractere_lu;
	char* debut_msg = message;
	
	while((caractere_lu = (*message)) != '\0')
	{
		if(!estDansAlphabet(caractere_lu)) // les caracteres supportés pour l'instant sont ceux de 'a' à 'z' (extension envisagée)
			return 2;
		
		int intermediaire = caractere_lu + (k % 26);
		
		(intermediaire <= 'z') ? ((*message) = intermediaire) : ((*message) = ((intermediaire % ('z' + 1)) + 'a'));
		
		message++; // la chaîne provient du terminal et ne subit aucun traitement avant l'appel de cette fonction,
				   // on est donc restreint par le caractère nul de fin de chaîne, et ainsi l'incrémention n'est pas censée donner d'overflow
	}
	
	printf("\nLe chiffré est : %s\n", debut_msg);
	return 0;
}

CERTIFICATION chiffrement_fichier(FILE* message, CLE k)
{
	return 0;
}

CERTIFICATION dechiffrement_message(char* message, CLE k)
{
	return 0;
}

CERTIFICATION dechiffrement_fichier(FILE* message, CLE k)
{
	return 0;
}
