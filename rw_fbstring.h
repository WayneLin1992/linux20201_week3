#ifndef FBSTRING_H_INCLUDED
#define FBSTRING_H_INCLUDED
#define MAX_STR_LEN_BITS (54)
#define MAX_STR_LEN ((1UL << MAX_STR_LEN_BITS) - 1)

#define LARGE_STRING_LEN 256

//#include "intern.h"
typedef union {
    /* allow strings up to 15 bytes to stay on the stack
     * use the last byte as a null terminator and to store flags
     * much like fbstring:
     * https://github.com/facebook/folly/blob/master/folly/docs/FBString.md
     */
    //char data[16];

    struct {
        uint8_t filler[15],
            /* how many free bytes in this stack allocated string
             * same idea as fbstring
             */
          space_left : 4,
            /* if it is on heap, set to 1 */
            is_ptr : 1, is_large_string : 1, flag2 : 1, flag3 : 1;
    };

    /* heap allocated */
    struct {
        char *ptr;
        /* supports strings up to 2^MAX_STR_LEN_BITS - 1 bytes */
        size_t size : MAX_STR_LEN_BITS,
                      /* capacity is always a power of 2 (unsigned)-1 */
                      capacity : 6;
        /* the last 4 bits are important flags */
    };
} xs;

static inline bool xs_is_ptr(const xs *x) { return x->is_ptr; }

static inline bool xs_is_large_string(const xs *x)
{
    return x->is_large_string;
}

static inline size_t xs_size(const xs *x)
{
    return xs_is_ptr(x) ? x->size : 15 - x->space_left;
}

static inline char *xs_data(const xs *x)
{
    if (!xs_is_ptr(x))
        return (char *) x->filler;

    if (xs_is_large_string(x))
        return (char *) (x->ptr + 4);
    return (char *) x->ptr;
}

static inline size_t xs_capacity(const xs *x)
{
    return xs_is_ptr(x) ? ((size_t) 1 << x->capacity) - 1 : 15;
}

static inline void xs_set_refcnt(const xs *x, int val)
{
    *((int *) ((size_t) x->ptr)) = val;
}

static inline void xs_inc_refcnt(const xs *x)
{
    if (xs_is_large_string(x))
        ++(*(int *) ((size_t) x->ptr));
}

static inline int xs_dec_refcnt(const xs *x)
{
    if (!xs_is_large_string(x))
        return 0;
    return --(*(int *) ((size_t) x->ptr));
}

static inline int xs_get_refcnt(const xs *x)
{
    if (!xs_is_large_string(x))
        return 0;
    return *(int *) ((size_t) x->ptr);
}

#define xs_literal_empty() \
    (xs) { .space_left = 15 }

/* lowerbound (floor log2) */
static inline int ilog2(uint32_t n) { return 32-__builtin_clz(n) -1; }

static void xs_allocate_data(xs *x, size_t len, bool reallocate)
{
    /* Medium string */
    if (len < LARGE_STRING_LEN) {
        if(reallocate){
            char* tmp = x->ptr;
            free(tmp);
        }
        x->ptr = reallocate ? realloc(x->ptr, (size_t) 1 << x->capacity)
                            : malloc((size_t) 1 << x->capacity);
        return;
    }

    /* Large string */
    x->is_large_string = 1;

    /* The extra 4 bytes are used to store the reference count */
    if(reallocate){
        char* tmp = x->ptr;
        free(tmp);
    }
    x->ptr = reallocate ? realloc(x->ptr, (size_t)(1 << x->capacity) + 4)
                        : malloc((size_t)(1 << x->capacity) + 4);

    xs_set_refcnt(x, 1);
}

xs *xs_new(xs *x, const void *p)
{
    *x = xs_literal_empty();
    size_t len = strlen(p) + 1;
    if (len > 16) {
        x->capacity = ilog2(len) + 1;
        x->size = len - 1;
        x->is_ptr = true;
        xs_allocate_data(x, x->size, 0);
        memcpy(xs_data(x), p, len);
    } else {
        memcpy(x->filler, p, len);
        x->space_left = 15 - (len - 1);
        //x->size = len-1;
    }
    return x;
}

/* Memory leaks happen if the string is too long but it is still useful for
 * short strings.
 */
#define xs_tmp(x)                                                   \
    ((void) ((struct {                                              \
         _Static_assert(sizeof(x) <= MAX_STR_LEN, "it is too big"); \
         int dummy;                                                 \
     }){1}),                                                        \
     xs_new(&xs_literal_empty(), x))

/* grow up to specified size */
xs *xs_grow(xs *x, size_t len)
{
    char buf[16];

    if (len <= xs_capacity(x))
        return x;
    if (!xs_is_ptr(x))
        memcpy(buf, x->filler, 16);
    /* Backup first */
    if(len < LARGE_STRING_LEN){
        x->is_ptr = true;
        x->capacity = ilog2(len) + 1;
        if (xs_is_ptr(x)) {
            xs_allocate_data(x, len, 1);
        }
    }else{
        x->is_ptr = true;
        x->is_large_string = true;
        x->capacity = ilog2(len) + 1;
        xs_allocate_data(x, len, 1);
    }
    return x;
}

static inline xs *xs_newempty(xs *x)
{
    *x = xs_literal_empty();
    return x;
}

