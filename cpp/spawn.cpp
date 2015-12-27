/**************************************************************************/
/* Server example that uses sendmsg() to create worker jobs               */
/**************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
//#include <spawn.h>
#include <unistd.h>
 
#define SERVER_PORT  12345

#define CONTROLLEN sizeof (struct cmsghdr) + sizeof (int)
int send_fd (int sockfd, int fd) {
	char tmpbuf[CONTROLLEN];
	struct cmsghdr *cmptr = (struct cmsghdr *) tmpbuf;
	struct iovec iov[1];
	struct msghdr msg;
	char buf[1];
	iov[0].iov_base = buf;
	iov[0].iov_len = 1;
	msg.msg_iov = iov;
	msg.msg_iovlen = 1;
	msg.msg_name = NULL;
	msg.msg_namelen = 0;
	msg.msg_control = cmptr;
	msg.msg_controllen = CONTROLLEN;
	cmptr->cmsg_level = SOL_SOCKET;
	cmptr->cmsg_type = SCM_RIGHTS;
	cmptr->cmsg_len = CONTROLLEN;
	*(int *)CMSG_DATA (cmptr) = fd;
	for( ; ; ) {
		if (sendmsg(sockfd, &msg, 0) != 1) {
			if( EINTR == errno ) {
				continue;
			} else {
				return -1;    }
		} else {
			break;
		}
	}
	return 0;
}
int recv_fd( int sockfd ) {
	char tmpbuf[CONTROLLEN];
	struct cmsghdr *cmptr = (struct cmsghdr *) tmpbuf;
	struct iovec iov[1];
	struct msghdr msg;
	char buf[1];
	iov[0].iov_base = buf;
	iov[0].iov_len = sizeof (buf);
	msg.msg_iov = iov;
	msg.msg_iovlen = 1;
	msg.msg_name = NULL;
	msg.msg_namelen = 0;
	msg.msg_control = cmptr;
	msg.msg_controllen = CONTROLLEN;
	for( ; ; ) {
		if (recvmsg(sockfd, &msg, 0) <= 0) {
			if( EINTR == errno ) {
				continue;
			}
			else {
				return -1;
			}
		} else {
			break;
		}
	}
	return *(int *) CMSG_DATA (cmptr);
}



int main (int argc, char *argv[])
{
   int    i, num, rc, on = 1;
   int    listen_sd, accept_sd;
   int    server_sd, worker_sd, pair_sd[2];
   pid_t pid;
//   int    spawn_fdmap[1];
 //  char  *spawn_argv[1];
  // char  *spawn_envp[1];
   //struct inheritance   inherit;
   //struct msghdr        msg;
   struct sockaddr_in   addr;


   /*************************************************/
   /* If an argument was specified, use it to       */
   /* control the number of incoming connections    */
   /*************************************************/
   if (argc >= 2)
      num = atoi(argv[1]);
   else
      num = 1;

   /*************************************************/
   /* Create an AF_INET stream socket to receive    */
   /* incoming connections on                       */
   /*************************************************/
   listen_sd = socket(AF_INET, SOCK_STREAM, 0);
   if (listen_sd < 0)
   {
      perror("socket() failed");
      exit(-1);
   }

   /*************************************************/
   /* Allow socket descriptor to be reuseable       */
   /*************************************************/
   rc = setsockopt(listen_sd,
                   SOL_SOCKET,  SO_REUSEADDR,
                   (char *)&on, sizeof(on));
   if (rc < 0)
   {
      perror("setsockopt() failed");
      close(listen_sd);
      exit(-1);
   }

   /*************************************************/
   /* Bind the socket                               */
   /*************************************************/
   memset(&addr, 0, sizeof(addr));
   addr.sin_family      = AF_INET;
   addr.sin_addr.s_addr = htonl(INADDR_ANY);
   addr.sin_port        = htons(SERVER_PORT);
   rc = bind(listen_sd,
             (struct sockaddr *)&addr, sizeof(addr));
   if (rc < 0)
   {
      perror("bind() failed");
      close(listen_sd);
      exit(-1);
   }

   /*************************************************/
   /* Set the listen back log                       */
   /*************************************************/
   rc = listen(listen_sd, 5);
   if (rc < 0)
   {
      perror("listen() failed");
      close(listen_sd);
      exit(-1);
   }

   /*************************************************/
   /* Create a pair of UNIX datagram sockets        */
   /*************************************************/
   rc = socketpair(AF_UNIX, SOCK_DGRAM, 0, pair_sd);
   if (rc != 0)
   {
      perror("socketpair() failed");
      close(listen_sd);
      exit(-1);
   }
   server_sd = pair_sd[0];
   worker_sd = pair_sd[1];

   /*************************************************/
   /* Initialize parms prior to entering for loop   */
   /*                                               */
   /* The worker socket descriptor is mapped to     */
   /* descriptor 0 in the child program.            */
   /*************************************************/
   //memset(&inherit, 0, sizeof(inherit));
   //spawn_argv[0]  = NULL;
   //spawn_envp[0]  = NULL;
   //spawn_fdmap[0] = worker_sd;

   /*************************************************/
   /* Create each of the worker jobs                */
   /*************************************************/
   printf("Creating worker jobs...\n");
   for (i=0; i < 2; i++)
   {
    //  pid = spawn("/QSYS.LIB/QGPL.LIB/WRKR2.PGM",
    //             1, spawn_fdmap, &inherit,
    //            spawn_argv, spawn_envp);
	  pid = fork();
      if (pid < 0)
      {
         perror("fork() failed");
         close(listen_sd);
         close(server_sd);
         close(worker_sd);
         exit(-1);
      } else if (pid == 0){
		  close(server_sd);
		  close(listen_sd);
		  printf("* worker = %d started\n", getpid());
		  while(1){
			  int fd_recv = recv_fd(worker_sd);
			  printf("* WORKER sd=%d fd=%d pid=%d\n", worker_sd, fd_recv, getpid());
			  if(fd_recv<0){
				  return 0;
			  }
			  char buf[5210]="";
			  int tmplen = read(fd_recv, buf,sizeof(buf));
			  printf("*** DATA Begin***\n%s\n+++ DATA End\n",buf);
			  int len = snprintf(buf,sizeof(buf),"HTTP/1.1 200 OK\r\n\r\nsuccess. fid=%u, pid=%d\n", fd_recv, getpid());

			  tmplen = write(fd_recv, buf, len);
			  close(fd_recv);
			  sleep(1);
		  }
		  return 0;
	  }
   }

   /*************************************************/
   /* Close down the worker side of the socketpair  */
   /*************************************************/
   close(worker_sd);

   /*************************************************/
   /* Inform the user that the server is ready      */
   /*************************************************/
   printf("The server is ready. port=%d\n", SERVER_PORT);

   /*************************************************/
   /* Go through the loop once for each connection  */
   /*************************************************/
   for (i=0; i < num; i++)
   {
      /**********************************************/
      /* Wait for an incoming connection            */
      /**********************************************/
      printf("--------------- Interation: %d\n", i+1);
      printf("master: waiting on accept()\n");
      accept_sd = accept(listen_sd, NULL, NULL);
      if (accept_sd < 0)
      {
         perror("accept() failed");
         close(listen_sd);
         close(server_sd);
         exit(-1);
      }
      printf("accept completed successfully\n");

      /**********************************************/
      /* Initialize message header structure        */
      /**********************************************/
//      memset(&msg, 0, sizeof(msg));

      /**********************************************/
      /* We are not sending any data so we do not   */
      /* need to set either of the msg_iov fields.  */
      /* The memset of the message header structure */
      /* will set the msg_iov pointer to NULL and   */
      /* it will set the msg_iovcnt field to 0.     */
      /**********************************************/

      /**********************************************/
      /* The only fields in the message header      */
      /* structure that need to be filled in are    */
      /* the msg_accrights fields.                  */
      /**********************************************/
      //msg.msg_accrights  = (char *)&accept_sd;
      //msg.msg_accrightslen = sizeof(accept_sd);

      /**********************************************/
      /* Give the incoming connection to one of the */
      /* worker jobs.                               */
      /*                                            */
      /* NOTE: We do not know which worker job will */
      /*       get this inbound connection.         */
      /**********************************************/
//     rc = sendmsg(server_sd, &msg, 0);
	  rc = send_fd(server_sd, accept_sd);
      if (rc < 0)
      {
         perror("sendmsg() failed");
         close(listen_sd);
         close(accept_sd);
         close(server_sd);
         exit(-1);
      }
      printf("sendmsg completed successfully\n");

      /**********************************************/
      /* Close down the incoming connection since   */
      /* it has been given to a worker to handle    */
      /**********************************************/
      close(accept_sd);
   }

   /*************************************************/
   /* Close down the server and listen sockets      */
   /*************************************************/
   close(server_sd);
   close(listen_sd);
   return 0;
}
