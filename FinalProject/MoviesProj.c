#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define M 1000
#define N 100 
#define MOVIEFILE "moviesData.txt"
#define VOTEFILE "votingData.txt"


typedef struct 
{
	int value;
	char* p2comments;
	char country[15];
}vote;

typedef struct 
{
	int id;
	char* p2name;
	char* p2genre;
	char studio[30];
	int year;
	vote* p2list;
	int Nvotes;
}movie;

typedef struct 
{
	int Nvotes;
	char country[20];
}Cvotes;

int countLines(const char* file1)
{
	FILE* File = fopen(file1, "r");
	if (!File)
	{
		printf("Error:cant open the movies file for count");
		exit(1);
	}
	int NumOfLines = 0;
	char temp[M];

	while (fgets(temp, sizeof(temp), File))
	{
		NumOfLines++;
		strcpy(temp, "");
	}
	NumOfLines--;
	fclose(File);
	return NumOfLines;
}

int StringsTheSame(const char string1[], const char string2[])
{
	int i = 0; 
	while (string1[i] || string2[i])
	{
		if (string1[i] == string2[i] || string1[i] - 32 == string2[i] || string1[i] == string2[i] - 32)
		{
			i++;
		}
		else
		{
			return 0;
		}
	}
	return 1; 
}

void FromFile2Movies(const char* mfile,movie** MovieArray, const int size)
{
	FILE* MovieFile = fopen(mfile, "r");
	if (!MovieFile)
	{
		printf("Error:cant open the movie file to create array");
		exit(1);
	}
	char temp[N];
	int i = 0;
	fgets(temp, N, MovieFile);
	strcpy(temp, "");

	while (fgets(temp, N, MovieFile) != NULL)
	{
		char* comma = strtok(temp, ",");
		(*MovieArray)[i].id = atoi(comma);

		comma = strtok(NULL, ",");
		(*MovieArray)[i].p2name = malloc((strlen(comma) + 1) * sizeof(char));
		if ((*MovieArray)[i].p2name == NULL)
		{
			printf("Error: can't open the voting file\n");
			exit(1);
		}
		strcpy((*MovieArray)[i].p2name, comma);

		comma = strtok(NULL, ",");
		(*MovieArray)[i].p2genre = malloc((strlen(comma) + 1) * sizeof(char));
		if ((*MovieArray)[i].p2genre == NULL)
		{
			printf("Error: can't open the voting file\n");
		    exit(1);
		}
		strcpy((*MovieArray)[i].p2genre, comma);

		comma = strtok(NULL, ",");
		strncpy((*MovieArray)[i].studio, comma, sizeof((*MovieArray)[i].studio));
		(*MovieArray)[i].studio[N - 1] = '\0';

		comma = strtok(NULL, ",");
		(*MovieArray)[i].year = atoi(comma);

		(*MovieArray)[i].Nvotes = 0;
		i++;
	}
	fclose(MovieFile);
}

void FromFile2votes(const char* mfile, movie** MovieArray,const int size)
{
	FILE* votefile = fopen(mfile, "r");
	if (!votefile)
	{
		printf("Error: can't open the voting file\n");
		exit(1);
	}

	char temp[N];
	fgets(temp, N, votefile);

	while (fgets(temp, N, votefile) != NULL)
	{
		char* comma = strtok(temp, ":");
		int tempid = atoi(comma);

		for (int j = 0; j < size; j++)
		{
			if (tempid == (*MovieArray)[j].id)
			{
				(*MovieArray)[j].Nvotes++;
				if ((*MovieArray)[j].Nvotes == 1)
				{
					(*MovieArray)[j].p2list = malloc(((*MovieArray)[j].Nvotes+1) * sizeof(vote));
					if ((*MovieArray)[j].p2list == NULL)
					{
						printf("Error: memory allocation failed\n");
						fclose(votefile);
						exit(1);
					}
				}
				else
				{
					(*MovieArray)[j].p2list = (vote*)realloc((*MovieArray)[j].p2list, (*MovieArray)[j].Nvotes * sizeof(vote));
					if ((*MovieArray)[j].p2list == NULL) 
					{
						printf("Error: memory allocation failed\n");
						fclose(votefile);
						exit(1);
					}
				}
				int index = 0;
				index =	(*MovieArray)[j].Nvotes - 1;
				comma = strtok(NULL, ":");
				(*MovieArray)[j].p2list[index].value = atoi(comma);

				comma = strtok(NULL, ":");
				strncpy((*MovieArray)[j].p2list[index].country, comma, sizeof((*MovieArray)[j].p2list[index].country) - 1);
				(*MovieArray)[j].p2list[index].country[sizeof((*MovieArray)[j].p2list[index].country) - 1] = '\0';
				
				comma = strtok(NULL, ":");
				(*MovieArray)[j].p2list[index].p2comments = malloc(strlen(comma) + 1);
				if ((*MovieArray)[j].p2list[index].p2comments == NULL)
				{
					printf("Error: memory allocation failed\n");
					fclose(votefile);
					exit(1);
				}
				strcpy((*MovieArray)[j].p2list[index].p2comments, comma);
				break;
			}
		}
	}

	fclose(votefile);
}

