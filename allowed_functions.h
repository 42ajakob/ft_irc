/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   allowed_functions.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 16:31:19 by JFikents          #+#    #+#             */
/*   Updated: 2024/10/08 20:38:31 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ALLOWED_FUNCTIONS_H
# define ALLOWED_FUNCTIONS_H

# include <sys/socket.h> /* socket, setsockopt, getsockname, getaddrinfo, bind,
	connect, listen, accept, send, recv
*/
# include <netdb.h> // getprotobyname, gethostbyname, getaddrinfo
# include <sys/types.h> // getaddrinfo, bind, connect
# include <arpa/inet.h> // htons, htonl, ntohl, ntohs, inet_addr, inet_ntoa
# include <signal.h> // signal
# include <unistd.h> // close, lseek
# include <sys/stat.h> // fstat
# include <fcntl.h> // fcntl
# include <poll.h> // poll

/*
@brief creates a communication endpoint (TCP/UDP) and returns a file
 *	descriptor that refers to that endpoint.
@param domain refers to which protocol family the socket belongs to.
 *	can be AF_INET (IPv4), AF_INET6 (IPv6), AF_LOCAL (local socket),
 *	AF_ROUTE (routing sockets), AF_KEY (key management sockets), AF_SYSTEM
 *	(system sockets), AF_NDRV (RAW access to network devices).
@param type Defines the communication semantics.
 *	Can be SOCK_STREAM (TCP), SOCK_DGRAM (UDP), SOCK_RAW (raw socket)
 *	SOCK_STREAM: It is used mostly with TCP and its focus is on reliability and
 *	integrity.
 *	SOCK_DGRAM: It is used mostly with UDP and its focus is on speed and
 *	low latency. It is used for stuff like video streaming, VoIP, etc. where
 *	losing a few packets is acceptable.
?	Bonus: There are Flags that are meant to be given as an ORing with one of the
 *	above types. They are SOCK_NONBLOCK, SOCK_CLOEXEC, SOCK_SEQPACKET, SOCK_RDM.
 *	The one that is gonna come in handy is SOCK_NONBLOCK. It is used to make
 *	the socket non-blocking.
@param protocol It is an argument that is used in case the domain family has
 *	more than one protocol, but for most of the cases, setting it to 0 will
 *	suffice, as the protocol will be set to the default protocol for the
 *	given domain.
@return int that refers to the FD that the sockets connects to, or -1 when an
 *	error occurs, and sets the errno accordingly.
@note include <sys/socket.h>
 */
int				socket(int domain, int type, int protocol);
// ! SO_NOSIGPIPE

