#ifndef jachor_ip_cdll__
#define jachor_ip_cdll__

struct ip_cdll_entry {
    struct ip_cdll_entry *next, *prev;
};

static inline
struct ip_cdll_entry *ip_cdll_clear(struct ip_cdll_entry *entry) {
    entry->next=entry->prev=entry;
    return entry;
}

static inline
struct ip_cdll_entry *ip_cdll_remove(struct ip_cdll_entry *entry) {
    entry->prev->next = entry->next;
    entry->next->prev = entry->prev;
    entry->next=entry->prev=entry;
    return entry;
}

static inline 
struct ip_cdll_entry *ip_cdll_insert(struct ip_cdll_entry *entry, struct ip_cdll_entry *after) {
    entry->next = after->next;
    entry->prev = after;
    after->next = entry;
    entry->next->prev = entry;
    return entry;
}

#endif