int addMovie(movie** MovieArray, int* size)
{
	int Tempid = 0, Tempyear = 0;
	char Tmpmoviename[N], Tempgenre[N] = { 0 }, TempLead[30] = { 0 };
	printf("Please enter movie id:\n");
	scanf("%d", &Tempid);
	printf("Please enter movie name:\n");
	while (getchar() != '\n');
	gets(Tmpmoviename,sizeof(Tmpmoviename),stdin);
	for (int i = 0; i < *size; i++)
	{
		if ((*MovieArray)[i].id == Tempid || (StringsTheSame(Tmpmoviename, (*MovieArray)[i].p2name) == 1))
		{
			printf("The movie exists\n");
			return 0;
		}
	}
	printf("Please enter the genre of the movie:\n");
	gets(Tempgenre, sizeof(Tempgenre), stdin);
	printf("Please enter the lead studio of the movie:\n");
	gets(TempLead, sizeof(TempLead), stdin);
	printf("Please enter the year of the movie:\n");
	scanf("%d", &Tempyear);
	
	
	movie* temp = realloc(*MovieArray, (*size + 1) * sizeof(movie));
	if (temp == NULL)
	{
		printf("Error: memory allocation failed\n");
		return 0;
	}
	*MovieArray = temp;

	(*MovieArray)[*size].id = Tempid;
	(*MovieArray)[*size].p2name = malloc((strlen(Tmpmoviename) + 1) * sizeof(char));
	if ((*MovieArray)[*size].p2name == NULL)
	{
		printf("Error: memory allocation failed\n");
		return 0;
	}
	strcpy((*MovieArray)[*size].p2name, Tmpmoviename);

	(*MovieArray)[*size].p2genre = malloc((strlen(Tempgenre) + 1) * sizeof(char));
	if ((*MovieArray)[*size].p2genre == NULL)
	{
		printf("Error: memory allocation failed\n");
		free((*MovieArray)[*size].p2name);
		return 0;
	}
	strcpy((*MovieArray)[*size].p2genre, Tempgenre);

	strcpy((*MovieArray)[*size].studio, TempLead);
	(*MovieArray)[*size].year = Tempyear;
	(*MovieArray)[*size].Nvotes = 0;
	(*MovieArray)[*size].p2list = NULL; 
	(*size)++;

	return 1;
}

