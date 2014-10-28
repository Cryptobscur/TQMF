#include "decalage.h"
#include <string.h>

/* Définitions de constantes afin de changer
 * très facilement d'alphabet de traitement */
 
#define BORNE_INF 'a'
#define BORNE_SUP 'z'
#define TAILLE_ALPHABET 26

typedef int boolean; // Ce type ne doit prendre que les valeurs 0 ou 1

/* Détermine si la clé entrée est valable */
boolean cleValable(CLE k)
{
	if(k < 0)
		return 0;
	else return 1;
}

/* Détermine si le caractère à chiffrer ou déchiffrer 
 * se trouve dans l'alphabet de traitement */
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
		if(!estDansAlphabet(caractere_lu)) // les caracteres supportés actuellement sont ceux de 'a' à 'z'
			return 2;
		
		int intermediaire = caractere_lu + (k % TAILLE_ALPHABET);
		
		(intermediaire <= BORNE_SUP) ? ((*message) = intermediaire) : ((*message) = (intermediaire - TAILLE_ALPHABET));
		
		message++;
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
		if(!estDansAlphabet(caractere_lu)) // les caracteres supportés actuellement sont ceux de 'a' à 'z'
			return 2;
		
		int intermediaire = caractere_lu - (k % TAILLE_ALPHABET);
		
		(intermediaire >= BORNE_INF) ? ((*message) = intermediaire) : ((*message) = (intermediaire + TAILLE_ALPHABET));
		
		message++;
	}
	
	return 0;
}

/* Crée le nom du fichier clair ou chiffré. la chaîne 'ajout' est ajoutée à la fin
 * et la chaîne 'suppression' est supprimée si elle existe dans 'nom_fichier' */
char* creerNom(char* nom_fichier, char* ajout, char* suppression)
{	
	/* Initialisation de quelques variables pour le traitement */
	
	int l_ajout = strlen(ajout);
	int l_suppression = strlen(suppression);
	int l_nom_fichier = strlen(nom_fichier);
	
	char* pos_nom = nom_fichier;	
	pos_nom += l_nom_fichier;
	
	int intermediaire = l_nom_fichier;
	
	/* On supprime l'expression '(clair)' ou '(chiffre)', selon le mode, si elle est présente dans le nom du fichier */
	
	char* pos_etat = strstr(nom_fichier, suppression);
	
	if(pos_etat != NULL)
	{
		strcpy(pos_etat, (pos_etat + l_suppression));
		l_nom_fichier -= l_suppression;
		pos_nom -= l_suppression;
		intermediaire = l_nom_fichier;
	}
	
	/* On ajoute l'expression '(chiffre)' ou '(clair)', selon le mode, au nom du fichier */
	
	char* nom_chiffre = malloc(sizeof(char) * (l_ajout + l_nom_fichier + 1));
	
	while((*pos_nom) != '.')
	{
		pos_nom--;
		intermediaire--;
		
		if(intermediaire <= 0) // si le fichier ne possède pas d'extension dans son nom
		{
			strcpy(nom_chiffre, nom_fichier);
			strcat(nom_chiffre, ajout);
			return nom_chiffre;
		}
	}
	
	strncpy(nom_chiffre, nom_fichier, intermediaire); // pas de caractère nul à la fin mais il est ajouté juste après
	strcat(nom_chiffre, ajout);
	strcat(nom_chiffre, pos_nom);
	
	return nom_chiffre;
}

/* Crée le nom du fichier clair ou chiffré selon le mode spécifié*/
char* creerNomFichier(char* nom_fichier, char mode)
{
	char* ajout;	
	char* suppression;
	
	if(mode == 'c') // chiffrement
	{
		ajout = "(chiffre)";		
		suppression = "(clair)";
	}
	else if(mode == 'd') // déchiffrement
	{
		ajout = "(clair)";	
		suppression = "(chiffre)";
	}
	else return NULL; // n'a pas lieu d'arriver normalement...
	
	return creerNom(nom_fichier, ajout, suppression);
}

CERTIFICATION chiffrement_fichier(char* nom_fichier, CLE k)
{
	FILE* f_message = fopen(nom_fichier, "r");
	if(f_message == NULL)
		return 3;
	
	char* nom_fichier_chiffre = creerNomFichier(nom_fichier, 'c');
	
	FILE* f_chiffre = fopen(nom_fichier_chiffre, "w");
	if(f_chiffre == NULL)
	{
		fclose(f_message);
		return 4;
	}
	
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
	FILE* f_chiffre = fopen(nom_fichier, "r");
	if(f_chiffre == NULL)
		return 3;
	
	char* nom_fichier_clair = creerNomFichier(nom_fichier, 'd');
	
	FILE* f_message = fopen(nom_fichier_clair, "w");
	if(f_message == NULL)
	{
		fclose(f_chiffre);
		return 4;
	}
	
	char tampon[1024];
	int nb_carac_lus;
	
	CERTIFICATION retour;
	
	while((nb_carac_lus = fread(tampon, sizeof(char), 1024, f_chiffre)) > 0)
	{
		tampon[nb_carac_lus] = '\0'; // détermine la fin de la chaîne à chiffrer (caractères indéterminés par la suite)
		
		retour = dechiffrement_message(tampon, k);
		
		switch(retour)
		{
			case 0: fwrite(tampon, sizeof(char), nb_carac_lus, f_message);
					break;
			
			default: fclose(f_chiffre);
					 remove(nom_fichier_clair);
					 return retour; // si on écrit pas de caractère, c'est qu'il y a eu un problème lors du chiffrement, et on renvoie donc l'erreur
		}
	}
	
	fclose(f_message);
	fclose(f_chiffre);
	return 0;
}
