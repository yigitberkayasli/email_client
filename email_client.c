/*
	The txt files needs to have structure like this;
	
	1;cdaniel@mail.com;22/10/2018 15:55;Notes; Can you send me your lecture notes?
	2;jennifer@mail.co.uk;23/10/2018 16:45;Hello; Hi, how are you?
	3;cthomas@xyz.com;23/10/2018 16:45;Book; Yes you can

	And also the number of lines needs to be exactly same as number of emails. If there is a empty line in txt file, the program could not read the file completely.
*/


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define EMAIL_ADRESS_SIZE 50
#define SUBJECT_SIZE 50
#define CONTENT_SIZE 200

struct Time {
	int day;
	int month;
	int year;
	int hour;
	int min;
};

struct Mail {
	int id;
	char email_adress[EMAIL_ADRESS_SIZE];
	struct Time t;
	char subject[SUBJECT_SIZE];
	char content[CONTENT_SIZE];
};

struct ListRecord {
	struct ListRecord* next;
	struct Mail mail;
};

typedef struct ListRecord * List;

List initialiseInbox(void);
List initialiseSent(void);
List sendEmail(List);
List deleteSentEmail(List);
List deleteInboxEmail(List);
void printEmails(List, List);
void searchEmail(List, List);
void overwrite(List, List);

int main()
{
	List inboxList = initialiseInbox();
	List sentList = initialiseSent();
	
	int selection;

	do
	{
		system("cls");
		printf("Welcome to Your Email Client!\n");
		if (inboxList != NULL && sentList != NULL)
			printf("Your inbox and sent box have been loaded successfully.\n");
		else if (inboxList == NULL && sentList != NULL)
			printf("Your sent box has been loaded successfully.\n");
		else if (inboxList != NULL && sentList == NULL)
			printf("Your inbox has been loaded successfully.\n");
		printf("--- MENU --- \n");
		printf("1. Send an email\n");
		printf("2. Delete an email from your inbox\n");
		printf("3. Delete an email for your sent box\n");
		printf("4. See your emails\n");
		printf("5. Search for an email\n");
		printf("6. Exit\n");
		printf("Option: ");
		scanf("%d", &selection);
		fflush(stdin);
				
		switch (selection)
		{
			case 1:
				sentList = sendEmail(sentList);
				break;
			case 2:
				inboxList = deleteInboxEmail(inboxList);
				break;
			case 3:
				sentList = deleteSentEmail(sentList);
				break;
			case 4:
				printEmails(sentList, inboxList);
				break;
			case 5:
				searchEmail(sentList, inboxList);
				break;
			case 6:
				overwrite(sentList, inboxList);
				printf("\nThe inbox.txt and sent.txt files have been updated.\n");
				printf("See you later!\n\n");
				system("pause");
				break;
		}
	} while (selection != 6);
	
	return 0;
}

int checkFile(char fileName[])
{
	FILE *txt = fopen(fileName, "r");
	
	if (txt == NULL) // file is not present
		return 1;
		
	char firstLine;
	fscanf(txt, "%c", &firstLine);
		
	if (firstLine == 0) // file is empty
	{
		fclose(txt);
		return 1;
	}
	
	fclose(txt);
	
	return 0;
}

List fillList(List head, List current, FILE *txt)
{	
	char ch_tmp;
	int ch_order = 0;
	
	fscanf(txt, "%d;", &current->mail.id);

	do
	{
		fscanf(txt, "%c", &ch_tmp);
		if (ch_tmp != ';')
		{
			current->mail.email_adress[ch_order] = ch_tmp;
			ch_order++;
		if (ch_order > 48)
		{
			printf("There is something wrong with sent.txt or inbox.txt!\n");
			system("pause");
			exit(0);
		}
		}
	} while (ch_tmp != ';');
	current->mail.email_adress[ch_order] = '\0';
	
	fscanf(txt, "%d/%d/%d %d:%d;", &current->mail.t.day, &current->mail.t.month, &current->mail.t.year, &current->mail.t.hour, &current->mail.t.min);
	
	ch_order = 0;
	do
	{
		fscanf(txt, "%c", &ch_tmp);
		if (ch_tmp != ';')
		{
			current->mail.subject[ch_order] = ch_tmp;
			ch_order++;
		}
		if (ch_order > 48)
		{
			printf("There is something wrong with sent.txt or inbox.txt!\n");
			system("pause");
			exit(0);
		}
		
	} while (ch_tmp != ';');
	current->mail.subject[ch_order] = '\0';

	ch_order = 0;
	int eof_status;
	do
	{
		eof_status = fscanf(txt, "%c", &ch_tmp);
		if (!(ch_tmp == 10 || eof_status == EOF))
		{
			current->mail.content[ch_order] = ch_tmp;
			ch_order++;
		}
		if (ch_order > 198)
		{
			printf("There is something wrong with sent.txt or inbox.txt!\n");
			system("pause");
			exit(0);
		}
	} while (!(ch_tmp == 10 || eof_status == EOF));
	current->mail.content[ch_order] = '\0';
	
	if (eof_status == EOF)
	{
		current->next = NULL;
		return head;
	}
	else
	{
		current->next = (List) malloc (sizeof(struct ListRecord));
		current = current->next;		
		return fillList(head, current, txt);
	}
}

