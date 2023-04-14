#include <stdio.h> // entrees-sorties
#include <string.h> // formattage de chaines
#include <stdlib.h> // random, ...
#include <time.h> // time usage
#include <math.h> // math usage
typedef enum bool{true, false} bool ; // boolean type (don't use shit library)

//#include<windows.h>  // no such file or directory // pas de sons avec windows

// nb de lignes a ne pas dépasser
static int LIGNES_MAX_FICHIER = 10;


// enregistre dans un fichier vide déja créé les éléments
// persistance des données
// doit ajouter dans le fichier chaque lignes qui a été saisi par le joueur ainsi que les autre infos
int enregistrerScore(char*strData, char*pseudo, int niveau, int score, char*nomFichier){

    /* Variable to store user content */
	char niveauchar[2];
	char scorestr[6]; 
	// convert data 
	sprintf(niveauchar, "%d", niveau);
	sprintf(scorestr, "%d", score);
    char data[strlen(strData)];
    /* File pointer to hold reference to our file */
    FILE * fPtr;
	// write in existing file
    fPtr = fopen(nomFichier, "w");
    /* fopen() return NULL if last operation was unsuccessful */
    if(fPtr == NULL){
        /* File not created hence exit */
        printf("Unable to create file.\n");
        exit(EXIT_FAILURE);
    }
	// escape sequence 
	strcat(niveauchar, "\n");
	strcat(scorestr, "\n");
	strcat(strData, "\n");
	strcat(pseudo, "\n");
    /* Write data to file */
    fputs(strData, fPtr);
	fputs("niveau : ", fPtr);
	fputs(pseudo, fPtr);
	fputs("score : ", fPtr);
    /* Close file to save file data */
    fclose(fPtr);
    /* Success message */
    printf("File created and saved successfully. :) \n");
    return EXIT_SUCCESS;
}

/// ça marche
// lire un fichier et afficher la ligne lue
int readOneLine(char*filepath,int line_num,char* getLine){
	//printf("demande ouverture fichier : %s\n",filepath);
	int linen = 0;
	FILE * file;
	char displayText[100];
	bool carry = true;
	file = fopen(filepath, "r");
	if(file == NULL){
		printf("echec ouverture du fichier : %s\n", filepath);
	}
	while(!feof(file) && carry == true){
		/*
		 warning: the `gets' function is dangerous and should not be used.
		*/
		fgets(displayText, 100, file);
		// afichage si vrai :
		if (linen == line_num){
			printf("%s\n", displayText);
			strcpy(getLine, displayText);
			line_num = linen;
			carry = false;
		}
		linen++;
	}
	fclose(file);
	return EXIT_SUCCESS;
}

// osx/macos (apple sys)


bool charIsInListStr(char oneChar, char*listString){
	int cpt = 0;
	bool carry = true;
	while (cpt < strlen(listString) && carry == true){
		if (oneChar == listString[cpt]){
			carry = false;
		}
		cpt++;
	}
	return carry;
}

bool intIsInListints(int oneNum, int*listNum, int tailletab){
	int cpt = 0;
	bool carry = true;
	while (cpt < tailletab && carry == true){
		if (oneNum == listNum[cpt]){
			carry = false;
		}
		cpt++;
	}
	return carry;
}




// créer un menu de sélection 
char menu(void){
	char res;
	printf("\n");
	printf("┌─────────── Menu ───────────┐\n");
	printf("│   q : quitter              │\n");
	printf("│   c : continuer            │\n");
	printf("│   s : enregistrer          │\n");
	printf("│   a : afficher points      │\n");	
	printf("│   l : changer de niveau    │\n");		
	printf("└────────────────────────────┘\n");
	printf("→");
	scanf("%c", &res); // car on sait pas si saisie user il y a déja une valeur dedans
	return res;
}

// resize the filesize at input keyboard (From : Google) not require initiasize
char*inputString(){
//The size is extended by the input with the value of the provisional
	FILE*fp= stdin; // get stdinput
	size_t size=10; // unknow
	/*IMPORTANT : si la chaine est de taille constante
		le free() est automatiquement appliqué par le langage C
		sauf déclaration de départ avec une taille omise*/
    char *str;
    int ch;
    size_t len = 0;
	//size is start size
    str = realloc(NULL, sizeof(*str)*size);
    if(!str)return str;
    while(EOF!=(ch=fgetc(fp)) && ch != '\n'){
        str[len++]=ch;
        if(len==size){
            str = realloc(str, sizeof(*str)*(size+=16));
            if(!str)return str;
        }
    }
    str[len++]='\0';
    return realloc(str, sizeof(*str)*len);
}




