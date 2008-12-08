#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "items.h"

const char *ITEM_FILE = "items.def";

int BasePrices[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
					12, 15, 16, 17, 20, 25, 30, 35, 40, 45,
					50, 60, 75, 80, 100, 105, 150, 175, 200, 240,
					250, 300, 400, 500, 600, 700, 800, 900, 1200};

//int Charisma = 0;
//bool Angry = false;
//bool Sucker = true;
//bool Gem = false;

/* void ItemStateReset(ItemSection *reset) 
 * Puts the item structure back to their root items
 */

void ItemStateReset(ItemSection *reset) {
	ItemSection *root = reset;

	reset->item = reset->rootItem;	

	while (reset != NULL ) {
		reset->item = reset->rootItem;	
		reset->subsection = reset->rootSubsection;
		
		while (reset->subsection != NULL) {
			reset->subsection->item = reset->subsection->rootItem;
			reset->subsection = reset->subsection->next;
		}
		
		reset->subsection = reset->rootSubsection;
		
		reset = reset->next;
	}

	reset = root;
}

/* void UnloadItems() 
 * Cycle through the item structure and free all resources
 * We're shutting down shop! */

void UnloadItems(ItemSection *unload) {
	ItemStateReset(unload);
	
	while (unload != NULL ) {
		while (unload->subsection != NULL) {
			while (unload->subsection->item != NULL) {
				Item *next = unload->subsection->item->next;
				delete unload->subsection->item;
				unload->subsection->item = next;
			}

			ItemSection *next = unload->subsection->next;
			delete unload->subsection;
			unload->subsection = next;
		}

		while (unload->item != NULL) {
			Item *next = unload->item->next;
			delete unload->item;
			unload->item = next;
		}
			
		ItemSection *next = unload->next;
		delete unload;
		unload = next;
	}
}

/* ItemSection *LoadDefFiles()
 * Loads all the object definition files from the current process and loads
 * the data into memory */

ItemSection *LoadDefFiles() {
	FILE *file;
	size_t len,read;
	char *line = NULL;
	size_t i;
	char tagname[20];
	char symbol[2];
	bool inSection = false;
	bool inSubSection = false;
	int count = 0;
	ItemSection *section = NULL;
	ItemSection *rootSection = NULL;

	file = fopen(ITEM_FILE, "r");
	if (file == NULL) {
		return NULL;
	}
	
	while (ferror(file) == 0 && feof(file) == 0) {
		line = NULL;
		len = 0;
		read = 0;
		read = getline(&line, &len, file);
		
		if (read == -1)							// make sure we read something
			break;

		if ((strncmp(line, "[", 1)) == 0) { 	// is this an opening tag 
			if (strncmp(line, "[/", 2) == 0) { 	// is this a closing tag
				if (inSubSection) {
					inSubSection = false;
				}
				else {
					inSection = false;
				}

				i = strcspn(line + 2, "]");
				snprintf(tagname, (int)i + 1, "%s", line + 2);
			}
			else {
				char *ret = NULL;
				if ((ret = strchr(line + 1, ',')) != NULL) {	// main section
					inSection = true;
					if (section == NULL) {
						section = new ItemSection();
						rootSection = section;
					}
					else {
						section->next = new ItemSection();
						section = section->next;
					}

					int pos = ret - line + 1;
					snprintf(section->name, pos - 1, "%s", line + 1);
					snprintf(section->symbol, 2, "%s", line + pos);	

				}
				else {											// sub section
					if (section->subsection == NULL) {			// remember the root section node
						section->subsection = new ItemSection();
						section->rootSubsection = section->subsection;
					}
					else {
						section->subsection->next = new ItemSection();
						section->subsection = section->subsection->next;
					}

					inSubSection = true;

					size_t len = strcspn(line + 1, "]");
					snprintf(section->subsection->name, (int)len + 1 , "%s", line + 1);
				}
			}
		}
		else {	// normal item
			if (read > 1 && line[1] != ';') {
				char *ret = NULL;
				ItemSection *current = NULL;
			
				if (inSubSection) {
					current = section->subsection;
				}
				else {
					current = section;
				}

				if (current->item == NULL) {
					current->item = new Item();
					current->rootItem = current->item;			// remember the (sub)sections root item
				}
				else {
					current->item->next = new Item();
					current->item = current->item->next;
				}
	
				size_t len = 0;

				// get item name
				len = strcspn(line, ",");
				snprintf(current->item->name, (int)len + 1, "%s", line);

				// get item price
				size_t pos = len + 1;
				len = strcspn(line + pos, ",");
				char tmp[10];
				memset(&tmp, '\0', sizeof(tmp));
				snprintf(tmp, (int)len + 1, "%s", line + pos);
				current->item->price = atoi(tmp);

				pos = pos + len + 1;
				len = strcspn(line + pos, "\n");
				snprintf(current->item->description, (int)len + 1, "%s", line + pos);

			}

		}
		
	}
	if (file != NULL) {
		fclose(file);
	}

	// return the first section
	// NOTE: we still need to do a state reset on this!
	section = rootSection;

	return section;
}

