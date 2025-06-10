#define WTAP_ENCAP_BACNET_MS_TP			63
#define WTAP_ENCAP_RAW_ICMP			64
#define WTAP_ENCAP_RAW_ICMPV6			65
#define WTAP_ENCAP_MPEG2			66

/* last WTAP_ENCAP_ value + 1 */
#define WTAP_NUM_ENCAP_TYPES			66
#define WTAP_NUM_ENCAP_TYPES			67

/* File types that can be read by wiretap.
We support writing some many of these file types, too, so we
@ -202,9 +203,10 @@
#define WTAP_FILE_NETWORK_INSTRUMENTS_V9	37
#define WTAP_FILE_AIROPEEK_V9			38
#define WTAP_FILE_EYESDN			39
#define WTAP_FILE_MPEG2				40

/* last WTAP_FILE_ value + 1 */
#define WTAP_NUM_FILE_TYPES			40
#define WTAP_NUM_FILE_TYPES			41

/*
* Maximum packet size we'll support. 
*/