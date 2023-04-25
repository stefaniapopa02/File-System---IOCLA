#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LINE_SIZE 300
#define MAX_PWD_PATH 50

struct Dir;
struct File;

typedef struct Dir{
	char *name;
	struct Dir* parent;
	struct File* head_children_files;
	struct Dir* head_children_dirs;
	struct Dir* next;
} Dir;

typedef struct File {
	char *name;
	struct Dir* parent;
	struct File* next;
} File;

Dir* create_home(){
    Dir* home_dir = malloc(sizeof(Dir));
    home_dir->name = malloc(5 * sizeof(char));
    strcpy(home_dir->name, "home");
    home_dir->parent = NULL;
    home_dir->head_children_files = NULL;
    home_dir->head_children_dirs = NULL;
    home_dir->next = NULL;
    return home_dir;
}

Dir* create_dir(Dir* parent, char* name){
    Dir* dir = malloc(sizeof(Dir));
    dir->name = malloc(sizeof(name));
    strcpy(dir->name, name);
    dir->parent = parent;
    dir->head_children_files = NULL;
    dir->head_children_dirs = NULL;
    dir->next = NULL;
    return dir;
}

File* create_file(Dir* parent, char* name){
    File* file = malloc(sizeof(File));
    file->name = malloc(sizeof(name)); 
    strcpy(file->name, name);
    file->parent = parent;
    file->next = NULL;
    return file;
}

File* add_file(Dir* parent, char* name){
    if(parent == NULL){
        return NULL;
    }
    if(parent->head_children_files == NULL){
        File* first_file = create_file(parent, name);
        parent->head_children_files = first_file;
        return parent->head_children_files;
    }
    else{
        File* iter = parent->head_children_files;
        while(iter->next != NULL){
            iter = iter->next;
        }
        File* new_file = create_file(parent, name);
        iter->next = new_file;
        return parent->head_children_files;

    }

}

Dir* add_dir(Dir* parent, char* name){
    if(parent == NULL){
        return NULL;
    }
    if(parent->head_children_dirs == NULL){
        Dir* first_dir = create_dir(parent, name);
        parent->head_children_dirs = first_dir;
        return parent->head_children_dirs;
    }
    else{
        Dir* iter = parent->head_children_dirs;
        while(iter->next != NULL){
            iter = iter->next;
        }
        Dir* new_dir = create_dir(parent, name);
        iter->next = new_dir;
        return parent->head_children_dirs;

    }

}

File* file_already_exists(Dir* parent, char* name){
    File* iter = parent->head_children_files;
    while(iter != NULL){
        if(!strcmp(iter->name, name)){
            return iter;
        }
        iter = iter->next;
    }
    return NULL;
}

Dir* directory_already_exists(Dir* parent, char* name){
    Dir* iter = parent->head_children_dirs;
    while(iter != NULL){
        if(!strcmp(iter->name, name)){
            return iter;
        }
        iter = iter->next;
    }
    return NULL;
}

void free_file(File* file){
    free(file->name);
    free(file);
}

