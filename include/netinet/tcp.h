#ifndef _NETINET_TCP_H
#define _NETINET_TCP_H

#include <inttypes.h>
#include <endian.h>
#include <sys/socket.h> /* for sockaddr_storage */

__BEGIN_DECLS

struct tcphdr {		/* size 20/0x14      40/0x28 with IP header */
  uint16_t source;	/* offset 0          20/0x14 */
  uint16_t dest;	/* offset 2          22/0x16 */
  uint32_t seq;		/* offset 4          24/0x18 */
  uint32_t ack_seq;	/* offset 8          28/0x1c */
#if __BYTE_ORDER == __LITTLE_ENDIAN
  uint16_t res1:4, doff:4, fin:1, syn:1, rst:1, psh:1, ack:1, urg:1, ece:1, cwr:1;
#else
  uint16_t doff:4, res1:4, cwr:1, ece:1, urg:1, ack:1, psh:1, rst:1, syn:1, fin:1;
#endif
			/* offset 12/0xc     32/0x20 */
  uint16_t window;	/* offset 14/0xe     34/0x22 */
  uint16_t check;	/* offset 16/0x10    36/0x24 */
  uint16_t urg_ptr;	/* offset 18/0x12    38/0x26 */
};


enum {
  TCP_ESTABLISHED = 1,
  TCP_SYN_SENT,
  TCP_SYN_RECV,
  TCP_FIN_WAIT1,
  TCP_FIN_WAIT2,
  TCP_TIME_WAIT,
  TCP_CLOSE,
  TCP_CLOSE_WAIT,
  TCP_LAST_ACK,
  TCP_LISTEN,
  TCP_CLOSING,	 /* now a valid state */

  TCP_MAX_STATES /* Leave at the end! */
};

#define TCP_STATE_MASK 0xF
#define TCP_ACTION_FIN (1 << 7)

enum {
  TCPF_ESTABLISHED = (1 << 1),
  TCPF_SYN_SENT  = (1 << 2),
  TCPF_SYN_RECV  = (1 << 3),
  TCPF_FIN_WAIT1 = (1 << 4),
  TCPF_FIN_WAIT2 = (1 << 5),
  TCPF_TIME_WAIT = (1 << 6),
  TCPF_CLOSE     = (1 << 7),
  TCPF_CLOSE_WAIT = (1 << 8),
  TCPF_LAST_ACK  = (1 << 9),
  TCPF_LISTEN    = (1 << 10),
  TCPF_CLOSING   = (1 << 11)
};

/*
 *	The union cast uses a gcc extension to avoid aliasing problems
 *  (union is compatible to any of its members)
 *  This means this part of the code is -fstrict-aliasing safe now.
 */
union tcp_word_hdr {
  struct tcphdr hdr;
  uint32_t words[5];
};

#define tcp_flag_word(tp) ( ((union tcp_word_hdr *)(tp))->words [3]) 

enum {
#if __BYTE_ORDER == __LITTLE_ENDIAN
  TCP_FLAG_CWR = 0x00008000,
  TCP_FLAG_ECE = 0x00004000,
  TCP_FLAG_URG = 0x00002000,
  TCP_FLAG_ACK = 0x00001000,
  TCP_FLAG_PSH = 0x00000800,
  TCP_FLAG_RST = 0x00000400,
  TCP_FLAG_SYN = 0x00000200,
  TCP_FLAG_FIN = 0x00000100,
  TCP_RESERVED_BITS = 0x0000C00F,
  TCP_DATA_OFFSET = 0x000000F0
#else
  TCP_FLAG_CWR = 0x00800000,
  TCP_FLAG_ECE = 0x00400000,
  TCP_FLAG_URG = 0x00200000,
  TCP_FLAG_ACK = 0x00100000,
  TCP_FLAG_PSH = 0x00080000,
  TCP_FLAG_RST = 0x00040000,
  TCP_FLAG_SYN = 0x00020000,
  TCP_FLAG_FIN = 0x00010000,
  TCP_RESERVED_BITS = 0x0FC00000,
  TCP_DATA_OFFSET = 0xF0000000
#endif
};

/*
 * TCP general constants
 */
#define TCP_MSS_DEFAULT		 536U	/* IPv4 (RFC1122, RFC2581) */
#define TCP_MSS_DESIRED		1220U	/* IPv6 (tunneled), EDNS0 (RFC3226) */

