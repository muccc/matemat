#include "matemat_net.h"
#include "../uip/uip.h"
#include "../config.h"
#include "../matemat/matemat.h"

#ifdef MATEMAT_SUPPORT
#define BUF ((struct uip_udpip_hdr *) (uip_appdata - UIP_IPUDPH_LEN))

void
matemat_net_init (void)
{
    uip_ipaddr_t ip;
    //uip_ipaddr_copy (&ip, all_ones_addr);
    MATEMAT_TEMP_IPADDR(ip);

    uip_udp_conn_t *matemat_temp_conn = uip_udp_new (&ip, HTONS(MATEMAT_TEMP_PORT), matemat_net_main);

    if (!matemat_temp_conn) 
	return; /* dammit. */

    //uip_udp_bind (udp_echo_conn, HTONS (MATEMAT__PORT));
}


void
matemat_net_main(void)
{
    if (!uip_newdata ()){
        if(uip_poll() && matemat_global.push){
            matemat_global.matemat_packetcount++;
            sprintf(uip_appdata,"temp0=");
            sprintf_temp(uip_appdata+strlen(uip_appdata),matemat_global.temps[0],0);
//            sprintf(uip_appdata+strlen(uip_appdata),"\r\ntemp1=");
            sprintf(uip_appdata+strlen(uip_appdata)," temp1=");
            sprintf_temp(uip_appdata+strlen(uip_appdata),matemat_global.temps[1],0);
            uip_udp_send(strlen(uip_appdata));
            //uip_slen = 5;
            uip_process(UIP_UDP_SEND_CONN); 
            fill_llh_and_transmit();
            uip_slen = 0;
            matemat_global.push = 0;
        }
	return;
    }
    /*uip_udp_conn_t echo_conn;
    uip_ipaddr_copy(echo_conn.ripaddr, BUF->srcipaddr);
    echo_conn.rport = BUF->srcport;
    echo_conn.lport = HTONS(MATEMAT_PORT);

    uip_udp_conn = &echo_conn;

    uip_slen = uip_len;
    uip_process(UIP_UDP_SEND_CONN); 
    fill_llh_and_transmit();

    uip_slen = 0;*/
}

#endif
