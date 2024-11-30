#define MAX_TEXT 25

typedef struct
{
    long type;
    char mess[25];
} Message;

void mittente(int queue);
void ricevente(int queue);