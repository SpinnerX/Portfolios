#include <stdio.h>
#include <time.h>
#include <math.h>

#include <stdlib.h>
#include <string.h>

#define LINESIZE 800
#define MAXLINES 3000
#define SPAMBOX "./spam"
#define NORMBOX "./goodmail"

#define DEBUG(X)

int filter(char *execline, char *fromline,char *date,char *transmitting_host,char *realto,char *apparentlyto,char *from,char *replyto,char *errorsto,char *subject,char *cc,char *content_type,char *body);
void stripbraces(char *strin);
void striplinefeed(char *strin);

static char fromline[LINESIZE]; /* First line: from baurens@12move.be... */
static char date[LINESIZE]; /* First line: Time of arrival */
static char transmitting_host[LINESIZE]; /* Received: from shineun.co.kr */
static char realto[LINESIZE]; /* ...for abcd1234@altavista.net (part of received) */
static char apparentlyto[LINESIZE]; /* To: <c.luefti@123net.co.uk> */
static char from[LINESIZE]; /* From: baurens@12move.be */
static char replyto[LINESIZE]; /* Reply-To: baurens@12move.be */
static char errorsto[LINESIZE]; /* Errors-To: jackson@arabia.com */
static char subject[LINESIZE]; /* Billions of dollars! */
static char cc[LINESIZE]; /* bkerns@hotmail, bkernez@hotmail, etc */
static char content_type[LINESIZE]; /* text-plain, text-html */
static char body[LINESIZE * MAXLINES]; /* Main body of text */
static char savedmessage[LINESIZE * MAXLINES]; /* Whole message saved */

int main(int argc,char **argv) {
	FILE *outs1;
	FILE *outs2;
	char *tmp;
	char *normbox=NORMBOX,*spambox=SPAMBOX;
	char buf[LINESIZE]; /* Current line read */
	int is_spam,lastwordwasfor = 0,linesread = 0,inbody = 0;
	savedmessage[0] = 0; /* Not needed */
	if (argc > 1 && argc <= 3) {
		normbox = argv[1];
		spambox = argv[2];
	}
	outs1 = fopen(spambox,"a");
	outs2 = fopen(normbox,"a");
	if (!(outs1) || !(outs2)) {
		DEBUG(printf("Error opening mailboxes\n");)
		exit(1);
	}
	while (1) {
		fgets(buf,LINESIZE,stdin);
		if (feof(stdin)) break;

		strcat(savedmessage,buf); /* Keep a copy around for writing at the end */

		/* Now we go into a finite state machine to process headers */
		if (linesread == 0) { /* From processing */
			strtok(buf," "); /* From */
			tmp = strtok((char *)NULL," "); /* Email addr */
			if (tmp == NULL) break;
			strcpy(fromline,tmp); /* Email addr */
			DEBUG(printf("From: %s\n",fromline));
			date[0] = '\0';
			do {
				tmp = (char *)strtok((char *)NULL," ");
				if (tmp == NULL) break;
				if (strlen(date) > 0) strcat(date," ");
				strcat(date,tmp); /* Time */
			} while (tmp);
			DEBUG(printf("Date: %s",date));
		}
		else {
			if (!inbody) {
			tmp = (char *)strtok(buf," ");
			if (tmp == NULL || tmp[0] == '\n') { /* Blank line read */
				inbody = 1; /* Blank line means we're at end of header */
				continue;
			}

			/* Go into complex switch statements here */
			if (!strcmp(tmp, "Received:")) { /* We want only the first sending host */
				strtok((char *)NULL," "); /* Garbage: 'from:' */
				tmp = (char *)strtok((char *)NULL," "); /* This is the hostname */
				strcpy(transmitting_host,tmp);
				DEBUG(printf("Transmitting host: %s\n",transmitting_host));

				/* Get real "to:" from this line, also */
				while (1) {
					tmp = (char *)strtok((char *)NULL," ");
					if (tmp == NULL) break;
					if (!strcmp(tmp,"for")) {
						strcpy(realto,strtok((char *)NULL," ")); /* This should be it */
						if (realto[strlen(realto)-1] == ';') /* Strip trailing ';' */
							realto[strlen(realto)-1] = '\0';
						DEBUG(printf("Realto: %s\n",realto));
						break;
					}
				}

			}
			else if (!strcmp(tmp,"From:")) {
				strcpy(from,strtok((char *)NULL," ")); /* This is the email addr */
				DEBUG(printf("From: %s",from));
			}
			else if (!strcmp(tmp,"To:")) {
				strcpy(apparentlyto,strtok((char *)NULL," ")); /* This is the email addr */
				DEBUG(printf("Apparentlyto: %s",apparentlyto));
			}
			else if (!strcmp(tmp,"Reply-To:")) {
				strcpy(replyto,strtok((char *)NULL," ")); /* This is the email addr */
				DEBUG(printf("Replyto: %s",replyto));
			}
			else if (!strcmp(tmp,"Errors-To:")) {
				strcpy(errorsto,strtok((char *)NULL," ")); /* This is the email addr */
				DEBUG(printf("errorsto: %s",errorsto));
			}
			else if (!strcmp(tmp,"Subject:")) { /* Subject */
				strcpy(subject,strtok((char *)NULL,"\n"));
				DEBUG(printf("Subject: %s\n",subject));
			}
			else if (!strcmp(tmp,"Cc:")) { /* CC: */
				strcpy(cc,strtok((char *)NULL,"\n"));
				DEBUG(printf("CC: %s\n",cc));
			}
			else if (!strcmp(tmp,"Content-Type:")) { /* CC: */
				strcpy(content_type,strtok((char *)NULL,";"));
				DEBUG(printf("Content Type: %s\n",content_type));
			}
			} /* End if (!inbody) */
			else {
 				/* Can't have a line in body starting with "From" */
				/* It confuses mail readers since thats what they tokenize on */
				if (!strncmp(buf,"From ",strlen("From ")))
					strcat(body,">");
				strcat(body,buf);
			}
		}

		linesread++;
		if (linesread >= MAXLINES) break;
	} /* End while(1) */

	/* Clean up all the lines */
	stripbraces(fromline);
	striplinefeed(fromline);
	stripbraces(date);
	striplinefeed(date);
	stripbraces(transmitting_host);
	striplinefeed(transmitting_host);
	stripbraces(realto);
	striplinefeed(realto);
	stripbraces(apparentlyto);
	striplinefeed(apparentlyto);
	stripbraces(from);
	striplinefeed(from);
	stripbraces(replyto);
	striplinefeed(replyto);
	stripbraces(errorsto);
	striplinefeed(errorsto);
	stripbraces(subject);
	striplinefeed(subject);
	stripbraces(cc);
	striplinefeed(cc);
	stripbraces(content_type);
	striplinefeed(content_type);

	/* Call the spam filter */
	is_spam = filter(argv[0],fromline,date,transmitting_host,realto,apparentlyto,from,replyto,errorsto,subject,cc,content_type,body);

	/* Store message in mailbox depending on result */
	savedmessage[sizeof(savedmessage)-1] = 0; /* Always null terminate */
	if (is_spam) { fprintf(outs1,"%s\n",savedmessage); }
	else { fprintf(outs2,"%s\n",savedmessage); }

	return 0;
}

