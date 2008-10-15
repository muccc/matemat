#ifndef __MATEMAT_NET_H_
#define __MATEMAT_NET_H_

#define MATEMAT_PORT 2323
#define MATEMAT_TEMP_PORT 2323

#define MATEMAT_TEMP_IPADDR(a) uip_ipaddr(a, 255, 255,255,255)
//#define MATEMAT_TEMP_IPADDR(a) uip_ipaddr(a, 224, 0,23,2)
//#define MATEMAT_TEMP_IPADDR(a) uip_ipaddr(a, 10, 0,0,49)

void matemat_net_init(void);
void matemat_net_main(void);

#endif
