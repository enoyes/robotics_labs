#include <stdio.h>
#include <stdlib.h>

double *v;
int count;
int size;

void print_menu(){
	printf("Main menu:\n\n");
	printf("1. Print the array\n");
	printf("2. Append element at the end\n");
	printf("3. Remove last element\n");
	printf("4. Insert one element\n");
	printf("5. Exit\n\n");
	printf("Select an option: \n");
}

void Initialize(){
	count = 0;
	size = 2;
	v = malloc(size * sizeof(double));
}

void Finalize(){
	free(v);
}

int main(){

	int x;
	while(1){
		print_menu();
		scanf("%d", &x);
		switch(x){
			case 1:
				printf("You selected \"Print the array\"\n\n");
				break;
			case 2:
				printf("You selected \"Append element at the end\"\n\n");
				break;
			case 3:
				printf("You selected \"Remove last element\"\n\n");
				break;
			case 4:
				printf("You selected \"Insert one element\"\n\n");
				break;
			case 5:
				printf("Goodbye!\n\n");
				Finalize();
				exit(0);
			default:
				printf("Invalid Option!\n\n");
		}
	}


}
