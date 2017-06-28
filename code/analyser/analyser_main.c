#include <linux/bio.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kprobes.h>
#include <linux/blkdev.h>
#include <linux/sched.h>
#include <linux/init.h> 
#include <linux/mount.h>
#include <linux/path.h>
#include <linux/namei.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/pagemap.h>
#include <linux/blk_types.h>
#include <linux/mm_types.h>
#include<linux/cgroup-defs.h> 
#include <linux/timer.h>
#include <linux/list.h>
#include <linux/slab.h>
#include "parda.h"


//list operations
struct io_access {
	int address;
	struct list_head next;
};
struct my_lookup_block {
        struct rb_node node;
        int dbn;
        int val;
	void *ptr;

};
#define NUM_CONTAINERS 4
int count[NUM_CONTAINERS];
struct io_access container_access_list[NUM_CONTAINERS];
struct rb_root lookup_tree[NUM_CONTAINERS];
int nbuckets  = DEFAULT_NBUCKETS;
void parda_input_with_filename(struct io_access *list, program_data_t* pdt, long begin, long end, int id);
void process_one_access(int block, program_data_t* pdt, const long tim, int id);
int classical_tree_based_stackdist(struct io_access *list, long count, int id);

extern int (* flashcache_change_container_size_ptr) (int, int);
extern void (*inst_generic_make_request_ptr) (struct bio *bio);
/*******************timer functions ************************/
struct timer_list my_timer;
/**********************************************************/

/**********************************************************/
/*********************Rb tree functions********************/
/*********************************************************/

/*rb tree insert function : assumes the new node has dnb and cbn already set*/

void my_rb_insert(struct rb_root *root, struct my_lookup_block *obj) {
        struct rb_node **new, *parent=NULL;
        int value = obj->dbn;
        struct my_lookup_block *this;
        new= &(root->rb_node);
	
        while (*new)
        {
		
            
            this = container_of(*new, struct my_lookup_block, node);
            parent=*new;
		//DPRINTK("inside while:%lu \n",this->dbn);
            if (this->dbn > value)
                new = &((*new)->rb_left);
            else
                new = &((*new)->rb_right);
        }
	//DPRINTK("after loop\n");
       
        rb_link_node(&obj->node, parent, new);
        rb_insert_color(&obj->node, root);
      //  DPRINTK("end of insert\n");
}

struct my_lookup_block *my_rb_search(struct rb_root *root, sector_t dbn) {
	struct rb_node *node = root->rb_node;  
        while (node) {
            struct my_lookup_block *my_node = container_of(node, struct my_lookup_block, node);
            if (my_node->dbn > dbn)
                node = node->rb_left;
            else if (my_node->dbn < dbn)
                node = node->rb_right;
            else
                return my_node;  
        }
        return NULL;
}

//void rb_erase(struct rb_node *victim, struct rb_root *tree);

//PARDA functions
program_data_t parda_init(int x) {
	
	  program_data_t pdt;
	  lookup_tree[x] = RB_ROOT;
	  narray_t* ga = narray_new(sizeof(HKEY), 1000);
	  Tree* root;
	  unsigned int *histogram;
	  printk("parda initialising .........\n");
	  histogram = kmalloc(sizeof(unsigned int) * (nbuckets+2), GFP_KERNEL);
	  root = NULL;
	  memset(histogram, 0, (nbuckets + 2) * sizeof(unsigned int));
	  pdt.ga = ga;
	  //pdt.gh = gh;
	  pdt.root = root;
	  pdt.histogram = histogram;
	  return pdt;
}
void parda_input_with_filename(struct io_access *list, program_data_t* pdt, long begin, long end, int id) {
	long i=0;

	struct io_access *tmp;
	struct list_head *pos, *q;
	
	list_for_each_safe(pos, q, &list->next) {
		 tmp = list_entry(pos, struct io_access, next);
		 if(tmp != NULL) {
			process_one_access(tmp->address, pdt, i, id);
		 }
		 i++;
     }
}