/*
@brief Changes the options associated with a socket. You need to define from
 *	which level you want to change the option, Link layer(Ethernet, WLAN), IP
 *	layer, Transport layer (TCP, UDP), Application layer (HTTP, FTP).
@param sockfd The file descriptor of the socket you want to change the option.
@param level The Level at which the option resides. SOL_SOCKET indicates the
 *	option resides at the socket layer.
@param optname the name of the option you want to change. The following options
 *	are available at the socket layer:
 *	SO_DEBUG: Enables recording of debugging information.
 *	SO_REUSEADDR: Allows other sockets to bind() to this port, so a server can
 *		restart without waiting for the old connection to time out.
 *	SO_REUSEPORT: Enables duplicate address and port bindings, so multiple
 *		sockets can use the same address and port combination. It can introduce
 *		security risks (such as port hijacking) and is disabled by default.
 *	SO_KEEPALIVE: Keeps connections active by enabling the periodic transmission
 *		of empty (keep-alive) messages. It will just affect the TCP connections.
 *	SO_DONTROUTE: Bypasses the standard routing mechanisms when sending data.
 *		When set, the data is sent directly to the destination host, without
 *		consulting the routing tables. (NetPractice)
 *	SO_LINGER: It uses a structure to define the behavior of close() in a socket
 *		that still has data to send. It can be set to 0 to close the socket
 *		immediately, or to a positive value to wait for that amount of time
 *		until the data is sent or the time is up. This option when used carries
 *		security risks, as it can be used to perform a DoS attack.
 *	SO_BROADCAST: Allows the socket to send broadcast messages. It applies only
 *		to datagram sockets (UDP).
 *	SO_OOBINLINE: Receives out-of-band data in the normal data stream. That is,
 *		the data is placed in the receive buffer along with the normal data,
 *		rather than being placed in a separate out-of-band data buffer that is
 *		read separately with recv() and MSG_OOB. It is not recommended to use
 *		this option, as it is unreliable normally, and if not it comes with
 *		security risks.
 *	SO_SNDBUF: Sets the size of the send buffer. The send buffer is where data
 *		is stored temporarily before it is sent to the destination. The size of
 *		the send buffer can affect the performance of the socket. If the buffer
 *		is too small, the socket will be unable to send data quickly enough to
 *		keep up with the application. If the buffer is too large, the socket
 *		will use more memory than necessary. The default size of the send buffer
 *		is determined by the operating system. The size will be doubled by the
 *		kernel because of the metadata overhead.
 *	SO_RCVBUF: Sets the size of the receive buffer. The same behavior as the
 *		send buffer, but for the receive buffer. If the buffer is larger than
 *		the set limit, new incoming packets may be dropped.
 *	SO_SNDLOWAT: Sets the minimum number of bytes in the send buffer before
 *		it can send data. If the buffer has fewer bytes than the set limit, the
 *		socket will block until the buffer has enough data to send. This option
 *		is useful for asynchronous I/O, by encouraging the socket to wait until
 *		there is enough data to send, rather than sending data in small chunks.
 *		In Linux the value is constant and is 1.
 *	SO_RCVLOWAT: Sets the minimum count for Receive buffer. Similar to the
 *		SO_SNDLOWAT, but for the receive buffer. Poll() or select() will return
 *		ready to read when the buffer has more than the set limit.
 *	SO_SNDTIMEO: Sets the timeout for sending data. Mostly irrelevant, as the
 *		IRC server most have a Non-Blocking I/O, so it will not block the
 *		application in the first place.
 *	SO_RCVTIMEO: Sets the timeout for receiving data. Mostly irrelevant, as the
 *		IRC server most have a Non-Blocking I/O, so it will not block the
 *		application in the first place.
 *	SO_TYPE: Returns the type of the socket. (get only)
 *	SO_ERROR: Returns and clears the error status of the socket. (get only)
 *	SO_SIGPIPE: Disables the SIGPIPE signal when the socket is closed, and
 *		returns EPIPE instead (Apple and FreeBSD). Better way to handle is
 *		SigIgn and read errno for errors.
 *	SO_NREAD: will give back the amount of bytes that are available to read,
 *		without actually reading them. (get only)
 *	SO_NWRITE: will give back the amount of bytes that are available to write,
 *		without actually writing them. (get only)
 *	SO_LINGER_SEC: Same as SO_LINGER, but the time is in seconds.
@param opt_val The buffer that contains the value of the option you want to
 *		set.
@param opt_len The size of the buffer that contains the value of the option.
@return int 0 on success, -1 on failure, and sets the errno accordingly.
 */
int				getsockopt(int sockfd, int level, int optname, void *opt_val,
					socklen_t *opt_len);
// ! This is forbidden function but it is similar to the setsockopt function
// !	that is allowed, so it will stay here for reference.

/*
	@brief Sets the options associated with a socket. You need to define from
 *		which level you want to change the option, Link layer(Ethernet, WLAN),
 *		IP layer, Transport layer (TCP, UDP), Application layer (HTTP, FTP).
?		It is the same as getsockopt, but it is used to set the options.
 */
int				setsockopt(int sockfd, int level, int optname,
					const void *opt_val, socklen_t opt_len);

/*
	@brief Returns the current address to which the socket is bound.
 *	
	@param sockfd The file descriptor of the socket you want to get the address.
	@param addr The structure that will contain the address of the socket.
	@param addrlen The space allocated for the address structure.
?		The address is truncated if the buffer is too small.
	@return int 0 on success, -1 on failure, and sets the errno accordingly.
 *	For the error codes, see man getsockname.
 */
int				getsockname(int sockfd, struct sockaddr *addr,
					socklen_t *addrlen);


/*
	@param p_name
 *		official protocol name
	@param p_aliases
 *		alias list
	@param p_proto
 *		protocol number
 */
struct protoent
{
	char	*p_name;
	char	**p_aliases;
	int		p_proto;
};

