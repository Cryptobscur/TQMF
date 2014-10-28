#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "decalage.h"

int main(int argc, char* argv[])
{
	if(argc != 4) // nom du programme + le mode + la clé + la chaîne à chiffrer (ou déchiffrer)
	{
		fprintf(stderr, "\nErreur : vous devez renseigner un mode (chiffrement ou déchiffrement), une clé et une chaîne.\n\n");
		exit(1);
	}
	
	CERTIFICATION retour = -1;
	char* res_mode;
	
	int l_mode = strlen(argv[1]);
	
	if(l_mode == 2)
	{
		if(strcmp(argv[1], "-c") == 0)
		{
			retour = chiffrement_message(argv[3], atoi(argv[2])); // atoi ne prendra en compte que les caractères numériques de la clé
			res_mode = "chiffré";
		}
		else if(strcmp(argv[1], "-d") == 0)
		{
			retour = dechiffrement_message(argv[3], atoi(argv[2])); // atoi ne prendra en compte que les caractères numériques de la clé
			res_mode = "déchiffré";																	
		}
		else
		{
			fprintf(stderr, "\nErreur : mode inconnu.\n\n");
			exit(2);
		}
		
		switch(retour)
		{
			case 0: printf("\nVotre message à été %s avec succès !\nLe résultat est : %s.\n\n", res_mode, argv[3]); // la chaîne est modifée directement dans les fonctions
					break;
			
			case 1: fprintf(stderr, "\nErreur : votre clé doit spécifier une valeur positive.\n\n");
					break;
					
			case 2: fprintf(stderr, "\nErreur : le chiffrement a échoué car un ou plusieurs caractères de la chaîne sont invalides.\n\n");
					break;
			
			default: break;
		}
	}
	else if(l_mode == 3) // modes par fichier
	{
		if(strcmp(argv[1], "-cf") == 0)
		{
			retour = chiffrement_fichier(argv[3], atoi(argv[2])); // atoi ne prendra en compte que les caractères numériques de la clé
			res_mode = "chiffré";
		}
		else if(strcmp(argv[1], "-df") == 0)
		{
			retour = dechiffrement_fichier(argv[3], atoi(argv[2])); // atoi ne prendra en compte que les caractères numériques de la clé
			res_mode = "déchiffré";
		}
		else
		{
			fprintf(stderr, "\nErreur : mode inconnu.\n\n");
			exit(2);
		}
		
		switch(retour)
		{
			case 0: printf("\nVotre message à été %s avec succès !\nLe résultat est disponible dans le fichier associé.\n\n", res_mode); // la chaîne est modifée directement dans les fonctions
					break;
			
			case 1: fprintf(stderr, "\nErreur : votre clé doit spécifier une valeur positive.\n\n");
					break;
					
			case 2: fprintf(stderr, "\nErreur : le chiffrement a échoué car un ou plusieurs caractères de la chaîne sont invalides.\n\n");
					break;
			
			case 3: fprintf(stderr, "\nErreur : impossible d'ouvrir le fichier %s.\n\n", argv[3]);
					break;
			
			case 4: fprintf(stderr, "\nErreur : impossible de créer le fichier du résultat.\n\n");
					break;
					
			default: break;
		}
	}
	else 
	{
		fprintf(stderr, "\nErreur : mode inconnu.\n\n");
		exit(2);
	}
	
	exit(0);
}
