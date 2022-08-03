#ifndef SHMB_RXBUF_H
#define SHMB_RXBUF_H

#include "shmb.h"

#include <sys/types.h>

struct mctp_astlpc_rxbuf {
	struct shmb shmb;
	struct mctp_astlpc_kcs *kcs;
	void *buf;
	size_t len;
};

void mctp_astlpc_rxbuf_init(struct mctp_astlpc_rxbuf *ctx, struct mctp_astlpc_kcs *kcs,
			    void *buf, size_t len);
struct shmb *mctp_astlpc_rxbuf_get(struct mctp_astlpc_rxbuf *ctx);

#endif
