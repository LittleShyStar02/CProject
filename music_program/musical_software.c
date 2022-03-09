#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#pragma comment (lib , "winmm.lib")

#define DEFAULT_STR_LEN 32
#define EXIT_SELECTION 53
#define TRUE 1
#define FALSE 0

void appendNotes();
void bFlush();
int charToInt(char);
void clear();
int confirm(char*);
int execute(int);
void fileCreation();
int fileExists(char*);
int getFrequence(char*);
int getKey();
char* getNMFolder();
char* getWMFolder();
int getScale(char*);
int getSemitone(char*);
int getSemitoneInScale(char*);
char kcToChar(int);
void mkdirs(char*);
void pause();
void playFile();
void playWav();
char readChar();
int readInt();
char* readString();
char* readStringLen(int);
char* removeScale(char*);
char* toLowerCase(char*);
char* toPath(char*,char*);
void* xmalloc(size_t);

int main(void)
{	
	int op;
	mkdirs(getNMFolder());
	mkdirs(getWMFolder());
	do{
		clear();
		puts("\n  Premi:");
		puts("    1. per creare un file musicale");
		puts("    2. per aggiungere le note a un file musicale esistente");
		puts("    3. per riprodurre un file musicale");
		puts("    4. per riprodurre un file .wav");
		puts("    5. per uscire dal programma");
		printf("\n  Cosa vuoi fare? ");
		
		op = execute(getKey());
		puts("");
		
		if(op == FALSE)
		{
			puts("\n  Opzione non valida. Riprova");
		}
		if(op != EXIT_SELECTION) pause();
	}while(op != EXIT_SELECTION);
	return 0;
}

/*
  Add a notes in a musical file
*/
void appendNotes()
{
	FILE *file;
	char *path;
	char *var;
	int dur;
	
	puts("\n  Come si chiama il file a cui vuoi aggiungere le note? (senza estensione finale .etc)");
	puts("  Se hai sbagliato a premere, digita \'exit\'");
	
	do{

		printf("\n  Nome del file: ");
		
		var = readString();
		
		if(!strcmp(toLowerCase(var),"exit")) return;
		
		path = toPath(getNMFolder(),strcat(var,".txt"));
		
		if(!fileExists(path)){
			puts("\n File non trovato");	
		}
	}while(!fileExists(path));
	
	file = fopen(path,"a");
	
	clear();
	puts("\n  Digita \'exit\' quando vuoi terminare l'aggiunta");
	puts("  Scrivi \'pause\' quando vuoi inserire una pausa");
	puts("  Potrai cambiare le durate nel file stesso");
	
	while(strcmp(var,"exit"))
	{
		printf("\n  Inserisci la nota da aggiungere: ");
		var = readString();
		if(!strcmp(toLowerCase(var),"exit")) break;
		fprintf(file,"\n");
		if(!strcmp(var,"pause"))
		{
			fprintf(file,"%s %d",var,250);
			continue;
		}
		if(getSemitoneInScale(var) == -1)
		{
			puts("\n  Nota non valida. Riprova.");
			continue;
		}
		fprintf(file,"%s %d",var,150);
	}
	
	fflush(file);
	fclose(file);
}

/*
  Clean a buffer
*/
void bFlush()
{
	fseek(stdin,0L,SEEK_END);
}

/*
  Convert a char number
  in int
  
  @param ch
*/
int charToInt(char ch)
{
	if(ch == '0') return 0;
	if(ch == '1') return 1;
	if(ch == '2') return 2;
	if(ch == '3') return 3;
	if(ch == '4') return 4;
	if(ch == '5') return 5;
	if(ch == '6') return 6;
	if(ch == '7') return 7;
	if(ch == '8') return 8;
	if(ch == '9') return 9;
	return -1;
}

/*
  Clean a promt
*/
void clear()
{
	system("cls");
}

/*
  Request the confirm.
  Message from parameter
  
  @param msg
*/
int confirm(char *msg)
{
	char ch;
	do{
		printf(msg);
		ch = kcToChar(getKey());
		if(ch != 'y' && ch != 'n')
		{
			puts("\n  Opzione non valida. Riprova.");
		}
	}while(tolower(ch) != 'y' && tolower(ch) != 'n');
	if(tolower(ch) == 'y') return TRUE;
	return FALSE;
}

