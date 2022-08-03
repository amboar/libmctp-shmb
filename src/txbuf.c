#include "txbuf.h"

#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

static int mctp_astlpc_txbuf_acquire(__unused struct shmb *shmb)
{
	/* FIXME: pass binding */
	mctp_binding_set_tx_enabled(NULL, true);

	return 0;
}

static int mctp_astlpc_txbuf_prepare(struct shmb *shmb, struct mctp_pktbuf *pkt)
{
	struct mctp_astlpc_txbuf *ctx = container_of(shmb, struct mctp_astlpc_txbuf, shmb);

	if (mctp_pktbuf_size(pkt) > ctx->len)
		return -EINVAL;

	/* FIXME: pass binding */
	mctp_binding_set_tx_enabled(NULL, false);

	memcpy(ctx->buf, mctp_pktbuf_data(pkt), mctp_pktbuf_size(pkt));

	return 0;
}

static int mctp_astlpc_txbuf_release(struct shmb *shmb)
{
	struct mctp_astlpc_txbuf *ctx = container_of(shmb, struct mctp_astlpc_txbuf, shmb);

	return mctp_astlpc_kcs_tx_begin(ctx->kcs);
}

static const struct shmb_ops mctp_astlpc_txbuf_ops = {
	.acquire = mctp_astlpc_txbuf_acquire,
	.prepare = mctp_astlpc_txbuf_prepare,
	.release = mctp_astlpc_txbuf_release,
};

void mctp_astlpc_txbuf_init(struct mctp_astlpc_txbuf *ctx,
					     struct mctp_astlpc_kcs *kcs,
					     void *buf, size_t len)
{
	shmb_init(&ctx->shmb, &mctp_astlpc_txbuf_ops, shmb_acquired);
	ctx->kcs = kcs;
	ctx->buf = buf;
	ctx->len = len;
}

struct shmb *mctp_astlpc_txbuf_get(struct mctp_astlpc_txbuf *ctx)
{
	return &ctx->shmb;
}

int mctp_astlpc_txbuf_xmit(struct mctp_astlpc_txbuf *ctx, struct mctp_pktbuf *pkt)
{
	int rc;

	rc = shmb_prepare(&ctx->shmb, pkt);
	if (rc)
		return rc;

	return shmb_release(&ctx->shmb);
}
