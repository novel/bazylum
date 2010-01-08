#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "bazylum.h"

void list_append(struct listnode **head, void *data)
{
	if (*head == NULL) {
		struct listnode *new_head;

		new_head = malloc(sizeof(struct listnode));
		new_head->data = data;
		new_head->next = NULL;

		*head = new_head;
	} else {
		struct listnode *cur = *head;
		
		while (cur->next != NULL)
			cur = cur->next;
	
		cur->next = malloc(sizeof(struct listnode));
		cur = cur->next;
		cur->data = data;
		cur->next = NULL;
	}
}

void list_do(struct listnode *head, void (*callback)(void *))
{
	struct listnode *cur = head;

	if (head == NULL) {
		printf("list empty!\n");
		return;
	}

	while (cur->next != NULL) {
		callback(cur->data);
		cur = cur->next;
	}
}

void list_free(struct listnode **head, void (*callback)(void *))
{
	if (*head == NULL) {
		/* nothing to do on an empty list */
		return; 
	} else {
		struct listnode *cur = *head;
		
		while (cur->next != NULL) {
			struct listnode *tmp;

			if (cur->data != NULL) {
				/* call it to free user data */
				callback(cur->data);
				free(cur->data);
			}
				
			tmp = cur->next;

			free(cur);
			cur = tmp;
		}
	}
}
