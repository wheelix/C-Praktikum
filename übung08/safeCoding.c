#define _GNU_SOURCE
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* formatMoney(int64_t cents) {
	bool isNegative = cents < 0;
	cents = (isNegative ? -cents : cents);
	int64_t euros = cents/100;
	cents -= 100*euros;

	size_t characterCount = 0;
	if(isNegative) characterCount++;	//space for the sign
	for(int64_t i = euros; i; i /= 10) characterCount++;	//space for the full euros
	if(!euros) characterCount++;	//space for the zero if we don't have full euros
	characterCount += 3;	//space for the dot and the two cent digits
	characterCount += strlen(" euros");	//space for unit string

	char* result = malloc(characterCount + 1);	//+1 for the terminating null byte
	sprintf(result, "%s%"PRId64".%02"PRId64" euros", (isNegative ? "-" : ""), euros, cents);
	return result;
}

char* formatMoney1(int64_t cents) {
	bool isNegative = cents < 0;
	cents = (isNegative ? -cents : cents);
	int64_t euros = cents/100;
	cents -= 100*euros;

	char* result;
	asprintf(&result, "%s%"PRId64".%02"PRId64" euros", (isNegative ? "-" : ""), euros, cents);
	return result;
}
void formatMoney2(int64_t cents, FILE* stream) 
{
	bool isNegative = cents < 0;
	cents = (isNegative ? -cents : cents);
	int64_t euros = cents/100;
	cents -= 100*euros;
	
	fprintf(stream, "%s%"PRId64".%02"PRId64" euros", (isNegative ? "-" : ""), euros, cents);
}
char* formatMoney3(int64_t cents) {
	char *result;
	size_t size;
	
	FILE *stream = open_memstream(&result, &size);
	formatMoney2(cents, stream);
	fclose(stream);
	
	return result;
}

void printMoney(uint64_t cents, char* (*formatter)(int64_t cents)) {
	char* string = formatter(cents);
	printf("formatMoney(%"PRId64", %p) = %s\n", cents, formatter, string);
	free(string);
}

int main() {
	printMoney(-196720000000000, formatMoney);
	printMoney(-314, formatMoney);
	printMoney(-31, formatMoney);
	printMoney(-3, formatMoney);
	printMoney(0, formatMoney);
	printMoney(2, formatMoney);
	printMoney(27, formatMoney);
	printMoney(271, formatMoney);

	//XXX Bitte auskommentieren, falls Ihr auf Windows arbeitet und formatMoney1 nicht implementiert.
	printMoney(-196720000000000, formatMoney1);
	printMoney(-314, formatMoney1);
	printMoney(-31, formatMoney1);
	printMoney(-3, formatMoney1);
	printMoney(0, formatMoney1);
	printMoney(2, formatMoney1);
	printMoney(27, formatMoney1);
	printMoney(271, formatMoney1);

	printMoney(-196720000000000, formatMoney3);
	printMoney(-314, formatMoney3);
	printMoney(-31, formatMoney3);
	printMoney(-3, formatMoney3);
	printMoney(0, formatMoney3);
	printMoney(2, formatMoney3);
	printMoney(27, formatMoney3);
	printMoney(271, formatMoney3);
}