List initialiseInbox()
{
	List head;
	
	if(!(checkFile("inbox.txt")))
	{
		head = (List) malloc (sizeof(struct ListRecord));
		FILE *txt = fopen("inbox.txt", "r");
		head = fillList(head, head, txt);
		fclose(txt);
	}
	else
		head = NULL;
	
	return head;
}

List initialiseSent()
{
	List head;
	
	if(!(checkFile("sent.txt")))
	{
		head = (List) malloc (sizeof(struct ListRecord));
		FILE *txt = fopen("sent.txt", "r");
		head = fillList(head, head, txt);
		fclose(txt);
	}
	else
		head = NULL;
	
	return head;
}

List sendEmail(List sentList)
{
	system("cls");
	printf("---SENT EMAIL---\n");
	int temp_id = 1;
	
	List temp;
	
	if (sentList != NULL) // we took a non-empty list
	{
		temp = sentList;
		while (1)
		{
			if (sentList->mail.id == temp_id)
				temp_id++;
					
			if (sentList->next == NULL)
				break;
			sentList = sentList->next;
		}
		sentList->next = (List) malloc (sizeof(struct ListRecord));
		sentList = sentList->next;
	}
	else
	{
		sentList = (List) malloc (sizeof(struct ListRecord));
		temp = sentList;
	}
	
	printf("Current ID: %d\n", temp_id);
	
	sentList->mail.id = temp_id;
	
	printf("Recipient: ");
	gets(sentList->mail.email_adress);
	
	printf("Subject: ");
	gets(sentList->mail.subject);
	
	printf("Content: ");
	gets(sentList->mail.content);
	
	time_t ti = time(NULL);
	struct tm t = *localtime(&ti);
	sentList->mail.t.day = t.tm_mday;
	sentList->mail.t.month = t.tm_mon+1;
	sentList->mail.t.year = t.tm_year+1900;
	sentList->mail.t.hour = t.tm_hour;
	sentList->mail.t.min = t.tm_min;
	
	sentList->next = NULL;

	return temp;
}

void printEmails (List sentList, List inboxList)
{
	system("cls");
	printf("---SENT LIST---\n");
	while(sentList != NULL)
	{
		printf("%-5d %-25s %02d/%02d/%04d %02d:%02d %-15s %s\n", sentList->mail.id, sentList->mail.email_adress, sentList->mail.t.day,
															sentList->mail.t.month, sentList->mail.t.year, sentList->mail.t.hour,
															sentList->mail.t.min, sentList->mail.subject, sentList->mail.content);

		sentList = sentList->next;
	}
	printf("\n");
	printf("---INBOX LIST---\n");
	while(inboxList != NULL)
	{
		printf("%-5d %-25s %02d/%02d/%04d %02d:%02d %-15s %s\n", inboxList->mail.id, inboxList->mail.email_adress, inboxList->mail.t.day,
															inboxList->mail.t.month, inboxList->mail.t.year, inboxList->mail.t.hour,
															inboxList->mail.t.min, inboxList->mail.subject, inboxList->mail.content);

		inboxList = inboxList->next;
	}	

	system("pause");
}

List deleteEmail(List mailList)
{
	if (mailList == NULL)
	{
		printf("The list is already empty!\n");
		system("pause");
		return mailList;
	}
	
	int id;
	List temp1, temp2, head;
	
	head = mailList;
	temp1 = head;
	temp2 = temp1->next;

	printf("Enter the ID of email you want to delete: ");
	scanf("%d", &id);
	
	if (temp1->mail.id == id) // checking first
	{
		head = temp2;
		free(temp1);
		return head;
	} else if (temp2 != NULL)
	{
		if(temp2->mail.id == id) // checking second
		{
			head->next = temp2->next;
			free(temp2);
			return head;
		}
		
		temp1 = temp2;
		temp2 = temp2->next;
		
		while (temp2 != NULL) // checking the rest
		{
			if (temp2->mail.id == id)
			{
				temp1->next = temp2->next;
				free(temp2);
				return head;
			}
			
			temp1 = temp2;
			temp2 = temp2->next;
		}
	}
	
	printf("Cant find the ID!\n");
	system("pause");
	return head;
}

