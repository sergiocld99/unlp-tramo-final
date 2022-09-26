/*######################################################################

 Example 5 : MPI_Send and MPI_Recv

 Description:
   Examples 5 and 6 demonstrate the difference between blocking
   and non-blocking point-to-point communication.
          Example 5: MPI_Send/MPI_Recv (blocking)
          Example 6: MPI_Isend/MPI_Irecv (non-blocking)
   
              sendbuff   recvbuff       sendbuff   recvbuff
                                         
              ########   ########       ########   ########
              #      #   #      #       #      #   #      #
          0   #  AA  #   #      #       #  AA  #   #  EE  #
              #      #   #      #       #      #   #      #
              ########   ########       ########   ########
     T        #      #   #      #       #      #   #      #
          1   #  BB  #   #      #       #  BB  #   #  AA  #
     a        #      #   #      #       #      #   #      #
              ########   ########       ########   ########
     s        #      #   #      #       #      #   #      #
          2   #  CC  #   #      #       #  CC  #   #  BB  #
     k        #      #   #      #       #      #   #      #
              ########   ########       ########   ########
     s        #      #   #      #       #      #   #      #
          3   #  DD  #   #      #       #  DD  #   #  CC  #
              #      #   #      #       #      #   #      #
              ########   ########       ########   ########
              #      #   #      #       #      #   #      #
          4   #  EE  #   #      #       #  EE  #   #  DD  #
              #      #   #      #       #      #   #      #
              ########   ########       ########   ########
              
                     BEFORE                    AFTER

   Each task transfers a vector of random numbers (sendbuff) to the
   next task (taskid+1). The last task transfers it to task 0. 
   Consequently, each task receives a vector from the preceding task
   and puts it in recvbuff.
   
   This example shows that MPI_Send and MPI_Recv are not the most
   efficient functions to perform this work. Since they work in
   blocking mode (i.e. waits for the transfer to finish before 
   continuing its execution). In order to receive their vector, 
   each task must post an MPI_Recv corresponding to the MPI_Send
   of the sender, and so wait for the reception to complete before
   sending sendbuff to the next task. In order to avoid a deadlock,
   one of the task must initiate the communication and post its 
   MPI_Recv after the MPI_Send. In this example, the last task 
   initiate this cascading process where each consecutive task is
   waiting for the complete reception from the preceding task before
   starting to send "sendbuff" to the next. The whole process completes
   when the last task have finished its reception.
   
   Before the communication, task 0 gather the sum of all the vectors
   to sent from each tasks, and prints them out. Similarly after
   the communication, task 0 gathers all the sum of the vectors received
   by each task and prints them out along with the communication times.
   
   Example 6 show how to use non-blocking communication (MPI_Isend and
   MPI_Irecv) to accomplish the same work is much less time.

   The size of the vector (buffsize) is given as an argument to
   the program at run time.
   
 Author: Carol Gauthier
         Centre de Calcul scientifique
         Universite de Sherbrooke

 Last revision: September 2005

######################################################################*/

#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "math.h"
#include "mpi.h"

