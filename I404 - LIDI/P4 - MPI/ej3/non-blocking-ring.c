/*######################################################################

 Example 6 : MPI_Isend MPI_Irecv

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

   This example shows that MPI_Isend and MPI_Irecv are much more
   appropriate to accomplish this work.
   
   MPI_Isend and MPI_Irecv are non-blocking, meaning that the function
   call returns before the communication is completed. Deadlock then
   becomes impossible with non-blocking communication, but other
   precautions must be taken when using them. In particular you will
   want to be sure at a certain point, that your data has effectively
   arrived! You will then place an MPI_Wait call for each send and/or
   receive you want to be completed before advancing in the program.
   
   It is clear that in using non-blocking call in this example, all
   the exchanges between the tasks occur at the same time.

   Before the communication, task 0 gather the sum of all the vectors
   to sent from each tasks, and prints them out. Similarly after
   the communication, task 0 gathers all the sum of the vectors received
   by each task and prints them out along with the communication times.
   
   Example 5 show how to use blocking communication (MPI_Send and
   MPI_Recv) to accomplish the same work much less efficiently.

   The size of the vecteur (buffsize) is given as an argument to
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
   MPI_Request	send_request,recv_request;
   int          ierr,i,j,itask,recvtaskid;
   int	        buffsize;
   double       *sendbuff,*recvbuff;
   double       sendbuffsum,recvbuffsum;
   double       sendbuffsums[1024],recvbuffsums[1024];
   double       inittime,totaltime,recvtime,recvtimes[1024];
   
   /*===============================================================*/
   /* MPI Initialisation. It's important to put this call at the    */
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
     printf(" Comunicacion punto-a-punto no bloqueante: MPI_Isend, MPI_Irecv \n\n");
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
     ierr=MPI_Isend(sendbuff,buffsize,MPI_DOUBLE,
	           taskid+1,0,MPI_COMM_WORLD,&send_request);   
     ierr=MPI_Irecv(recvbuff,buffsize,MPI_DOUBLE,
	           ntasks-1,MPI_ANY_TAG,MPI_COMM_WORLD,&recv_request);
     recvtime = MPI_Wtime();
   }
   else if( taskid == ntasks-1 ){
     ierr=MPI_Isend(sendbuff,buffsize,MPI_DOUBLE,
	           0,0,MPI_COMM_WORLD,&send_request);   
     ierr=MPI_Irecv(recvbuff,buffsize,MPI_DOUBLE,
	           taskid-1,MPI_ANY_TAG,MPI_COMM_WORLD,&recv_request);
     recvtime = MPI_Wtime();
   }
   else{
     ierr=MPI_Isend(sendbuff,buffsize,MPI_DOUBLE,
	           taskid+1,0,MPI_COMM_WORLD,&send_request);
     ierr=MPI_Irecv(recvbuff,buffsize,MPI_DOUBLE,
	           taskid-1,MPI_ANY_TAG,MPI_COMM_WORLD,&recv_request);
     recvtime = MPI_Wtime();
   }
   ierr=MPI_Wait(&send_request,&status);
   ierr=MPI_Wait(&recv_request,&status);

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
   /* Free the allocated memory.                                    */
   free(recvbuff);
   free(sendbuff);

   /*===============================================================*/
   /* MPI finalisation.                                             */
   MPI_Finalize();

}