/*
	@brief
 *	Used to get the protocol entry number by the protocol name.
	@param name
 *		The name of the protocol. (e.g. "tcp", "udp", "icmp")
	@return struct protoent*
 *		Returns a pointer to the protocol entry, or NULL if the entry is not
 *		found or an error occurs.
 */
struct protoent	*getprotobyname(const char *name);

/*
	@param h_name
 *		official name of host
	@param h_aliases
 *		alias list
	@param h_addrtype
 *		host address type
	@param h_length
 *		length of address
	@param h_addr_list
 *		list of addresses from name server
 */
struct hostent
{
	char	*h_name;	/* official name of host */
	char	**h_aliases;	/* alias list */
	int		h_addrtype;	/* host address type */
	int		h_length;	/* length of address */
	char	**h_addr_list;	/* list of addresses from name server */
};

/*
	@brief
 *	Used to get the host entry by the host name.
	@param name
 *		The name of the host. (e.g. "localhost", "google.com", "192.168.1.1")
	@return struct hostent*
 *		Returns a pointer to the host entry, or NULL if the entry is not found
 *		or an error occurs.
	@warning
 *		This function is not thread-safe, as it uses a static buffer to store
 *		the host entry. Use getaddrinfo() instead.
 */
struct hostent	*gethostbyname(const char *name);

/*
	@brief 
 *	A better alternative to gethostbyname(). It is thread-safe and supports
 *	both IPv4 and IPv6. It is used to get the address information of the host,
 *	including the IP addresses
	@param hostname 
 *	The hostname or IP address of the host. (it can be NULL)
	@param service_name 
 *	port number or service name (e.g. "http", "ftp", "ssh") or NULL. The service
 *	names can be seen in services(5).
	@param hints 
 *	Is an optional pointer to a struct addrinfo that provides hints about the
 *	type of socket that the caller supports. (ai_flags, ai_family, ai_socktype
 *	and ai_protocol are the only fields that can be set the rest should be 0)
 *	It can be NULL.
 *	When hints is NULL, getaddrinfo() will assume ai_family = AF_UNSPEC, and the
 *	other fields are set to 0/NULL. (ai_flags when 0 is equivalent to AI_DEFAULT)
	@param res
 *	It is a pointer to a pointer to an addrinfo structure that will be filled
 *	with the address information. It should be freed using freeaddrinfo().
 *	It does not need to be initialized, as getaddrinfo() will allocate memory
 *	for it.
	@return int
 *	0 on success, or one of the error codes listed in gai_strerror(3).
	@warning
 *	Remember to free the memory allocated for the addrinfo structure using
 *	freeaddrinfo().
	@warning
 *	At least one of the hostname and service_name must be non-NULL.
 */
int				getaddrinfo(const char *hostname, const char *service_name,
					const struct addrinfo *hints, struct addrinfo **res);

// ?	Example of using getaddrinfo():
	// struct addrinfo hints, *res;
	// memset(&hints, 0, sizeof hints);
	// hints.ai_family = AF_UNSPEC;  // IPv4 or IPv6
	// hints.ai_socktype = SOCK_STREAM;
	// getaddrinfo("example.com", "http", &hints, &res);

/*
	@brief
 *	Used to free the memory allocated for the addrinfo structure by getaddrinfo().
	@param res
 *	A pointer to the addrinfo structure that was allocated by getaddrinfo().
 *	It will be set to NULL after the memory is freed.
 */
void			freeaddrinfo(struct addrinfo *res);

/*
	@brief
 *	Assigns a name to an unnamed socket. After creating a socket with socket(),
 *	it is necessary to bind(), because it creates the basic backbone of the
 *	endpoint connection, but it does not assign an address and port to the
 *	socket until bind() is called. (for server, the client does not need to
 *	bind(), it will be done automatically by connect())
	@param addr
 *	A pointer to a sockaddr that is a generic structure for socket addresses,
 *	which is cast to a specific structure depending on the domai. To create
 *	the sockaddr structure, you need to create a structure of the specific
 *	domain (e.g. sockaddr_in for IPv4, sockaddr_in6 for IPv6) and cast it to
 *	sockaddr. (There is some Pointer Trickery involved)
	@param addrlen
 *	The size of the address structure.
	@return int
 *	0 on success, -1 on failure, and sets the errno accordingly.
 */
int				bind(int sockfd, const struct sockaddr *addr,
					socklen_t addrlen);

