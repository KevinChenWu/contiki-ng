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

#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_INFO

#define WITH_SERVER_REPLY  1
#define UDP_CLIENT_PORT	8765
#define UDP_SERVER_PORT	5678

#define SEND_INTERVAL		  (10 * CLOCK_SECOND)

static struct simple_udp_connection udp_conn;
static uint32_t rx_count = 0;
uint8_t vna_mode = 0;

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
  rx_count++;
}
/*---------------------------------------------------------------------------*/
#if ENERGEST_TYPE == 1
static inline unsigned long
to_seconds(uint64_t time)
{
  return (unsigned long)(time / ENERGEST_SECOND);
}
#elif ENERGEST_TYPE == 2
static uint64_t last_tx, last_rx, last_time, last_cpu, last_lpm, last_deep_lpm;
#endif
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(udp_client_process, ev, data)
{
  static struct etimer periodic_timer;
  static char str[64];
  uip_ipaddr_t dest_ipaddr;
  uip_ipaddr_t server_ipaddr;
  static uint32_t tx_count;
  static uint32_t missed_tx_count;
  static uint32_t temperature;

  PROCESS_BEGIN();
  
  #if ENERGEST_TYPE == 2
    uint64_t curr_tx, curr_rx, curr_time, curr_cpu, curr_lpm, curr_deep_lpm;
    uint64_t delta_time;
    energest_flush();
    last_time = ENERGEST_GET_TOTAL_TIME();
    last_cpu = energest_type_time(ENERGEST_TYPE_CPU);
    last_lpm = energest_type_time(ENERGEST_TYPE_LPM);
    last_deep_lpm = energest_type_time(ENERGEST_TYPE_DEEP_LPM);
    last_tx = energest_type_time(ENERGEST_TYPE_TRANSMIT);
    last_rx = energest_type_time(ENERGEST_TYPE_LISTEN);
  #endif

  /* Initialize UDP connection */
  simple_udp_register(&udp_conn, UDP_CLIENT_PORT, NULL,
                      UDP_SERVER_PORT, udp_rx_callback);

  etimer_set(&periodic_timer, random_rand() % SEND_INTERVAL);
  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));

    if(NETSTACK_ROUTING.node_is_reachable() &&
        NETSTACK_ROUTING.get_root_ipaddr(&dest_ipaddr)) {

      /* Print statistics every 10th TX */
      if(tx_count % 10 == 0) {
        LOG_INFO("Tx/Rx/MissedTx: %" PRIu32 "/%" PRIu32 "/%" PRIu32 "\n",
                 tx_count, rx_count, missed_tx_count);
      }

      /* Send to DAG root */
      //uip_ip6addr(&server_ipaddr, 0xfd00, 0, 0, 0, 0, 0, 0, 1);
      //LOG_INFO("Sending request %"PRIu32" to ", tx_count);
      //LOG_INFO_6ADDR(&server_ipaddr);
      //LOG_INFO_("\n");
      //snprintf(str, sizeof(str), "hello %" PRIu32 "", tx_count);
      //simple_udp_sendto(&udp_conn, str, strlen(str), &server_ipaddr);
      //tx_count++;
      
      temperature = 21 + (random_rand() % (29 - 21 + 1));
      uip_ip6addr(&server_ipaddr, 0xfd80, 0, 0, 0, 0, 0, 0, 1);
      LOG_INFO("Sending data (temperature: %"PRIu32" °C) to ", temperature);
      LOG_INFO_6ADDR(&server_ipaddr);
      LOG_INFO_("\n");
      snprintf(str, sizeof(str), "temperature: %" PRIu32 " °C", temperature);
      simple_udp_sendto(&udp_conn, str, strlen(str), &server_ipaddr);
      
      uip_ip6addr(&server_ipaddr, 0xfd00, 0, 0, 0, 0, 0, 0, 1);
      LOG_INFO("Sending metric %"PRIu32" to ", tx_count);
      LOG_INFO_6ADDR(&server_ipaddr);
      LOG_INFO_("\n");
      tx_count++;
      
      #if ENERGEST_TYPE == 1
        // adapted from examples/libs/energest/example-energest.c
        energest_flush();
        // value order: CPU,LPM,DEEP_LPM,Total_time,Radio_LISTEN,Radio_TRANSMIT,Radio_OFF
        snprintf(str, sizeof(str), "%lus,%lus,%lus,%lus,%lus,%lus,%lus", to_seconds(energest_type_time(ENERGEST_TYPE_CPU)), to_seconds(energest_type_time(ENERGEST_TYPE_LPM)), to_seconds(energest_type_time(ENERGEST_TYPE_DEEP_LPM)), to_seconds(ENERGEST_GET_TOTAL_TIME()), to_seconds(energest_type_time(ENERGEST_TYPE_LISTEN)), to_seconds(energest_type_time(ENERGEST_TYPE_TRANSMIT)), to_seconds(ENERGEST_GET_TOTAL_TIME() - energest_type_time(ENERGEST_TYPE_TRANSMIT) - energest_type_time(ENERGEST_TYPE_LISTEN)));
        simple_udp_sendto(&udp_conn, str, strlen(str), &server_ipaddr);
      #elif ENERGEST_TYPE == 2
        // adapted from os/services/simple-energest/simple-energest.c
        energest_flush();
        curr_time = ENERGEST_GET_TOTAL_TIME();
        curr_cpu = energest_type_time(ENERGEST_TYPE_CPU);
        curr_lpm = energest_type_time(ENERGEST_TYPE_LPM);
        curr_deep_lpm = energest_type_time(ENERGEST_TYPE_DEEP_LPM);
        curr_tx = energest_type_time(ENERGEST_TYPE_TRANSMIT);
        curr_rx = energest_type_time(ENERGEST_TYPE_LISTEN);
        delta_time = MAX(curr_time - last_time, 1);
        
        // value order: Total_time,CPU,LPM,Deep_LPM,Radio_Tx,Radio_Rx,Radio_total
        snprintf(str, sizeof(str), "%"PRIu64"s,%"PRIu64"s,%"PRIu64"s,%"PRIu64"s,%"PRIu64"s,%"PRIu64"s,%"PRIu64"s", delta_time, curr_cpu - last_cpu, curr_lpm - last_lpm, curr_deep_lpm - last_deep_lpm, curr_tx - last_tx, curr_rx - last_rx, curr_tx - last_tx + curr_rx - last_rx);
        simple_udp_sendto(&udp_conn, str, strlen(str), &server_ipaddr);
        
        last_time = curr_time;
        last_cpu = curr_cpu;
        last_lpm = curr_lpm;
        last_deep_lpm = curr_deep_lpm;
        last_tx = curr_tx;
        last_rx = curr_rx;
      #endif
      
    } else {
      LOG_INFO("Not reachable yet\n");
      if(tx_count > 0) {
        missed_tx_count++;
      }
    }

    /* Add some jitter */
    etimer_set(&periodic_timer, SEND_INTERVAL
      - CLOCK_SECOND + (random_rand() % (2 * CLOCK_SECOND)));
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
