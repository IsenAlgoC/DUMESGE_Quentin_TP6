#include <stdio.h>   /* pour les entrées-sorties */
#include <string.h>  /* pour les manipulations de chaînes de caractères */
#include <conio.h>
#include <ctype.h>
#include <stdlib.h>
#include <locale.h>
#include "rep.h"

#define VERSION 3.0
#define SQUELET
/**************************************************************************/
/* Compléter votre nom ici                                                */
/*   Nom : DUMESGE                 Prénom : Quentin                              */
/**************************************************************************/

extern bool modif;


/**********************************************************************/
/*  Ajout d'un contact dans le répertoire stocké en mémoire           */
/**********************************************************************/

int ajouter_un_contact_dans_rep(Repertoire* rep, Enregistrement enr)
{
#ifdef IMPL_TAB

	if (rep->nb_elts < MAX_ENREG) {
		Enregistrement* save = rep->tab;		//Sauvegarde au cas où il n'y a pas assez de mémoire
		rep->tab = (Enregistrement*)realloc(rep->tab, (rep->nb_elts + 1) * sizeof(Enregistrement));
		if (rep->tab == NULL) { /*Pas assez de mémoire*/
			rep->tab = save;
			return(ERROR);
		}
		else {		/*On écrit le contact en dernière position dans le répertoire agrandi*/
			for (int i = 0; i < MAX_NOM; i++) {
				(rep->tab + rep->nb_elts)->nom[i] = enr.nom[i];
				(rep->tab + rep->nb_elts)->prenom[i] = enr.prenom[i];
			}
			for (int i = 0; i < MAX_TEL; i++) {
				(rep->tab + rep->nb_elts)->tel[i] = enr.tel[i];
			}
			rep->nb_elts += 1;
			rep->est_trie = false;
			modif = true;
			return(OK);
		}
	}
	else {
		return(ERROR);
	}


#else
#ifdef IMPL_LIST

	bool inserted = false;
	if (rep->nb_elts == 0) {		/*La liste est videdonc le liste est trié*/
		if (InsertElementAt(rep->liste, rep->liste->size, enr) != 0) {
			rep->nb_elts += 1;
			modif = true;
			rep->est_trie = true;
			return(OK);
		}

	}
	else {
		for (int i = 1; i < rep->liste->size; i++) {				/*On ajoute l'enregistrement au bon endroit dans la liste*/
			if (est_sup(enr, GetElementAt(rep->liste, i)->pers)) {	/*pour qu'elle soit toujours triée*/
				InsertElementAt(rep->liste, i, enr);
				rep->nb_elts += 1;
				modif = true;
				rep->est_trie = true;
				return(OK);
			}
		}
	}
	return(ERROR);

#endif

#endif


	return(OK);

} /* fin ajout */

  /**********************************************************************/
  /* supprime du répertoire l'enregistrement dont l'indice est donné en */
  /*   paramètre       et place modif = true                            */
  /**********************************************************************/
#ifdef IMPL_TAB
void supprimer_un_contact_dans_rep(Repertoire* rep, int indice) {

	// compléter code ici pour tableau
	if ((rep->nb_elts >= 1) && (indice > 0) && (indice < rep->nb_elts)) {		/* s'il y a au moins un element ds le tableau */
																				/* et que l'indice pointe a l'interieur */
		Enregistrement* save = rep->tab;
		if (indice == rep->nb_elts) {			/*Si il s'agit du dernier élement, on réalloue le tableau à une taille nb_elts - 1 et donc le contact est bien supprimé */
			rep->tab = (Enregistrement*)realloc(rep->tab, (rep->nb_elts - 1) * sizeof(Enregistrement));
			if (rep->tab == NULL) {
				rep->tab = save; //Il n'y a pas de raison d'entrer dans ce cas car le realloc va juste libérer le dernier élément puisqu'il s'agit d'une taille inférieur
			}
		}
		else {														/*Sinon */
			for (int i = 0; i < (rep->nb_elts - indice); i++) {		/* On réécrit les contacts suivants à partir du contact a supprimer puis on réalloue la bonne taille d'emplacement mémoire*/
				*(rep->tab + indice + i) = *(rep->tab + indice + i + 1);
			}
			rep->tab = (Enregistrement*)realloc(rep->tab, (rep->nb_elts - 1) * sizeof(Enregistrement));
			if (rep->tab == NULL) {
				rep->tab = save; //Idem
			}
		}

		rep->nb_elts -= 1;		/* ds ts les cas, il y a un element en moins */
		modif = true;
	}

	return;
} /* fin supprimer */

