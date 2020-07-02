#include "../squarefree.h"

//testing squarefree example 1.7.1 in chap 1 of bronstein book, with terminal output 
int main()
{

	FILE *polyfile;
	
	polyfile = fopen("book_examples/squarefree.txt", "r");
	
	if(polyfile ==NULL) {
		printf("Invalid File Path\n");
		exit(1);
	}
	
	int polys, i, len;	
	poly **squarefree;
	
	poly **input = from_file_p(polyfile, &polys);
	fclose(polyfile);
	

	squarefree = squarefree_p(input[0], &len);

	for(i=0; i<=len; ++i) {
		print_p(squarefree[i]);
	}	

	for(i=0; i<=len; ++i) {
		free_p(squarefree[i]);
	}

	free(squarefree);
	free_p(input[0]);
	free(input);

	
	


return 0;
}