/* Item *FindItems(int zorks, char *symbol, char *name, char *description)
 * price - filter the result by price
 * symbol - filter by item type eg: spellbooks +
 * name - filter by name. either item or subsection name
 * description - filter by item description
 *
 * RETURN: returns a linked list of ItemSection */

ItemSection *FindItems(ItemSection *section, int price, char *symbol, \
		char *name, char *description){
	ItemSection *result = NULL; 
	ItemSection *rootResult = NULL;
	int found = 0;

	// NOTE: must do a manual ItemStateReset on the section
	//ItemStateReset(section);

	bool loop = true;
	while (section != NULL && loop) {		
		if (symbol == NULL || strcmp(section->symbol, symbol) == 0) {	
			// cycle through section items
			while (section->item != NULL) { 
				bool match = false;

				// match on price
				if (price == -1 || (price != -1 && section->item->price == price)) {
					match = true;
				}
			
				// match on name
				if (name != NULL && strstr(section->item->name, name) != NULL) {
					if (price != -1 && !match) {
					match = false;
					}
					else {
						match = true;
					}
				}
				else if (name != NULL) {
					match = false;
				}

				// match on description
				if (description != NULL && strstr(section->item->description, description) != NULL) {
					if (price != -1 && name != NULL && !match) {
						match = false;
					}
					else {
						match = true;
					}
				}
				else if (description != NULL) {
					match = false;
				}
		
				// we've got a match!
				if (match) {
					if (result == NULL ) {
						result = new ItemSection();

						memcpy(result->name, section->name, sizeof(section->name));
						memcpy(result->symbol, section->symbol, sizeof(section->symbol));
		
						if (rootResult == NULL) {
							rootResult = result;
						}
					}
					else {
						if (\
						strcmp(result->symbol, section->symbol) != 0) {
							result->next = new ItemSection();
							result = result->next;

							memcpy(result->name, section->name, sizeof(section->name));
							memcpy(result->symbol, section->symbol, sizeof(section->symbol));
						}
					}
					
					// check if this is the first item we've matched
					// if it is, create the root node
					// if not, assign ->next
					if (result->item == NULL) {
						result->item = new Item();
						result->rootItem = result->item;
					}
					else {
						result->item->next = new Item();
						result->item = result->item->next;
					}
					
	
					memcpy(result->item->name, section->item->name, sizeof(section->item->name));
					result->item->price = section->item->price;
					memcpy(result->item->description, section->item->description, \
						sizeof(section->item->description));
				}
	
				section->item = section->item->next;
			}
	
			// cycle through subsection items
			while (section->subsection != NULL) {
				while (section->subsection->item != NULL) {
					bool match = false;
							
					// match on price
					if (price == -1 || (price != -1 && section->subsection->item->price == price)) {
						match = true;
					}
				
					// match on name
					if (name != NULL && strstr(section->subsection->item->name, name) != NULL) {
						if (price != -1 && !match) {
							match = false;
						}
						else {
							match = true;
						}
					}
					else if (name != NULL) {
						match = false;
					}
	
					// match on description
					if (description != NULL && strstr(section->subsection->item->description, \
							description) != NULL) {
						if (price != -1 && name != NULL && !match) {
							match = false;
						}
						else {
							match = true;
						}
					}
					else if (description != NULL) {
						match = false;
					}
		
					// we've got a match!
					if (match) {
						// if we haven't created a section yet, do so
						if (result == NULL ) {
							result = new ItemSection();

							memcpy(result->name, section->name, sizeof(section->name));
							memcpy(result->symbol, section->symbol, sizeof(section->symbol));
		
							if (rootResult == NULL) {
								rootResult = result;
							}
						}
						else {
							if (\
							strcmp(result->symbol, section->symbol) != 0) {
								result->next = new ItemSection();
								result = result->next;

								memcpy(result->name, section->name, sizeof(section->name));
								memcpy(result->symbol, section->symbol, sizeof(section->symbol));
							}
						}
						
						// check if this is the first item we've matched
						// if it is, create the root node
						// if not, assign ->next
						if (result->item == NULL) {
							result->item = new Item();
							result->rootItem = result->item;
						}
						else {
							result->item->next = new Item();
							result->item = result->item->next;
						}
	
						memcpy(result->item->name, section->subsection->item->name, \
							sizeof(section->subsection->item->name));
						result->item->price = section->subsection->item->price;
						memcpy(result->item->description, section->subsection->item->description, \
							sizeof(section->subsection->item->description));
					}
				
					section->subsection->item = section->subsection->item->next;
				}
				
				section->subsection = section->subsection->next;
			}
			
		}
		
		if (result != NULL && result->item != NULL)	// only if we found something	
			result->item = result->rootItem;

		section = section->next;
	}
		
	return rootResult;
}


