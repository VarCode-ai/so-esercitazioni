#define LIBERO 0
#define OCCUPATO 2
#define IN_AGGIORNAMNTO 1


#define MUTEXP 0

#define NUM_CLIENTI 50
#define POSTI 80
typedef struct {
    unsigned int id_cliente;
    unsigned int stato; 
} posto;

typedef struct {
    posto posti[POSTI];
    int disponibilità;
} prodcons;


void cliente (prodcons *p,int ds_sem);
