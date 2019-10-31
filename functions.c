#include "headers.h"

char *err_msg = 0;
sqlite3 *db;

int openDatabase(){
    int rc = sqlite3_open("phonebook.db", &db);

    if(rc != SQLITE_OK){
        printf("Error trying to connect to database\n");
        return 1;
    }
    return 0;

}

void create_tables(){
    int rc = sqlite3_open("phonebook.db", &db);

    char *sql = "CREATE TABLE `person` (first_name varchar(100) NOT NULL, second_name varchar(100), PRIMARY KEY (first_name));"
                "CREATE TABLE `contact` (contact_id INTEGER PRIMARY KEY AUTOINCREMENT, first_name varchar(100) NOT NULL REFERENCES person(first_name), number varchar(100) NOT NULL REFERENCES phone(number));";
    
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

    if(rc != SQLITE_OK){
        fprintf(stderr,"SQL Error: %s\n", err_msg);

        sqlite3_free(err_msg);
        sqlite3_close(db);
    }   
}


static int callback(void* data, int argc, char** argv, char** azColName){ 
    int i; 

    for (i = 0; i < argc; i++) { 
        printf("%s ", argv[i] ? argv[i] : "NULL"); 
    } 
  
    printf("\n"); 
    return 0; 
} 

bool check_tables(){ 
    char *sql = "CREATE TABLE `phone` (number varchar(100) NOT NULL, number_name varchar(100), PRIMARY KEY(number));";
    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

     if(rc != SQLITE_OK){
        return true;
    }else{
        return false;
    }

}

void print_GUI(){

    system("clear");
    printf("         PHONEBOOK\n");
    printf(" ----------------------------\n");
    printf("| COMMAND |      ACTION      |\n");
    printf("|---------|------------------|\n");
    printf("|    1    |  show  Contacts  |\n");
    printf("|---------|------------------|\n");
    printf("|    2    |  insert contact  |\n");
    printf("|---------|------------------|\n");
    printf("|    3    |  edit  contact   |\n");
    printf("|---------|------------------|\n");
    printf("|    4    |  remove contact  |\n");
    printf("|---------|------------------|\n");
    printf("|    5    |   exit program   |\n");
    printf(" ----------------------------\n");

}

bool check_id(int id){
    char check[900];
    
    sprintf(check, "Select * from contact where contact_id = %d", id);
    
    struct sqlite3_stmt *selectstmt;
    
    int result = sqlite3_prepare_v2(db, check, -1, &selectstmt, NULL);
    
    if(result == SQLITE_OK){
        if (sqlite3_step(selectstmt) == SQLITE_ROW){
            return true;
        }else{
            sqlite3_finalize(selectstmt);
            return false;
        }
    }

}

void show_contacts(){
        char *sql = "SELECT * FROM contact;";
        int rc = sqlite3_exec(db,sql,callback,"",&err_msg);
}

void insert_contact(){
    char name[100];
    char number[20];
    char inserting[900];
    printf("What is the name you want to save? ");
    setbuf(stdin,NULL);
    scanf("%[^\n]%*c", name);
    printf("What is the number you want to save? ");
    setbuf(stdin,NULL);
    scanf("%[^\n]%*c", number);

    sprintf(inserting ,"INSERT INTO contact(first_name, number) VALUES('%s','%s')", name, number);
    int rc = sqlite3_exec(db,inserting, 0, 0, &err_msg);

    if(rc != SQLITE_OK){
        fprintf(stderr, "Inserting error\nSQL ERROR: %s\n", err_msg);
    }else{
        printf("Contact SAVED!!\n");
    }
}

void edit_contact(){
    char  new_name[100], new_number[20];
    int opt = 0, rc, id = 0;
    char changes[900];
    bool achou = false;

    show_contacts();

    printf("the contact ID you want to edit: ");
    scanf("%d", &id);

    achou = check_id(id);

    while(!achou){
        printf("ID not found, Try again. ");
        scanf("%d", &id);
        achou = check_id(id);    
    }

    printf("Now, what you want to edit.\n1 = number\n2 = name\n");
    scanf("%d",&opt);

    while(opt < 1  && opt > 2){   
        printf("Option choosed dont exists, Try again. ");
        scanf("%d", &opt);
    } 
    switch(opt){
        case 1:
            printf("Type the new number: ");
            setbuf(stdin,NULL);
            scanf("%[^\n]%*c", new_number);
            sprintf(changes, "UPDATE contact SET number = '%s' WHERE contact_id = %d", new_number, id);
            break;
        case 2:
            printf("Type the new name: ");
            setbuf(stdin,NULL);
            scanf("%[^\n]%*c", new_name);
            sprintf(changes, "UPDATE contact SET first_name = '%s' WHERE contact_id = %d", new_name, id);
            break;
    }

    rc = sqlite3_exec(db,changes, 0, 0, &err_msg);

    if(rc != SQLITE_OK){
        fprintf(stderr, "Edit contact error\n SQL ERROR: %s\n", err_msg);
    }else{
        printf("Contact updated!!\n");
    }
}

void remove_contact(){
    bool achou = false;
    int id = 0;
    char remove[900];

    show_contacts();

    printf("the contact ID you want to remove: ");
    scanf("%d", &id);

    achou = check_id(id);

    while(!achou){
        printf("ID not found, Try again. ");
        scanf("%d", &id);
        achou = check_id(id);      
    }
    
    sprintf(remove, "DELETE FROM contact WHERE contact_id = %d", id);
    int rc = sqlite3_exec(db, remove, 0, 0, &err_msg);

    if(rc != SQLITE_OK){
        fprintf(stderr,"Error deleting\nSQL ERROR = %s\n", err_msg);
    }else{
        printf("Contact removed!!\n");
    }

}   
