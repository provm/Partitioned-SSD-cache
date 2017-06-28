#include <asm/atomic.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/blkdev.h>
#include <linux/bio.h>
#include <linux/slab.h>
#include <linux/hash.h>
#include <linux/spinlock.h>
#include <linux/workqueue.h>
#include <linux/pagemap.h>
#include <linux/random.h>
#include <linux/hardirq.h>
#include <linux/sysctl.h>
#include <linux/version.h>
#include <linux/sort.h>
#include <linux/time.h>
#include <asm/kmap_types.h>

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,26)
#include "dm.h"
#include "dm-io.h"
#include "dm-bio-list.h"
#include "kcopyd.h"
#else
#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,27)
#include "dm.h"
#endif
#include <linux/device-mapper.h>
#include <linux/bio.h>
#include <linux/dm-kcopyd.h>
#endif
#include "flashcache.h"

// fifo functions
 int fifo_init (struct container_eviction_list *list) {
 	printk("initialising the list\n");
 	INIT_LIST_HEAD(&(list->next));
 	printk("done init of list\n");
 	return 1;
 }
 
 int fifo_insert ( struct container_eviction_list *list, struct container_eviction_list *block) {
 	//printk("fifo insert 1\n");
 	list_add_tail(&(block->next), &(list->next));
 	//printk("insert2\n");
 	return 1;
 }
 
struct container_eviction_list * fifo_del (struct container_eviction_list *list) {
	struct list_head *pos,*q;
	struct container_eviction_list *refered_block=NULL;
	
	list_for_each_safe (pos, q, &(list->next)) {
                        	refered_block = list_entry(pos, struct container_eviction_list, next);
                        	//printk("\ndeleting. from FIFO: %lu\n",refered_block->cbn);
                        	list_del(pos); // deletes from the list but does not free the memory
                        	break;	
	 }
	return refered_block;
}

struct container_eviction_list *   fifo_get (struct container_eviction_list *list) {
	return NULL;
}

void fifo_update( struct container_eviction_list *list, struct container_eviction_list *block) {
// do nothing
}
// lru functions

int lru_init (struct container_eviction_list *list) {
 	printk("initialising the list\n");
 	INIT_LIST_HEAD(&(list->next));
 	printk("done init of list\n");
 	return 1;
 }
 
 int lru_insert ( struct container_eviction_list *list, struct container_eviction_list *block) {
 	//printk("lru insert 1\n");
 	list_add_tail(&(block->next), &(list->next));
 	//printk("insert2\n");
 	return 1;
 }
 
struct container_eviction_list * lru_del (struct container_eviction_list *list) {
	struct list_head *pos,*q;
	struct container_eviction_list *refered_block=NULL;
	
	list_for_each_safe (pos, q, &(list->next)) {
                        	refered_block = list_entry(pos, struct container_eviction_list, next);
                        	DPRINTK("deleting. from FIFO: %lu\n",refered_block->cbn);
                        	list_del(pos); // deletes from the list but does not free the memory
                        	break;	
	 }
	return refered_block;
}

struct container_eviction_list *   lru_get (struct container_eviction_list *list) {
	return NULL;
}

void lru_update( struct container_eviction_list *list, struct container_eviction_list *block) {
	// delete the block and insert it in first position
//	struct container_eviction_list 
	list_del(&(block->next));
	list_add(&(block->next), &(list->next));
}
