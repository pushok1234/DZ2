#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <time.h>

#define size 100
#define max 1000

struct user
{
  char login[size];
  char pass[size];
  int book;
	int student;
};

struct book{
	unsigned long long isbn;
	char author[size];
	char title[size];
	int totalbook;
	int access;
};

struct student {
    char id[size];
    char surname[size];
    char name[size];
    char middlename[size];
    char faculty[size];
    char specialty[size];
};


int count_books()
{
	int line = 0;
  FILE *f = fopen("book.csv", "r");
	if (f == NULL)
	{
		printf("No file!\n");
	}
	else
	{
		while (!feof(f))
		{
			if (fgetc(f)=='\n')
				line++;
		}
		return ++line;
		fclose(f);
	}
}

struct book *read_book_csv()
{
	struct book *book =  calloc(0, sizeof(struct book));
	FILE *f = fopen("book.csv", "r");
	if (f == NULL)
	{
		printf("Not file\n");
		return 0;
	}
	else
	{
		int line = count_books();
		char tmp[max];
		for (int i = 0; i < line; i++)
		{
			fgets(tmp, max, f);

			book = realloc(book, (i+1) * sizeof(struct book));

			book[i].isbn = atoll(strtok(tmp,";"));
			strcpy(book[i].author,strtok(NULL,";"));
			strcpy(book[i].title,strtok(NULL,";"));
			book[i].totalbook = atoi(strtok(NULL,";"));
			book[i].access = atoi(strtok(NULL,"\n"));
		}
		return book;
	}
}

struct book *write_book_csv(struct book *books, int line)
{
	FILE *f = fopen("book.csv", "w+");
	rewind(f);

	for (int i = 0; i < 1; i++)
	{
		fprintf(f,"%lli",books[i].isbn); fprintf(f,";%s",books[i].author); fprintf(f,";%s",books[i].title);
		fprintf(f,";%d",books[i].totalbook); fprintf(f,";%d",books[i].access);
	}
	for (int i = 1; i < line; i++)
	{
		fprintf(f,"\n%lli",books[i].isbn); fprintf(f,";%s",books[i].author); fprintf(f,";%s",books[i].title);
		fprintf(f,";%d",books[i].totalbook); fprintf(f,";%d",books[i].access);
	}
	fclose(f);
}

int availability_book(struct book *books, long long ISBN, int line)
{
	int k = -1;
	for (int i = 0; i < line; i++)
	{
		if (ISBN == books[i].isbn)
			k = i;
	}
	return k;
}

void print_book_csv(struct book *books, int line)
{
	for(int i = 0; i < line; i++)
	{
		if (books[i].isbn != 0) printf("%lli; %s; %s; %d; %d\n", books[i].isbn,books[i].author, books[i].title, books[i].totalbook, books[i].access);
	}
}

struct book *add_book(struct book *books, int line)
{
	struct book newbook;
	int k;
	printf("Enter ISBN : ");
	do {
		fflush(stdin);
		scanf("%lli",&newbook.isbn);
		k = availability_book(books, newbook.isbn, line);
		if(k != -1)
		{
			printf("Error:This book in the file!\n"); printf("Enter ISBN : ");
		}
	} while(k != -1);

	printf("Enter author : ");
	fflush(stdin);
	gets(newbook.author);
	printf("Enter title : ");
	fflush(stdin);
	gets(newbook.title);
	printf("Enter all book : ");
	fflush(stdin);
	scanf("%d",&newbook.totalbook);
	printf("Enter used book : ");
	fflush(stdin);
	scanf("%d",&newbook.access);
	books = realloc(books, (line+1) * sizeof(struct book));

	books[line].isbn = newbook.isbn;
	strcpy(books[line].author,newbook.author);
	strcpy(books[line].title,newbook.title);
	books[line].totalbook = newbook.totalbook;
	books[line].access = newbook.access;

	printf("Good!\n");

	return books;
}

struct book *delete_book(struct book *books, int line)
{
	struct book *tmpbooks = calloc(line-1, sizeof(struct book));

