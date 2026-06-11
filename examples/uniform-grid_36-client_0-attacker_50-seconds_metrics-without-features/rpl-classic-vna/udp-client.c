#include "contiki.h"
#include "net/routing/routing.h"
#include "random.h"
#include "net/netstack.h"
#include "net/ipv6/simple-udp.h"
#include "vna.h"
#include <stdint.h>
#include <inttypes.h>

#include "sys/log.h"
#include "sys/energest.h"
#include "sys/node-id.h"

#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_INFO

#define UDP_CLIENT_PORT	8765
#define UDP_SERVER_PORT	5678

#define SEND_INTERVAL		  (FEATURES_SEND_INTERVAL * CLOCK_SECOND)

static struct simple_udp_connection udp_conn;
uint8_t vna_mode = 0;
char features[67] = "00000,  ,   ,     ,   ,        ,        ,   ,   ,   ,   ,   ,   , ";
struct data features_data;
static uint64_t last_tx, last_rx, last_time, last_cpu, last_lpm;
static uint32_t seqno = 0;
/*---------------------------------------------------------------------------*/
int hex_to_bin(char* c) {
  int val = 0;
  while (*c) {
    uint8_t byte = *c++; 
    if (byte >= '0' && byte <= '9') {
      byte = byte - '0';
    } else if (byte >= 'A' && byte <='F') {
      byte = byte - 'A' + 10;   
    }
    val = (val << 4) | (byte & 0xF);
  }
  return val;
}
/*---------------------------------------------------------------------------*/
PROCESS(udp_client_process, "UDP client");
AUTOSTART_PROCESSES(&udp_client_process);
/*---------------------------------------------------------------------------*/
static void
udp_rx_callback(struct simple_udp_connection *c,
         const uip_ipaddr_t *sender_addr,
         uint16_t sender_port,
         const uip_ipaddr_t *receiver_addr,
         uint16_t receiver_port,
         const uint8_t *data,
         uint16_t datalen)
{

  LOG_INFO("Received response '%.*s' from ", datalen, (char *) data);
  LOG_INFO_6ADDR(sender_addr);
#if LLSEC802154_CONF_ENABLED
  LOG_INFO_(" LLSEC LV:%d", uipbuf_get_attr(UIPBUF_ATTR_LLSEC_LEVEL));
#endif
  LOG_INFO_("\n");
}
/*---------------------------------------------------------------------------*/
static void data_struct_init(struct data features_data) {
  features_data.flags = 0x00000;
  snprintf(features_data.dio_version,      sizeof(features_data.dio_version),      "   "              );
  snprintf(features_data.dio_rank,         sizeof(features_data.dio_rank),         "     "            );
  snprintf(features_data.frame_len,        sizeof(features_data.frame_len),        "   "              );
  snprintf(features_data.sixlowpan_src,    sizeof(features_data.sixlowpan_src),    "                 ");
  snprintf(features_data.sixlowpan_dst,    sizeof(features_data.sixlowpan_dst),    "                 ");
  snprintf(features_data.dio_dtsn,         sizeof(features_data.dio_dtsn),         "   "              );
  snprintf(features_data.dao_sequence,     sizeof(features_data.dao_sequence),     "   "              );
  snprintf(features_data.ipv6_hlim,        sizeof(features_data.ipv6_hlim),        "   "              );
  snprintf(features_data.wpan_seq_no,      sizeof(features_data.wpan_seq_no),      "   "              );
  snprintf(features_data.ipv6_plen,        sizeof(features_data.ipv6_plen),        "   "              );
  snprintf(features_data.icmpv6_type,      sizeof(features_data.icmpv6_type),      "   "              );
  snprintf(features_data.icmpv6_code,      sizeof(features_data.icmpv6_code),      "   "              );
  snprintf(features_data.wpan_ack_request, sizeof(features_data.wpan_ack_request), " "                );
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(udp_client_process, ev, data)
{
  static struct etimer periodic_timer;
  static struct stimer metrics_timer;
  static char str[64];
  uip_ipaddr_t dest_ipaddr;
  uip_ipaddr_t wsn_server_ipaddr;
  uip_ipaddr_t data_server_ipaddr;
  static uint32_t temperature;
  
  data_struct_init(features_data);

  PROCESS_BEGIN();
  
  energest_flush();
  last_time = ENERGEST_GET_TOTAL_TIME();
  last_cpu = energest_type_time(ENERGEST_TYPE_CPU);
  last_lpm = energest_type_time(ENERGEST_TYPE_LPM);
  last_tx = energest_type_time(ENERGEST_TYPE_TRANSMIT);
  last_rx = energest_type_time(ENERGEST_TYPE_LISTEN);
  
  etimer_set(&periodic_timer, node_id * CLOCK_SECOND);
  PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));
  
  /* Initialize UDP connection */
  simple_udp_register(&udp_conn, UDP_CLIENT_PORT, NULL,
                      UDP_SERVER_PORT, udp_rx_callback);

  etimer_set(&periodic_timer, SEND_INTERVAL);
  stimer_set(&metrics_timer, 30);
  
  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));

    if(NETSTACK_ROUTING.node_has_joined() && NETSTACK_ROUTING.node_is_reachable() &&
        NETSTACK_ROUTING.get_root_ipaddr(&dest_ipaddr)) {
      
      uip_ip6addr(&wsn_server_ipaddr, WSN_SERVER_IP, 0, 0, 0, 0, 0, 0, 1);
      //snprintf(features, sizeof(features), "%05lX,%.1d,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s", features_data.flags, node_id, features_data.dio_version, features_data.dio_rank, features_data.frame_len, features_data.sixlowpan_src, features_data.sixlowpan_dst, features_data.dio_dtsn, features_data.dao_sequence, features_data.ipv6_hlim, features_data.wpan_seq_no, features_data.ipv6_plen, features_data.icmpv6_code, features_data.wpan_ack_request);
      //simple_udp_sendto(&udp_conn, features, strlen(features), &wsn_server_ipaddr);
      //data_struct_init(features_data);
      //LOG_INFO("Sending features to ");
      //LOG_INFO_6ADDR(&wsn_server_ipaddr);
      //LOG_INFO_("\n");
      
      if(stimer_expired(&metrics_timer)) {
        uint64_t curr_tx, curr_rx, curr_time, curr_cpu, curr_lpm;
        uint64_t delta_time;
        
        energest_flush();
        
        curr_time = ENERGEST_GET_TOTAL_TIME();
        curr_cpu = energest_type_time(ENERGEST_TYPE_CPU);
        curr_lpm = energest_type_time(ENERGEST_TYPE_LPM);
        curr_tx = energest_type_time(ENERGEST_TYPE_TRANSMIT);
        curr_rx = energest_type_time(ENERGEST_TYPE_LISTEN);
        
        delta_time = MAX(curr_time - last_time, 1);
        
        temperature = 21 + (random_rand() % (29 - 21 + 1));
        uip_ip6addr(&data_server_ipaddr, 0xfdff, 0, 0, 0, 0, 0, 0, 1);
        LOG_INFO("Sending data to ");
        LOG_INFO_6ADDR(&data_server_ipaddr);
        LOG_INFO_("\n");
        snprintf(str, sizeof(str), "%06"PRIu32",%06d,%02"PRIu16",%03"PRIu32",%07"PRIu64",%07"PRIu64",%07"PRIu64",%07"PRIu64",%07"PRIu64",%02"PRIu32, clock_time(), 0, node_id, seqno++, curr_cpu-last_cpu, curr_lpm-last_lpm, curr_tx-last_tx, curr_rx-last_rx, delta_time, temperature);
        simple_udp_sendto(&udp_conn, str, strlen(str), &data_server_ipaddr);
        
        last_time = curr_time;
        last_cpu = curr_cpu;
        last_lpm = curr_lpm;
        last_tx = curr_tx;
        last_rx = curr_rx;
        stimer_reset(&metrics_timer);
      }
      
    } else {
      LOG_INFO("Not reachable yet\n");
    }

    /* Add some jitter */
    etimer_set(&periodic_timer, SEND_INTERVAL);
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
