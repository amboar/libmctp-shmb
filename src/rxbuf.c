#include "kcs.h"
#include "rxbuf.h"

#include <string.h>

static int mctp_astlpc_rxbuf_prepare(struct shmb *shmb, struct mctp_pktbuf *pkt)
{
	struct mctp_astlpc_rxbuf *ctx = container_of(shmb, struct mctp_astlpc_rxbuf, shmb);

	/* FIXME: lengths */
	memcpy(mctp_pktbuf_data(pkt), ctx->buf, ctx->len);

	return 0;
}

static int mctp_astlpc_rxbuf_release(struct shmb *shmb)
{
	struct mctp_astlpc_rxbuf *ctx = container_of(shmb, struct mctp_astlpc_rxbuf, shmb);

	return mctp_astlpc_kcs_rx_complete(ctx->kcs);
}

static const struct shmb_ops mctp_astlpc_rxbuf_ops = {
	.prepare = mctp_astlpc_rxbuf_prepare,
	.release = mctp_astlpc_rxbuf_release,
};

void mctp_astlpc_rxbuf_init(struct mctp_astlpc_rxbuf *ctx,
				     struct mctp_astlpc_kcs *kcs,
				     void *buf, size_t len)
{
	shmb_init(&ctx->shmb, &mctp_astlpc_rxbuf_ops, shmb_released);
	ctx->kcs = kcs;
	ctx->buf = buf;
	ctx->len = len;
}

struct shmb *mctp_astlpc_rxbuf_get(struct mctp_astlpc_rxbuf *ctx)
{
	return &ctx->shmb;
}
