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
///////////////////////////timer implementation//////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////

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
 int sendbase;
 int next_seq_no;
 int max_seq_no;
 struct pkt sndpkt[1000000];//infinite(large) sequence number space
 //sndpkt[sendbase+n+1];// +1 to prevent using 0 index of array
 float timeout;
 float tick;
 void make_pkt(int seq, struct msg message) 
 {
 	 sndpkt[next_seq_no].seqnum=seq;
   	 sndpkt[next_seq_no].acknum=seq;
   	 strcpy(sndpkt[next_seq_no].payload,message.data);
   	 int int_message=checksum(sndpkt[next_seq_no].payload);//atoi(sndpkt.payload);
   	 sndpkt[next_seq_no].checksum=int_message+sndpkt[next_seq_no].seqnum+sndpkt[next_seq_no].acknum;
 }
 float timers[1000000];//timers correspondiing to sequence number
 int t_no;
 void starttimer_seq(int seq)
 {
   
 	//timers[t_no]=timeout;
 	timers[t_no]=timeout;


 }
/* called from layer 5, passed the data to be sent to other side */
int packet_no;//0 in first call set in init
int ack0a,ack1a;//0 in first call set in init
struct msg buffer[100000];
int i=0;//buffer traverse variables
int buff_read=0;
int rnd=1;
int first_packet;
int acks[1000000];
void A_output(struct msg message)
{    
	buffer[i]= message;
	i++;//saved the messages in the buffer
 	 
    if(next_seq_no<sendbase+n)
    {
    	make_pkt(next_seq_no,buffer[buff_read]);
    	buff_read++;
    	tolayer3(0, sndpkt[next_seq_no]);
    	//if(sendbase==next_seq_no)
    	//	starttimer(0,timeout);
    	starttimer_seq(next_seq_no);
    	if(first_packet)
    		{
    			starttimer(0,tick);
    			
    			first_packet=0;
    		}
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

int timeout_timer_no;
void decrement_timers()
{
	for(int x=0;sendbase+x<=next_seq_no-1;x++)
	     	{
	     		timers[sendbase+x]=timers[sendbase+x]-1;
	     	}
}

// int smallest_unacked_packet()
// {
//   for(int trav=0;sendbase+trav<sendbase+n;trav++)
//   {
//   	if(acks[sendbase+trav]==-1)
//   	{return sendbase+trav;}
//     else
//     	return -1;
//   }

// }

// void next_unacked_packet()
// {

// }
		
/* called from layer 3, when a packet arrives for layer 4 */
float received_mark;
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
        
		// sendbase = packet.acknum+1;//printf("received ack %d\n",packet.acknum );
		// if(sendbase== next_seq_no)
		// 	stoptimer(0);
		// else
		// 	starttimer(0,timeout);
		if(packet.acknum<=sendbase+n && packet.acknum>=sendbase)
		{
			acks[packet.seqnum]=packet.seqnum;//mark received, unreceived packets have acknum value -1
			timers[packet.seqnum]=received_mark;//stop timer for that seq no
			for(int trav=0;sendbase+trav<sendbase+n;trav++)
			  {
			  	if(acks[sendbase+trav]!=-1)
			  		{
			  			sendbase++;
			  			break;
			  		}
			  }		


			// sendbase=smallest_unacked_packet();//move window to smallest unacked packet
			// if (sendbase== -1)
			// 	{printf("problem at 165\n"); exit(1);}


		}
		// if(packet.seqnum==smallest_unacked_packet())
		// {
		// 	sendbase=next_unacked_packet();
		// }
	}
	else
		{//printf("corrupted ack received\n");
         }
}

/* called when A's timer goes off */

void A_timerinterrupt()
 {   //printf("timeout packet %d\n",sndpkt[sendbase].seqnum);
	 timeout_timer_no=-1;
    
     //for(int x=0;sendbase+x<=next_seq_no-1;x++)
     
     
     for(int x=0;sendbase+x<=next_seq_no-1;x++)
     	{
     		if(timers[sendbase+x]!=received_mark&&timers[sendbase+x]==0)
     			{
     				timeout_timer_no=sendbase+x;
     			}
     	}
     
     tolayer3(0, sndpkt[timeout_timer_no]);//printf("retransmitting packet %d\n",sndpkt[sendbase+i].seqnum);
     timers[timeout_timer_no]=timeout;//restart timer of lost ack packet
     starttimer(0,tick);
     
}  
 