int addVote(int idM, movie** MovieArr, int* size)
{
	int Tmpvalue = 0, tempindex = -1, tmpnvotes = 0;
	char Tmpcountry[15] = { 0 },Tempcomment[N] = { 0 };
	printf("please enter your country and the value:\n");
	scanf("%s %d", Tmpcountry, &Tmpvalue);
	printf("please enter your comment\n");
	while (getchar() != '\n');
	gets(Tempcomment,sizeof(Tempcomment),stdin);
	for (int i = 0; i < *size; i++)
	{
		if (idM == ((*MovieArr)[i]).id)
		{
			tempindex = i;
			break;
		}
	}
	if (tempindex == -1)
	{
		printf("the movie doesn't exist\n");
			return 0;
	}
	tmpnvotes = (*MovieArr)[tempindex].Nvotes;
	for (int k = 0; k < tmpnvotes; k++)
    {
		if (((((strcmp(Tmpcountry, ((*MovieArr)[tempindex]).p2list[k].country)))) == 0) && (((*MovieArr)[tempindex].p2list[k].value) == Tmpvalue) && ((strncmp(Tempcomment, ((*MovieArr)[tempindex].p2list[k].p2comments),strlen(Tempcomment)) == 0)))// בודק אם קיים דעה כזאת
		{
			printf("This vote is already exist\n");
			return 0;
		}
	}
	if ((*MovieArr)[tempindex].Nvotes == 0)
	{
		vote* temp = malloc((tmpnvotes + 1) * sizeof(vote));
		if (temp == NULL)
		{
			printf("Error: memory allocation failed\n");
			return 0;
		}
		(*MovieArr)[tempindex].p2list = temp;
	}
	else
	{
		vote* temp = realloc((*MovieArr)[tempindex].p2list, (tmpnvotes + 1) * sizeof(vote));
		if (temp == NULL)
		{
			printf("Error: memory allocation failed\n");
			return 0;
		}
		(*MovieArr)[tempindex].p2list = temp;
	}
	(*MovieArr)[tempindex].p2list[tmpnvotes].value = Tmpvalue;
	(*MovieArr)[tempindex].p2list[tmpnvotes].p2comments = malloc((strlen(Tempcomment) + 1) * sizeof(char));
	if ((*MovieArr)[tempindex].p2list[tmpnvotes].p2comments == NULL)
	{
		printf("Error: memory allocation failed\n");
		return 0;
	}
	strcpy((*MovieArr)[tempindex].p2list[tmpnvotes].p2comments, Tempcomment);
	strcpy((*MovieArr)[tempindex].p2list[tmpnvotes].country, Tmpcountry);
	(*MovieArr)[tempindex].Nvotes++;
	return 1;
}

void FreeMovieArray(const movie* MovieArray,const int size)
{
	for (int j = 0; j < size; j++)
	{
		free(MovieArray[j].p2name);
		free(MovieArray[j].p2genre);
		
		if (MovieArray[j].Nvotes != 0)
		{
				for (int k = 0; k < MovieArray[j].Nvotes; k++)
				{
					free(MovieArray[j].p2list[k].p2comments);
				}
				free(MovieArray[j].p2list);
		}
	}
	free(MovieArray);
}

int printVotes(const char* moviename,const movie** MovieArr,const int size)
{
	bool Exist = false;
	int i = 0, Mindex = 0;
	while (i < size)
	{
		if (StringsTheSame((*MovieArr)[i].p2name, moviename) == 1)
		{
			Exist = true;
			Mindex = i;
			break;
		}
		i++;
	}
	if (Exist == false) 
	{
		printf("The movie doesn't exist\n");
		return -1;
	}
	if ((*MovieArr)[Mindex].p2list == NULL)
	{
		printf("There are no vote for this movie");
		return 0;
	}
	for (int j = 0; j < (*MovieArr)[Mindex].Nvotes; j++)
	{
		printf("  country: %s\n", (*MovieArr)[Mindex].p2list[j].country);
		printf("  comment: %s\n", (*MovieArr)[Mindex].p2list[j].p2comments);
	}
	return 1;
}

void countGenre(const char* Gen,const movie** MovieArr,const int size)
{
	bool Exist = false;
	char MovieTemp[N] = { 0 }, GenreTemp[N] = { 0 };

	for (int i = 0; i < size; i++)
	{
		strncpy(GenreTemp, (*MovieArr)[i].p2genre,sizeof(GenreTemp)-1);
		if (StringsTheSame(GenreTemp, Gen) == 1)
		{
			strcpy(MovieTemp,(*MovieArr)[i].p2name);
			printf("%s\n", MovieTemp);
			Exist = true;
			MovieTemp[0] = '\0';
		}
		GenreTemp[0] = '\0';
	}
	if (Exist == false)
	{
		printf("There are no movies from this genre\n");
	}
}

