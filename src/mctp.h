#ifndef SHMB_MCTP_H
#define SHMB_MCTP_H

#include "compiler.h"

#include <stdbool.h>
#include <sys/types.h>

struct mctp_pktbuf {
	size_t start, end;
	char buf[];
};
/* FIXME */
#define MCTP_PKTBUF_MAX	4096

struct mctp_pktbuf *mctp_pktbuf_alloc(__unused void *ctx, size_t payload);
void mctp_pktbuf_free(struct mctp_pktbuf *pkt);
size_t mctp_pktbuf_size(struct mctp_pktbuf *pkt);
void *mctp_pktbuf_data(struct mctp_pktbuf *pkt);

void mctp_binding_set_tx_enabled(__unused void *binding, __unused bool enabled);

#endif
