#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MAX_DATA 512
#define MAX_ROWS 100

struct Address {
	int id;
	int set;
	char *name;
	char *email;
	char data[];
};

struct Database {
	int max_rows;
	int max_data;
	struct Address rows[];
};

struct Connection {
	FILE *file;
	struct Database *db;
};

void die(const char *message) {
	if(errno) {
		perror(message);
	} else {
		printf("ERROR: %s\n", message);
	}
	exit(1);
}


void Address_print(struct Address *address) {
	char *name = address->data;
	char *email = address->data + MAX_DATA + 1;
	printf("> %d %s %s\n", address->id, name, email);
}

void Database_load(struct Connection *conn, int size) {
	int rc = fread(conn->db, size, 1, conn->file);
	if(rc != 1) die("Failed to load database.");
}

struct Connection* Database_open(const char *filename, char mode, int max_rows, int max_data) {
	struct Connection *conn = malloc(sizeof(struct Connection));
	if(!conn) die("Memory Error");

//	conn->db = malloc(sizeof(struct Database));
	long size = sizeof(struct Database) + ((sizeof(struct Address) + (max_data * 2)) * max_rows);
	conn->db = malloc(size);
	if(!conn->db) die ("Memory Error");

	if(mode == 'c') {
		conn->file = fopen(filename, "w");
	} else {
		conn->file = fopen(filename, "r+");

		if(conn->file) {
			Database_load(conn, size);
			printf("loaded db with max rows %d and max data %d\n", conn->db->max_rows, conn->db->max_data);
		}
	}
	
	if(!conn->file) die("Failed to open the file");

	return conn;
}

void Database_close(struct Connection *conn) {
	if(conn) {
		if(conn->file) fclose(conn->file);
		if(conn->db) free(conn->db);
		free(conn);
	}
}

void Database_write(struct Connection *conn) {
	rewind(conn->file);
	long size = sizeof(struct Database) + ((sizeof(struct Address) + (conn->db->max_data * 2)) * conn->db->max_rows);
	printf("writing db with max data %d and max rows %d\n", MAX_DATA, MAX_ROWS);
	int rc = fwrite(conn->db, size, 1, conn->file);
//	int rc = fwrite(conn->db, sizeof(struct Database), 1, conn->file);
	if(rc != 1) die("Failed to write database.");

	rc = fflush(conn->file);
	if(rc == -1) die("Cannot flush database.");	
}

void Database_create(struct Connection *conn, int max_rows, int max_data) {
	int i = 0;
	
	for(i = 0; i < max_rows; i++) {
		// make a prototype to initialize it.
		struct Address addr = {.id = i, .set = 0};
		// then just assign it.
		conn->db->rows[i] = addr;
	}

	conn->db->max_rows = max_rows;
	conn->db->max_data = max_data;
	printf("created database with max rows %d and max data %d\n", conn->db->max_rows, conn->db->max_data);
}

void Database_set(struct Connection *conn, int id, char *name, char *email) {
	struct Address *addr = &conn->db->rows[id];

	addr->set = 1;

	//bug fix: forcing last character to null byte
	int str_size = strlen(name);
	if(str_size >= MAX_DATA) {
		name[MAX_DATA-1] = '\0';
	} else {
		name[str_size] = '\0';
	}

	//WARNING: bug, read the "How To Break It" and fix this.
	printf("max data %d\n", MAX_DATA);
	addr->name = addr->data;
	char *res = strncpy(addr->name, name, MAX_DATA);

	//demonstrate the strncpy bug.
	if(!res) die("Name copy failed!");

	//bug fix: forcing last character to null byte
	str_size = strlen(email);
	if(str_size >= MAX_DATA) {
		email[MAX_DATA-1] = '\0';
	} else {
		email[str_size] = '\0';
	}
	addr->email = addr->data + MAX_DATA + 1;
	res = strncpy(addr->email, email, MAX_DATA);


	if(!res) die("Email copy failed");
}

void Database_get(struct Connection *conn, int id) {
	struct Address *addr = &conn->db->rows[id];

	if(addr->set) {
		Address_print(addr);
	} else {
		die("ID is not set");
	}
}

void Database_delete(struct Connection *conn, int id) {
	struct Address addr = {.id=id, .set = 0};
	conn->db->rows[id] = addr;
}

void Database_list(struct Connection *conn) {
	int i=0;
	struct Database *db = conn->db;

	for(i = 0; i < MAX_ROWS; i++) {
		struct Address *cur = &db->rows[i];
		if(cur->set) {
			printf("found set\n");
			Address_print(cur);
		}
	}
}

int main(int argc, char *argv[]) {
	if(argc < 3) die("Usage: ex17 <dbfile> <action> [action params]");

	char *filename = argv[1];
	char action = argv[2][0];

	int max_rows = MAX_ROWS;
	int max_data = MAX_DATA;
	if(action == 'c') {
		if(argc > 4) {
			max_rows = atoi(argv[3]);
			max_data = atoi(argv[4]);
		} else if(argc == 4 || !max_data || !max_rows) {
				printf("Usage: %s %s c <max_rows> <max_data>\n", argv[0], argv[1]); 
				exit(1);
		}
	}

	struct Connection *conn = Database_open(filename, action, max_rows, max_data);
	int id = 0;

	if(action != 'c' && argc > 3) id = atoi(argv[3]);
	if(action != 'c' && id >= MAX_ROWS) die("There's not that many records");


	switch(action) {
		case 'c':
			Database_create(conn, max_rows, max_data);
			Database_write(conn);
			break;

		case 'g':
			if(argc != 4) die("Need an id to get");
			
			Database_get(conn, id);
			break;
	
		case 's': 
        	if(argc != 6) die("Need id, name, email to set"); 
				printf("Setting id %d\n", id);
	            Database_set(conn, id, argv[4], argv[5]); 
				Database_write(conn);
				break;
   
		case 'd':
              if(argc != 4) die("Need id to delete"); 

				Database_delete(conn, id); 
				Database_write(conn);
				break;
  
		case 'l': 
				Database_list(conn);
				break;
		
		default: 
				die("Invalid action, only: c=create, g=get, s=set, d=del, l=list"); 
	}
  
	Database_close(conn);

	return 0;
}