/*
	@brief
 *	Used to connect a socket to a remote host. It is used by clients to connect
 *	to a server. It is used with TCP sockets, as it establishes a connection
 *	between the client and the server. It can also be used with UDP sockets, but
 *	it is not necessary, and it will behave differently.
	@param addr
 *	A pointer to a sockaddr that is a generic structure for socket addresses,
 *	which is cast to a specific structure depending on the domain. To create
 *	the sockaddr structure, you need to create a structure of the specific
 *	domain (e.g. sockaddr_in for IPv4, sockaddr_in6 for IPv6) and cast it to
 *	sockaddr. (There is some Pointer Trickery involved)
	@param addrlen
 *	The size of the address structure.
	@return int
 *	0 on success, -1 on failure, and sets the errno accordingly.
 */
int				connect(int sockfd, const struct sockaddr *addr,
					socklen_t addrlen);

/*
	@brief
 *	Used to listen for incoming connections on a socket. It is used by servers
 *	to listen for incoming connections from clients aswell as defining the
 *	maximum number of connections that can be queued.
	@param sockfd
 *	The file descriptor of the socket that will be used to listen for incoming
 *	connections.
	@param backlog
 *	The maximum number of connections that can be queued. If the number of
 *	incoming connections exceeds the backlog, the operating system will drop
 *	the connections and the client will receive an ECONNREFUSED error.
	@return int
 *	0 on success, -1 on failure, and sets the errno accordingly.
 */
int				listen(int sockfd, int backlog);

/*
	@brief
 *	Accepts a connection on a socket. It is used by servers to accept incoming
 *	connections from clients. It creates a new socket for the connection and
 *	returns a file descriptor that refers to that socket.
	@param sockfd
 *	The file descriptor of the socket that is used to listen for incoming
 *	connections.
	@param addr
 *	A pointer to a sockaddr that is a generic structure for socket addresses,
 *	which is cast to a specific structure depending on the domain. After the
 *	connection is accepted, the structure will be filled with the address of
 *	the client.
	@param addrlen
 *	The size of the address structure.
	@return int
 *	The file descriptor of the new socket that refers to the connection, or -1
 *	on failure, and sets the errno accordingly. It will return -1 and set the
 *	errno to EWOULDBLOCK if the socket is non-blocking and there are no
 *	connections to accept.
 */
int				accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

/*
	@brief
 *	Converts a 16-bit number from host byte order to network byte order.
 *	Normally, the host byte order is little-endian, and the network byte order
 *	is big-endian. little-endian is the order in which the least significant
 *	byte is stored at the lowest memory address, and big-endian is the order in
 *	which the most significant byte is stored at the lowest memory address.
	@param hostshort
 *	The 16-bit number in host byte order.
	@return uint16_t
 *	The 16-bit number in network byte order.
 */
uint16_t		htons(uint16_t hostshort);

/*
	@brief
 *	Converts a 32-bit number from host byte order to network byte order.
 *	Normally, the host byte order is little-endian, and the network byte order
 *	is big-endian. little-endian is the order in which the least significant
 *	byte is stored at the lowest memory address, and big-endian is the order in
 *	which the most significant byte is stored at the lowest memory address.
	@param hostlong
 *	The 32-bit number in host byte order.
	@return uint32_t
 *	The 32-bit number in network byte order.
 */
uint32_t		htonl(uint32_t hostlong);

/*
	@brief
 *	Converts a 16-bit number from network byte order to host byte order.
 *	Normally, the host byte order is little-endian, and the network byte order
 *	is big-endian. little-endian is the order in which the least significant
 *	byte is stored at the lowest memory address, and big-endian is the order in
 *	which the most significant byte is stored at the lowest memory address.
	@param netshort
 *	The 16-bit number in network byte order.
	@return uint16_t
 *	The 16-bit number in host byte order.
 */
uint16_t		ntohs(uint16_t netshort);

/*
	@brief
 *	Converts a 32-bit number from network byte order to host byte order.
 *	Normally, the host byte order is little-endian, and the network byte order
 *	is big-endian. little-endian is the order in which the least significant
 *	byte is stored at the lowest memory address, and big-endian is the order in
 *	which the most significant byte is stored at the lowest memory address.
	@param netlong
 *	The 32-bit number in network byte order.
	@return uint32_t
 *	The 32-bit number in host byte order.
 */
