/*
 * pcaputil.c
 *
 * Copyright (c) 2000 Dug Song <dugsong@monkey.org>
 *
 * $Id: pcaputil.c,v 1.2 2001/03/15 08:33:04 dugsong Exp $
 */

#include "config.h"

#include <sys/param.h>
#include <sys/types.h>
#include <sys/ioctl.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include <pcap.h>
#ifdef BSD
#include <pcap-int.h>
#endif

#include "pcaputil.h"

#ifdef BSD
static int
bpf_immediate(int fd, int on)
{
	return (ioctl(fd, BIOCIMMEDIATE, &on));
}
#endif

int
pcap_dloff(pcap_t *pd)
{
	int offset = -1;
	
	switch (pcap_datalink(pd)) {
	case DLT_EN10MB:
		offset = 14;
		break;
	case DLT_IEEE802:
		offset = 22;
		break;
	case DLT_FDDI:
		offset = 21;
		break;
#ifdef DLT_LOOP
	case DLT_LOOP:
#endif
	case DLT_NULL:
		offset = 4;
		break;
	case DLT_LINUX_SLL:   /* e.g. ppp */
		offset = 16;
		break;
	default:
		warnx("unsupported datalink type");
		break;
	}
	return (offset);
}

pcap_t *
pcap_init(char *intf, char *filter, int snaplen)
{
	pcap_t *pd;
	u_int net, mask;
	struct bpf_program fcode;
	char ebuf[PCAP_ERRBUF_SIZE];
	pcap_if_t *interfaces;

        if (intf == NULL) {
		if (pcap_findalldevs(&interfaces, ebuf)) {
			warnx("%s", ebuf);
			return (NULL); }
		else intf = interfaces->name;
	}

	if ((pd = pcap_create(intf, ebuf)) == NULL) {
		warnx("%s", ebuf);
		return (NULL);
	}
	if (pcap_lookupnet(intf, &net, &mask, ebuf) == -1) {
		warnx("%s", ebuf);
		return (NULL);
	} 
        if (pcap_set_immediate_mode(pd, 1))
		warnx("immediate mode not set");

	if (pcap_activate(pd) < 0) {
		perror("did not activate");
		pcap_close(pd);
		return (NULL);
	}
	if (pcap_compile(pd, &fcode, filter, 1, mask) < 0) {
		pcap_perror(pd, "pcap_compile");
		return (NULL);
	}
	if (pcap_setfilter(pd, &fcode) == -1) {
		pcap_perror(pd, "pcap_setfilter");
		return (NULL);
	}
#ifdef BSD
	if (bpf_immediate(pd->fd, 1) < 0) {
		perror("ioctl");
		return (NULL);
	}
#endif
	return (pd);
}

/* from tcpdump util.c. */
char *
copy_argv(char **argv)
{
	char **p, *buf, *src, *dst;
	u_int len = 0;
	
	p = argv;
	if (*p == 0)
		return (0);
	
	while (*p)
		len += strlen(*p++) + 1;
	
	if ((buf = (char *)malloc(len)) == NULL)
		err(1, "copy_argv: malloc");
	
	p = argv;
	dst = buf;
	
	while ((src = *p++) != NULL) {
		while ((*dst++ = *src++) != '\0')
			;
		dst[-1] = ' ';
	}
	dst[-1] = '\0';
	
	return (buf);
}