void process_one_access(int block, program_data_t* pdt, const long tim, int id) {
		int distance;
		int lookup;
		struct my_lookup_block *result,*new;
		 result = my_rb_search(&lookup_tree[id], block);
		 
		// Cold start: Not in the list yet
		if (result == NULL) {
				pdt->root=insert(tim, pdt->root);
        			narray_append_val(pdt->ga, block);
				new = (struct my_lookup_block*) kmalloc(sizeof(struct my_lookup_block), GFP_KERNEL);
				new->dbn = block;
				new->val = tim;
				my_rb_insert(&(lookup_tree[id]), new);
		}
		// Hit: We've seen this data before
		else {
				lookup = result->val;
				pdt->root = insert(lookup, pdt->root);
				distance = node_size(pdt->root->right);
				pdt->root = delete(lookup, pdt->root);
				pdt->root = insert(tim, pdt->root);
				rb_erase(&result->node, &lookup_tree[id]);
				result->dbn = block;
				result-> val = tim;
				my_rb_insert(&(lookup_tree[id]), result);
				if (distance > nbuckets)
						pdt->histogram[B_OVFL] += 1;
				else
						pdt->histogram[distance] += 1;
		}
}
int classical_tree_based_stackdist(struct io_access *list, long count, int id) {
	program_data_t pdt_c = parda_init(id);
	parda_input_with_filename(list, &pdt_c, 0, count - 1, id);
	program_data_t* pdt = &pdt_c;
	printk("classical tree based stack list\n");
	pdt->histogram[B_INF] += narray_get_len(pdt->ga);
	return parda_print_histogram(pdt->histogram);
}

int parda_print_histogram(const unsigned* histogram) {
	int last_bucket;
	int i;
	unsigned long long sum = 0;  // For normalizing
	unsigned long long cum = 0;  // Cumulative output

	// Find the last non-zero bucket
	last_bucket = nbuckets-1;
	while (histogram[last_bucket] == 0)
		last_bucket--;
	//return last_bucket;
	for (i = 0; i <= last_bucket; i++)
		sum += histogram[i];
		
	sum += histogram[B_OVFL];
	sum += histogram[B_INF];

	for (i = 0; i <= last_bucket; i++) {
		cum += histogram[i];
		if (histogram[i]) 
			printk("VAL %6d\t%9llu ,,  ", i, cum);
	}
	return i;
}

int list_init (struct io_access *list) {
 	printk("initialising the list\n");
 	INIT_LIST_HEAD(&(list->next));
 	printk("done init of list\n");
 	return 1;
 }
 
int list_insert ( struct  io_access *list, struct io_access *block) {
 	list_add_tail(&(block->next), &(list->next));
 	return 1;
}

int get_containerID(struct bio *bio)
{
        int retval = -EINVAL;
        struct cgroup *cgp;
        const char *name;
      
        if(bio == NULL) {
                goto out;
        }
        bio_associate_current(bio);
        
        if(!bio->bi_css)
                 goto out;

        cgp = bio->bi_css->cgroup;
        if(!cgp)
                 goto out;
                
         name = cgp->kn->name;
         if(strcmp(name,"test_container5")==0)
                                retval = 3;
          else if(strcmp(name,"test_container2")==0)
          			retval = 2;
          else if(strcmp(name,"test_container1")==0)
          			retval = 1;
          else if(strcmp(name,"test_container")==0)
                                retval = 0;
         else
         			retval = 0;
  out:      
        return retval;
}

void inst_generic_make_request(struct bio *bio) {
	struct io_access *refered_address;
	int x;
	sector_t block;
	if(bio == NULL) return;
	x = get_containerID(bio);
	if(x < 0) return;
	block = bio->bi_iter.bi_sector;
	refered_address = (struct io_access*) kmalloc(sizeof(struct io_access), GFP_KERNEL);
	refered_address->address = block;
	list_insert(&container_access_list[x], refered_address);
	count[x]++;
}

void reset(void) {
	int num = NUM_CONTAINERS, i;
	int size=0;
	for(i=0; i<num; i++) {
			size = classical_tree_based_stackdist(&container_access_list[i], count[i]-1, i);
			printk("Container:%d -> RESULT:%d\n",i, size);
			if(flashcache_change_container_size_ptr != NULL)
				(*flashcache_change_container_size_ptr) (i, size);
	}	
	my_timer.expires = jiffies + 120* HZ;
	add_timer (&my_timer);
}

static int __init my_init(void) {
	int delay = 120;
	int i;
	printk("Module inserted \n");
	for(i=0; i<NUM_CONTAINERS; i++) {
		list_init(&container_access_list[i]);
	}
	inst_generic_make_request_ptr = &inst_generic_make_request;
	my_timer.expires = jiffies + delay * HZ;
        my_timer.function = reset;
        my_timer.data = 0;
        add_timer(&my_timer);
        return 0;
}

static void __exit my_exit(void) {
	printk("module removed\n");
	del_timer(&my_timer); 
	inst_generic_make_request_ptr = NULL;
	
}
MODULE_LICENSE("GPL");                      
module_init(my_init);
module_exit(my_exit);