List deleteSentEmail(List sentList)
{
	system("cls");
	printf("---DELETE EMAIL FROM SENT BOX---\n");
	return deleteEmail(sentList);
}

List deleteInboxEmail(List inboxList)
{
	system("cls");
	printf("---DELETE EMAIL FROM INBOX---\n");
	return deleteEmail(inboxList);
}


void searchEmailCommon(List mailList)
{
	if (mailList == NULL)
	{
		printf("The list is empty!\n");
		system("pause");
		return;
	}
	
	int selection;
	int ch_order[2]; // first for the ch in email_adress, the second one is ch in mailList
	int found;
	char email_adress[EMAIL_ADRESS_SIZE];
	char subject[SUBJECT_SIZE];
	
	printf("\nSelect the section you want to search\n");
	printf("(1) Mail adress | (2) Subject : ");
	scanf("%d", &selection);
	fflush(stdin);
	
	switch(selection)
	{
		case 1:
			printf("\nEnter the mail adress you want to search: ");
			gets(email_adress);
			
			while (mailList != NULL)
			{
				ch_order[0] = 0; ch_order[1] = 0; found = 0; // reset the values
				do
				{
					if (mailList->mail.email_adress[ch_order[0]] != email_adress[ch_order[1]])
						ch_order[1] = -1; // it will reset at the end of the loop
					
					if (email_adress[ch_order[1] + 1] == 0)
						found = 1;
					
					ch_order[0]++; ch_order[1]++;
				} while (mailList->mail.email_adress[ch_order[0]] != 0 && email_adress[ch_order[1]] != 0);
				
				if (found)
					printf("%-5d %-25s %02d/%02d/%04d %02d:%02d %-15s %s\n", mailList->mail.id, mailList->mail.email_adress, mailList->mail.t.day,
																			mailList->mail.t.month, mailList->mail.t.year, mailList->mail.t.hour,
																			mailList->mail.t.min, mailList->mail.subject, mailList->mail.content);
				mailList = mailList->next;
			}
			
			break;
		case 2:
			printf("\nEnter the subject you want to search: ");
			gets(subject);
			
			while (mailList != NULL)
			{
				ch_order[0] = 0; ch_order[1] = 0; found = 0; // reset the values
				do
				{
					if (mailList->mail.subject[ch_order[0]] != subject[ch_order[1]])
						ch_order[1] = -1; // it will reset at the end of the loop
					
					if (subject[ch_order[1] + 1] == 0)
						found = 1;
					
					ch_order[0]++; ch_order[1]++;
				} while (mailList->mail.subject[ch_order[0]] != 0 && subject[ch_order[1]] != 0);
				
				if (found)
					printf("%-5d %-25s %02d/%02d/%04d %02d:%02d %-15s %s\n", mailList->mail.id, mailList->mail.email_adress, mailList->mail.t.day,
																			mailList->mail.t.month, mailList->mail.t.year, mailList->mail.t.hour,
																			mailList->mail.t.min, mailList->mail.subject, mailList->mail.content);
				mailList = mailList->next;
			}
			
			break;
			
	}
	system("pause");
}

void searchEmail(List sentList, List inboxList)
{
	system("cls");
	
	int selection;
	
	printf("Which mail box you want to search?\n");
	printf("(1) Inbox | (2) Sent Box : ");
	scanf("%d", &selection);
	
	switch(selection)
	{
		case 1:
			searchEmailCommon(inboxList);
			break;
		case 2:
			searchEmailCommon(sentList);
			break;
	}	
}

void overwrite(List sentList, List inboxList)
{
	FILE *txt = fopen("sent.txt", "w");
	
	while (sentList != NULL)
	{
		fprintf(txt,"%d;%s;%d/%d/%d %d:%d;%s;%s", sentList->mail.id, sentList->mail.email_adress, sentList->mail.t.day, 
												  sentList->mail.t.month, sentList->mail.t.year, sentList->mail.t.hour, 
												  sentList->mail.t.min, sentList->mail.subject, sentList->mail.content);
		if (sentList->next != NULL)
			fprintf(txt, "\n");
		
		sentList = sentList->next;
	}
	fclose(txt);
	
	txt = fopen("inbox.txt", "w");
	
	while (inboxList != NULL)
	{
		fprintf(txt,"%d;%s;%d/%d/%d %d:%d;%s;%s", inboxList->mail.id, inboxList->mail.email_adress, inboxList->mail.t.day, 
												  inboxList->mail.t.month, inboxList->mail.t.year, inboxList->mail.t.hour, 
												  inboxList->mail.t.min, inboxList->mail.subject, inboxList->mail.content);
		if (inboxList->next != NULL)
			fprintf(txt, "\n");
		
		inboxList = inboxList->next;
	}
	fclose(txt);
}
