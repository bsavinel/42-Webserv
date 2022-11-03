#ifndef DEFINE_HPP
#define DEFINE_HPP

/*
 *	Typedef
 */

typedef	struct sockaddr_in	t_sockaddr_in;
typedef	struct sockaddr		t_sockaddr;
typedef int t_socket;
typedef int t_instEpoll;
typedef struct epoll_event t_epoll_event;

/*
 *	Define
 */

# define LEN_TO_READ 1000
# define DIR_PATH_TMP_UPLOAD_FILE "./bsavinel/site/tmpFilePost/tmpUploadFile_"

 /*
  *	Enumeration
  */

typedef enum e_method
{
	GET,
	POST,
	DELETE
} t_method;

#endif