int main(int argc,char** argv)
{

   int          taskid, ntasks;
   MPI_Status   status;
   MPI_Request	req[1024];
   int          ierr,i,j,itask,recvtaskid;
   int	        buffsize;
   double       *sendbuff,*recvbuff;
   double       sendbuffsum,recvbuffsum;
   double       sendbuffsums[1024],recvbuffsums[1024];
   double       inittime,totaltime,recvtime,recvtimes[1024];
   
   /*===============================================================*/
   /* MPI Initialisation. Its important to put this call at the     */
   /* begining of the program, after variable declarations.         */
   MPI_Init(&argc, &argv);

   /*===============================================================*/
   /* Get the number of MPI tasks and the taskid of this task.      */
   MPI_Comm_rank(MPI_COMM_WORLD,&taskid);
   MPI_Comm_size(MPI_COMM_WORLD,&ntasks);


   /*===============================================================*/
   /* Get buffsize value from program arguments.                    */
	if ((argc != 2) || ((buffsize = atoi(argv[1])) <= 0) ) {
	    printf("\nUsar: %s size \n  size: Dimension del vector\n", argv[0]);
		exit(1);
	}


   /*===============================================================*/
   /* Printing out the description of the example.                  */
   if ( taskid == 0 ){
     printf("\n\n\n");
     printf("##########################################################\n\n");
     printf(" Comunicacion punto-a-punto bloqueante: MPI_Send, MPI_Recv \n\n");
     printf(" Dimension del vector: %d\n",buffsize);
     printf(" Numero de procesos: %d\n\n",ntasks);
     printf("##########################################################\n\n");
     printf("                --> ANTES DE LA COMUNICACION <--\n\n");
   }

   /*=============================================================*/
   /* Memory allocation.                                          */ 
   sendbuff=(double *)malloc(sizeof(double)*buffsize);
   recvbuff=(double *)malloc(sizeof(double)*buffsize);

   /*=============================================================*/
   /* Vectors and/or matrices initalisation.                      */
   srand((unsigned)time( NULL ) + taskid);
   for(i=0;i<buffsize;i++){
     sendbuff[i]=(double)rand()/RAND_MAX;
   }
      
   /*==============================================================*/
   /* Print out before communication.                              */

   sendbuffsum=0.0;
   for(i=0;i<buffsize;i++){
     sendbuffsum += sendbuff[i];
   }   
   ierr=MPI_Gather(&sendbuffsum,1,MPI_DOUBLE,
                   sendbuffsums,1, MPI_DOUBLE,
                   0,MPI_COMM_WORLD);
                   
   if(taskid==0){
     for(itask=0;itask<ntasks;itask++){
       recvtaskid=itask+1;
       if(itask==(ntasks-1))recvtaskid=0;
       printf("Proceso %d : Suma del vector enviado a %d = %e\n",
               itask,recvtaskid,sendbuffsums[itask]);
     }  
   }

   /*===============================================================*/
   /* Communication.                                                */

   inittime = MPI_Wtime();

   if ( taskid == 0 ){
     ierr=MPI_Recv(recvbuff,buffsize,MPI_DOUBLE,
	           ntasks-1,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
     recvtime = MPI_Wtime();
     ierr=MPI_Send(sendbuff,buffsize,MPI_DOUBLE,
	           taskid+1,0,MPI_COMM_WORLD);   
   }
   else if( taskid == ntasks-1 ){
     ierr=MPI_Send(sendbuff,buffsize,MPI_DOUBLE,
	           0,0,MPI_COMM_WORLD);   
     ierr=MPI_Recv(recvbuff,buffsize,MPI_DOUBLE,
	           taskid-1,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
     recvtime = MPI_Wtime();
   }
   else{
     ierr=MPI_Recv(recvbuff,buffsize,MPI_DOUBLE,
	           taskid-1,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
     recvtime = MPI_Wtime();
     ierr=MPI_Send(sendbuff,buffsize,MPI_DOUBLE,
	           taskid+1,0,MPI_COMM_WORLD);
   }
   
   MPI_Barrier(MPI_COMM_WORLD);
   
   totaltime = MPI_Wtime() - inittime;

   /*===============================================================*/
   /* Print out after communication.                                */

   recvbuffsum=0.0;
   for(i=0;i<buffsize;i++){
     recvbuffsum += recvbuff[i];
   }   

   ierr=MPI_Gather(&recvbuffsum,1,MPI_DOUBLE,
                   recvbuffsums,1, MPI_DOUBLE,
                   0,MPI_COMM_WORLD);
                   
   ierr=MPI_Gather(&recvtime,1,MPI_DOUBLE,
                   recvtimes,1, MPI_DOUBLE,
                   0,MPI_COMM_WORLD);

   if(taskid==0){
     printf("\n");
     printf("##########################################################\n\n");
     printf("                --> DESPUES DE LA COMUNICACION <-- \n\n");
     for(itask=0;itask<ntasks;itask++){
       printf("Proceso %d : Suma del vector recibido = %e : Tiempo=%f segundos\n",
               itask,recvbuffsums[itask],recvtimes[itask]);
     }
     printf("\n");
     printf("##########################################################\n\n");
     printf(" Tiempo de comunicacion : %f seconds\n\n",totaltime);  
     printf("##########################################################\n\n");
   }

   /*===============================================================*/
   /* Liberation de la memoire                                      */
   free(recvbuff);
   free(sendbuff);

   /*===============================================================*/
   /* Finalisation de MPI                                           */
   MPI_Finalize();

}