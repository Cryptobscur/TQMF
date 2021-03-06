#include <stdio.h>
#include <stdlib.h>

/** Header annoncant les fonctions liées au chiffrement
 *  et déchiffrement par décalage 
 */
 
/* On intègre une définition CERTIFICATION à forme numérique visant 
 * à confirmer le bon déroulement ou non des processus invoqués */
typedef int CERTIFICATION;

/* On intègre une définition CLE à forme numérique visant 
 * à rendre la compréhension du code plus simple */
typedef int CLE;


/* Fonction de chiffrement par décalage, avec une CLE k,
 * pour un message donné (sous forme de chaîne de caractères) */
CERTIFICATION chiffrement_message(char* message, CLE k);

/* Fonction de chiffrement par décalage, avec une CLE k,
 * pour un message donné (contenu dans un fichier) */
CERTIFICATION chiffrement_fichier(char* nom_fichier, CLE k);

/* Fonction de déchiffrement par décalage, avec une CLE k,
 * pour un message donné (sous forme de chaîne de caractères) */
CERTIFICATION dechiffrement_message(char* message, CLE k);

/* Fonction de déchiffrement par décalage, avec une CLE k,
 * pour un message donné (contenu dans un fichier) */
CERTIFICATION dechiffrement_fichier(char* nom_fichier, CLE k);
