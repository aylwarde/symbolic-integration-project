#include "../squarefree.h"


int main()
{

	FILE *polyfile;
	
	polyfile = fopen("book_examples/squarefree.txt", "r");
	
	if(polyfile ==NULL) {
		printf("Invalid File Path\n");
		exit(1);
	}
	
	int polys, i=0;	
	poly **squarefree;
	
	poly **input = from_file_p(polyfile, &polys);
	fclose(polyfile);
	

	squarefree = squarefree_p(input[0]);

	while(squarefree[i] != NULL) {
		display_p(squarefree[i]);
		++i;
	}

	i=0; 

	while(squarefree[i] != NULL) {
		free_p(squarefree[i]);
		++i;
	}

	free(squarefree);
	free_p(input[0]);
	free(input);

	
	


return 0;
}