	int k;
	printf("Enter ISBN: ");
	do {
		long long isbn;
		fflush(stdin);
		scanf("%lli",&isbn);
		k = availability_book(books, isbn, line);
		if(k == -1)
		{
			printf("No book found in file!\n"); printf("Enter ISBN: ");
		}
	} while(k == -1);

	for (int i = 0; i < line; i++)
	{
		if (i < k)
		{
			tmpbooks[i].isbn = books[i].isbn;
			strcpy(tmpbooks[i].author, books[i].author);
			strcpy(tmpbooks[i].title, books[i].title);
			tmpbooks[i].totalbook = books[i].totalbook;
			tmpbooks[i].access = books[i].access;
		}
		else if (i > k)
		{
			tmpbooks[i-1].isbn = books[i].isbn;
			strcpy(tmpbooks[i-1].author, books[i].author);
			strcpy(tmpbooks[i-1].title, books[i].title);
			tmpbooks[i-1].totalbook = books[i].totalbook;
			tmpbooks[i-1].access = books[i].access;
		}
	}
	printf("Good!\n");

	return tmpbooks;
}

struct book *edit_book(struct book *books, int line)
{
	struct book tmpbooks;

	int k;
	printf("Enter ISBN : ");
	do {
		long long isbn;
		fflush(stdin);
		scanf("%lli",&isbn);
		k = availability_book(books, isbn, line);
		if(k == -1)
		{
			printf("No book found in file!\n");
			printf("Enter ISBN : ");
		}
	} while(k == -1);
	printf("Enter new author : ");
	fflush(stdin);
	gets(tmpbooks.author);
	printf("Enter new title : ");
	fflush(stdin);
	gets(tmpbooks.title);

	for(int i = 0; i < line; i++)
	{
		if (i == k)
		{
			strcpy(books[i].author, tmpbooks.author);
			strcpy(books[i].title, tmpbooks.title);
		}
	}
	printf("Good!\n");

	return books;
}

struct book *change_book(struct book *books, int line)
{
	struct book tmpbooks;

	int k;
	printf("Enter ISBN : ");
	do {
		long long isbn;
		fflush(stdin);
		scanf("%lli",&isbn);
		k = availability_book(books, isbn, line);
		if(k == -1)
		{
			printf("No book found in file!\n");
			printf("Enter ISBN book to change : ");
		}
	} while(k == -1);
	printf("Enter new all book : ");
	fflush(stdin);
	scanf("%d",&tmpbooks.totalbook);
	printf("Enter new used book : ");
	fflush(stdin);
	scanf("%d",&tmpbooks.access);

	for (int i = 0; i < line; i++)
	{
		if (i == k)
		{
			books[i].totalbook = tmpbooks.totalbook;
			books[i].access = tmpbooks.access;
			printf("\n\tFree books in the library is : %d\n",books[i].totalbook - books[i].access);
		}
	}
	printf("Good!\n");

	return books;
}


int count_students()
{
	int line = 0;
	FILE *f = fopen("student.csv", "r");
	if (f == NULL)
	{
		printf("No file!\n");
	}
	else
	{
		while (!feof(f))
		{
			if (fgetc(f)=='\n')
			line++;
		}
		return ++line;
		fclose(f);
	}
}

struct student *read_student_csv()
{
	struct student *student =  calloc(0, sizeof(struct student));
	FILE *f = fopen("student.csv", "r");
	if (f == NULL)
	{
		printf("Not file\n");
		return 0;
	}
	else
	{
		int line = count_students();
		char tmp[max];
		for (int i = 0; i < line; i++)
		{
			fgets(tmp, max, f);

			student = realloc(student, (i+1) * sizeof(struct student));

			strcpy(student[i].id,strtok(tmp,";"));
			strcpy(student[i].surname,strtok(NULL,";"));
			strcpy(student[i].name,strtok(NULL,";"));
			strcpy(student[i].middlename,strtok(NULL,";"));
			strcpy(student[i].faculty,strtok(NULL,";"));
			strcpy(student[i].specialty,strtok(NULL,"\n"));
		}
		return student;
	}
}

struct student *write_student_csv(struct student *students, int line)
{
	FILE *f = fopen("student.csv", "w+");
	rewind(f);