#else
#ifdef IMPL_LIST
  /************************************************************************/
  /* supprime du répertoire l'enregistrement contenu dans le maillon elem */
  /*                   et fixe modif à vrai                              */
  /************************************************************************/
  // complet

int supprimer_un_contact_dans_rep_liste(Repertoire* rep, SingleLinkedListElem* elem) {

	int ret = DeleteLinkedListElem(rep->liste, elem);
	if (ret == 1) {
		rep->nb_elts--;
		modif = true;
	}

	return (0);
}
#endif
#endif


/**********************************************************************/
/*  fonction d'affichage d'un enregistrement sur une ligne à l'écran  */
/* ex Dupont, Jean                 0320304050                         */
/**********************************************************************/
void affichage_enreg(Enregistrement enr) {
	for (int i = 0; i < MAX_NOM; i++)
	{
		printf("u%c", enr.nom[i]);
	}
	printf(", ");
	for (int i = 0; i < MAX_NOM; i++)
	{
		printf("u%c", enr.prenom[i]);
	}
	printf(" :   ");
	for (int i = 0; i < MAX_TEL; i++)
	{
		printf("u%c", enr.tel[i]);
	}
	printf("\n");

} /* fin affichage_enreg */

  /**********************************************************************/
  /*  fonction d'affichage d'un enregistrement avec alignement des mots */
  /* pour les listes                                                    */
  /* ex | Dupont                |Jean                  |0320304050      */
  /**********************************************************************/
void affichage_enreg_frmt(Enregistrement enr)
{
	int compte = 0;
	/*Affichage nom*/
	printf("|");
	for (int i = 0; i < MAX_NOM; i++)
	{
		if (*(&(enr.nom) + i) != "") {
			printf("%c", enr.nom[i]);
			compte++;
		}
	}
	for (int i = compte; i < MAX_NOM; i++)
	{
		printf(" ");
	}
	compte = 0;

	/*Affichage prenom*/
	printf("|");
	for (int i = 0; i < MAX_NOM; i++)
	{
		if (*(&(enr.prenom) + i) != "") {
			printf("%c", enr.prenom[i]);
			compte++;
		}
	}
	for (int i = compte; i < MAX_NOM; i++)
	{
		printf(" ");
	}
	compte = 0;

	/*Affichage tel*/
	printf("|");
	for (int i = 0; i < MAX_TEL; i++)
	{
		if (*(&(enr.tel) + i) != "") {
			printf("%c", enr.tel[i]);
			compte++;
		}
	}
	for (int i = compte; i < MAX_TEL; i++)
	{
		printf(" ");
	}
	printf("\n");

} /* fin affichage_enreg */


  /**********************************************************************/
  /* test si, dans l'ordre alphabetique, un enregistrement est apres     */
  /* un autre                                                           */
  /**********************************************************************/
bool est_sup(Enregistrement enr1, Enregistrement enr2)
{
	bool sup = true;
	for (int i = 0; i < MAX_NOM; i++) {
		if ((toupper((enr2.nom)[i]) == '\0') && (toupper((enr1.nom)[i]) != '\0')) {		/*on arrive dans ces cas quand les premières lettres sont identiques */
			return(false);
		}
		else if ((toupper((enr1.nom)[i]) == '\0') && (toupper((enr2.nom)[i]) != '\0')) {			/*et que l'un des deux mots et plus court que l'autre*/
			return(true);
		}
		else if (toupper((enr2.nom)[i]) > toupper((enr1.nom)[i])) {
			return(false);
		}
		else if (toupper((enr1.nom)[i]) > toupper((enr2.nom)[i])) {
			return(true);
		}
	}
	return(true);
}

/*********************************************************************/
/*   Tri Alphabetique du tableau d'enregistrements                   */
/*********************************************************************/

