/*
Ignore esse arquivo, era um teste só. 
"Em desenvolvimento".
*/

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

typedef enum Status
{
    NOVA_LINHA,
    LABEL,
    NORMAL
} Status;

char *readFile(char *filename) {
    FILE *f = fopen(filename, "rt");
    if(!f)
        return 0;
    fseek(f, 0, SEEK_END);
    long length = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *buffer = (char *) malloc(length + 1);
    buffer[length] = '\0';
    fread(buffer, 1, length, f);
    fclose(f);
    return buffer;
}

int interpretar(char* arquivo){
    char* contents = readFile(arquivo);
    if (contents){
        
        int* labels = malloc(sizeof(int)*strlen(contents)/4);

        int n_labels = 0;

        char buffer[4];
        int buffOcup = 0;

        Status status = NOVA_LINHA;

        for (int i = 0; contents[i] != EOF; i++)
        {
            if(status==NOVA_LINHA){
                if(contents[i] == 'L')
                    status = LABEL;
                else
                    status = NORMAL;
            }
            else if(status==LABEL){
                if(!isdigit(contents[i])){
                    labels[n_labels++] = i;
                    status = NORMAL;
                }
                else
            }
            else{

            }
            
        }

        free(labels);
    }
}

int main(int argc, char *argv[]){
    if(argc<2){
        printf("Argumentos insuficientes. Passe como argumento um arquivo para ser interpretado\n");
    }
    else{
        for (int i = 1; i < argc; i++)
        {
            interpretar(argv[i]);
        }
        
    }
}