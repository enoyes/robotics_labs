#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int id_count = 1;

struct Person
{
	// Unique identifier for the person
	int id;
	// Information about person
	char name[20];
	int age;
	// Pointer to next person in list
	struct Person *next;
};

struct List
{
	// First person in the list. A value equal to NULL indicates that the
	// list is empty.
	struct Person *head;
	// Current person in the list. A value equal to NULL indicates a
	// past-the-end position.
	struct Person *current;
	// Pointer to the element appearing before 'current'. It can be NULL if
	// 'current' is NULL, or if 'current' is the first element in the list.
	struct Person *previous;
};

// Give an initial value to all the fields in the list.
void ListInitialize(struct List *list)
{
	list->head = NULL;
	list->current = NULL;
	list->previous = NULL;
}

// Move the current position in the list one element forward. If last element
// is exceeded, the current position is set to a special past-the-end value.
void ListNext(struct List *list)
{
	if (list->current)
	{
		list->previous = list->current;
		list->current = list->current->next;
	}
}

// Move the current position to the first element in the list.
void ListHead(struct List *list)
{
	list->previous = NULL;
	list->current = list->head;
}

// Get the element at the current position, or NULL if the current position is
// past-the-end.
struct Person *ListGet(struct List *list)
{
	return list->current;
}

// Set the current position to the person with the given id. If no person
// exists with that id, the current position is set to past-the-end.
void ListFind(struct List *list, int id)
{
	ListHead(list);
	while (list->current && list->current->id != id)
		ListNext(list);
}

// Insert a person before the element at the current position in the list. If
// the current position is past-the-end, the person is inserted at the end of
// the list. The new person is made the new current element in the list.
void ListInsert(struct List *list, struct Person *person)
{
	// Set 'next' pointer of current element
	person->next = list->current;
	// Set 'next' pointer of previous element. Treat the special case where
	// the current element was the head of the list.
	if (list->current == list->head)
		list->head = person;
	else
		list->previous->next = person;
	// Set the current element to the new person
	list->current = person;
}

// Remove the current element in the list. The new current element will be the
// element that appeared right after the removed element.
void ListRemove(struct List *list)
{
	// Ignore if current element is past-the-end
	if (!list->current)
		return;
	// Remove element. Consider special case where the current element is
	// in the head of the list.
	if (list->current == list->head)
		list->head = list->current->next;
	else
		list->previous->next = list->current->next;
	// Free element, but save pointer to next element first.
	struct Person *next = list->current->next;
	free(list->current);
	// Set new current element
	list->current = next;
}

void PrintPerson(struct Person *person)
{
	printf("\nPerson with ID %d:\n", person->id);
	printf("\tName: %s\n", person->name);
	printf("\tAge: %d\n\n", person->age);
}

void print_menu()
{
	printf("Main menu:\n\n");
	printf("1. Add a person\n");
	printf("2. Find a person\n");
	printf("3. Remove a person\n");
	printf("4. Print the list\n");
	printf("5. Exit\n\n");
	printf("Select an option: ");
}

void strip_newline(char *s)
{
	while (*s)
	{
		if (*s == '\n')
		{
			*s = 0;
			return;
		}
		s++;
	}
}

// Adds a person to the linked list
void AddPerson(struct List *list)
{
	// Allocate memory for the person
	struct Person *person;
	person = malloc(sizeof(struct Person));

	// Recieve user input
	printf("Enter name: ");
	fgets(person->name, 20, stdin);
	printf("Enter age: ");
	scanf("%d", &(person->age));

	strip_newline(person->name);

	// Assign next unused ID
	person->id = id_count;
	id_count++;

	ListInsert(list, person);
}

// Finds a person inside the linked list after asking for an ID
void FindPerson(struct List *list)
{
	// Get ID from the user
	int find_id;
	printf("Enter ID: ");
	scanf("%d", &find_id);

	ListFind(list, find_id);

	struct Person *person = ListGet(list);

	if (person)
		PrintPerson(person);
	else
		printf("Person with ID %d not found\n", find_id);
}

// Removes a person with the given ID
void RemovePerson(struct List *list)
{
	// Get ID from user
	int find_id;
	printf("Enter ID: ");
	scanf("%d", &find_id);

	ListFind(list, find_id);

	ListRemove(list);

}

void PrintList(struct List *list)
{
	ListHead(list);

	while(list->current)
	{
		PrintPerson(ListGet(list));
		ListNext(list);
	}
}


int main()
{
	struct List list;

	ListInitialize(&list);

	while(1)
	{
		int x = 0;
		print_menu();
		scanf("%d", &x);
		// Clear the buffer of newlines
		scanf("%*[^\n]");
		scanf("%*c");
		switch(x)
		{
			case 1:
				AddPerson(&list);
				break;
			case 2:
				FindPerson(&list);
				break;
			case 3:
				RemovePerson(&list);
				break;
			case 4:
				PrintList(&list);
				break;
			case 5:
				printf("Goodbye!\n\n");
				return 0;
			default:
				printf("Invalid Option!\n\n");
			}
		}
}