void printValue(const int v,const char* c,const movie** MovieArr,const int size)
{
	bool exist = false;
	int i =0;
	for (i = 0; i < size; i++)
	{
		for (int k = 0; k < (*MovieArr)[i].Nvotes; k++)
		{
			if ((StringsTheSame(c, (*MovieArr)[i].p2list[k].country) == 1) && (v == (*MovieArr)[i].p2list[k].value))
			{
				printf("%s\n", (*MovieArr)[i].p2name);
				exist = true;
			}
		}
	}
	if (exist == false)
	{
		printf("There is no movie with this value rating or from this country");
	}
}

int InString(const char* Big,const char* Small)
{
	char* token;
	int lenght = strlen(Big)+1;
	char* copy = malloc(strlen(Big) + 1);
	strcpy(copy, Big);
	token = strtok(copy, " ");
	while (token != NULL)
	{
		if ((StringsTheSame(token, Small) == 1))
		{
			free(copy);
			return 1;
		}
		else
		{
		token = strtok(NULL, " ");
		}
	}
	free(copy);
	return 0;
}

void countCountry(const int Y,const movie** MovieArr,const int size)
{
	int counter = 0;
	char space[] = " ";
	char C[M] = { 0 };
	for (int i = 0; i < size; i++)
	{
		if (Y == (*MovieArr)[i].year)
		{
			for (int k = 0; k < (*MovieArr)[i].Nvotes; k++)
			{
				if(InString(C,(*MovieArr)[i].p2list[k].country) == 1)
				{
					continue;
				}
				else
				{
					strcat(C, (*MovieArr)[i].p2list[k].country);
					strcat(C, space);
					counter++;
				}
			}
		}
	}
	if (counter == 0)
	{
		printf("There are no countries that rated the movies created in %d\n", Y);
	}
	else 
	{
		printf("There are %d diffrent countries that rated the movies created in %d\n", counter, Y);
	}
}

int maxBycountry(const movie** MovieArr,const int size)
{
	int csize = 1;
	Cvotes* Carr = malloc(csize * sizeof(Cvotes));
	if (Carr == NULL)
	{
		printf("Error:memory allocation for movieArray\n");
	}
	strcpy(Carr[0].country, (*MovieArr)[0].p2list[0].country);
	Carr[0].Nvotes = 1;
	int i = 0, maxcount = 0;
	char empty[3] = "-\n";
	while(i<size)
	{
		bool exist = false; 
		for (int k = 0; k < (*MovieArr)[i].Nvotes; k++)
		{
			if (StringsTheSame((*MovieArr)[i].p2list[k].p2comments, empty) == 1)
			{
				continue;
			}
			exist = false;
			for (int j = 0; j < csize; j++)
			{
				if (StringsTheSame((*MovieArr)[i].p2list[k].country, Carr[j].country) == 1)
				{
					Carr[j].Nvotes++;
					exist = true;
				}
			}
			if (exist == false)
			{
				Cvotes* temp = realloc(Carr, (csize + 1) * sizeof(Cvotes));
				if (temp == NULL)
				{
					printf("Error: memory allocation failed\n");
					return 0;
				}
				Carr = temp;
				strcpy(Carr[csize].country, (*MovieArr)[i].p2list[k].country);
				Carr[csize].Nvotes = 1;
				csize++;
			}
		}
		i++;
	}
	for (int t = 0; t < csize; t++)
	{
		if (Carr[t].Nvotes > maxcount)
		{
			maxcount = Carr[t].Nvotes;
		}
	}
	for (int t = 0; t < csize; t++)
	{
		if (Carr[t].Nvotes == maxcount)
		{
			printf("%s\n", Carr[t].country);
		}
	}
	free(Carr);
}

