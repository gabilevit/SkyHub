#define _USE_MATH_DEFINES
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Flight.h"
#include "General.h"


void	initFlight(Flight* pFlight, Plane* thePlane, const AirportManager* pManager)
{
	Airport* pPortOr = setAiportToFlight(pManager, "Enter code of origin airport:");
	strcpy(pFlight->sourceCode, pPortOr->code);
	int same;
	Airport* pPortDes;
	do {
		pPortDes = setAiportToFlight(pManager, "Enter code of destination airport:");
		same = isSameAirport(pPortOr, pPortDes);
		if (same)
			printf("Same origin and destination airport\n");
	} while (same);
	strcpy(pFlight->destCode, pPortDes->code);
	pFlight->flightPlane = *thePlane;
	getCorrectDate(&pFlight->date);
}

int		isFlightFromSourceAirport(const Flight* pFlight, const char* code)
{
	if (strcmp(pFlight->sourceCode, code) == 0)
		return 1;

	return 0;
}


int		isFlightToDestAirport(const Flight* pFlight, const char* code)
{
	if (strcmp(pFlight->destCode, code) == 0)
		return 1;

	return 0;


}

int		isPlaneTypeInFlight(const Flight* pFlight, ePlaneType type)
{
	return (pFlight->flightPlane.type == type);
}


void	printFlight(const Flight* pFlight)
{
	printf("Flight From %s To %s\t", pFlight->sourceCode, pFlight->destCode);
	printDate(&pFlight->date);
	printf("\t");
	printPlane(&pFlight->flightPlane);
}

Airport* setAiportToFlight(const AirportManager* pManager, const char* msg)
{
	char code[MAX_STR_LEN];
	Airport* port;
	do
	{
		printf("%s\t", msg);
		getAirportCode(code);
		port = findAirportByCode(pManager, code);
		if (port == NULL)
			printf("No airport with this code - try again\n");
	} while (port == NULL);

	return port;
}

void* compareFlightBySourceCode(const void* pF1, const void* pF2)
{
	const Flight* f1 = *(const Flight**)pF1;
	const Flight* f2 = *(const Flight**)pF2;

	return strcmp(f1->sourceCode, f2->sourceCode);
}

void* compareFlightByDestCode(const void* pF1, const void* pF2)
{
	const Flight* f1 = *(const Flight**)pF1;
	const Flight* f2 = *(const Flight**)pF2;

	return strcmp(f1->destCode, f2->destCode);
}

void* compareFlightByDate(const void* pF1, const void* pF2)
{
	const Flight* f1 = *(const Flight**)pF1;
	const Flight* f2 = *(const Flight**)pF2;

	if (f1->date.year - f2->date.year == 0)
	{
		if (f1->date.month - f2->date.month == 0)
		{
			if (f1->date.day - f2->date.day == 0)
				return 0;
			else return f1->date.day - f1->date.day;
		}
		else return f1->date.month - f2->date.month;
	}
	else return f1->date.year - f2->date.year;
}

int saveFlightToFile(FILE* f, Flight* pFlight)
{
	int len = strlen(pFlight->sourceCode);
	if (fwrite(&len, sizeof(int), 1, f) != 1)
		return 0;
	if (fwrite(pFlight->sourceCode, sizeof(char), len, f) != len)
		return 0;
	if (fwrite(&len, sizeof(int), 1, f) != 1)
		return 0;
	if (fwrite(pFlight->destCode, sizeof(char), len, f) != len)
		return 0;
	if (fwrite(&pFlight->flightPlane.serialNum, sizeof(int), 1, f) != 1)
		return 0;
	if (!saveDateToFile(f, &pFlight->date))
	{
		fclose(f);
		return;
	}
	return 1;
}