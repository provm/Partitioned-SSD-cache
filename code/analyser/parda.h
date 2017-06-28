//#define DEFAULT_NBUCKETS 100000
#define DEFAULT_NBUCKETS 1000000
#define B_OVFL   nbuckets
#define B_INF    nbuckets+1
#define SLEN 20
#include "narray.h"
#include"splay.h"
/* Tunable parameters */
extern int nbuckets;


/*data structure for parda*/
typedef char HKEY[SLEN];

typedef struct end_keytime_s {
		narray_t* gkeys;
		narray_t* gtimes;
}end_keytime_t;

typedef struct processor_info_s {
	int pid,psize;
	long tstart,tlen,tend,sum;
}processor_info_t;

typedef struct program_data_s {
	//GHashTable* gh;
	narray_t* ga;
	end_keytime_t ekt;
	Tree* root;
	unsigned int *histogram;
}program_data_t;

//void classical_tree_based_stackdist(struct io_access *block, long tim);
/*functions for parda core*/
program_data_t parda_init( int);
//void parda_input_with_filename(struct io_access *list, program_data_t* pdt, long begin, long end);
void parda_free(program_data_t* pdt);
//void process_one_access(int block, program_data_t* pdt, const long tim);
/*functions for parda print*/
int parda_print_histogram(const unsigned* histogram);

