#ifndef ITEMS_H
#define ITEMS_H

typedef int PriceType;
const PriceType SHOP_BUY = 0;
const PriceType SHOP_SELL = 1;


struct Item {
	char name[30];
	int price;
	char description[30];
	bool identified;
	
	Item *next;
};

struct ItemSection {
	char name[30];
	char symbol[2];

	Item *item;
	Item *rootItem;

	ItemSection *subsection;
	ItemSection *rootSubsection;
	ItemSection *next;
};

void ItemStateReset(ItemSection*);
void UnloadItems(ItemSection*);
ItemSection *LoadDefFiles();
ItemSection *FindItems(ItemSection *, int, char *, char *, char *);
float CharismaAdjustmentFactor(int);
float MiscAdjustmentFactor(bool, bool, bool);
int DetermineBase(PriceType, float, int, bool, bool, bool);

#endif 