/*
  Execute an action.
  This depend from parameter value.
  Return 1 if find a valid actions
  Return 0 if don't find a valid action
  Return EXIT_SELECTION if value is EXIT_SELECTION
  
  @param value
*/
int execute(int value)
{
	if(value >= 49 && value <= 52) clear();
	switch(value)
	{
		case 49:
			fileCreation();
			break;
		case 50:
			appendNotes();
			break;
		case 51:
			playFile();
			break;
		case 52:
			playWav();
			break;
		case EXIT_SELECTION:
			return EXIT_SELECTION;
		default:
			return FALSE;
	}
	return TRUE;
}

/*
  Create a new file
  if exists will be overwrite
  if confirmation is positive
*/
void fileCreation()
{
	char *path;
	char *name;
	int ow_check;
	
	ow_check = -1;
	
	puts("\n  Come si chiama il file da creare? (senza estensione finale .etc)");
	puts("  Se hai sbagliato a premere, digita \'exit\'");
	printf("\n  Nome del file: ");
	
	//Other
	name = readString();
	
	if(!strcmp(toLowerCase(name),"exit")) return;
	
	path = toPath(getNMFolder(),strcat(name,".txt"));
	
	if(fileExists(path)){
		ow_check = confirm("\n  Esiste un file con lo stesso nome vuoi sovrascriverlo? (y/n)  ");
		puts("");	
	}
	
	if(ow_check == -1 || ow_check)
	{
		FILE *file;
		file = fopen(path,"w");
		fflush(file);
		fclose(file);
	}
	
	free(path);
}

/*
  Verify if file exist by path
  
  @param path
*/
int fileExists(char *path)
{
	int check;
	FILE *file;
	file = fopen(path,"r");
	check = (file != NULL);
	fflush(file);
	fclose(file);
	return check;
}

/*
  Return note frequence
  
  @param note
*/
int getFrequence(char *note)
{
	return 66 * pow(1.05946,getSemitone(note)-1);
}

/*
  Return the code of
  character pressed
  from keybord
*/
int getKey()
{
	int key;
	key = getch();
	if(key == 0 || key == 224) key = getch();
	return key;
}

/*
  Return folder's name
  with note's file
*/
char* getNMFolder()
{
	return "note_music";
}

/*
  Return a folder's name
  with .wav files
*/
char* getWMFolder()
{
	return "wav_music";
}

/*
  Return a scale of note
*/
int getScale(char *note)
{
	if(isdigit(note[2])) return charToInt(note[2]);
	return charToInt(note[3]);
}

/*
  Return a semitone of note
  
  @param note
*/
int getSemitone(char *note)
{
	return ((getScale(note)-2)*12) + getSemitoneInScale(note);
}

/*
  Return a note semitone
  in single scale
  
  @param note
*/
int getSemitoneInScale(char *note)
{
	note = removeScale(note);
	if(!strcmp(note,"DO")) return 1;
	if(!strcmp(note,"DO#") || !strcmp(note,"REb")) return 2;
	if(!strcmp(note,"RE")) return 3;
	if(!strcmp(note,"RE#") || !strcmp(note,"MIb")) return 4;
	if(!strcmp(note,"MI")) return 5;
	if(!strcmp(note,"FA")) return 6;
	if(!strcmp(note,"FA#") || !strcmp(note,"SOLb")) return 7;
	if(!strcmp(note,"SOL")) return 8;
	if(!strcmp(note,"SOL#") || !strcmp(note,"LAb")) return 9;
	if(!strcmp(note,"LA")) return 10;
	if(!strcmp(note,"LA#") || !strcmp(note,"SIb")) return 11;
	if(!strcmp(note,"SI")) return 12;
	return -1;
}

/*
  Return a char of
  key code in parameter
  
  @param code
*/
char kcToChar(int code)
{
	return (char)code;
}

/*
  Create a folder
  
  @param path
*/
void mkdirs(char* path)
{
	char *cmd;
	cmd = xmalloc(7);
	strcpy(cmd,"mkdir ");
	strcat(cmd,path);
	system(cmd);
	free(cmd);
}

/*
  Imitation of system("pause")
*/
void pause()
{
	puts("\n  Premi un tasto per continuare...");
	getch();
}

