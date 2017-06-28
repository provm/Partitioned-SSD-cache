#ifndef _NARRAY_H
#define _NARRAY_H

typedef struct narray_s {
    void* data;
    unsigned len, capacity, element_size;
}narray_t;

narray_t* narray_heaparray_new(void* data, unsigned len, unsigned element_size);
narray_t* narray_new(unsigned element_size, unsigned capacity);
void narray_append_val(narray_t* na, const void* value);
void narray_free(narray_t* na);



static inline unsigned narray_get_len(const narray_t* na) {
    return na->len/na->element_size;
}

#endif