	for (int i = 0; i < 1; i++)
	{
		fprintf(f,"%s",students[i].id); fprintf(f,";%s",students[i].surname); fprintf(f,";%s",students[i].name);
		fprintf(f,";%s",students[i].middlename); fprintf(f,";%s",students[i].faculty); fprintf(f,";%s",students[i].specialty);
	}
	for (int i = 1; i < line; i++)
	{
		fprintf(f,"\n%s",students[i].id); fprintf(f,";%s",students[i].surname); fprintf(f,";%s",students[i].name);
		fprintf(f,";%s",students[i].middlename); fprintf(f,";%s",students[i].faculty); fprintf(f,";%s",students[i].specialty);
	}
	fclose(f);
}

int availability_student(struct student *students, char id[size], int line)
{
	int k = -1;
	for (int i = 0; i < line; i++)
	{
		if (strcmp(id, students[i].id) == 0)
			k = i;
	}
	return k;
}

void print_student_csv(struct student *students, int line)
{
	for(int i = 0; i < line; i++)
	{
		if (students[i].id != 0) printf("%s; %s; %s; %s; %s; %s\n", students[i].id,students[i].surname, students[i].name, students[i].middlename, students[i].faculty, students[i].specialty);
	}
}

struct student *add_student(struct student *students, int line)
{
	struct student newstudent;

	int k;
	printf("Enter ID : ");
	do {
		fflush(stdin);
		gets(newstudent.id);
		k = availability_student(students, newstudent.id, line);
		if( k != -1)
		{
			printf("Error:This student in the file!\n");
			printf("Enter ID : ");
		}
	} while( k != -1);

	printf("Enter surname : ");
	fflush(stdin);
	gets(newstudent.surname);
	printf("Enter name : ");
	fflush(stdin);
	gets(newstudent.name);
	printf("Enter middlename : ");
	fflush(stdin);
	gets(newstudent.middlename);
	printf("Enter faculty : ");
	fflush(stdin);
	gets(newstudent.faculty);
	printf("Enter specialty : ");
	fflush(stdin);
	gets(newstudent.specialty);

	students = realloc(students, (line+1) * sizeof(struct student));

	strcpy(students[line].id,newstudent.id);
	strcpy(students[line].surname,newstudent.surname);
	strcpy(students[line].name,newstudent.name);
	strcpy(students[line].middlename,newstudent.middlename);
	strcpy(students[line].faculty,newstudent.faculty);
	strcpy(students[line].specialty,newstudent.specialty);

	printf("Good!\n");
	return students;
}

struct student *delete_student(struct student *students, int line)
{
	struct student *tmpstudents = calloc(line-1, sizeof(struct student));

	int k;
	printf("Enter ID : ");
	do {
		char id[size];
		fflush(stdin);
		gets(id);
		k = availability_student(students, id, line);
		if(k == -1)
		{
			printf("No student found in file!\n");
			printf("Enter ID : ");
		}
	} while(k == -1);

	for (int i = 0; i < line; i++)
	{
		if (i < k)
		{
			strcpy(tmpstudents[i].id,students[i].id);
			strcpy(tmpstudents[i].surname,students[i].surname);
			strcpy(tmpstudents[i].name,students[i].name);
			strcpy(tmpstudents[i].middlename,students[i].middlename);
			strcpy(tmpstudents[i].faculty,students[i].faculty);
			strcpy(tmpstudents[i].specialty,students[i].specialty);
		}
		else if (i > k)
		{
			strcpy(tmpstudents[i-1].id,students[i].id);
			strcpy(tmpstudents[i-1].surname,students[i].surname);
			strcpy(tmpstudents[i-1].name,students[i].name);
			strcpy(tmpstudents[i-1].middlename,students[i].middlename);
			strcpy(tmpstudents[i-1].faculty,students[i].faculty);
			strcpy(tmpstudents[i-1].specialty,students[i].specialty);
		}
	}
	printf("Good!\n");

	return tmpstudents;
}