int filter(char *execname, char *fromline,char *date,char *transmitting_host,char *realto,char *apparentlyto,char *from,char *replyto,char *errorsto,char *subject,char *cc,char *content_type,char *body) {

	int retval = 0;
	char commandString[LINESIZE], *pathname, *copy;
	FILE * fid = fopen( "/tmp/mail.poss.spam", "w" ) ;
	/*
	fprintf( fid, "%s\n", fromline ) ;
	fprintf( fid, "%s\n", date ) ;
	fprintf( fid, "%s\n", transmitting_host ) ;
	fprintf( fid, "%s\n", realto ) ;
	fprintf( fid, "%s\n", apparentlyto ) ;
	fprintf( fid, "%s\n", from ) ;
	fprintf( fid, "%s\n", replyto ) ;
	fprintf( fid, "%s\n", errorsto ) ;
	fprintf( fid, "%s\n", cc ) ;
	*/
	fprintf( fid, "%s\n", subject ) ;
	fprintf( fid, "%s\n", content_type ) ;
	fprintf( fid, "%s\n", body ) ;
	fclose( fid ) ;

	copy = strdup (execname);
	pathname = (char *)dirname (copy);
	free (copy);

	sprintf(commandString,"cd %s; ./jmail.script /tmp/mail.poss.spam ; ./test.script debug.out", pathname ) ;
	retval = system( commandString ) ;
	if (retval) printf("Message is spam\n");
	else printf("Message is not spam\n");
	system("rm /tmp/mail.poss.spam");
	return retval;
}

/* Removes all '<' and '>' characters from string */
void stripbraces(char *strin) {
	int i;
	int chars_seen = 0;
	for (i = 0; i < strlen(strin); i++) {
		if (strin[i] == '<' || strin[i] == '>') continue;
		strin[chars_seen] = strin[i];
		chars_seen++;
	}
	strin[chars_seen] = '\0';
}

void striplinefeed(char *strin) {
	if (strlen(strin) <= 0) return;
	if (strin[strlen(strin)-1] == '\n')
		strin[strlen(strin)-1] = '\0';
}
#if 0
	// fork Ldbm 
	switch ( ldbm_pid = int( vfork() ) ) {
	case -1:
		perror( "error during fork" ) ;
		exit( 1 ) ;
	case 0:						// child 
		xargc = 0 ;
		xargv[ xargc++ ] = "Link16-Dbm" ;
		xargv[ xargc++ ] = "LLST_SERVER" ;
		xargv[ xargc++ ] = (char *) chan.name();

		if ( L16IsActive( mer->online_status ) ) {
			xargv[ xargc++ ] = "-ownship" ;
			xargv[ xargc++ ] = ownShipTnString ;
		}

		xargv[ xargc++ ] = ( char * )FwdDebugLevel() ;

		if ( keep_real_time ) 
			xargv[ xargc++ ] = "-keep-real-time" ;

		xargv[ xargc++ ] = 0 ;

		execvp( "Link16-Dbm", xargv ) ;
		
		// should not get here 
		perror( "error during execvp of Ldbm" ) ;
		exit( 1 ) ;
	default:
		::chan.addPid( ldbm_pid ) ;
		( ( LCCommsChannel & )chan).writeF() ;
	}
#endif
