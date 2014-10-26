#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "decalage.h"

int main(int argc, char* argv[])
{
	if(argc != 4) // nom du programme + le mode + la clé + la chaîne à chiffrer (ou déchiffrer)
	{
		fprintf(stderr, "\nErreur : vous devez renseigner un mode (chiffrement ou déchiffrement), une clé et une chaîne\n");
		exit(1);
	}
	
	CERTIFICATION retour;
	
	if(strcmp(argv[1], "-c") == 0)
	{
		retour = chiffrement_message(argv[3], atoi(argv[2])); // atoi ne prendra en compte que les caractères numériques de la clé
	}
	else if(strcmp(argv[1], "-d") == 0)
	{
		retour = dechiffrement_message(argv[3], atoi(argv[2])); // atoi ne prendra en compte que les caractères numériques de la clé																	
	}
	else
	{
		fprintf(stderr, "\nErreur : mode inconnu\n");
		exit(2);
	}
	
	switch(retour)
	{
		case 0: printf("\nVotre message à été chiffré avec succès !\n");
				break;
		
		case 1: fprintf(stderr, "\nErreur : votre clé doit spécifier une valeur positive\n");
				break;
				
		case 2: fprintf(stderr, "\nErreur : le chiffrement a échoué car un ou plusieurs caractères de la chaîne sont invalides.\n");
				break;
	}
	
	exit(0);
}
