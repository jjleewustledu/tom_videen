//static char sccsid[] = "%W% UCL-TOPO %E%";
#include <Lists/charlist.h>
#include <Lists/intlist.h>
#include <stdio.h>
#include <ctype.h>
#include <strings.h>

#define ctrl(c)		( c & 077 )
static char* SEPARATOR = "______";
static char* MENU = "menu";
static char* END = "end";

struct MenuInfo {
    const char* str;        // label
/*
    AppTypeAction func;     // action
*/
    const char* msg;        // messageId (for macro file)
    char key;           // menu key
    int status;         // status needed (enabled)
    int mask;           // status unneeded (disabled)
    MenuInfo* submenu;      // [submenu]
    int options;        // [submenu option]
};

declareList(MenuInfos,MenuInfo)
implementList(MenuInfos,MenuInfo)

static int decode(char* line, char*& name, char*& value) {
	int ret = 0;
	char* tail = 0;
	char* start = line;
	while (isspace(*start)) start++;	// skip leading spaces
	if (*start == '#' || *start=='\0') return 0;	// comment or blank line
	start[strlen(start)-1] = '\0';                  // clear trailing newline
	if (strchr(start,'"')) {
		name = strtok(start,"\"");
		value = strtok(NULL,"\"");
	} else {
		name = start;
		while (isspace(*name)) name++;
		value = name;
		while (*value && !isspace(*value)) value++;
		if (*value) *value++ = '\0';
	}
// skip leading spaces and clear trailing spaces
	if (name) {
		while (isspace(*name)) name++;
		tail = name+strlen(name)-1;
		if (tail>name)
			while (isspace(*tail)) *tail-- = '\0';
		if (strlen(name) >0) ret++;
	}
	if (value) {
		while (isspace(*value)) value++;
		tail = value+strlen(value)-1;
		if (tail>value) 
			while (isspace(*tail)) *tail-- = '\0';
		if (strlen(value) >0) ret++;
	}
	return ret;
}
	
static int load_next_menu(FILE* fp, MenuInfo& named_info, MenuInfos& defined) {
	int n = 0;
	char line[256], *name, *value;
	CharList item_labels, item_msgs;
	IntList short_keys;
	char* menu_name = 0;
// find "menu" keyword
	while (!menu_name && fgets(line,256,fp))
		if (decode(line,name,value) == 2 && strcmp(value,MENU)==0)
			menu_name = strdup(name);
	if (!menu_name) return 0;
// find items
	while (fgets(line,256,fp)) {
		if ((n = decode(line,name,value)) <= 0) continue; //invalid line
		char* label = strtok(strdup(name),",");
		char* key = strtok(NULL,",");
		if (key) {
			while (isspace(*key)) key++;
			if (key[0]=='^') key[0] = ctrl(key[1]);
		}
		int ikey = key? key[0] : 0;
		char* msg = SEPARATOR;
		if (n == 1) {
			if (strcmp(name,END) == 0) break;				// end menu
			else if (*name != *SEPARATOR) continue;			// invalid line
		} else msg = strdup(value);
		item_labels.append(label);
		short_keys.append(ikey);
		item_msgs.append(msg);
	}
	n = (int)item_labels.count();
	if (n == 0) return 0;
	named_info.str = menu_name;
	named_info.submenu = new MenuInfo[n+1];
	for (int i=0; i<n; i++) {
		MenuInfo& cur = named_info.submenu[i];
		cur.str = item_labels.item(i);
		cur.key = short_keys.item(i);
		cur.msg = item_msgs.item(i);
		cur.status = cur.mask = cur.options = 0;
		cur.submenu = 0;
		for (int j=0; j<defined.count(); j++) {
			if (strcmp(cur.msg,defined.item(j).str) == 0) {
				cur.submenu = defined.item(j).submenu;
				break;
			}
		}
	}
	named_info.submenu[i].str = NULL;
	return n;
}

int load_menu(const char* file, MenuInfos& named_infos) {
	MenuInfo cur;
	FILE* fp = fopen(file, "r");
	if (fp == NULL) {
		perror(file);
		return 0;
	}
	named_infos.remove_all();
	while (load_next_menu(fp,cur,named_infos)) named_infos.append(cur);
	return (int)named_infos.count();
}

void print(const MenuInfo& info) {
	if (info.submenu) {
		fprintf(stderr,"%s [\n",info.str);
		for (MenuInfo* i = info.submenu; i->str != nil; i++) 
			print(*i);
		fprintf(stderr,"]\n");
	} else fprintf(stderr,"%s \\%d: %s\n",info.str,info.key,info.msg);
}

int main(int argc, char** argv) {
	MenuInfos infos;
	if (argc < 2) {
		fprintf(stderr, "usage : %s file_name\n",argv[0]);
		return 1;
	}
	if (!load_menu(argv[1], infos)) {
		fprintf(stderr, "%s : no menu loaded\n",argv[1]);
		return 1;
	}
	for (int i=0; i<infos.count(); i++) {
		const MenuInfo& cur = infos.item(i);
		if (strcmp(cur.str,"menubar") == 0) print(cur);
		else if (strcmp(cur.str,"buttonbar") == 0) print(cur);
	}
}