/* TCP socket options */
#define TCP_NODELAY		1	/* Turn off Nagle's algorithm. */
#define TCP_MAXSEG		2	/* Limit MSS */
#define TCP_CORK		3	/* Never send partially complete segments */
#define TCP_KEEPIDLE		4	/* Start keeplives after this period */
#define TCP_KEEPINTVL		5	/* Interval between keepalives */
#define TCP_KEEPCNT		6	/* Number of keepalives before death */
#define TCP_SYNCNT		7	/* Number of SYN retransmits */
#define TCP_LINGER2		8	/* Life time of orphaned FIN-WAIT-2 state */
#define TCP_DEFER_ACCEPT	9	/* Wake up listener only when data arrive */
#define TCP_WINDOW_CLAMP	10	/* Bound advertised window */
#define TCP_INFO		11	/* Information about this connection. */
#define TCP_QUICKACK		12	/* Block/reenable quick acks */
#define TCP_CONGESTION		13	/* Congestion control algorithm */
#define TCP_MD5SIG		14	/* TCP MD5 Signature (RFC2385) */
#define TCP_THIN_LINEAR_TIMEOUTS 16      /* Use linear timeouts for thin streams*/
#define TCP_THIN_DUPACK         17      /* Fast retrans. after 1 dupack */
#define TCP_USER_TIMEOUT	18	/* How long for loss retry before timeout */
#define TCP_REPAIR		19	/* TCP sock is under repair right now */
#define TCP_REPAIR_QUEUE	20
#define TCP_QUEUE_SEQ		21
#define TCP_REPAIR_OPTIONS	22
#define TCP_FASTOPEN		23	/* Enable FastOpen on listeners */
#define TCP_TIMESTAMP		24
#define TCP_NOTSENT_LOWAT	25	/* limit number of unsent bytes in write queue */

#define TCPI_OPT_TIMESTAMPS	1
#define TCPI_OPT_SACK		2
#define TCPI_OPT_WSCALE		4
#define TCPI_OPT_ECN		8
#define TCPI_OPT_ECN_SEEN	16 /* we received at least one packet with ECT */
#define TCPI_OPT_SYN_DATA	32 /* SYN-ACK acked data in SYN sent or rcvd */

enum tcp_ca_state {
  TCP_CA_Open = 0,
#define TCPF_CA_Open	(1<<TCP_CA_Open)
  TCP_CA_Disorder = 1,
#define TCPF_CA_Disorder (1<<TCP_CA_Disorder)
  TCP_CA_CWR = 2,
#define TCPF_CA_CWR	(1<<TCP_CA_CWR)
  TCP_CA_Recovery = 3,
#define TCPF_CA_Recovery (1<<TCP_CA_Recovery)
  TCP_CA_Loss = 4
#define TCPF_CA_Loss	(1<<TCP_CA_Loss)
};

struct tcp_info {
  uint8_t tcpi_state;
  uint8_t tcpi_ca_state;
  uint8_t tcpi_retransmits;
  uint8_t tcpi_probes;
  uint8_t tcpi_backoff;
  uint8_t tcpi_options;
  uint8_t tcpi_snd_wscale : 4, tcpi_rcv_wscale : 4;

  uint32_t tcpi_rto;
  uint32_t tcpi_ato;
  uint32_t tcpi_snd_mss;
  uint32_t tcpi_rcv_mss;

  uint32_t tcpi_unacked;
  uint32_t tcpi_sacked;
  uint32_t tcpi_lost;
  uint32_t tcpi_retrans;
  uint32_t tcpi_fackets;

  /* Times. */
  uint32_t tcpi_last_data_sent;
  uint32_t tcpi_last_ack_sent;     /* Not remembered, sorry. */
  uint32_t tcpi_last_data_recv;
  uint32_t tcpi_last_ack_recv;

  /* Metrics. */
  uint32_t tcpi_pmtu;
  uint32_t tcpi_rcv_ssthresh;
  uint32_t tcpi_rtt;
  uint32_t tcpi_rttvar;
  uint32_t tcpi_snd_ssthresh;
  uint32_t tcpi_snd_cwnd;
  uint32_t tcpi_advmss;
  uint32_t tcpi_reordering;
  uint32_t tcpi_rcv_rtt;
  uint32_t tcpi_rcv_space;

  uint32_t tcpi_total_retrans;

  uint64_t tcpi_pacing_rate;
  uint64_t tcpi_max_pacing_rate;
};

/* for TCP_MD5SIG socket option */
#define TCP_MD5SIG_MAXKEYLEN	80

struct tcp_md5sig {
  struct sockaddr_storage tcpm_addr;	/* address associated */
  uint16_t __tcpm_pad1;				/* zero */
  uint16_t tcpm_keylen;				/* key length */
  uint32_t __tcpm_pad2;				/* zero */
  uint8_t tcpm_key[TCP_MD5SIG_MAXKEYLEN];	/* key (binary) */
};

__END_DECLS

#endif
