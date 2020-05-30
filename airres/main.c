/*
 * airres.c
 *
 *  Created on: Apr. 10, 2020
 *      Author: Alyson Pickett
 */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <conio.h>


struct flight {
	char flightnum[15];
	char departcity[15];
	char arrivecity[15];
	int departyear;
	int departmonth;
	int departday;
};

struct reservation {
	int seat;
	char lastname[15];
	char firstname[15];
};


void menu (void){
	int choice;
	printf("\tMake your choice:\n"
			"\t 1. \t Make a new flight \n"
			"\t 2. \t Pick a flight \n"
			"\t 3. \t View active flights \n"
			"\t 4. \t Return to Welcome Screen\n\n");
	scanf("%d", &choice);
		if (choice == 1){
		addflight();
	} else if (choice == 2) {
		pickflight();
	} else if (choice == 3) {
		displayflights();
	} else {
		start();
	};
};


void addflight(void){
	FILE *flight;
	FILE *allflights;
	//char flightno[10];

	struct flight newflight;
	int j;

	printf("Enter new flight number:");
	scanf("%s", &newflight.flightnum);
	printf("Enter departure city:");
	scanf("%s", &newflight.departcity);
	printf("Enter destination city:");
	scanf("%s", &newflight.arrivecity);
	printf("Enter departure year:");
	scanf("%d", &newflight.departyear);
	printf("Enter departure month:");
	scanf("%d", &newflight.departmonth);
	printf("Enter departure day:");
	scanf("%d", &newflight.departday);


	allflights = fopen("flightnumbers.dat", "a+");
	fprintf(allflights,"%s %s %s %d %d %d\n", newflight.flightnum, newflight.departcity, newflight.arrivecity, newflight.departyear, newflight.departmonth, newflight.departday);
	fclose(allflights);


	strncat(newflight.flightnum, ".dat", 4);

	flight = fopen(newflight.flightnum, "w");
	struct reservation new = {0, "NO", "PASSENGER"};

	for (j=1; j<=12; j++){
		new.seat = j;

		fprintf(flight, "%d %s %s\n", new.seat, new.lastname, new.firstname);
	}

	fclose(flight);
	menu();
}


void pickflight(void){
	FILE *flight;
	char searchedflight[15];
	int choice;

	enter: printf("Enter flight number:\n");
	scanf("%s", &searchedflight);
	strncat(searchedflight, ".dat", 4);

	flight = fopen(searchedflight, "r");

	if(flight==NULL) {
		printf("That flight doesn't exist\n");
		printf("Do you want to try again?\n"
				"\t 1. \t Yes\n"
				"\t 2. \t No\n");
		scanf("%d", &choice);
		if (choice == 1){
			goto enter;
		} else {
			menu();
		}
	}

	fclose(flight);
	flightoptions(searchedflight);
}

void flightoptions(char* flightno){
	FILE *flight;
	int choice;
	struct reservation exist;

	path: printf("What do you want to do?\n"
		"\t 1. \t View Bookings\n"
		"\t 2. \t Make a new booking\n"
		"\t 3. \t Exit to Main Menu\n\n");
	scanf("%d", &choice);

	if (choice == 1){
		flight = fopen(flightno, "r");
		while(fscanf(flight, "%d %s %s\n", &exist.seat, &exist.lastname, &exist.firstname)!=EOF) {
			printf("%d - %s, %s\n", exist.seat, exist.lastname, exist.firstname);
		};
		fclose(flight);
		goto path;
	} else if (choice ==2) {
		newbooking(flightno);
	} else {
		menu();
	}


};



void newbooking(char* flightno){

	FILE *oldflight;
	FILE *newflight;

	int seatselect = 0;
	struct reservation old;
	struct reservation new;

	printf("\t **Flight Selected is: %s**\n", flightno);
	seatpick: printf("\t Which seat would you like to book? Enter Number: \n");
	scanf("%d", &seatselect);

	oldflight = fopen(flightno, "r");
	newflight = fopen("new.dat", "w");
	char cmpstringlast[] = "NO";
	char cmpstringfirst[] = "PASSENGER";

	while(fscanf(oldflight, "%d %s %s\n", &old.seat, &old.lastname, &old.firstname)!=EOF) {
		if (old.seat == seatselect){
			int resultlast = strcmp(cmpstringlast, old.lastname);
			int resultfirst = strcmp(cmpstringfirst, old.firstname);
			if (resultlast == 0){
				new.seat = old.seat;
				printf("Enter Passenger Last Name:\n");
				scanf("%s", &new.lastname);
				printf("Enter Passenger First Name:\n");
				scanf("%s", &new.firstname);
				fprintf(newflight, "%d %s %s\n", new.seat, new.lastname, new.firstname);
			} else {
				printf("That seat is already booked!");
				fclose(oldflight);
				fclose(newflight);
				goto seatpick;
			}

		} else {
			new.seat= old.seat;
			strncpy(new.lastname, old.lastname, 15);
			strncpy(new.firstname, old.firstname, 15);
			fprintf(newflight, "%d %s %s\n", new.seat, new.lastname, new.firstname);

		};
	};

	fclose(newflight);
	fclose(oldflight);


	printf("Updated reservation successful!\n\n");



	remove(flightno);
	rename("new.dat", flightno);

	printf("Returning to menu\n\n\n");


	flightoptions(flightno);

}

void displayflights(void){

	FILE *list;
	struct flight recall;
	list = fopen("flightnumbers.dat", "r");
	printf("Flights in system:\n");
	while(fscanf(list, "%s %s %s %d %d %d", &recall.flightnum, &recall.departcity, &recall.arrivecity, &recall.departyear, &recall.departmonth, &recall.departday)!=EOF){
		printf("\t %s - %s to %s on %d/%d/%d\n", recall.flightnum, recall.departcity, recall.arrivecity, recall.departyear, recall.departmonth, recall.departday);
	};
	printf("\n");


	menu();
}
int start (	){
    int choice;
    system("cls");
	printf("\n\tWelcome to Puddle Jumper Airways!\n "
			"\t 1. Proceed"
			"\t 2. Exit");
	scanf("%d", &choice);
	if (choice ==1){
        system("cls");
		menu();
	}
};

int main() {
    system ("cls");
    start();

	return (0);

}