uint32_t		ntohl(uint32_t netlong);

/*
	@brief
 *	Converts an IPv4 address from a string to a 32-bit number in network byte
 *	order. The string must be in the standard IPv4 dotted-decimal notation
 *	(e.g. "192.0.0.1").
	@param cp
 *	The string that contains the IPv4 address in dotted-decimal notation.
	@return in_addr_t
 *	The 32-bit number in network byte order.
 */
in_addr_t		inet_addr(const char *cp);

/*
	@brief
 *	Converts an IPv4 address from a 32-bit number in network byte order to a
 *	string. The string will be in the standard IPv4 dotted-decimal notation
 *	(e.g. "192.0.0.1").
	@param in
 *	The 32-bit number in network byte order.
	@return char*
 *	The string that contains the IPv4 address in dotted-decimal notation.
	@warning
 *	This function is not thread-safe, as it uses a static buffer to store the
 *	string. It will be overwritten by subsequent calls to inet_ntoa().
	@warning
 *	DO NOT FREE THE RETURNED STRING.
 */
char			*inet_ntoa(struct in_addr in);

/*
	@brief
 *	It will be use to transmit a message to another socket in a connected state.
	@param sockfd
 *	The file descriptor of the socket that will be used to send the message.
	@param buf
 *	A pointer to the buffer that contains the message to be sent.
	@param len
 *	The size of the message in bytes.
	@param flags
 *	Flags that modify the behavior of the function. It can be 0 or a
 *	combination of the following flags:
 *		MSG_OOB: Sends out-of-band data.
 *		MSG_DONTROUTE: Bypasses the standard routing mechanisms when sending
 *	data. When set, the data is sent directly to the destination host, without
 *	consulting the routing tables.
	@return ssize_t
 *	The number of bytes sent on success, -1 on failure, and sets the errno
 *	accordingly.
 */
ssize_t			send(int sockfd, const void *buf, size_t len, int flags);

/*
	@brief
 *	It will be used to receive a message from another socket in a connected
 *	state. It is the same as recvfrom(), but with the address parameter set to
 *	NULL, because the socket is already connected.
	@param sockfd
 *	The file descriptor of the socket that will be used to receive the message.
	@param buf
 *	A pointer to the buffer that will store the received message.
	@param len
 *	The size of the buffer in bytes.
	@param flags
 *	Flags that modify the behavior of the function. It can be 0 or a
 *	combination of the following flags:
 *		MSG_OOB: Receives out-of-band data.
 *		MSG_PEEK: Peeks at the incoming message without removing it from the
 *	receive buffer.
 *		MSG_WAITALL: Blocks until the full amount of data is received. It will
 *	block until the buffer is full, or an error occurs, or a signal is caught.
	@return ssize_t 
 */
ssize_t			recv(int sockfd, void *buf, size_t len, int flags);

/*
	@brief 
 *	Configures the signal handler for a specific signal.
	@param signum
 *	The signal number. (e.g. SIGINT, SIGTERM, SIGKILL)
	@param handler
 *	A pointer to the signal handler function. It can be a function pointer or
 *	SIG_IGN (to ignore the signal) or SIG_DFL (to use the default handler).
	@return void*
 *	The previous signal handler function on success, SIG_ERR on failure, and
 *	sets the errno accordingly.
 */
void			(*signal(int signum, void (*handler)(int)))(int);

/*
	@brief
 *	Changes the action taken by a process on receipt of a specific signal.
	@param signum
 *	The signal number. (e.g. SIGINT, SIGTERM, SIGKILL)
	@param act
 *	A pointer to a struct sigaction that contains the new signal action, the
 *	signal mask, and the signal flags. The mask is used to block signals while
 *	the handler is running. The flags are used to modify the behavior of the
 *	signal. Some available flags are SA_RESETHAND (resets the handler to
 *	SIG_DFL after the signal is caught), SA_NODEFER (allows the signal to be
 *	delivered while the handler is running), SA_RESTART (restarts system calls
 *	that are interrupted by the signal), SA_SIGINFO (uses the sa_sigaction
 *	handler instead of the sa_handler handler).
	@param oldact
 *	A pointer to a struct sigaction that will be filled with the previous signal
 *	action, the previous signal mask, and the previous signal flags. It can be
 *	NULL if the previous action is not needed.
	@return int
 *	0 on success, -1 on failure, and sets the errno accordingly.
 */
