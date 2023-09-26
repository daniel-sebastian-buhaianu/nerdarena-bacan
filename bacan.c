#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MIN_N 1
#define MAX_N 100000
#define MAX_PRODUSE 1000
#define LGMAX_NUME_PRODUS 20
#define LGMAX_LINIE 30

typedef struct
{
	char nume[LGMAX_NUME_PRODUS+1];
	unsigned vandute;
} PRODUS;

int main()
{
	FILE *fin = fopen("bacan.in", "r");

	if (!fin)
	{
		printf("Eroare fisier bacan.in\n");
		return 1;
	}

	char *linie = (char*)malloc(LGMAX_LINIE);

	if (!linie)
	{
		printf("Eroare alocare memorie *linie\n");
		return 2;
	}

	if (fgets(linie, LGMAX_LINIE, fin) == NULL)
	{
		printf("Eroare citire prima linie\n");
		return 3;
	}

	unsigned n;

	if(sscanf(linie, "%u\n", &n) < 1)
	{
		printf("Eroare format prima linie\n");
		return 4;
	}
	
	if (n < MIN_N || n > MAX_N)
	{
		printf("Eroare valoare N\n");
		return 5;
	}

	size_t lgmax = LGMAX_LINIE;

	char *nume = (char*)malloc(LGMAX_NUME_PRODUS+1);

	int st, dr, mij, i, j;

	unsigned buc, nrprod = 0;

	PRODUS prod[MAX_PRODUSE];

	for (i = 0; i < n; i++)
	{
		if (getline(&linie, &lgmax, fin) > 0)
		{
			if (sscanf(linie, "%[^*]* %u\n", nume, &buc) < 2)
			{
				printf("Eroare format linia %d\n", i+2);
				return 6;
			}

			nume[strlen(nume)-1] = '\0'; // elimin ' '(ultimul caracter)
			
			// folosesc cautare binara pentru a determina pozitia unde trebuie
			// sa inserez/actualizez acest produs astfel incat
			// vectorul de produse sa ramana ordonat.

			st = -1, dr = nrprod;

			while (dr-st > 1)
			{
				mij = st + (dr-st)/2;

				if (strcmp(prod[mij].nume, nume) < 0)
				{
					st = mij;
				}
				else
				{
					dr = mij;
				}
			}

			// dr = pozitia unde trebuie sa inserez/actualizez datele in prod[]

			if (strcmp(prod[dr].nume, nume)) // inserez
			{	
				nrprod++;

				for (j = nrprod; j > dr; j--)
				{
					strcpy(prod[j].nume, prod[j-1].nume);

					prod[j].vandute = prod[j-1].vandute;
				}

				strcpy(prod[dr].nume, nume);

				prod[dr].vandute = buc;	
			}
			else // actualizez
			{ 
				prod[dr].vandute += buc;
			}
		}
	}

	fclose(fin);

	FILE *fout = fopen("bacan.out", "w");

	fprintf(fout, "%u\n", nrprod);

	for (i = 0; i < nrprod; i++)
	{
		fprintf(fout, "%s * %u\n", prod[i].nume, prod[i].vandute);
	}

	fclose(fout);

	free(linie);
	free(nume);

	return 0;
}

// scor 100