/* float CharismaAdjustmentFactor()
 * Using a combination of charisma and other character attributes, determine 
 * the adjustment applied to the item for sale at a shop */

float CharismaAdjustmentFactor(int Charisma) {
	float factor = 1;
	// Charisma adjustments
	if (Charisma < 6) {
		factor += 1;	// add 100% markup
	}
	else if (Charisma == 6 || Charisma == 7) {
		factor += 0.5;	// add 50% markup
	}
	else if (Charisma >= 8 && Charisma <= 10) {
		factor += 0.33;	// add 33% markup
	}
	else if (Charisma >= 11 && Charisma <= 15) {
		// no markup
	}
	else if (Charisma >= 16 && Charisma <= 17) {
		factor -= 0.25;	// 25% off
	}
	else if (Charisma == 18) {
		factor -= 0.33; // 33% off
	}
	else if (Charisma > 18) {
		factor -= 0.5; 	// 50% off
	}
	else {
		// Should never get here
	}

	return factor;
}

/* MiscAdjustmentFactor()
 * Determine adjustment factor from optional toggles */

float MiscAdjustmentFactor(bool Angry, bool Sucker, bool Gem) {
	float factor = 1;
	
	// add an extra 33% if shop keeper is angry
	if (Angry) {
		factor += 0.33;
	}

	if (Sucker) {
		factor += 0.33;
	}

	return factor;
}

int DetermineBase(PriceType type, float zorks, int Charisma, bool Angry, bool Sucker, bool Gem) {
	if (type == SHOP_SELL) { 		// if we are buying
		float factor = CharismaAdjustmentFactor(Charisma);
		if (Angry) {
			factor += 0.33;
		}
		if (Sucker) {
			factor += 0.33;
		}
		//printf("%f\n", factor);
		zorks /= factor;
	}
	else if (type == SHOP_BUY) {	// if we are selling
		float factor = 0;
		if (Sucker) {
			factor += 0.33;
		}
		else {
			factor += 0.5;
		}
		zorks /= factor;
	}

	int base = (int)zorks;
	//printf("%f\n", zorks);

	// get closest match
	float minDiff = base - BasePrices[0];
	int indDiff = 0;
	float curDiff = minDiff;
	for (int i = 1; i < (sizeof(BasePrices) / sizeof(int)); i++) {
		if (base < BasePrices[i])
			curDiff = BasePrices[i] - base;
		else if (base > BasePrices[i])
			curDiff = base - BasePrices[i];
		else if (base == BasePrices[i])
			return BasePrices[i];

		if (curDiff < minDiff) {
			minDiff = curDiff;
			indDiff = i;
		}
	}
	base = BasePrices[indDiff];
	return base;
}
