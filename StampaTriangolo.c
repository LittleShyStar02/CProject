#include <stdio.h>

void stampaTriangolo(int h);

int main(void){
	int h;
	printf("Inserisci l'altezza del triangolo: ");
	scanf("%d",&h);
	printf("\n\n");
	stampaTriangolo(h);
	return 0;
}

void stampaTriangolo(int h){
	int x,y;
	for(x = 1 ; x <= h ; x++){
		for(y = 1 ; y < (h+x) ; y++){
			if(y > (h-x)) printf("*");
			else printf(" ");
		}
		printf("\n");
	}
}
