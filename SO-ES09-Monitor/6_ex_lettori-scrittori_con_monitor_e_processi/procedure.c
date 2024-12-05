#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "header.h"

void InizioLettura(Monitor* m, Buffer * buf){
	
	enter_monitor(m);
	if (buf->num_scrittori>0)
	{
		wait_condition(m,VARCOND_LETTORI);
	}
	buf->num_lettori++;
	signal_condition(m,VARCOND_LETTORI);
	leave_monitor(m);
	
	/* TBD: Effettuare inizio lettura */

}

void FineLettura(Monitor* m, Buffer * buf) {

	/* TBD: Effettuare fine lettura */
	enter_monitor(m);
	buf->num_lettori--;
	if (buf->num_lettori==0)
	{
		signal_condition(m,VARCOND_SCRITTORI);
	}
	
	leave_monitor(m);
}



//Procedure di inizio e fine scrittura

void InizioScrittura(Monitor* m,Buffer*buf){
	enter_monitor(m);
	if (buf->num_lettori>0 || buf->num_scrittori>0)
	{
		wait_condition(m,VARCOND_SCRITTORI);
	}
	buf->num_scrittori++;
	leave_monitor(m);
	/* TBD: Effettuare inizio scrittura */
}

void FineScrittura(Monitor* m, Buffer*buf){
	enter_monitor(m);
	buf->num_scrittori--;
		if( queue_condition( m, VARCOND_SCRITTORI ) ) {

		printf("Scrittura - signal su scrittori\n");

		signal_condition( m, VARCOND_SCRITTORI );

	} else {

		/*
		 * NOTA: si effettua una singola signal_cond e si riattiva
		 * un solo un processo lettore. Per questo motivo,
		 * il primo lettore a svegliarsi dovrà fare anche lui una
		 * signal_cond per riattivare il secondo lettore in attesa;
		 * il secondo lettore dovrà riattivare il terzo lettore; etc.
		 */

		printf("Scrittura - signal su lettori\n");

		signal_condition( m, VARCOND_LETTORI );
	}

	leave_monitor(m);
	/* TBD: Effettuare inizio scrittura */
}



void Servizio(Monitor* m, Buffer * buf){

	srand(time(0));

	int i;
	for(i=0; i<20; i++) {

		/* TBD: Richiamare InizioScrittura e FineScrittura */
		InizioScrittura(m,buf);
		buf->meteo.temperatura = (rand()%101)-50;
		buf->meteo.umidita = rand()%101;
		buf->meteo.pioggia = rand()%2;

		printf("<%d> scrittura: Temperatura=%d, Umidità=%d, Pioggia=%s\n", getpid(), buf->meteo.temperatura, buf->meteo.umidita, (buf->meteo.pioggia ? "si" : "no") );

		sleep(2);
		FineScrittura(m,buf);
	}

}

void Utente(Monitor* m, Buffer * buf) {

	int i;
	for(i=0; i<10; i++) {
		InizioLettura(m,buf);
		/* TBD: Richiamare InizioLettura e FineLettura */

		printf("<%d> lettura: Temperatura=%d, Umidità=%d, Pioggia=%s\n", getpid(), buf->meteo.temperatura, buf->meteo.umidita, (buf->meteo.pioggia ? "si" : "no") );

		sleep(1);
		FineLettura(m,buf);
	}

}


