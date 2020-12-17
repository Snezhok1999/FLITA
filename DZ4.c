#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void function(char ***name, int ***conn, int num, int delete){
    for(int i=0; i<num; i++){
        (*conn)[i][delete] = 0;
        (*conn)[delete][i] = 0;
    }
    
    int *stepen = calloc(num, sizeof(int));

    for(int i=0; i<num; i++){
        for(int j=0; j<num; j++){
            stepen[i] += (*conn)[i][j];
            stepen[i] += (*conn)[j][i];
        }
    }

    //shell sort
    for (int gap = num/2; gap > 0; gap /= 2) {
        for (int i = gap; i < num; i++) {
            for (int j = i-gap; j >= 0 && stepen[j] > stepen[j+gap]; j-=gap) {
                int temp = stepen[j];
                stepen[j] = stepen[j+gap];
                stepen[j+gap] = temp;

                int *temp_conn = (*conn)[j];
                (*conn)[j] = (*conn)[j+gap];
                (*conn)[j+gap] = temp_conn;
                
                for(int k=0; k<num; k++){
                    int temp_conn2 = (*conn)[k][j];
                    (*conn)[k][j] = (*conn)[k][j+gap];
                    (*conn)[k][j+gap] = temp_conn2;
                }
                
                char *temp_name = (*name)[j];
                (*name)[j] = (*name)[j+gap];
                (*name)[j+gap] = temp_name;
            }
        }
    }
}

char *getarr(int all){
    int n=1;
    char *arr,c;
    arr = calloc(n+1, sizeof(char));
    char trigger = ';';
    if(all){
        trigger = '\n';
    }
    int i = 0;
    while((c=getchar()) != trigger){
        if(all == 1 || (('0' <= c && c <= '9') || ('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z'))){ //[a-zA-Z0-9 - ;]
            char *temp_arr;
            temp_arr = calloc(n+1, sizeof(char));
            
            for(int j = 0; j < n; j++){
                temp_arr[j] = arr[j];
            }
            temp_arr[n-1] = c;
            n++;
            i++;
            free(arr);
            arr = temp_arr;
        }
        else if(c == '\n'){
            arr[n]='\0';
            return arr;
        }
    }
    arr[n-1]='\0';
    return arr;
}

int main(){
    int graph;
    printf("1 - graph\n2 - digraph\n# ");
    scanf("%d", &graph);
    
    if(!( graph == 1 || graph == 2 ))
        exit(9);
    
    printf("Amount: ");
    int n;
    scanf("%d",&n);
    if (n <= 0)
        exit(9);
    
    char **name = calloc(n, sizeof(char*));
    int **conn = calloc(n, sizeof(int*));
    for(int i=0; i<n; i++){
        conn[i] = calloc(n, sizeof(int));
        for(int j=0; j<n; j++){
            conn[i][j] = 0;
        }
    }
    
    printf("Enter names:\n");
    getchar();
        
    for(int i=0; i<n; i++){
        printf("# ");
        name[i] = getarr(0);
        
        for (int j=0; j<i; j++){
            if( strcmp(name[i], name[j]) == 0){
                printf("There are similar elements, enter another...\n");
                i--;
            }
        }
    }
            
    printf("Enter connection\n");
    char *str = getarr(1);
    int i=0;
    while(str[i] != '\0'){
        char name1[32] = {0}, name2[32] = {0};
        int numname1 = -1, numname2 = -1;
        char ch = 0;
        int j=0;
        while((ch = str[i++]) != '-'){ name1[j++] = ch; }
        j=0;
        while((ch = str[i++]) != ';' && ch != '\0'){ name2[j++] = ch; }
        
        for(int k=0; k<n; k++){
            if(strcmp(name[k], name1) == 0){
                numname1 = k;
            }
            if(strcmp(name[k], name2) == 0){
                numname2 = k;
            }
        }
        conn[numname1][numname2]++;
    }
        
    printf("\n");
    _Bool relat_graph = 1;
    for(int i=0; i<n; i++){
        _Bool temp_relat_graph = 0;
        for(int j=0; j<n; j++){
            if(conn[i][j] == 1)
                temp_relat_graph = 1;
            
            if(conn[j][i] == 1)
                temp_relat_graph = 1;
        }
        if(temp_relat_graph == 0)
            relat_graph=0;
    }
    if(relat_graph == 0){
        printf("unrelated\n");
    } else {
        printf("related\n");

    }
    printf("Connection table\n");
    for(int i=0; i<n; i++) {
        printf("%s\t", name[i]);
        for (int j=0; j<n; j++) {
            printf("%d ", conn[i][j]);
        }
        printf("\n");
    }

    char strsys[1024] = {0};
    char strsys2[1024] = {0};
    strcat(strsys, "echo '");
    strcat(strsys2, "echo '");


    char arr[3] = "";
    if(graph == 2){
        strcat(strsys, "digraph G {");
        strcat(arr, "->");
        strcat(strsys2, "digraph G {");
    } else {
        strcat(strsys, "graph G {");
        strcat(arr, "--");
        strcat(strsys2, "graph G {");
    }

    for(int i=0; i<n; i++){
        strcat(strsys, name[i]);
        strcat(strsys, "; ");
    }
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            for(int k=0; k<conn[i][j]; k++){
                strcat(strsys, name[i]);
                strcat(strsys, arr);
                strcat(strsys, name[j]);
                strcat(strsys, "; ");
            }
        }
    }
    strcat(strsys, "}' | dot -Tpng > ./graph.png");

    system(strsys);

    
    printf("Enter num element to delete: ");
    int delete;
    scanf("%d", &delete);
    function(&name, &conn, n, delete-1);
    
    printf("Connection table\n");
    for(int i=0; i<n; i++) {
        printf("%s\t", name[i]);
        for (int j=0; j<n; j++) {
            printf("%d ", conn[i][j]);
        }
        printf("\n");
    }

    

    for(int i=0; i<n; i++){
        strcat(strsys2, name[i]);
        strcat(strsys2, "; ");
    }
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            for(int k=0; k<conn[i][j]; k++){
                strcat(strsys2, name[i]);
                strcat(strsys2, arr);
                strcat(strsys2, name[j]);
                strcat(strsys2, "; ");
            }
        }
    }
    strcat(strsys2, "}' | dot -Tpng > ./graph_after.png");

    system(strsys2);

    return 0;
}


