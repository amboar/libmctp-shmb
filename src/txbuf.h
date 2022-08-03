#ifndef SHMB_TXBUF_H
#define SHMB_TXBUF_H

#include "shmb.h"
#include "kcs.h"

#include <sys/types.h>

struct mctp_astlpc_txbuf {
	struct shmb shmb;
	struct mctp_astlpc_kcs *kcs;
	void *buf;
	size_t len;
};

void mctp_astlpc_txbuf_init(struct mctp_astlpc_txbuf *ctx, struct mctp_astlpc_kcs *kcs,
			    void *buf, size_t len);
struct shmb *mctp_astlpc_txbuf_get(struct mctp_astlpc_txbuf *ctx);
int mctp_astlpc_txbuf_xmit(struct mctp_astlpc_txbuf *ctx, struct mctp_pktbuf *pkt);

#endif