void trier(Repertoire* rep) {

#ifdef IMPL_TAB
	if (rep->est_trie) {
		return;
	}
	else {
		Enregistrement* tmp = (Enregistrement*)malloc(sizeof(Enregistrement));
		if (tmp == NULL) {
			return;
		}
		else {
			bool trie = true;
			do {
				trie = true;
				for (int i = 0; i < (rep->nb_elts - 1); i++) {		/*On parcourt la liste en comparant un élément avec le suivant*/
					if ((est_sup(*(rep->tab + i + 1), *(rep->tab + i)))) {		/*Si le 2eme element est supérieur au 1er*/
						trie = false;
						for (int k = 0; k < MAX_NOM; k++) {		/*On inverse les deux enregistrement de place*/
							tmp->nom[k] = (rep->tab + i)->nom[k];					/*1->tmp*/
							(rep->tab + i)->nom[k] = (rep->tab + i + 1)->nom[k];	/*2->1*/
							(rep->tab + i + 1)->nom[k] = tmp->nom[k];					/*tmp->2*/
						}
					}
				}
			} while (trie == false);		/*On repete le parcours de la liste jusqu'a ce qu'elle soit lue triee (ce n'est pas la manière la plus optimiser, complexicité en O(n²)dans le pire des cas...)*/
			free(tmp);
		}
	}
	return;


#else
#ifdef IMPL_LIST
	// ajouter code ici pour Liste
	// rien à faire !
	// la liste est toujours triée
#endif
#endif


	rep->est_trie = true;

} /* fin trier */

  /**********************************************************************/
  /* recherche dans le répertoire d'un enregistrement correspondant au  */
  /*   nom à partir de l'indice ind                                     */
  /*   retourne l'indice de l'enregistrement correspondant au critère ou*/
  /*   un entier négatif si la recherche est négative				    */
  /**********************************************************************/

int rechercher_nom(Repertoire* rep, char nom[], int ind)
{
	int i = ind;						/* position (indice) de début de recherche dans tableau/liste rep */
	int ind_fin = rep->nb_elts;			/* position (indice) de fin de tableau/liste rep */

	char tmp_nom[MAX_NOM];	/* 2 variables temporaires dans lesquelles */
	char tmp_nom2[MAX_NOM];	/* on place la chaine recherchee et la chaine lue dans le */
							/* tableau, afin de les convertir en majuscules et les comparer */
	bool trouve = false;



#ifdef IMPL_TAB
	for (int k = 0; k < MAX_NOM; k++) {
		tmp_nom[k] = toupper(nom[k]);
	}
	while ((!trouve) && (i < ind_fin)) {				/*Tant que le nom n'est pas trouvé et qu'on a pas ateint la fin du tableau*/
		for (int u = 0; u < MAX_NOM; u++) {					/*On copie dans tmp_nom2 chaque nom dasn le repertoire*/
			tmp_nom2[u] = toupper((rep->tab + i)->nom[u]);
		}
		trouve = true;
		for (int v = 0; v < MAX_NOM; v++) {					/*Si une lettre est différente, alors ce n'est pas le bon nom et on passe au suivant en passant trouve à false*/
			if (tmp_nom[v] != tmp_nom2[v]) {
				trouve = false;
			}
		}
		i++;
	}

#else
#ifdef IMPL_LIST
	SingleLinkedListElem* CurrentElement = rep->liste->head;
	for (int k = 0; k < MAX_NOM; k++) {
		tmp_nom[k] = toupper(nom[k]);
	}

	while ((!trouve) && (i < ind_fin)) {
		trouve = true;
		for (int k = 0; k < MAX_NOM; k++) {
			tmp_nom2[k] = toupper((CurrentElement->pers).nom[k]);
			if (tmp_nom[k] == tmp_nom2[k]) {
				trouve = false;
			}
		}
		CurrentElement = CurrentElement->next;
		i++;
	}

#endif
#endif

	return((trouve) ? i : -1);
} /* fin rechercher_nom */

  /*********************************************************************/
  /* Supprimer tous les caracteres non numériques de la chaines        */
  /*********************************************************************/
