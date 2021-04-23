#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>

typedef struct Card_s {
	char suit[10];
	int value;
	char name[3];
	struct Card_s* pt;
}Card;

void StartGame(Card** head);
void FileScan(Card** head, FILE* inp);
void CreateCard(Card** card, int value, char suit);
void CreateRandom(Card** head);
void ShuffleDeck(Card** head);
int my_rand(int R_iN);
int CountDeck(Card* head);
Card* RemoveNthNode(Card** HN, int N);
void AddCard(Card** HN, Card* NodeToAdd, int N);
Card* ReturnNthNode(Card* HN, int N);
void FreeNodes(Card* head);

int main(void) {

	Card* head = NULL;
	StartGame(&head);


	FreeNodes(head);
	// MAKE SURE TO FREE EVERYTHING AND CLOSE FILES
	return(0);
}

void StartGame(Card** head) {
	/*
	This function initiailzes the game. It returns the head node of the shuffled deck that is created
	*/
	my_rand(0);
	char userInput;
	FILE* inp;
	printf("Would you like to enter a file? (y/n)\n");
	scanf("%c", &userInput);
	if (userInput == 'y') {
		char fileName[100];

		printf("Enter file name: \n");
		do {
			scanf("%s", fileName);

			inp = fopen(fileName, "r");
		} while (inp == NULL);
		FileScan(head, inp);
		Card* tmp = *head;
		while (tmp != NULL) {
			printf("%s %s\n", tmp->suit, tmp->name);

			tmp = tmp->pt;
		}

	}
	else if (userInput == 'n') {
		CreateRandom(head);
	}

	Card* tmp = *head;
	while (tmp != NULL) {
		printf("%s %s\n", tmp->name, tmp->suit);
		tmp = tmp->pt;
	}

}

void FileScan(Card** head, FILE* inp) {
	/*
	This function scans in info from a .txt file
	head: the head of the linked list that represents the deck
	inp: the input file
	NOTE: This function does assume that the file is in the proper format 
	(this is a safe assumption, as I am the one creating these files for testing purposes)
	*/
	Card* temp;
	Card* temp2 = NULL;
	int value;
	char suit;
	
	fscanf(inp, "%d", &value);
	fscanf(inp, " %c", &suit);
	*head = (Card*)malloc(sizeof(Card));

	CreateCard(head, value, suit);
	temp = *head;

	while (fscanf(inp, " %d", &value) == 1) {

		do {
			fscanf(inp, "%c", &suit);
		} while (suit == ' ' || suit == '\n');
		
		temp2 = (Card*)malloc(sizeof(Card));
		CreateCard(&temp2, value, suit);
		temp->pt = temp2;
		temp = temp->pt;
	}
	temp->pt = NULL;

	fclose(inp);


}

void CreateCard(Card** card, int value, char suit) {
	/*
	This function creates a new card
	card: the card that will be created
	value: the value of the card
	suit: the suit of the card
	*/

	// Create the value for the card
	if (value <= 10 && value > 1) {
		(*card)->value = value;
	}
	else if (value == 1) {
		(*card)->value = 11;
	}
	else {
		(*card)->value = 10;
	}

	// Create the name for the card
	switch (value) {
	case 1:
		strcpy((*card)->name, "A");
		break;
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
		(*card)->name[0] = (char)(value + '0');
		(*card)->name[1] = '\0';
		break;
	case 10:
		strcpy((*card)->name, "10");
		break;
	case 11:
		strcpy((*card)->name, "J");
		break;
	case 12:
		strcpy((*card)->name, "Q");
		break;
	case 13:
		strcpy((*card)->name, "K");
		break;
	}

	switch (suit) {
	case 's':
		strcpy((*card)->suit, "Spades");
		break;
	case 'c':
		strcpy((*card)->suit, "Clubs");
		break;
	case 'd':
		strcpy((*card)->suit, "Diamonds");
		break;
	case 'h':
		strcpy((*card)->suit, "Hearts");
		break;
	}
}

void CreateRandom(Card** head) {

	Card* tmp;
	Card* tmp2;
	*head = (Card*)malloc(sizeof(Card));
	CreateCard(head, 1, 's');
	tmp = *head;
	char suit = 's';
	for (int i = 2; i <= 13; i++) {
		tmp2 = (Card*)malloc(sizeof(Card));
		CreateCard(&tmp2, i, suit);
		tmp->pt = tmp2;
		tmp = tmp->pt;
	}

	for (int i = 1; i <= 3; i++) {
		if (i == 1) {
			suit = 'c';
		}
		else if (i == 2) {
			suit = 'h';
		}
		else {
			suit = 'd';
		}
		for (int i = 1; i <= 13; i++) {
			tmp2 = (Card*)malloc(sizeof(Card));
			CreateCard(&tmp2, i, suit);
			tmp->pt = tmp2;
			tmp = tmp->pt;
		}
	}
	tmp->pt = NULL;

	ShuffleDeck(head);


}

void ShuffleDeck(Card** HN) {
	Card* tmp;
	int count = CountDeck(*HN);
	for (int i = 1; i < 1000; i++) {
		int remove = my_rand(count) + 1;
		int add = my_rand(count) + 1;
		tmp = RemoveNthNode(HN, remove);
		AddCard(HN, tmp, add);
	}
}

// Generates random numbers 0 through (R_in - 1).
// This code was received from Dr. McBride during lecture
int my_rand(int R_iN) {
	time_t t;

	if (R_iN == 0) {
		/* Intializes random number generator */
		srand((unsigned)time(&t));
		return(0);
	}
	else
		return(rand() % R_iN);
}

int CountDeck(Card* head) {
	int i = 0;
	while (head != NULL) {
		i++;
		head = head->pt;
	}
	return(i);
}

// Removes the Nth node from the list and returns its address
// This function was from class during lecture
Card* RemoveNthNode(Card** HN, int N) {

	// NO ERROR CHECKING
	Card* t1, * t2;
	t1 = NULL;
	t2 = NULL;
	// Pull the first card from the top of the deck
	if (N == 1) {
		t1 = *HN;
		*HN = t1->pt;
		t1->pt = NULL;

	}
	else {
		t2 = ReturnNthNode(*HN, N - 1);
		t1 = t2->pt;
		t2->pt = t1->pt;
		t1->pt = NULL;
	}


	return (t1);

}

// This function is from lecture
void AddCard(Card** HN, Card* NodeToAdd, int N) {
	Card* tmp;
	if (N == 1) {
		tmp = *HN;
		*HN = NodeToAdd;
		NodeToAdd->pt = tmp;
	}
	else {
		tmp = ReturnNthNode(*HN, N - 1);
		NodeToAdd->pt = tmp->pt;
		tmp->pt = NodeToAdd;
	}
}

// This function is from class
Card* ReturnNthNode(Card* HN, int N) {

	// NO ERROR CHECKING
	int i = 1;
	while (i < N) {
		i++;
		HN = HN->pt;
	}
	return(HN);
}

void FreeNodes(Card* head) {
	Card* tmp;
	while (head != NULL) {
		tmp = RemoveNthNode(&head, 1);
		free(tmp);
	}
}