struct student *edit_student(struct student *students, int line)
{
	struct student tmpstudents;
	int k;
	printf("Enter ID : ");
	do {
		fflush(stdin);
		gets(tmpstudents.id);
		k = availability_student(students, tmpstudents.id, line);
		if(k == -1)
		{
			printf("No student found in file!\n");
			printf("Enter ID student to edit : ");
		}
	} while(k == -1);

	printf("Enter new surname : ");
	fflush(stdin);
	gets(tmpstudents.surname);
	printf("Enter new name :");
	fflush(stdin);
	gets(tmpstudents.name);
	printf("Enter new middlename :");
	fflush(stdin);
	gets(tmpstudents.middlename);
	printf("Enter new faculty :");
	fflush(stdin);
	gets(tmpstudents.faculty);
	printf("Enter new specialty :");
	fflush(stdin);
	gets(tmpstudents.specialty);

	for (int i = 0; i < line; i++)
	{
		if (i == k)
		{
			strcpy(students[i].id, tmpstudents.id);
			strcpy(students[i].surname,tmpstudents.surname);
			strcpy(students[i].name,tmpstudents.name);
			strcpy(students[i].middlename,tmpstudents.middlename);
			strcpy(students[i].faculty,tmpstudents.faculty);
			strcpy(students[i].specialty,tmpstudents.specialty);
		}
	}
	printf("Good!\n");

	return students;
}

void info_student(struct student *students, int line)
{
	int k;
	printf("Enter ID : ");
	do {
		char id[size];
		fflush(stdin);
		gets(id);
		k = availability_student(students, id, line);
		if(k == -1)
		{
			printf("No student found in file!\n");
			printf("Enter ID : ");
		}
	} while(k == -1);

	for (int i = 0; i < line; i++)
	{
		if (i == k)
		printf("\tID: %s;\n\tSurname: %s;\n\tName: %s;\n\tMiddlename: %s;\n\tFaculty: %s;\n\tSpecialty: %s\n", students[i].id,students[i].surname, students[i].name, students[i].middlename, students[i].faculty, students[i].specialty);
	}
}

int count_user()
{
	int line = 0;
  FILE *f = fopen("user.csv", "r");
	if (f == NULL)
	{
		printf("No file!\n");
	}
	else
	{
		while (!feof(f))
		{
			if (fgetc(f)=='\n')
				line++;
		}
		return ++line;
		fclose(f);
	}
}

struct user *read_user_csv()
{
	struct user *user =  calloc(0, sizeof(struct user));
	FILE *f = fopen("user.csv", "r");
	if (f == NULL)
	{
		printf("Not file\n");
		return 0;
	}
	else
	{
		int line = count_user();
		char tmp[max];
		for (int i = 0; i < line; i++)
		{
			fgets(tmp, max, f);

			user = realloc(user, (i+1) * sizeof(struct user));

			strcpy(user[i].login,strtok(tmp,";"));
			strcpy(user[i].pass,strtok(NULL,";"));
			user[i].book = atoi(strtok(NULL,";"));
			user[i].student = atoi(strtok(NULL,"\n"));
		}
		return user;
	}
}

int availability_user(struct user *users, char log[size], char pass[size], int line)
{
	int k = -1;
		for(int i = 0; i < line; i++)
		{
			if ((strcmp(log, users[i].login) == 0) && (strcmp(pass, users[i].pass) == 0))
				k = i ;
		}
		return k;
}