/// VERIFIEE !! (ça marche)
// permet de mettre a jour le chemin de navigation temporaire
// si un utilisateur veut changer de niveau en cours de partie
int updateNav(char*toUpdate, int rank, char*base, char*endfilepath){
	// onvertir l'entier en chaine ici 
	char tempconvert[9];
	// copie la partie gauche du chemin d'accés 
	// doit écraser les données précédentes
	strcpy(toUpdate, base); 
	sprintf(tempconvert, "%d", rank);
	// concat le milieu (partie changeante)
	strcat(toUpdate, tempconvert);
	// concat la partie de fin (droite et aussi partie changeante)
	strcat(toUpdate, endfilepath);
	return EXIT_SUCCESS;
}

// compare les chaine et ajoute un point sinécessaire
int reponseJuste(char*bonneReponse, char*reponseJoueur, int point){
	// pas la même taille
	int osize = strlen(bonneReponse);  // on get la valeur depuis l'index que l'utilisateru va saisir 
	int cpt = 0;
	int res = 0;
	bool carry = true;
	if (osize != strlen(reponseJoueur)){
		carry = false;
	} else {
		while (cpt < osize && carry == true){
			if (bonneReponse[cpt] != reponseJoueur[cpt]){
				carry = false;
			}
			cpt++;
		}		
	}
	if (carry == true){
		point++;
	}
	return EXIT_SUCCESS;
}




