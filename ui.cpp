#include <curses.h>
#include <stdlib.h>
#include <string.h>
#include "ui.h"
#include "items.h"

WINDOW *screen;
WINDOW *msgwin;
WINDOW *mainwin;
WINDOW *statuswin;

extern ItemSection *rootSection;
extern ItemSection *section;

void InitUI() {
	// create curses screen
	screen = initscr();
	if (screen == NULL) { 
		printf("Failed to init curses.\n");
		return;
	}

	// enable key capture
	keypad(screen, TRUE);
	nonl();
	cbreak();
	noecho();

	refresh(); // why?
	wrefresh(screen);

	if (has_colors()) {
		start_color();
		
		init_pair(1, COLOR_WHITE, COLOR_BLACK);
		init_pair(2, COLOR_RED, COLOR_BLACK);
	}

	msgwin = newwin(1, 80, 0, 0);
	mainwin = newwin(22, 80, 1, 0);
	statuswin = newwin(1, 80, 23, 0);
}

void UILoop() {
	float zorks = -1;
	int base = -1;
	int charisma = 1;
	char *symbol = NULL;
	char *name = NULL;
	char *description = NULL;
	bool angry = false;
	bool sucker = false;
	bool gems = false;
	bool buy = true;

	ShowMessage("Welcome to shophack");

	ShowStatus(zorks, base, charisma, symbol, name, description);

	for (;;) {
		int c = getch();
		if (c == 'q') {
			ShowMessage("Are you sure you want to quit? (y/n) ");
			if (getch() == 'y') {
				break;
			}
			else {
				werase(msgwin);
				wrefresh(msgwin);
				refresh();
			}
		}
		else if (c == 's') {
			section = rootSection;
			ItemStateReset(section);
			ShowItems(section);
		}
		else if (c == ' ') {
			ShowItems(section);
		}
		else if (c == '$') { // filter by price
			// start with full list
			ItemStateReset(rootSection);
			section = rootSection;
			
			ItemSection *results = NULL; 
			char *str = new char[6];
			GetUserInput(msgwin, 0, 0, "Enter price amount : $", 5, str);
			if (str == NULL || strlen(str) == 0) {
				zorks = -1;
				base = -1;
			}
			else {
				zorks = atoi(str);
			}
			delete str;

			if (zorks != -1) {
				if (buy == true) {
					base = DetermineBase(SHOP_SELL, zorks, charisma, angry, sucker, gems); 
				}
				else {
					base = DetermineBase(SHOP_BUY, zorks, charisma, angry, sucker, gems); 	
				}
			}

			results = FindItems(section, base, symbol, name, description);
			ShowStatus((int)zorks, base, charisma, symbol, name, description);

			if (results != NULL) {
				ItemStateReset(results);
				ShowItems(results);
				delete results;
			}
			else {
				werase(mainwin);
				wrefresh(mainwin);
				refresh();
			}
		}
		else if (c == ';') {
			ItemStateReset(rootSection);
			section = rootSection;
			
			ItemSection *results = NULL;
			char *str = new char[2];
			GetUserInput(msgwin, 0, 0, "Enter object type: ", 1, str);

			if (str != NULL && strlen(str) > 0) {
				if (symbol == NULL) {
					symbol = new char[2];
				}
				sprintf(symbol, "%s", str);
			}
			else {
				if (symbol != NULL) {
					delete symbol;
					symbol = NULL;
				}
			}
			delete str;

			if (zorks != -1) {
				if (buy == true) {
					base = DetermineBase(SHOP_SELL, zorks, charisma, angry, sucker, gems); 
				}
				else {
					base = DetermineBase(SHOP_BUY, zorks, charisma, angry, sucker, gems); 	
				}
			}
	
			results = FindItems(section, base, symbol, name, description);
			ShowStatus(zorks, base, charisma, symbol, name, description);

			if (results != NULL) {
				 ItemStateReset(results);
				 ShowItems(results);
				 delete results;
			}
			else {
				werase(mainwin);
				wrefresh(mainwin);
				refresh();
			}
		}
		else if (c == '#') {
			ItemStateReset(rootSection);
			section = rootSection;

			ItemSection *results = NULL;
			char *str = new char[30];
			GetUserInput(msgwin, 0, 0, "Enter object name: ", 30, str);
			
			if (str != NULL && strlen(str) > 0) {
				if (name == NULL) {
					name = new char[30];
				}
				sprintf(name, "%s", str);
				delete str;
			}
			else {
				if (name != NULL) {
					delete name;
					name = NULL;
				}
			}
		
			if (zorks != -1) {
				if (buy == true) {
					base = DetermineBase(SHOP_SELL, zorks, charisma, angry, sucker, gems); 
				}
				else {
					base = DetermineBase(SHOP_BUY, zorks, charisma, angry, sucker, gems); 	
				}
			}

			results = FindItems(section, base, symbol, name, description);
			ShowStatus(zorks, base, charisma, symbol, name, description);

			if (results != NULL) {
				 ItemStateReset(results);
				 ShowItems(results);
				 delete results;
			}
			else {
				werase(mainwin);
				wrefresh(mainwin);
				refresh();
			}
		}
		else if (c == '/') {
			ItemStateReset(rootSection);
			section = rootSection;

			ItemSection *results = NULL;
			char *str = new char[30];
			GetUserInput(msgwin, 0, 0, "Enter object description: ", 30, str);
			
			if (str != NULL && strlen(str) > 0) {
				if (description == NULL) {
					description = new char[30];
				}
				sprintf(description, "%s", str);
				delete str;
			}
			else {
				if (description != NULL) {
					delete description;
					description = NULL;
				}
			}
			
			if (zorks != -1) {
				if (buy == true) {
					base = DetermineBase(SHOP_SELL, zorks, charisma, angry, sucker, gems); 
				}
				else {
					base = DetermineBase(SHOP_BUY, zorks, charisma, angry, sucker, gems); 	
				}
			}

			results = FindItems(section, base, symbol, name, description);
			ShowStatus(zorks, base, charisma, symbol, name, description);

			if (results != NULL) {
				 ItemStateReset(results);
				 ShowItems(results);
				 delete results;
			}
			else {
				werase(mainwin);
				wrefresh(mainwin);
				refresh();
			}
		}
		else if (c == 'c') { // filter by charisma 
			// start with full list
			ItemStateReset(rootSection);
			section = rootSection;
			
			ItemSection *results = NULL; 
			char *str = new char[6];
			GetUserInput(msgwin, 0, 0, "Enter character charisma: ", 2, str);
			if (str == NULL || strlen(str) == 0) {
				charisma = 12;
			}
			else {
				charisma = atoi(str);
			}
			delete str;

			if (zorks != -1) {
				if (buy == true) {
					base = DetermineBase(SHOP_SELL, zorks, charisma, angry, sucker, gems); 
				}
				else {
					base = DetermineBase(SHOP_BUY, zorks, charisma, angry, sucker, gems); 	
				}
			}

			results = FindItems(section, base, symbol, name, description);
			ShowStatus((int)zorks, base, charisma, symbol, name, description);

			if (results != NULL) {
				ItemStateReset(results);
				ShowItems(results);
				delete results;
			}
			else {
				werase(mainwin);
				wrefresh(mainwin);
				refresh();
			}
		}
		else if (c == 'h' || c == '?') {	// show the help
			werase(mainwin);
			wrefresh(mainwin);
			refresh();

			ShowMessage("Press any key to continue...");
			getch();	// wait for any key press

			ItemSection *results = FindItems(section, base, symbol, name, description);
			ShowStatus((int)zorks, base, charisma, symbol, name, description);

			if (results != NULL) {
				ItemStateReset(results);
				ShowItems(results);
				delete results;
			}
			else {
				werase(mainwin);
				wrefresh(mainwin);
				refresh();
			}
		}

		werase(msgwin);
		wrefresh(msgwin);
	}

	// clear some mem
/*	
 	delete name;
	delete symbol;
	delete description;
*/
}

