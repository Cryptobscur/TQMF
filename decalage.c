#include "decalage.h"
#include <string.h>

#define BORNE_INF 'a'
#define BORNE_SUP 'z'
#define TAILLE_ALPHABET 26

typedef int boolean; // Ce type ne doit prendre que les valeurs 0 ou 1

boolean cleValable(CLE k)
{
	if(k < 0)
		return 0;
	else return 1;
}

boolean estDansAlphabet(char caractere)
{
	if(caractere >= BORNE_INF && caractere <= BORNE_SUP)
		return 1;
	else return 0;
}

CERTIFICATION chiffrement_message(char* message, CLE k)
{
	if(!cleValable(k))
		return 1;
		
	char caractere_lu;
	
	while((caractere_lu = (*message)) != '\0')
	{
		if(!estDansAlphabet(caractere_lu)) // les caracteres supportés pour l'instant sont ceux de 'a' à 'z' (extension envisagée)
			return 2;
		
		int intermediaire = caractere_lu + (k % TAILLE_ALPHABET);
		
		(intermediaire <= BORNE_SUP) ? ((*message) = intermediaire) : ((*message) = (intermediaire - TAILLE_ALPHABET));
		
		message++; // la chaîne provient du terminal et ne subit aucun traitement avant l'appel de cette fonction,
				   // on est donc restreint par le caractère nul de fin de chaîne, et ainsi l'incrémention n'est pas censée donner d'overflow
	}
	
	return 0;
}

CERTIFICATION dechiffrement_message(char* message, CLE k)
{
	if(!cleValable(k))
		return 1;
		
	char caractere_lu;
	
	while((caractere_lu = (*message)) != '\0')
	{
		if(!estDansAlphabet(caractere_lu)) // les caracteres supportés pour l'instant sont ceux de 'a' à 'z' (extension envisagée)
			return 2;
		
		int intermediaire = caractere_lu - (k % TAILLE_ALPHABET);
		
		(intermediaire >= BORNE_INF) ? ((*message) = intermediaire) : ((*message) = (intermediaire + TAILLE_ALPHABET));
		
		message++; // la chaîne provient du terminal et ne subit aucun traitement avant l'appel de cette fonction,
				   // on est donc restreint par le caractère nul de fin de chaîne, et ainsi l'incrémention n'est pas censée donner d'overflow
	}
	
	return 0;
}

char* creerNomFichierChiffre(char* nom_fichier)
{
	int intermediaire = 0;
	
	char* pos_nom = nom_fichier;
	while((*pos_nom) != '\0')
	{
		pos_nom++;
		intermediaire++;
	}
	
	int l_nom_fichier = intermediaire; // '\0' n'est pas compté
	
	char* nom_chiffre = malloc(sizeof(char) * (8 + l_nom_fichier + 1)); // les 8 caractères correspondent à '_chiffre'
	
	while((*pos_nom) != '.')
	{
		pos_nom--;
		intermediaire--;
		
		if(intermediaire <= 0) // si le fichier ne possède pas d'extension dans son nom
		{
			strcpy(nom_chiffre, nom_fichier);
			strcat(nom_chiffre, "_chiffre");
			return nom_chiffre;
		}
	}
	
	strncpy(nom_chiffre, nom_fichier, intermediaire); // pas de caractère nul à la fin mais il est ajouté juste après
	strcat(nom_chiffre, "_chiffre");
	strcat(nom_chiffre, pos_nom);
	
	return nom_chiffre;
}

CERTIFICATION chiffrement_fichier(char* nom_fichier, CLE k)
{
	FILE* f_message = fopen(nom_fichier, "r");
	if(f_message == NULL)
		return 3;
	
	char* nom_fichier_chiffre = creerNomFichierChiffre(nom_fichier);
	
	FILE* f_chiffre = fopen(nom_fichier_chiffre, "w");
	if(f_chiffre == NULL)
		return 4;
	
	char tampon[1024];
	int nb_carac_lus;
	
	CERTIFICATION retour;
	
	while((nb_carac_lus = fread(tampon, sizeof(char), 1024, f_message)) > 0)
	{
		tampon[nb_carac_lus] = '\0'; // détermine la fin de la chaîne à chiffrer (caractères indéterminés par la suite)
		
		retour = chiffrement_message(tampon, k);
		
		switch(retour)
		{
			case 0: fwrite(tampon, sizeof(char), nb_carac_lus, f_chiffre);
					break;
			
			default: fclose(f_message);
					 remove(nom_fichier_chiffre);
					 return retour; // si on écrit pas de caractère, c'est qu'il y a eu un problème lors du chiffrement, et on renvoie donc l'erreur
		}
	}
	
	fclose(f_message);
	fclose(f_chiffre);
	return 0;
}

CERTIFICATION dechiffrement_fichier(char* nom_fichier, CLE k)
{
	return 0;
}