void free_dir(Dir* parent){ 
    File* iter = parent->head_children_files;
    if(iter != NULL){
        if(iter->next == NULL){
            free_file(iter);
        }else{
            File* prev = iter;
            iter = iter->next;
            while(iter != NULL){
                free_file(prev);
                prev = iter;
                iter = iter->next;
            }
            free_file(prev);
        }
    }
    Dir* iter_dirs = parent->head_children_dirs;
    if(iter_dirs == NULL){
        free(parent->name);
        free(parent);
        return;
    }else if(iter_dirs->next == NULL){
        free_dir(iter_dirs);
        free(parent->name);
        free(parent);    
        return;
    }else{
        Dir* prev_dir = iter_dirs;
        iter_dirs = iter_dirs->next;
        while(iter_dirs != NULL){
            free_dir(prev_dir);
            prev_dir = iter_dirs;
            iter_dirs = iter_dirs->next;
        }
        free_dir(prev_dir);
    }

    free(parent->name);
    free(parent);
    return;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////


void touch (Dir* parent, char* name) {
    if(file_already_exists(parent, name) == NULL){
        add_file(parent, name);
    }
    else{
        printf("File already exists\n");
    }
    return;
}

void mkdir (Dir* parent, char* name) {
    if(directory_already_exists(parent, name) == NULL){
        add_dir(parent, name);
    }
    else{
        printf("Directory already exists\n");
    }
    return;
}

void ls (Dir* parent) {
    Dir* iter_dirs = parent->head_children_dirs;
    while(iter_dirs != NULL){
        printf("%s\n", iter_dirs->name);
        iter_dirs = iter_dirs->next;
    }

    File* iter_files = parent->head_children_files;
    while(iter_files != NULL){
        printf("%s\n", iter_files->name);
        iter_files = iter_files->next;
    }

}

void rm (Dir* parent, char* name) {
    if(file_already_exists(parent, name) != NULL){
        File* prev = parent->head_children_files;
        if(!strcmp(prev->name, name)){
            parent->head_children_files = parent->head_children_files->next;    
            free_file(prev);
            return;
        }else {
            File* current = prev->next;
            while (current != NULL) {
                if(!strcmp(current->name, name)){
                    prev->next = current->next;
                    free_file(current);
                    return;
                }
                current = current->next;
                prev = prev->next;
            }
        }
    }else{
        printf("Could not find the file\n");
        return;
    }
}

void rmdir (Dir* parent, char* name) {
    if(directory_already_exists(parent, name) != NULL){
        Dir* prev = parent->head_children_dirs;
        if(!strcmp(prev->name, name)){
            parent->head_children_dirs = parent->head_children_dirs->next;    
            free_dir(prev);
            return;
        }else {
            Dir* current = prev->next;
            while (current != NULL) {
                if(!strcmp(current->name, name)){
                    prev->next = current->next;
                    free_dir(current);
                    return;
                }
                current = current->next;
                prev = prev->next;
            }
        }
    }else{
        printf("Could not find the dir\n");
        return;
    }
}

void cd(Dir** target, char *name) {
    Dir* dir_to_change = *target;
    if(!strcmp(name, "..")){
        if(dir_to_change->parent == NULL){
            return;
        }else{
            *target = dir_to_change->parent;
            return;
        }
        
    }else{
        Dir* new_cur_dir = directory_already_exists(dir_to_change, name);
        if(new_cur_dir != NULL){
            *target = new_cur_dir;
            return;
        }
        else{
            printf("No directories found!\n");
            return;
        }
    }
}

char *pwd (Dir* target) {
    char *path = malloc(MAX_PWD_PATH * sizeof(char));
    char *temp = malloc(MAX_PWD_PATH * sizeof(char));
    Dir* first = target;

    strcpy(path, "/");
    while(target != NULL){
        strcpy(temp, "/");
        strcat(temp, target->name);
        if(target != first)
            strcat(temp, path);
        strcpy(path, temp);
        target = target->parent;
    }
    free(temp);
    return path;

}

void stop (Dir* target) {
    return;
}

void tree (Dir* target, int level) {
    Dir* iter_dirs = target->head_children_dirs;
    while(iter_dirs != NULL){
        for(int i = 1; i <= level; i++){
            printf("    ");
        }
        printf("%s\n", iter_dirs->name);
        tree(iter_dirs, level + 1);
        iter_dirs = iter_dirs->next;
    }
    File* iter_files = target->head_children_files;
    if(iter_files != NULL){
        for(int i = 1; i <= level; i++){
            printf("    ");
        }
        printf("%s\n", iter_files->name);
    }

}

void mv_file(Dir* parent, char *oldname, char *newname){
    File* iter_files = parent->head_children_files;
    if((iter_files->next == NULL) && (strcmp(iter_files->name, oldname) == 0)){//daca directorul are un singur fisier si acesta este chiar cel cautat
        iter_files->name = newname;
        return;
    }else{ //daca directorul are mai multe fisiere
        File* prev = iter_files;
        iter_files = iter_files->next;

        File* last_file = parent->head_children_files;
        while(last_file->next != NULL){
            last_file = last_file->next;
        }

        if(strcmp(prev->name, oldname) == 0){  //daca fisierul cautat este chiar primul din director
            parent->head_children_files = parent->head_children_files->next;
            strcpy(prev->name, newname);
            last_file->next = prev;
            prev->next = NULL;
            return;
        }

        if(strcmp(last_file->name, oldname) == 0){ // daca fisierul cautat este ultimul din director
            strcpy(last_file->name, newname);
            return;
        }

        //daca fisierul cautat nu este nici primul, nici ultimul
        while(iter_files->next != NULL){
            if(strcmp(iter_files->name, oldname) == 0){
                prev->next = iter_files->next;
                strcpy(iter_files->name, newname);
                last_file->next = iter_files;
                iter_files->next = NULL;
            }
            prev = prev->next;
            iter_files = iter_files->next;
        }
        return;
    }
}

void mv_dir(Dir* parent, char *oldname, char *newname){
    Dir* iter_dirs = parent->head_children_dirs;
    if((iter_dirs->next == NULL) && (strcmp(iter_dirs->name, oldname) == 0)){//daca directorul are un singur director si acesta este chiar cel cautat
        strcpy(iter_dirs->name, newname);
        return;
    }else{ //daca directorul are mai multe directoare
        Dir* prev = iter_dirs;
        iter_dirs = iter_dirs->next;

        Dir* last_dir = parent->head_children_dirs;
        while(last_dir->next != NULL){
            last_dir = last_dir->next;
        }

        if(strcmp(prev->name, oldname) == 0){  //daca directorul cautat este chiar primul din director
            parent->head_children_dirs = parent->head_children_dirs->next;
            strcpy(prev->name, newname);
            last_dir->next = prev;
            prev->next = NULL;
            return;
        }

        if(strcmp(last_dir->name, oldname) == 0){ // daca directorul cautat este ultimull din director
            last_dir->name = newname;
            return;
        }

        //daca directorul cautat nu este nici primul, nici ultimul
        while(iter_dirs->next != NULL){
            if(strcmp(iter_dirs->name, oldname) == 0){
                prev->next = iter_dirs->next;
                strcpy(iter_dirs->name, newname);
                last_dir->next = iter_dirs;
                iter_dirs->next = NULL;
            }
            prev = prev->next;
            iter_dirs = iter_dirs->next;
        }
        return;
    }
}

void mv(Dir* parent, char *oldname, char *newname) {
    if((file_already_exists(parent, oldname) == NULL) && (directory_already_exists(parent, oldname) == NULL)){
        printf("File/Director not found\n");
        return;
    }else{
        if((file_already_exists(parent, newname) != NULL) || (directory_already_exists(parent, newname) != NULL)){
            printf("File/Director already exists\n");
            return;
        }
        if(file_already_exists(parent, oldname) != NULL){
            mv_file(parent, oldname, newname);
            return;
        }
        if(directory_already_exists(parent, oldname) != NULL){
            mv_dir(parent, oldname, newname);
            return;
        }
    }


}

int main () {

    char* input = malloc(MAX_INPUT_LINE_SIZE * sizeof(char));
    Dir* home = create_home();
    Dir* current_dir = home;
    char* name ;
    char* command ;

    do{
        fgets(input, MAX_INPUT_LINE_SIZE, stdin);
        
        command = strtok(input, " ");
        name = strtok(NULL, "  \n\0");

        if(!strcmp(command, "touch")){
            touch(current_dir, name);
        }else if(!strcmp(command, "mkdir")){
            mkdir(current_dir, name);
        }else if(strstr(command, "ls")){
            ls(current_dir);
        }else if(!strcmp(command, "rm")){
            rm(current_dir, name);
        }else if(!strcmp(command, "rmdir")){
            rmdir(current_dir, name);
        }else if(!strcmp(command, "cd")){
            cd(&current_dir, name);
        }else if(strstr(command, "tree")){
            tree(current_dir, 0);
        }else if(strstr(command, "pwd")){
            char* output_pwd = pwd(current_dir);
            printf("%s", output_pwd);
            free(output_pwd);
        }else if(strstr(command, "stop")){
            stop(current_dir);
        }else if(strstr(command, "mv")){
            char* name2 = strtok(NULL, "  \n\0");
            mv(current_dir, name, name2);
        }

    } while(!strstr(input, "stop"));

    free(command);
    free_dir(home);
    
	return 0;
}