void mainbook(char login[size])
{
	struct book *books = read_book_csv();
	int line = count_books();

	FILE *flog;
	time_t now;
	struct tm  ts;
	char buf[size];
	time(&now);
	ts = *localtime(&now);
	strftime(buf, sizeof(buf), "%a %Y-%m-%d %H:%M:%S", &ts);

	flog = fopen("library.log","a");

	for(;;)
	{
		printf("\nMENU:\n");
		printf("1) Add books\n");
		printf("2) Delete book\n");
		printf("3) Edit book\n");
		printf("4) Change book\n");
		printf("5) Print all books\n");
		printf("6) Exit\n");
		int com;
		scanf("%d",&com);
		switch(com)
		{
			case 1:
				books = add_book(books, line);
				++line;
				break;
			case 2:
				books = delete_book(books, line);
				--line;
				break;
			case 3:
				books = edit_book(books, line);
				break;
			case 4:
				books = change_book(books, line);
				break;
			case 5:
				print_book_csv(books, line);
				break;
		}
		if(com == 6)
			break;

		fprintf(flog,"\n\"%s\"",buf);
		fprintf(flog,",\"%s\"",login);
		if(com==1)
				fprintf(flog,",\"Add new books\"");
		else if(com==2)
				fprintf(flog,",\"Delete book\"");
		else if(com==3)
				fprintf(flog,",\"Edit book\"");
		else if(com==4)
				fprintf(flog,",\"Change book\"");
		else if(com==5)
				fprintf(flog,",\"List all book\"");
		else if(com==6)
				fprintf(flog,",\"Exit\"");
		else break;
		fclose(flog);
	}

	write_book_csv(books, line);
	free(books);

}


void mainstudent(char login[size])
{
	struct student *students = read_student_csv();
	int line = count_students();
	FILE *flog;
	time_t now;
	struct tm  ts;
	char buf[size];
	time(&now);
	ts = *localtime(&now);
	strftime(buf, sizeof(buf), "%a %Y-%m-%d %H:%M:%S", &ts);

	flog = fopen("library.log","a");
	for(;;)
	{
		printf("\nMENU:\n");
		printf("1) Add students\n");
		printf("2) Delete student\n");
		printf("3) Edit student\n");
		printf("4) Information about student\n");
		printf("5) Print all students\n");
		printf("6) Exit\n");
		int com;
		scanf("%d",&com);
		switch(com)
		{
			case 1:
				students = add_student(students, line);
				++line;
				break;
			case 2:
				students = delete_student(students, line);
				--line;
				break;
			case 3:
				students = edit_student(students, line);
				break;
			case 4:
				info_student(students, line);
				break;
			case 5:
				print_student_csv(students, line);
				break;
		}
		if(com == 6)
			break;

		fprintf(flog,"\n\"%s\"",buf);
		fprintf(flog,",\"%s\"",login);
		if(com==1)
			fprintf(flog,",\"Add new students\"");
		else if(com==2)
			fprintf(flog,",\"Delete student\"");
		else if(com==3)
			fprintf(flog,",\"Edit student\"");
		else if(com==4)
			fprintf(flog,",\"Information about student\"");
		else if(com==5)
			fprintf(flog,",\"Print all book\"");
		else if(com==6)
			fprintf(flog,",\"Exit\"");
		else break;
		fclose(flog);
	}

	write_student_csv(students, line);
	free(students);

}


void mainuser()
{
	struct user *users = read_user_csv();
	int line = count_user();

	char login[size];
	char pass[size];

	int k;
	do {
		fflush(stdin);
		printf("Enter login : "); gets(login);
		printf("Enter password : "); gets(pass);
		k = availability_user(users, login, pass, line);
		if( k == -1)
		{
			printf("Error:Your login or password is not correct!\n");
			printf("Try again?\n"); printf("1.Yes\n"); printf("2.No\n");
			int ans;
			scanf("%d", &ans);
			if (ans == 2)
			{
				break;
			}
		}
	} while (k == -1);

	if (k != -1)
	{
		printf("Good!\n");
		if(users[k].book == 1 && users[k].student == 0 )
		{
			printf("Welcome to file with book!\n");
			mainbook(login);
		}
		else if (users[k].book == 0 && users[k].student == 1 )
		{
			printf("Welcome to file with student!\n");
			mainstudent(login);
		}
		else if(users[k].book == 1 && users[k].student == 1 )
		{
			int com;
			for(;;)
			{
				printf("1) Menu book. \n");
				printf("2) Menu student. \n");
				printf("3) Exit. \n");
				scanf("%d",&com);
				if(com == 1)
				{
					printf("Welcome to file with book!\n");
					mainbook(login);
				}
				else if(com == 2)
				{
					printf("Welcome to file with student!\n");
					mainstudent(login);
				}
				else
				{
					break;
				}
			}
		}
	}

	free(users);
}

int main()
{
	mainuser();
	return 0;
}