void GetUserInput(WINDOW *win, int x, int y, char *message, int length, char *value) {
	werase(win);
	wrefresh(win);
	refresh();
	mvwaddstr(win, y, x, message);
	echo();
	mvwgetnstr(win, y, x + strlen(message), value, length);
	noecho();
	wrefresh(msgwin);
	refresh();
}

void ShowMessage(char *message) {
	werase(msgwin);
	mvwaddstr(msgwin, 0, 0, message);
	wrefresh(msgwin);
	refresh();
}

void ShowStatus(float price, int base, int charisma, char *type, char *name, char *description) {
	char *message = new char[80];

	werase(statuswin);

	// $
	wattron(statuswin, COLOR_PAIR(1) | A_BOLD);	
	mvwaddstr(statuswin, 0, 0, "$: ");
	wattroff(statuswin, COLOR_PAIR(2) | A_BOLD);
	memset(message, '\0', sizeof(message));
	if (price == -1) {
		sprintf(message, "all/");	
	}
	else {
		sprintf(message, "%2.f/", price);
	}
	waddstr(statuswin, message);
	wattron(statuswin, COLOR_PAIR(2) | A_BOLD);
	memset(message, '\0', sizeof(message));
	if (base == -1) {
		sprintf(message, "all ");
	}
	else {
		sprintf(message, "%d ", base);
	}
	waddstr(statuswin, message);
	wattroff(statuswin, COLOR_PAIR(2) | A_BOLD);

	// char
	wattron(statuswin, COLOR_PAIR(1) | A_BOLD);
	waddstr(statuswin, "CHA: ");
	wattroff(statuswin, COLOR_PAIR(1) | A_BOLD);
	memset(message, '\0', sizeof(message));
	sprintf(message, "%d ", charisma);
	waddstr(statuswin, message);

	// type 
	wattron(statuswin, COLOR_PAIR(1) | A_BOLD);
	waddstr(statuswin, "TYPE: ");
	wattroff(statuswin, COLOR_PAIR(1) | A_BOLD);
	memset(message, '\0', sizeof(message));
	if (type == NULL) {
		sprintf(message, "all ");
	}
	else {
		sprintf(message, "%s ", type);
	}
	waddstr(statuswin, message);

	// name 
	wattron(statuswin, COLOR_PAIR(1) | A_BOLD);
	waddstr(statuswin, "NAME: ");
	wattroff(statuswin, COLOR_PAIR(1) | A_BOLD);
	memset(message, '\0', sizeof(message));
	if (name == NULL) {
		sprintf(message, "all ");
	}
	else {
		sprintf(message, "%s ", name);
	}
	waddstr(statuswin, message);

	// name 
	wattron(statuswin, COLOR_PAIR(1) | A_BOLD);
	waddstr(statuswin, "DESC: ");
	wattroff(statuswin, COLOR_PAIR(1) | A_BOLD);
	memset(message, '\0', sizeof(message));
	if (description == NULL) {
		sprintf(message, "all ");
	}
	else {
		sprintf(message, "%s ", description);
	}
	waddstr(statuswin, message);

	if (message != NULL)
		delete message;
	
	wrefresh(statuswin);
	refresh();
}

void ShowItems(ItemSection *results) {
	int count = 0; // used for pages
	int maxlines = 22;
	werase(mainwin);

	while (count < maxlines && results != NULL) {
		if (results->item != NULL || results->subsection != NULL) {
			wattron(mainwin, A_REVERSE);
			mvwaddstr(mainwin, count++, 0, results->name);
			wattroff(mainwin, A_REVERSE);
		}

		while (count < maxlines && results->item != NULL) {
			char *str = new char[80];
			sprintf(str, "%s - %d - (%s)", results->item->name, results->item->price, \
				results->item->description);

			mvwaddstr(mainwin, count++, 0, str);
			results->item = results->item->next;

			if (str != NULL) {
				delete str;
			}
		}

		while (count < maxlines && results->subsection != NULL) {
			while (count <= maxlines && results->subsection->item != NULL) {
				mvwaddstr(mainwin, count++, 0, results->subsection->item->name);

				results->subsection->item = results->subsection->item->next;
			}
			results->subsection = results->subsection->next;
		}

		// this allows us to start from the section we left
		if (count >= maxlines) { 
			break;
		}
		else {
			results = results->next;
		}
	}

	section = results;
	wrefresh(mainwin);
	refresh();
}

void EndUI() {
	endwin();
	echo();

}

