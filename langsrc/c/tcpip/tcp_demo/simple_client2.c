#include <sys/types.h>
#if defined (__WIN32__)
	#include <winsock2.h>
#elif
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
#endif

// https://stackoverflow.com/questions/61988674/implement-netinet-in-h-in-windows-visual-studioc
#include <stdio.h>

void init(void)
{
	#if defined (__WIN32__)
		WSADATA wsadata;
		int iResult = WSAStartup( MAKEWORD( 2, 2 ), &wsadata );
		if (iResult != 0) {
			printf("WSAStartup failed with error: %d\n", iResult);
		}
	#elif
	#endif
}
void wsaclean(void)
{
	#if defined (__WIN32__)
		WSACleanup();
	#elif
	#endif
}

int main( void )
{
	struct sockaddr_in peer;
	int s;
	int rc;
	char buf[ 20 ];

	init();
	peer.sin_family = AF_INET;
	peer.sin_port = htons( 7500 );
	peer.sin_addr.s_addr = inet_addr( "127.0.0.1" );

	s = socket( AF_INET, SOCK_STREAM, 0 );
	if ( s < 0 )
	{
		perror( "socket call failed" );
		exit( 1 );
	}
/*@.bp*/
	rc = connect( s, ( struct sockaddr * )&peer, sizeof( peer ) );
	if ( rc )
	{
		perror( "connect call failed" );
		exit( 1 );
	}
	printf("please input:");
	scanf("%s", buf);
	while (strcmp(buf, "quit"))
	{
		/* code */
		rc = send( s, buf, strlen(buf), 0 );
		//rc = send( s, "1", 1, 0 );
		printf("send bytes:%d\n", rc);
		if ( rc < 0 )
		{
			perror( "send call failed" );
			exit( 1 );
		}
		rc = recv( s, buf, 1, 0 );
		if ( rc < 0 )
			perror( "recv call failed" );
		else
			printf( "%c\n", buf[ 0 ] );
		printf("please input:");
		scanf("%s", buf);
	}
	

	wsaclean();
	exit( 0 );
}
