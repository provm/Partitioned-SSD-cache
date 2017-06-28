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
#include "splay.h"

Tree * splay (T i, Tree *t)
/* Splay using the key i (which may or may not be in the tree.) */
/* The starting root is t, and the tree used is defined by rat  */
/* size fields are maintained */
{
    Tree N, *l, *r, *y;
    T comp,l_size, r_size;
    if (t == NULL) return t;
    N.left = N.right = NULL;
    l = r = &N;
    l_size = r_size = 0;
 
    for (;;) {
        comp = compare(i, t->key);
        if (comp < 0) {
            if (t->left == NULL) break;
            if (compare(i, t->left->key) < 0) {
                y = t->left;                           /* rotate right */
                t->left = y->right;
                y->right = t;
                t->size = node_size(t->left) + node_size(t->right) + 1;
                t = y;
                if (t->left == NULL) break;
            }
            r->left = t;                               /* link right */
            r = t;
            t = t->left;
            r_size += 1+node_size(r->right);
        } else if (comp > 0) {
            if (t->right == NULL) break;
            if (compare(i, t->right->key) > 0) {
                y = t->right;                          /* rotate left */
                t->right = y->left;
                y->left = t;
		t->size = node_size(t->left) + node_size(t->right) + 1;
                t = y;
                if (t->right == NULL) break;
            }
            l->right = t;                              /* link left */
            l = t;
            t = t->right;
            l_size += 1+node_size(l->left);
        } else {
            break;
        }
    }
    l_size += node_size(t->left);  /* Now l_size and r_size are the sizes of */
    r_size += node_size(t->right); /* the left and right trees we just built.*/
    t->size = l_size + r_size + 1;

    l->right = r->left = NULL;

    /* The following two loops correct the size fields of the right path  */
    /* from the left child of the root and the right path from the left   */
    /* child of the root.                                                 */
    for (y = N.right; y != NULL; y = y->right) {
        y->size = l_size;
        l_size -= 1+node_size(y->left);
    }
    for (y = N.left; y != NULL; y = y->left) {
        y->size = r_size;
        r_size -= 1+node_size(y->right);
    }
 
    l->right = t->left;                                /* assemble */
    r->left = t->right;
    t->left = N.right;
    t->right = N.left;

    return t;
}

Tree * insert(T i, Tree * t) {
/* Insert key i into the tree t, if it is not already there. */
/* Return a pointer to the resulting tree.                   */
    Tree * new;

    if (t != NULL) {
	t = splay(i,t);
	if (compare(i, t->key)==0) {
	    return t;  /* it's already there */
	}
    }
    new = (Tree *) kmalloc (sizeof (Tree), GFP_KERNEL);
    if (new == NULL) {printk("Ran out of space\n"); }
    if (t == NULL) {
	new->left = new->right = NULL;
    } else if (compare(i, t->key) < 0) {
	new->left = t->left;
	new->right = t;
	t->left = NULL;
	t->size = 1+node_size(t->right);
    } else {
	new->right = t->right;
	new->left = t;
	t->right = NULL;
	t->size = 1+node_size(t->left);
    }
    new->key = i;
    new->size = 1 + node_size(new->left) + node_size(new->right);
    return new;
}

Tree * delete(T i, Tree *t) {
/* Deletes i from the tree if it's there.               */
/* Return a pointer to the resulting tree.              */
    Tree * x;
    T tsize;

    if (t==NULL) return NULL;
    tsize = t->size;
    t = splay(i,t);
    if (compare(i, t->key) == 0) {               /* found it */
	if (t->left == NULL) {
	    x = t->right;
	} else {
	    x = splay(i, t->left);
	    x->right = t->right;
	}
	kfree(t);
	if (x != NULL) {
	    x->size = tsize-1;
	}
	return x;
    } else {
	return t;                         /* It wasn't there */
    }
}

Tree *find_rank(T r, Tree *t) {
/* Returns a pointer to the node in the tree with the given rank.  */
/* Returns NULL if there is no such node.                          */
/* Does not change the tree.  To guarantee logarithmic behavior,   */
/* the node found here should be splayed to the root.              */
    T lsize;
    if ((r < 0) || (r >= node_size(t))) return NULL;
    for (;;) {
	lsize = node_size(t->left);
	if (r < lsize) {
	    t = t->left;
	} else if (r > lsize) {
	    r = r - lsize -1;
	    t = t->right;
	} else {
	    return t;
	}
    }
}
void freetree(Tree* t)
{
    if(t==NULL) return;
    freetree(t->right);
    freetree(t->left);
    kfree(t);
}
void printtree(Tree * t, int d) {
    //printf("%p\n",t);
    int i;
    if (t == NULL) return;
    printtree(t->right, d+1);
    for (i=0; i<d; i++) printk("  ");
    printk("%d(%d)\n", t->key, t->size);
    printtree(t->left, d+1);
}