/* the following routine will be called once (only) before any other */
/* entity A routines are called. You can use it to do any initialization */
void A_init()
{
   packet_no=0;
   timeout=11;
   tick=10;
   ack1a=1;
   ack0a=1;  
   n= getwinsize();
   sendbase=1;
   next_seq_no=1;
   max_seq_no=n+1;
   first_packet=1;
   for(int z=0;z<1000000;z++)
   acks[z]=-1;
   received_mark=0.7167278738;
   
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
// int smallest_not_yet_recvd_packet()
// {
//   while(int trav=0;rcvbase+trav<rcvbase+n;trav++)
//   {
//   	if(received_packets_no[rcvbase+trav]==-1)
//   	{return rcvbase+trav;}
//     else
//     	return -1;
//   }

// }

/* called from layer 3, when a packet arrives for layer 4 at B*/
int exp_pack;
int received_packets_no[1000000];//stores seq no of uncorrupted received packets
struct pkt received_packets[1000000];//received packets buffer
int rcvbase;
void B_input(struct pkt packet)
{
	////printf("packet %d received\n",packet.seqnum);
	int int_payload = checksum(packet.payload);
	int calc_checksum = packet.seqnum+packet.acknum+int_payload;
	if ( packet.checksum==calc_checksum)
	{   
 		received_packets_no[packet.seqnum]=packet.seqnum;//mark received
 		if(packet.seqnum>=rcvbase&&packet.seqnum<=rcvbase+n-1)
 		{   make_ack(packet.acknum);
			tolayer3(1, ack);//printf("ack %d sent\n",ack.acknum);
			//printf("packet %d received,packet.checksum:%d,calc_checksum:%d,data:%s\n",packet.seqnum,packet.checksum,calc_checksum, packet.payload);
			if(exp_pack==packet.seqnum)
			{   
				tolayer5(1, packet.payload);
			    exp_pack++;
			    while(received_packets_no[exp_pack]!=-1)//deliver in ordered buffered packets
			    {
			    	tolayer5(1, received_packets[exp_pack].payload);
			    	exp_pack++;	
			    }
			    rcvbase=exp_pack;//shift window to next not yet received packet
			    // for(int p=0;rcvbase+p<=rcvbase+n-1;p++)//deliver in ordered buffered packets
			    // {
			    // 	if(received_packets_no[rcvbase+p]!=-1&&received_packets_no[rcvbase+p]!=-2)
			    // 	{
			    // 		tolayer5(1, received_packets[rcvbase+p].payload);
			    // 	}
			    // }

			    
			    // if(exp_pack==0)
			    // 	{exp_pack=1;//printf("ack 0 sent\n");}
			    
			    // else if(exp_pack==1)
			    // 	{exp_pack=0;//printf("ack 1 sent\n");}
			}
			else //out of order so buffer
			{
				received_packets[packet.seqnum]=packet;
			}
		}
		if(packet.seqnum>=rcvbase-n&&packet.seqnum<=rcvbase-1)
		{
			make_ack(packet.acknum);
			tolayer3(1, ack);//printf("ack %d sent\n",ack.acknum);
		}
	}	
	// 	else
	// 	{
	// 		//printf("packet %d received\n",packet.seqnum);
	// 		//printf("checksum mismatch, packet checksum: %d, calc_checksum: %d,packet %d dropped\n",packet.checksum,calc_checksum,packet.seqnum );
	// 		tolayer3(1, ack);//printf("ack %d resent\n",ack.acknum);
	// 	}
		
	// }

	// else
	// {   //printf("packet %d received\n",packet.seqnum);
	// 	//printf("out of order packet received dropped, was expecting packet%d\n",exp_pack);
	// 	tolayer3(1, ack);//printf("ack %d resent\n",ack.acknum);
	// }
}


/* the following rouytine will be called once (only) before any other */
/* entity B routines are called. You can use it to do any initialization */
void B_init()
{
	exp_pack = 1;
	make_ack(1);
	for(int z=0;z<1000000;z++)
    received_packets_no[z]=-1;
	rcvbase=1;
}
