#if !defined(__LIST_H)
#define __LIST_H

struct listnode {
	void *data;
	struct listnode *next;
};

void list_append(struct listnode **head, void *data);
void list_do(struct listnode *head, 
		void (*callback)(void *));
void list_free(struct listnode **head,
		void (*callback)(void *));

#endif /* __LIST_H */