void compact(char* s) {

	// Calcul de la londueur de la chaîne
	int l = 0;
	while (*(s + l) != '\0') {
		l++;
	}

	//Pour supprimer les caractères non numériques, on réécrit les chiffres, en début de chaîne, dans le parcourt de la chaîne
	int nb_chiffre = 0;
	for (int i = 0; i < l - 1; i++) {
		int ascii = (int)*(s + i);
		if ((ascii > 47) && (ascii < 58)) {		/*table ASCII, 0 à 9 codé de 48 à 57*/
			*(s + nb_chiffre) = *(s + i);
		}
	}

	//Puis on met '\0' a la suite des chiffres pour indiquer la fin de la chaîne
	*(s + nb_chiffre) = *(s + l);

	return;
}

/**********************************************************************/
/* sauvegarde le répertoire dans le fichier dont le nom est passé en  */
/* argument                                                           */
/* retourne OK si la sauvegarde a fonctionné ou ERROR sinon           */
/**********************************************************************/
int sauvegarder(Repertoire* rep, char nom_fichier[])
{
	FILE* fic_rep;					/* le fichier */
	errno_t err;

#ifdef IMPL_TAB
	if (modif) {
		err = fopen_s(&fic_rep, nom_fichier, "w");
		for (int i = 0; i < rep->nb_elts; i++) {
			fputs((rep->tab + i)->nom, fic_rep);
			fputs(";", fic_rep);
			fputs((rep->tab + i)->prenom, fic_rep);
			fputs(";", fic_rep);
			fputs((rep->tab + i)->tel, fic_rep);
			fputs("\n", fic_rep);
		}
	}

#else
#ifdef IMPL_LIST
	if (modif) {
		err = fopen_s(&fic_rep, nom_fichier, "w");
		SingleLinkedListElem* CurrentElement = rep->liste->head;
		for (int i = 0; i < rep->nb_elts; i++) {
			fputs((CurrentElement->pers).nom, fic_rep);
			fputs(";", fic_rep);
			fputs((CurrentElement->pers).prenom, fic_rep);
			fputs(";", fic_rep);
			fputs((CurrentElement->pers).tel, fic_rep);
			fputs("\n", fic_rep);
		}
	}
#endif
#endif

	return(OK);
} /* fin sauvegarder */


  /**********************************************************************/
  /*   charge dans le répertoire le contenu du fichier dont le nom est  */
  /*   passé en argument                                                */
  /*   retourne OK si le chargement a fonctionné et ERROR sinon         */
  /**********************************************************************/

int charger(Repertoire* rep, char nom_fichier[])
{
	FILE* fic_rep;					/* le fichier */
	errno_t err;
	int num_rec = 0;						/* index sur enregistrements */
	int long_max_rec = sizeof(Enregistrement);
	char buffer[sizeof(Enregistrement) + 1];
	int idx = 0;

	char* char_nw_line;

	_set_errno(0);
	if (((err = fopen_s(&fic_rep, nom_fichier, "r")) != 0) || (fic_rep == NULL))
	{
		return(err);
	}
	else
	{
		while (!feof(fic_rep) && (rep->nb_elts < MAX_ENREG))
		{
			if (fgets(buffer, long_max_rec, fic_rep) != NULL)
			{
				/* memorisation de l'enregistrement lu dans le tableau */
				buffer[long_max_rec] = 0;			/* en principe il y a deja un fin_de_chaine, cf fgets */

				if ((char_nw_line = strchr(buffer, '\n')) != NULL)
					*char_nw_line = '\0';			/* suppression du fin_de_ligne eventuel */

				idx = 0;								/* analyse depuis le debut de la ligne */
#ifdef IMPL_TAB
				if (lire_champ_suivant(buffer, &idx, rep->tab[num_rec].nom, MAX_NOM, SEPARATEUR) == OK)
				{
					idx++;							/* on saute le separateur */
					if (lire_champ_suivant(buffer, &idx, rep->tab[num_rec].prenom, MAX_NOM, SEPARATEUR) == OK)
					{
						idx++;
						if (lire_champ_suivant(buffer, &idx, rep->tab[num_rec].tel, MAX_TEL, SEPARATEUR) == OK)
							num_rec++;		/* element à priori correct, on le comptabilise */
					}
				}
#else
#ifdef IMPL_LIST
														// ajouter code implemention liste
#endif
#endif




			}

		}
		rep->nb_elts = num_rec;
		fclose(fic_rep);
		return(OK);
	}


} /* fin charger */