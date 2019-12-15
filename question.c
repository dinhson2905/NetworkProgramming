#define LENGTH_QUESTION 256
#define LENGTH_OPTION 128
#define END_FILE 0
#define FILE_NAME "data.txt"

typedef struct q {
    char question[LENGTH_QUESTION];
    char option[4][LENGTH_OPTION];
    int answer;
} Question;

Question q_arr[30];
int q_num;

void load_question() {
    q_num = 0;
    FILE * p = fopen(FILE_NAME, "r");
    
    char temp[LENGTH_QUESTION];
    while(fgets(temp, LENGTH_QUESTION, p) != NULL) {
        strcpy(q_arr[q_num].question, temp);
        for(int i = 0; i < 4; i++) {
            fgets(temp, LENGTH_QUESTION, p);
            strcpy(q_arr[q_num].option[i], temp);
        }
        fgets(temp, LENGTH_QUESTION, p);
        q_arr[q_num].answer = atoi(temp);
        q_num++;
    }

  	fclose(p);
}