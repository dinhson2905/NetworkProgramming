#define LENGTH_QUESTION 256
#define LENGTH_ANSWER 256
#define END_FILE 0
#define FILE_NAME "data.txt"

typedef struct q {
    char question[LENGTH_QUESTION];
    char answer[LENGTH_ANSWER];
    char hide_answer[LENGTH_ANSWER];
} Question;

Question load_question() {
    Question q;
    //q_num = 0;
    FILE *p = fopen(FILE_NAME, "r");
    
    char temp[LENGTH_QUESTION];

    fgets(temp, LENGTH_QUESTION, p);
    temp[strlen(temp) - 1] = '\0';
    strcpy(q.question, temp);

    fgets(temp, LENGTH_ANSWER, p);
    temp[strlen(temp) - 1] = '\0';
    strcpy(q.answer, temp);
    
    for (int i = 0; i < strlen(temp); i++) {
        if (temp[i] != ' ') temp[i] = '*';
    }
    strcpy(q.hide_answer, temp);

  	fclose(p);
    return q;
}