static inline xs *xs_free(xs *x)
{
    if (xs_is_ptr(x) && xs_dec_refcnt(x) <= 0)
        free(x->ptr);
    return xs_newempty(x);
}

static inline xs *xs_free_must(xs *x){
    if(xs_is_ptr(x))
        free(x->ptr);
    return xs_newempty(x);
}

static bool xs_cow_lazy_copy(xs *x, char **data)
{
    if (xs_get_refcnt(x) <= 1)
        return false;

    /* Lazy copy */
    xs_dec_refcnt(x);
    xs_allocate_data(x, x->size, 0);

    if (data) {
        memcpy(xs_data(x), *data, x->size);

        /* Update the newly allocated pointer */
        *data = xs_data(x);
    }
    return true;
}

xs *xs_concat(xs *string, const xs *prefix, const xs *suffix)
{
    size_t pres = xs_size(prefix), sufs = xs_size(suffix),
           size = xs_size(string), capacity = xs_capacity(string);

    char *pre = xs_data(prefix), *suf = xs_data(suffix),
         *data = xs_data(string);

    xs_cow_lazy_copy(string, &data);

    if (size + pres + sufs <= capacity) {
        memmove(data + pres, data, size);
        memcpy(data, pre, pres);
        memcpy(data + pres + size, suf, sufs + 1);

        if (xs_is_ptr(string))
            string->size = size + pres + sufs;
        else
            string->space_left = 15 - (size + pres + sufs);
    } else {
        xs tmps = xs_literal_empty();
        xs_grow(&tmps, size + pres + sufs);
        char *tmpdata = xs_data(&tmps);
        memcpy(tmpdata + pres, data, size);
        memcpy(tmpdata, pre, pres);
        memcpy(tmpdata + pres + size, suf, sufs + 1);
        xs_free(string);
        *string = tmps;
        string->size = size + pres + sufs;
    }
    return string;
}

xs *xs_trim(xs *x, const char *trimset)
{
    if (!trimset[0])
        return x;

    char *dataptr = xs_data(x), *orig = dataptr;

    if (xs_cow_lazy_copy(x, &dataptr))
        orig = dataptr;

    /* similar to strspn/strpbrk but it operates on binary data */
    uint8_t mask[32] = {0};

#define check_bit(byte) (mask[(uint8_t) byte / 8] & 1 << (uint8_t) byte % 8)
#define set_bit(byte) (mask[(uint8_t) byte / 8] |= 1 << (uint8_t) byte % 8)
    size_t i, slen = xs_size(x), trimlen = strlen(trimset);

    for (i = 0; i < trimlen; i++)
        set_bit(trimset[i]);
    for (i = 0; i < slen; i++)
        if (!check_bit(dataptr[i]))
            break;
    for (; slen > 0; slen--)
        if (!check_bit(dataptr[slen - 1]))
            break;
    dataptr += i;
    slen -= i;

    /* reserved space as a buffer on the heap.
     * Do not reallocate immediately. Instead, reuse it as possible.
     * Do not shrink to in place if < 16 bytes.
     */
    memmove(orig, dataptr, slen);
    /* do not dirty memory unless it is needed */
    if (orig[slen])
        orig[slen] = 0;

    if (xs_is_ptr(x))
        x->size = slen;
    else
        x->space_left = 15 - slen;
    return x;
#undef check_bit
#undef set_bit
}

static xs* xs_cow_copy_long(xs* src){
    xs* dest = malloc(sizeof(xs));
    if(!xs_is_ptr(src)){
         *dest =  *src;
    }else if(src->is_large_string){
        dest->capacity = src->capacity;
        dest->size = src->size;
        dest->is_large_string = src->is_large_string;
        xs_inc_refcnt(src);
        dest->ptr = xs_data(src);
//       xs_inc_refcnt(src);
//        xs_set_refcnt(dest, 1);
    }else{
        dest->capacity = src->capacity;
        dest->is_ptr = src->is_ptr;
        dest->size = src->size;
        size_t len = xs_size(src) + 1;
        xs_allocate_data(dest, len, 0);
        char *dest_data  = malloc(len*sizeof(char));
        strncpy(dest_data, xs_data(src), len);
        dest->ptr = dest_data;
    }
    return dest, src;
}



bool xs_cow_write_long(xs*src, char *data){
    if(!xs_is_large_string(src))
        return false;
    if(strlen(data) < LARGE_STRING_LEN)
        return false;
    char* data2 = malloc(strlen(data)*sizeof(char)+4);
    memcpy(data2+4,data, strlen(data));
    data2[strlen(data)+4] = '\0';
    src->ptr = data2;
    xs_set_refcnt(src, 1);
    src->size = strlen(data);
    return true;
}

static xs *xs_cow_copy(xs *des, xs *src) {
    if (!xs_is_ptr(src)) {
        *des = *src;
    } else if (xs_is_large_string(src)) {
        des->capacity = src->capacity;
        des->is_ptr   = src->is_ptr;
        des->size     = src->size;
        xs_inc_refcnt(src);
        des->ptr = xs_data(src);
    } else {
        des->capacity = src->capacity;
        des->is_ptr   = src->is_ptr;
        des->size     = src->size;
        size_t len = xs_size(src) + 1;
        xs_allocate_data(des, len, 0);
        memcpy(xs_data(des), xs_data(src), len);
    }
}
#endif // FBSTRING_H_INCLUDED
