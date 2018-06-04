#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <ctype.h>
#include <string.h>

#include "../include/simulator.h"

/* ******************************************************************
 ALTERNATING BIT AND GO-BACK-N NETWORK EMULATOR: VERSION 1.1  J.F.Kurose

   This code should be used for PA2, unidirectional data transfer 
   protocols (from A to B). Network properties:
   - one way network delay averages five time units (longer if there
     are other messages in the channel for GBN), but can be larger
   - packets can be corrupted (either the header or the data portion)
     or lost, according to user-defined probabilities
   - packets will be delivered in the order in which they were sent
     (although some can be lost).
**********************************************************************/

/********* STUDENTS WRITE THE NEXT SEVEN ROUTINES *********/

int checksum(char* s)
{
	int sum = 0;
	int j=0;
	while (j<20)
	{
		sum += s[j];
		j++;
	}
	return sum;
}
 int n;//window
 int base;
 int next_seq_no;
 int max_seq_no;
 struct pkt sndpkt[1000000];//infinite(large) sequence number space
 //sndpkt[base+n+1];// +1 to prevent using 0 index of array
 float timeout;
 void make_pkt(int seq, struct msg message) 
 {
 	 sndpkt[next_seq_no].seqnum=seq;
   	 sndpkt[next_seq_no].acknum=seq;
   	 strcpy(sndpkt[next_seq_no].payload,message.data);
   	 int int_message=checksum(sndpkt[next_seq_no].payload);//atoi(sndpkt.payload);
   	 sndpkt[next_seq_no].checksum=int_message+sndpkt[next_seq_no].seqnum+sndpkt[next_seq_no].acknum;
 }
/* called from layer 5, passed the data to be sent to other side */
int packet_no;//0 in first call set in init
int ack0a,ack1a;//0 in first call set in init
struct msg buffer[100000];
int i=0;//buffer traverse variables
int buff_read=0;
int rnd=1;
void A_output(struct msg message)
{    
	buffer[i]= message;
	i++;//saved the messages in the buffer
 	 
    if(next_seq_no<base+n)
    {
    	make_pkt(next_seq_no,buffer[buff_read]);
    	buff_read++;
    	tolayer3(0, sndpkt[next_seq_no]);
    	if(base==next_seq_no)
    		starttimer(0,timeout);
    	next_seq_no++;
    }

    
	// if(packet_no==0&&ack1a==1)
	// {
	//   	 ack0a=0;
	//   	 make_pkt(0,buffer[buff_read]);
	//   	 buff_read++;
	//   	 tolayer3(0, sndpkt[next_seq_no]);//0 called by A, 1 called by B
	//   	 //printf("\n//////////////round %d,buffer[%d],message:%s/////\n",rnd,i-1,message.data );
	//   	 rnd++;
	//   	 starttimer(0, timeout);//printf("packet 0 sent,timer 0 started,checksum:%d\n",sndpkt[next_seq_no].checksum);
	//   	 packet_no=1;
	  	 
	// }

	// if(packet_no==1&&ack0a==1)
	// {
	//   	 ack1a=0;
	//   	 make_pkt(1,buffer[buff_read]);
	//   	 buff_read++;
	//   	 tolayer3(0, sndpkt);//0 called by A, 1 called by B
	//   	 //printf("\n//////////////round %d,buffer[%d],message:%s//////\n",rnd,i-1,message.data );
	//   	 rnd++;
	//   	 starttimer(0, timeout);//printf("packet 1 sent,timer 1 started\n");
	//   	 packet_no=0;
	  	 
	// }
}

/* called from layer 3, when a packet arrives for layer 4 */
void A_input(struct pkt packet)
{   
	//int int_payload = atoi(packet.payload);
	//int calc_checksum = packet.seqnum+packet.acknum+int_payload;
	////printf("ack %d received\n",packet.acknum);
	int calc_checksum = packet.acknum+packet.seqnum;
	
	if ( packet.checksum==calc_checksum)
	{
		// if(packet.acknum==0)
		// {
		// 	ack0a=1;
		// 	stoptimer(0);//printf("ack 0 received,timer 0 stopped\n");
		// }

		// if(packet.acknum==1)
		// {
		// 	ack1a=1;
		// 	stoptimer(0);//printf("ack 1 received,timer 1 stopped\n");
		// }

		base = packet.acknum+1;//printf("received ack %d\n",packet.acknum );
		if(base== next_seq_no)
			stoptimer(0);
		else
			starttimer(0,timeout);
	}
	else
		{//printf("corrupted ack received\n");
         }
}

/* called when A's timer goes off */
void A_timerinterrupt()
 {   //printf("timeout packet %d\n",sndpkt[base].seqnum);
	 
    
     for(int x=0;base+x<=next_seq_no-1;x++)
     tolayer3(0, sndpkt[base+x]);//printf("retransmitting packet %d\n",sndpkt[base+i].seqnum);
    starttimer(0,timeout);
    
    
}  

/* the following routine will be called once (only) before any other */
/* entity A routines are called. You can use it to do any initialization */
void A_init()
{
   packet_no=0;
   timeout=15;
   ack1a=1;
   ack0a=1;  
   n= getwinsize();
   base=1;
   next_seq_no=1;
   max_seq_no=n+1;
}

/* Note that with simplex transfer from a-to-B, there is no B_output() */
struct pkt ack;
void make_ack(int seq) 
 {
 	 //ack.seqnum=seq;
   	 ack.acknum=seq;
   	 ack.seqnum=seq;
   	 //strcpy(sndpkt.payload,message.data);
   	 //int int_message=atoi(sndpkt.payload);
   	 ack.checksum=ack.acknum+ack.seqnum;
 }


/* called from layer 3, when a packet arrives for layer 4 at B*/
int exp_pack;
void B_input(struct pkt packet)
{
	////printf("packet %d received\n",packet.seqnum);
	
	if ( exp_pack==packet.seqnum)
	{  
 		int int_payload = checksum(packet.payload);
		int calc_checksum = packet.seqnum+packet.acknum+int_payload;
		//printf("packet %d received,packet.checksum:%d,calc_checksum:%d,data:%s\n",packet.seqnum,packet.checksum,calc_checksum, packet.payload);
		if(packet.checksum==calc_checksum)
		{   
			make_ack(packet.acknum);
			tolayer3(1, ack);//printf("ack %d sent\n",ack.acknum);
		    tolayer5(1, packet.payload);
		    exp_pack++;
		    
		    // if(exp_pack==0)
		    // 	{exp_pack=1;//printf("ack 0 sent\n");}
		    
		    // else if(exp_pack==1)
		    // 	{exp_pack=0;//printf("ack 1 sent\n");}
		}
		else
		{
			//printf("packet %d received\n",packet.seqnum);
			//printf("checksum mismatch, packet checksum: %d, calc_checksum: %d,packet %d dropped\n",packet.checksum,calc_checksum,packet.seqnum );
			tolayer3(1, ack);//printf("ack %d resent\n",ack.acknum);
		}
		
	}

	else
	{   //printf("packet %d received\n",packet.seqnum);
		//printf("out of order packet received dropped, was expecting packet%d\n",exp_pack);
		tolayer3(1, ack);//printf("ack %d resent\n",ack.acknum);
	}
}

/* the following rouytine will be called once (only) before any other */
/* entity B routines are called. You can use it to do any initialization */
void B_init()
{
	exp_pack = 1;
	make_ack(1);
}
