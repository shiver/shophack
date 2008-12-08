#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "items.h"
#include "ui.h"

ItemSection *section;
ItemSection *rootSection;

int main() {
	// load the item definition files
	rootSection = LoadDefFiles();
	section = rootSection;

	InitUI();

	UILoop();

	EndUI();
	
	section = rootSection;
	ItemStateReset(section);
	UnloadItems(section);

	
	
	
	/*ItemStateReset(section);
	ItemSection *result = FindItems(section, -1, "[", NULL, NULL);
	ItemSection *rootResult = result;

	int count = 0;
	while (result != NULL) {
		printf("%d: [%s]\n", count, result->name);
		
		while (result->subsection != NULL) {
			printf("%d: [%s]\n", count, result->subsection->name);
			
			while (result->subsection->item != NULL) {
				printf("%d: %s $%d (%s) \n", count, result->subsection->item->name, \
					result->subsection->item->price, \
				result->subsection->item->description);
				result->subsection->item = result->subsection->item->next;
			}
			printf("%d: [/%s]\n", count, result->subsection->name);

			result->subsection = result->subsection->next;
		}

		while (result->item != NULL) {
			printf("%d: %s $%d (%s)\n", count, result->item->name, result->item->price, \
					result->item->description);
			result->item = result->item->next;
		}

		printf("%d: [/%s]\n", count, result->name);
		result = result->next;
		count++;
	}

	result = rootResult;
	UnloadItems(result);*/

	/*Charisma = 5;
	printf("Cha: %d\n", Charisma);
	double price = 113;
	printf("Sell: %f Base: %d\n", price, DetermineBase(SHOP_BUY, price));*/

	// buying has a possible random 33% markup
	// selling has a possible random 25% markdown
	// selling for credit will only give 90% of the item value

	return 0;	
}
