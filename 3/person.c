#include <stdio.h>
#include <string.h>

struct Person
{
	char name[20];
	int age;
};

void PrintPerson(struct Person *person)
{
	printf("%s is %d years old\n",
	person->name,
	person->age);
}

int main()
{
	struct Person person;
	strcpy(person.name, "John");
	person.age = 10;
	PrintPerson(&person);
	return 0;
};