/*
  Play a file with note
*/
void playFile()
{
	FILE *file;
	char *path;
	char *name;
	char *note;
	int frequence,ms;
	
	note = xmalloc(6);
	
	puts("\n  Come si chiama il file da leggere? (senza estensione finale .etc)");
	puts("  Se hai sbagliato a premere, digita \'exit\'");
	do{
		printf("\n  Nome del file: ");
		
		name = readString();
		
		if(!strcmp(toLowerCase(name),"exit")) return;
		
		path = toPath(getNMFolder(),strcat(name,".txt"));
		
		if(!fileExists(path)){
			puts("\n File non trovato");	
		}
	}while(!fileExists(path));
	
	file = fopen(path,"r");
	
	while(!feof(file))
	{
		fscanf(file,"%s %d",note,&ms);
		if(strcmp(note,"pause"))
		{
			frequence = getFrequence(note);
			Beep(frequence,ms);
		}
		if(ms < 125) ms = 125;
		ms -= 25;
		Sleep(ms);
	}
	free(path);
	fclose(file);
	free(name);
}

/*
  Play a wav file
*/
void playWav()
{
	char *path;
	char *name;
	int check;
	
	do
	{
		clear();
		puts("\n  Inserisci il nome del file da ascoltare? (senza estensione finale .etc)");
		puts("  Se hai sbagliato a premere, digita \'exit\'");
		do{
			printf("\n  Nome del file: ");
		
			name = readString();
		
			if(!strcmp(toLowerCase(name),"exit")) return;
		
			path = toPath(getWMFolder(),strcat(name,".wav"));
		
			if(!fileExists(path)){
				puts("\n File non trovato");	
			}
		}while(!fileExists(path));
		
		PlaySound(path,NULL,SND_FILENAME | SND_SYNC);
		
	}while(confirm("\n  Vuoi ascoltare un altro brano? (y/n) "));
	free(path);
}

/*
  Read a char 
  from keyboard
*/
char readChar()
{
	char ch;
	scanf("%c",&ch);
	bFlush();
	return ch;
}

/*
  Read an integer
  from keyboard
*/
int readInt()
{
	int i;
	scanf("%d",&i);
	bFlush();
	return i;
}

/*
  Read a string from
  keyboard. This 
  string have a max lenght
  of DEFAULT_STR_LEN that
  it is 32.
*/
char* readString()
{
	return readStringLen(DEFAULT_STR_LEN);
}

/*
  Read a string from
  keyboard of max length
  in parameter
  
  @param lenght
*/
char* readStringLen(int lenght)
{
	char *str = xmalloc(lenght+1);
	fgets(str,lenght+1,stdin);
	if(str[strlen(str)-1] == '\n')
	{
		str[strlen(str)-1] = '\0';
	}
	bFlush();
	return str;
}

/*
  Remove scale from note
  
  @param note
*/
char* removeScale(char *note)
{
	int x;
	char *ws;
	ws = xmalloc(strlen(note));
	strcpy(ws,note);
	for(x = 0;x < strlen(ws);x++)
	{
		if(isdigit(ws[x]))
		{
			if(strlen(ws) > (x+1))
			{
				ws[x] = ws[x+1];
				ws[x+1] = '\0';
				break;
			}
			ws[x] = '\0';
		}
	}
	return ws;
}

/*
  Convert all chars of string
  in lower character
  
  @param str
*/
char* toLowerCase(char *str)
{
	int x;
	char *lower;
	lower = xmalloc(strlen(str));
	strcpy(lower,str);
	for(x=0;x < strlen(lower);x++)
	{
		lower[x] = tolower(lower[x]);
	}
	return lower;
}

/*
  Transform location file
  and name file in a path
  location/name
  
  The location can be a folder,
  or a complete path tree, 
  or relative location "../folder"
  
  @param location
  @param name
*/
char* toPath(char *location,char *name)
{
	char *path;
	path = xmalloc(strlen(location));
	strcpy(path,location);
	strcat(path,"/");
	strcat(path,name);
	return path;
}

/*
  Function fork of malloc
  
  @param bytes
*/
void* xmalloc(size_t bytes)
{
	void *p;
	p = malloc(bytes);
	if(p == NULL) exit(-1);
	return p;
}