void RecommendMovie(const double val,const movie** MovieArr,const int size)
{
	FILE* file = fopen("Recommendation.txt", "w");
	if (file == NULL)
	{
		printf("Error creating file\n");
	}
	double AVG = 0, sum = 0; 
	for (int i = 0; i < size; i++)
	{
		sum = 0, AVG = 0;
		for (int j = 0; j < (*MovieArr)[i].Nvotes; j++) 
		{
			sum = sum + (*MovieArr)[i].p2list[j].value; 
		}
		AVG = sum / (*MovieArr)[i].Nvotes;
		if (val <= AVG)
		{
			fprintf(file, "%s, %s\n", (*MovieArr)[i].p2name, (*MovieArr)[i].p2genre);
		}
	}
	printf("The file was created successfuly\n");
	fclose(file);
}

void DeleteWorst(const char* G, movie** MovieArr,const int size)
{
	bool Exist = false;
	int index = 0;
	for (int i = 0; i < size; i++)
	{
		if (StringsTheSame(G, (*MovieArr)[i].p2genre) == 1)
		{
			Exist = true;
			index = i;
			break;
		}
	}
	if (Exist == false)
	{
		printf("There is no movie of this genre");
		exit(1);
	}
	int minvalue = 10;
	for (int j = index; j < size; j++)
	{
		for (int k = 0; k < (*MovieArr)[j].Nvotes; k++)
		{
			if ((minvalue > (*MovieArr)[j].p2list[k].value) && (StringsTheSame(G, (*MovieArr)[j].p2genre) == 1))
			{
				minvalue = (*MovieArr)[j].p2list[k].value;
			}
		}
	}
	for (int t = index; t < size; t++)
	{
		for (int k = 0; k < (*MovieArr)[t].Nvotes; k++)
		{
			if ((minvalue == (*MovieArr)[t].p2list[k].value) && (StringsTheSame(G, (*MovieArr)[t].p2genre) == 1))
			{
				for (int p = k; p < (*MovieArr)[t].Nvotes-1; p++)
				{
					(*MovieArr)[t].p2list[p] = (*MovieArr)[t].p2list[p + 1];
				}
				(*MovieArr)[t].Nvotes--;
			}
		}
	}
	printf("The worst vote from this genre was deleted successfuly.\n");
}

void UpdateMovieFile(const movie** MovieArr,const int size)
{
	FILE* file = fopen("moviesData.txt", "w");
	if (file == NULL)
	{
		printf("Error: can't open the voting file\n");
		exit(1);
	}
	fprintf(file,"format:m_id,movie_name,Genre,Lead Studio,Year\n");
	for (int i = 0; i < size; i++)
	{
		fprintf(file, "%d,%s,%s,%s,%d\n", (*MovieArr)[i].id, (*MovieArr)[i].p2name, (*MovieArr)[i].p2genre, (*MovieArr)[i].studio, (*MovieArr)[i].year);
	}
	fclose(file);
}

void UpdateVotesFile(const movie** MovieArr,const int size)
{
	FILE* file = fopen("votingData.txt", "w");
	if (file == NULL)
	{
		printf("Error: can't open the voting file\n");
		exit(1);
	}
	fprintf(file, "m_id:vote:coutry:comment //- means an empty comment\n");
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < (*MovieArr)[i].Nvotes; j++)
		{
			fprintf(file, "%d:%d:%s:%s", (*MovieArr)[i].id, (*MovieArr)[i].p2list[j].value, (*MovieArr)[i].p2list[j].country, (*MovieArr)[i].p2list[j].p2comments);
		}
	}
	fclose(file);
}

void Printmenu()
{
	printf("Please enter a number between(0-9):\n");
	printf("(1) - Add movie\n");
	printf("(2) - Add vote\n");
	printf("(3) - Print movie notes \n");
	printf("(4) - Show movies of a certain type\n");
	printf("(5) - Insert value and country and see who rate this movie\n");
	printf("(6) - Insert a year and see how many different country rated movies from this year\n");
	printf("(7) - Get the name of the country who rated the most movies\n");
	printf("(8) - Enter a voting value for the movie and get a file with all the movies rated higher than this value\n");
	printf("(9) - Insert a name of movie and delete the worst vote\n");
	printf("(0) - End the program\n");
}