int				sigaction(int signum, const struct sigaction *act,
					struct sigaction *oldact);

/*
	@brief 
 *	Is used to move the file pointer to a specific position in a file, or to
 *	obtain the current position of the file pointer, between other file
 *	manipulations, like finding the size of a file, re-writing in a file,
 *	finding holes in a file (The implementation of the file system may not), etc.
	@param fd
 *	The file descriptor of the file.
	@param offset
 *	The offset from the origin.
	@param whence
 *	The origin of the offset. It can be one of the following:
 *		SEEK_SET: The offset is set to the beginning of the file.
 *		SEEK_CUR: The offset is set to the current position of the file pointer.
 *		SEEK_END: The offset is set to the end of the file.
 *	Some systems may have additional options like SEEK_DATA and SEEK_HOLE
 *	(used to find holes in a file), but there implementation is not guaranteed.
	@return off_t
 *	The new position of the file pointer on success (relative to the beginning),
 *	-1 on failure, and sets the errno accordingly.
 */
off_t			lseek(int fd, off_t offset, int whence);

/*
	@brief
 *	Closes a file descriptor, so that it no longer refers to any file and may be
 *	reused. Any record locks held on the file it was associated with, and owned
 *	by the process, are removed (regardless of the file descriptor that was used
 *	to obtain the lock).
	@param fd
 *	The file descriptor of the file.
	@return int
 *	0 on success, -1 on failure, and sets the errno accordingly.
 */
int				close(int fd);

/*
	@brief
 *	Obtains information about the file pointed to by the file descriptor.
	@param fd
 *	The file descriptor of the file.
	@param buf
 *	A pointer to a struct stat that will be filled with the information about
 *	the file.
	@return int
 *	0 on success, -1 on failure, and sets the errno accordingly.
 */
int				fstat(int fd, struct stat *buf);

/*
	@brief
 *	Gives control and information over the file descriptor.
 *	A common use is to set the file descriptor to non-blocking mode with GETFL
 *	and SETFL.
	@param fd
 *	The file descriptor of the file.
	@param cmd
 *	The command that will be executed. It can be one of the following:
 *		F_DUPFD: Duplicate the file descriptor.
 *		F_GETFD: Get the file descriptor flags.
 *		F_SETFD: Set the file descriptor flags.
 *		F_GETFL: Get the file status flags and file access modes.
 *		F_SETFL: Set the file status flags and file access modes.
 *	Between other commands that are system-dependent, this are the most common.
	@param ...
 *	Depends on the command.
	@return int
 *	Depends on the command.
 *	For F_DUPFD, it returns the new file descriptor.
 *	For F_GETFD and F_GETFL, it returns the flags.
 *	For F_GETOWN, it returns the value of descriptor owner.
 *	Everything else returns 0 on success, -1 on failure, and sets the errno
 *	accordingly.
 */
int				fcntl(int fd, int cmd, ...);

struct pollfd
{
	int		fd;			/* file descriptor */
	short	events;		/* requested events */
	short	revents;	/* returned events */
};

/*
	@brief
 *	Checks the status of multiple file descriptors if they are ready for I/O or
 *	if they have an exception. It is used to monitor multiple file descriptors,
 *	waiting until one or more of the file descriptors become "ready" for some
 *	class of I/O operation (e.g. reading, writing, or error condition).
	@param fds
 *	An array of struct pollfd that contains the file descriptors to be
 *	monitored, the events to monitor for, and the events that may have occurred.
 *	The events can be:
 *		POLLIN: There is data to read.
 *		POLLOUT: Writing is possible.
 *		POLLERR: An error has occurred.
 *		POLLHUP: The other end has hung up, this is flag is output only.
 *		POLLNVAL: The file descriptor is not open.
 *	Between other events that are system-dependent, this are the most common.
	@param nfds
 *	The number of file descriptors in the array.
	@param timeout
 *	The amount of time to wait for an event. It can be:
 *		-1: Wait indefinitely.
 *		0: Return immediately.
 *		>0: Wait for the specified amount of time in milliseconds for an event.
	@return int
 *	The number of file descriptors that are ready for I/O, 0 if the timeout
 *	occurred, -1 on failure, and sets the errno accordingly.
 */
int				poll(struct pollfd *fds, nfds_t nfds, int timeout);
#endif