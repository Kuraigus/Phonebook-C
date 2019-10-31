/*  Joao "Kuraigus" Victor
    This program is a simple phonebook, without a proper front-end, I made this more for practicing my skills
in back-end + database usage.
    TO DO list:
        -Comments
*/
#include "functions.c"

int main(){
    int rc = openDatabase(db);//Opens database
    bool check_db;
    int opt = 0;

    printf("Checking Database tables\n");
    check_db = check_tables();
    printf("Check complete\n");

    if(!check_db){
        printf("Creating tables...\n");
        create_tables();
        sleep(1);
        printf("tables created successfully\n");
    }
    sleep(1);
    
    print_GUI();

    while(opt != 5){
        scanf("%d", &opt);
        switch(opt){
            case 1:
                show_contacts();
                break;
            case 2:
                insert_contact();
                break;
            case 3:
                edit_contact();
                break;
            case 4:
                remove_contact();
                break;
        }
    }

    sqlite3_close(db);

    return 0;
}