void main()
{
	int size = countLines(MOVIEFILE);
	movie* movieArray = malloc(size * sizeof(movie));
	if (movieArray == NULL)
	{
		printf("Error:memory allocation for movieArray\n");
	}
	FromFile2Movies(MOVIEFILE, &movieArray, size);
	int sizevotes = countLines(VOTEFILE);
	FromFile2votes(VOTEFILE, &movieArray, sizevotes);
	
	int activate = 1, choice; 
	while (activate)
	{
		Printmenu();
		printf("Please enter your choice:\n");
		if (scanf("%d", &choice) != 1 || choice < 0 || choice > 10)
		{
			printf("ERROR: please enter a number between (0-9):\n");
			while (getchar() != '\n');
			continue;
		}
		switch (choice)
		{
		case 1:
		{
			int answer;
			answer = addMovie(&movieArray, &size);
			if (answer == 1)
			{
				printf("The movie was successfully added\n");
			}
			else
			{
				printf("Something failed, please try again\n");
			}
			break;
		}
		case 2:
		{
			printf("please enter an id movie:\n");
			int TempIdmovie = 0;
			scanf("%d", &TempIdmovie);
			int answer2 = 0;
			answer2 = addVote(TempIdmovie, &movieArray, &size);
			if (answer2 == 1)
			{
				printf("The movie was successfully added\n");
			}
			else
			{
				printf("Something failed, please try again\n");
			}
			break;
		}
		case 3://מקבלת סרט ומדפיסה את כל ההערות והמדינות של הסרט
		{
			printf("please enter a movie name:\n");
			char* MovieN[N] = { 0 };
			int answer3 = 0;
			scanf("%s", MovieN);
			answer3 = printVotes(MovieN, &movieArray, size);
			break;
		}
		case 4://מקבלת סוג סרט ומדפיסה את כל הסרטים מאותו סוג
		{
			printf("please enter a genre of movie:\n");
			char* GenreN[N] = { 0 };
			scanf("%s", GenreN);
			countGenre(GenreN, &movieArray, size);
			break;
		}
		case 5://מקבלת ציון ומדינה ומדפיסה את כל הסרטים שקיבלו ערך זה ממדינה זו
		{
			printf("please enter a value and a country\n");
			char country[N];
			int Value;
			scanf("%d %s", &Value, country);
			printValue(Value, country, &movieArray, size);
			break;
		}
		case 6:// מקבלת שנה ומדפיסה את מספר המדינות השונות שהצביעו עבור סרט מהשנה שהתקבלה
		{
			printf("please enter The year the film was released:\n");
			int Year = 0;
			scanf("%d", &Year);
			countCountry(Year, &movieArray, size);
			break;
		}
		case 7://מדפיסה את שם המדינה שצופים שלה כתבו הכי הערות
		{
			maxBycountry(&movieArray, size);
			break;
		}
		case 8://מקבלת ערך הצבעה ויוצרת קובץ בו נרשמים כל הסרטים שממוצעם גבוה יותר מהערך
		{
			printf("please enter the value you rate the movie:\n");
			double X;
			scanf("%lf", &X);
			RecommendMovie(X, &movieArray, size);
			break;
		}
		case 9://מקבלת סוג סרט ומוחקת את ההצבעה בעלת הערך הנמוך ביותר עבור סרט זה
		{
			printf("Please enter a gernre of movie\n");
			char Genre[20] = { 0 };
			scanf("%s", Genre);
			DeleteWorst(Genre, &movieArray, size);
			break;
		}
		case 0:
		{
			printf("Good bye\n");
			activate = 0;
			break;
		}
		default:
		{
			printf("ERROR: please enter a number between (0-9):\n");
		}
		}
	}
	UpdateMovieFile(&movieArray, size);// עדכון קובץ הסרטים
	UpdateVotesFile(&movieArray, size);//עדכון קובץ ההצבעות
	size = countLines(MOVIEFILE);// מעדכן את המשתנה ששומר את כמות הסרטים
	sizevotes = countLines(VOTEFILE); //מעדכן את המשתנה ששומר את כמות ההצבעות
	FreeMovieArray(movieArray, size);// שחרור כל הזיכרון הדינאמי
}