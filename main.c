#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Log {
    int log_id;
    char type[20];
    int quantity;
    struct Log *next;
} Log;

typedef struct Item {
    char id[50];
    char name[100];
    int quantity;
    double price;
    Log *logs;
    struct Item *next;
} Item;

Item *items_head = NULL;
int next_log_id = 1;

Item* find_item(const char *id) {
    Item *curr = items_head;
    while (curr) {
        if (strcmp(curr->id, id) == 0) {
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}

void add_log(Item *item, const char *type, int qty) {
    Log *new_log = (Log *)malloc(sizeof(Log));
    if (!new_log) return;
    new_log->log_id = next_log_id++;
    strcpy(new_log->type, type);
    new_log->quantity = qty;
    new_log->next = NULL;

    if (!item->logs) {
        item->logs = new_log;
    } else {
        Log *curr = item->logs;
        while (curr->next) {
            curr = curr->next;
        }
        curr->next = new_log;
    }
}

void handle_receive(char *id, char *name, int qty, double price) {
    if (qty <= 0 || price < 0 || find_item(id)) return; // unik + valid

    Item *new_item = (Item *)malloc(sizeof(Item));
    if (!new_item) return;

    strcpy(new_item->id, id);
    strcpy(new_item->name, name);
    new_item->quantity = qty;
    new_item->price = price;
    new_item->logs = NULL;
    new_item->next = NULL;

    add_log(new_item, "receive", qty);

    if (!items_head) {
        items_head = new_item;
    } else {
        Item *curr = items_head;
        while (curr->next) {
            curr = curr->next;
        }
        curr->next = new_item;
    }
}

void handle_ship(char *id, int qty) {
    if (qty <= 0) return;
    Item *item = find_item(id);
    if (!item || item->quantity < qty) return;
    item->quantity -= qty;
    add_log(item, "ship", qty);
}

void handle_restock(char *id, int qty) {
    if (qty <= 0) return;
    Item *item = find_item(id);
    if (!item) return;
    item->quantity += qty;
    add_log(item, "restock", qty);
}

void handle_report() {
    Item *curr = items_head;
    while (curr) {
        printf("%s|%s|%d|%.1f\n", curr->id, curr->name, curr->quantity, curr->price);
        curr = curr->next;
    }
}

void handle_audit(char *id) {
    Item *item = find_item(id);
    if (!item) return;
    printf("%s|%s|%d|%.1f\n", item->id, item->name, item->quantity, item->price);
    Log *log = item->logs;
    while (log) {
        printf("%d|%s|%d\n", log->log_id, log->type, log->quantity);
        log = log->next;
    }
}



