


void parda_input_with_filename(char* inFileName, program_data_t* pdt, long begin, long end) {
	FILE* fp;
	fp = fopen(inFileName, "r");
	parda_input_with_textfilepointer(fp, pdt, begin, end);
	fclose(fp);
}

void parda_input_with_textfilepointer(FILE* fp, program_data_t* pdt, long begin, long end) {
  HKEY input;
  long i;
  for(i = begin; i <= end; i++) {
    process_one_access(input, pdt, i);
  }
}


static inline void process_one_access(char* input, program_data_t* pdt, const long tim) {
		int distance;
		int *lookup;
		lookup = g_hash_table_lookup(pdt->gh, input);
    //printf("gh=%p process_one\n",pdt->gh);
		// Cold start: Not in the list yet
		if (lookup == NULL) {
				char *data = strdup(input);
				pdt->root=insert(tim,pdt->root);
				long *p_data;
        			narray_append_val(pdt->ga,input);
				if ( !(p_data = (long*)malloc(sizeof(long))) )
				{
						printf("no memory for p_data\n");assert(0);exit(-1);
				}
				*p_data = tim;
				g_hash_table_insert(pdt->gh, data, p_data);  // Store pointer to list element
		}
		// Hit: We've seen this data before
		else {
				char *data = strdup(input);
				pdt->root = insert((*lookup), pdt->root);
				distance = node_size(pdt->root->right);
				pdt->root = delete(*lookup, pdt->root);
				pdt->root = insert(tim, pdt->root);
				int *p_data;
				if ( !(p_data = (int*)malloc(sizeof(int)))) {
						printf("no memory for p_data\n");
           					 assert(0); exit(-1);
				}
				*p_data = tim;
				g_hash_table_replace(pdt->gh, data, p_data);
				// Is distance greater than the largest bucket
				if (distance > nbuckets)
						pdt->histogram[B_OVFL] += 1;
				else
						pdt->histogram[distance] += 1;
		}
}


void parda_print_histogram(const unsigned* histogram) {
  int last_bucket;
  int i;
  unsigned long long sum = 0;  // For normalizing
  unsigned long long cum = 0;  // Cumulative output

  // Find the last non-zero bucket
  last_bucket = nbuckets-1;
  while (histogram[last_bucket] == 0)
    last_bucket--;

  for (i = 0; i <= last_bucket; i++)
    sum += histogram[i];
  sum += histogram[B_OVFL];
  sum += histogram[B_INF];

  printf("# Dist\t     Refs\t   Refs(%%)\t  Cum_Ref\tCum_Ref(%%)\n");

  for (i = 0; i <= last_bucket; i++) {
    cum += histogram[i];
    if (histogram[i]) 
        printf("%6d\t%9u\t%0.8lf\t%9llu\t%0.8lf\n", i, histogram[i],
            histogram[i] / (double)sum, cum, cum / (double)sum);
  }

  cum += histogram[B_OVFL];
  printf("#OVFL \t%9u\t%0.8f\t%9llu\t%0.8lf\n", histogram[B_OVFL], histogram[B_OVFL]/(double)sum, cum, cum/(double)sum);
  cum += histogram[B_INF];
  printf("#INF  \t%9u\t%0.8f\t%9llu\t%0.8lf\n", histogram[B_INF], histogram[B_INF]/(double)sum, cum, cum/(double)sum);
  //printf("#INF  \t%9u\n", histogram[B_INF]);
}
