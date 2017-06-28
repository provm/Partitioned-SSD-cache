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
#include <linux/reboot.h>
#include <linux/delay.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/vmalloc.h>
#include "narray.h"

narray_t* narray_new(unsigned element_size, unsigned capacity) {
    narray_t* na = (narray_t*)kmalloc(sizeof(narray_t), GFP_KERNEL);
    na->element_size = element_size;
    na->len = 0;
    na->capacity = capacity * element_size;
    na->data = kmalloc(na->capacity, GFP_KERNEL);
    return na;
}

void narray_append_val(narray_t* na, const void* value) {
   if(na->len == na->capacity) {
       unsigned new_capacity = na->capacity + na->capacity + 10 * na->element_size;
       void* ndata = kmalloc(new_capacity, GFP_KERNEL);
       //memcpy(ndata, na->data, na->len);
       
       kfree(na->data);
       na->data = ndata;
       na->capacity = new_capacity;
   }
   //memcpy(na->data + na->len, value, na->element_size);
	 na->len += na->element_size;
}

void narray_free(narray_t* na) {
		kfree(na->data);
		kfree(na);
}


narray_t* narray_heaparray_new(void* data, const unsigned len, const unsigned element_size) {
    narray_t* na = (narray_t*)kmalloc(sizeof(narray_t), GFP_KERNEL);
    na->data = data;
    na->len = len;
    na->capacity = len;
    na->element_size = element_size;
    return na;
}
