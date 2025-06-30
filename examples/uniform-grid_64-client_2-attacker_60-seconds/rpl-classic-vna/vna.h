#include <stdint.h>
#include <inttypes.h>
extern uint8_t vna_mode;
extern char features[UIP_CONF_BUFFER_SIZE - 56 + 1];
extern int hex_to_bin(char* c);
struct data {
  long int flags;
  char dio_version[DIO_VERSION_LEN + 1];
  char dio_rank[DIO_RANK_LEN + 1];
  char frame_len[FRAME_LEN_LEN + 1];
  char sixlowpan_src[SIXLOWPAN_SRC_LEN + 1];
  char sixlowpan_dst[SIXLOWPAN_DST_LEN + 1];
  char dio_dtsn[DIO_DTSN_LEN + 1];
  char dao_sequence[DAO_SEQUENCE_LEN + 1];
  char ipv6_hlim[IPV6_HLIM_LEN + 1];
  char wpan_seq_no[WPAN_SEQ_NO_LEN + 1];
  char ipv6_plen[IPV6_PLEN_LEN + 1];
  char icmpv6_type[ICMPV6_TYPE_LEN + 1];
  char icmpv6_code[ICMPV6_CODE_LEN + 1];
  char wpan_ack_request[WPAN_ACK_REQUEST_LEN + 1];
};
extern struct data features_data;