// programme principal
int main(){
	// initialisation fonction aléatoire :
	srand(time(NULL));
	// choix ligne aléatoire pour la question
	char ch;
	int choixLigne; 
	char pseudo[25];
	int points = 0; 
	bool carry = true;
	char saisieUser;
	// ne dois pas dépasser le nombre de question dans le fichier
	int nbTours = 0; 
	bool gagne = false; 
	int level; // store player level
	// ne pas recommencer 2 fois la même question
	// allocation dynamique : (tableau d'entiers pour le n° de lignes)
	char chemin[19] = "./level_ask/"; // filepath navigation
	char tempChemin[9] = ""; // stockage du nombre saisi par l'utilisateur convertit en chaine
	char navChemin[19] = "./level_ask/"; // chaine pour le déplacement (modifiable)
	char reponseJoueur[10][300]; // une liste de 10 chaines de caractères de 300 caractères
	char bonneReponse[300]; // chaine de 300 charactères
	int choixniveau;  // saisi utilisateur
	int listNumNiveaux[3][10]; // stocker toutes les réponse (n°lignes) déja faites pour chaque niveau 
	printf("Bienvenu dans le Quizz sur l'informatique\n\n");

	printf("entrez votre pseudo : (25 caractères max)\n");
	strcpy(pseudo, inputString());
	printf("choisir le niveau : \n(facile, moyen, difficile)\n");
	printf("avec les touches respectives 1, 2, et 3 : \n");
	scanf("%d", &choixniveau);
	// on saisie le niveau qu'une seule fois (sauf si le joueur veut changer)
    if ( choixniveau >= 1 && choixniveau <= 3){
		updateNav(navChemin, choixniveau, chemin, "/ask.txt");
    } else { // saisie invalide
        printf("saisie non valide\n");
        carry = false;
    }


	while(carry == true){
		getchar();
		saisieUser = menu();
		// on fait un nouveau aléatoire tant que qu'il est déja présent dans la liste
		// on aurait pu aussi faire une liste des éléments autorisés et les supprimer au fur et a mesure
		// que le joueur choisi des choix et faire un nombre aléatoire des index de cette liste (éviter le while)
		while (intIsInListints(choixLigne, listNumNiveaux[level], nbTours+1)){
			choixLigne = rand() % LIGNES_MAX_FICHIER;
		}
		
		// je déteste les switch(s) désolé
		if (saisieUser == 'c'){
			int a, b, c, d; // mélanger les questions (pour pas voir la solution toujours au même endroit)
			// on continue de jouer
			printf("répondez à la question suivante avec (1, 2, 3):\n\n\t");
			// affichage question :
			readOneLine(navChemin, choixLigne, bonneReponse);			

			// affichage des 3 questions
			char tempNumcp[21] = "./level_ask/1/r1.txt"; // convertit le nombre en entier
			char temprescharcp[30] = "./level_ask/1/soluce.txt";
			tempNumcp[12] = level + 1 + '0';
			for (int ii=1;ii<4;ii++){
				// PROBLEME ICI !!!
				tempNumcp[14] = ii + '0';
				readOneLine(tempNumcp, choixLigne, bonneReponse);
			}
			temprescharcp[12] = level + 1 + '0';			
			readOneLine(temprescharcp, choixLigne, bonneReponse);

			// for (int r=1;r<4;r++){
			// 	char tempNumQuestion[20]; // stocke chaque filepath de questions
			// 	char tempNumConvert; // convertit le nombre en entier
			// 	printf("%d) ", r);
			// 	// je modifie le fichier ouvert pour avoir la bonne question
			// 	tempNumQuestion[0] = 'r';// met le premier a "r"
			// 	//sprintf(tempNumConvert, "%d", r); // convertit en chaine
			// 	tempNumQuestion[1] = r + '0'; 
			// 	tempNumQuestion[2] = '.';
			// 	tempNumQuestion[3] = 't';				
			// 	tempNumQuestion[4] = 'x';
			// 	tempNumQuestion[5] = 't';
			// 	updateNav(navChemin, choixniveau, "./level_ask/", tempNumQuestion);
			// 	readOneLine(navChemin, choixLigne, bonneReponse);
			// }
			// pour l'enregistrement des données
			scanf("%s", reponseJoueur[nbTours]);
			getchar();
			printf("saisie marche\n");			
			// ajouter points 
			updateNav(navChemin, level, chemin, "/soluce.txt");
			readOneLine(navChemin, choixLigne, bonneReponse);
			reponseJuste(bonneReponse, reponseJoueur[nbTours], points);
			// ajouter la ligne parcourue (pour pas reposer la même question)
			listNumNiveaux[level][nbTours] = choixLigne; // un entier dans un entier
			// on buff le niveau comme écrit dans les cahier des charges
			if (level < 3 && points >= 8){
				level++;
			}
		} else if (saisieUser == 's'){
			// on enregistre dans un fichier toutes les saisies précédentes 
			//enregistrerScore(char* strData, char*pseudo, int niveau, float score, char*nomFichier)
			char tempStartTrame[2378] = "-----------------------------------------------------------------\n";
			strcat(tempStartTrame, "QUESTIONS : \n");
			char tempsLineNumberChar[2];
			// pour toutes les question on les stack dans le fichier :
			for (int i=0; i<11;i++){
				sprintf(tempsLineNumberChar, "%d", i); // delete last sprintf
				strcat(tempStartTrame, "question n°");
				strcat(tempStartTrame, tempsLineNumberChar);
				strcat(tempStartTrame, reponseJoueur[i]);
			}
			
			enregistrerScore(tempStartTrame, pseudo, level, points, "userQuestions.txt");
		} else if (saisieUser == 'q'){
			// on part sans enregistrer
			carry = false;
		} else if (saisieUser == 'a'){
			// afficher les points (peut être flottant)
			printf("vous avez : %d\t points.\n", points);
		} else if (points >= 8){
			// gagné !!!
			// rappel : 1 poit par bonne réponses 
			// pour gagner il faut 8 points (et on joue un son)
			printf("vous avez gagné\n");
			// jouer un son...
			printf("%c\a", ' ');
			carry = false;
		} else if (nbTours > 30){
			// tt les questions terminées
			printf("barvo,vous avez fait toutes les questions\n");
			carry = false;		
		} else if (saisieUser == 'l'){
			// changer de niveau
			int tempLevelError = level; // crée une copie de la variable level (si on modifie level il n'y a pas d'incidence)
			printf("vous choisissez quel niveau ?\n");
			scanf("%d", &level);
			if (level < 1 && level > 3){
				level = tempLevelError;
				printf("mauvaise saisie, le niveau va rester le même\n");
			}
		} else {
			// mauvaise saisie
			printf("vous avez mal répondu (%c)!!!\n", saisieUser);
			carry = false;
		}
		nbTours++;
	}
	printf("au revoir %s...\n\n", pseudo);
	return EXIT_SUCCESS;
}








// end page