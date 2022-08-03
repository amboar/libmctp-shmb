#include "kcs.h"
#include "mctp.h"

#include <errno.h>
#include <stddef.h>
#include <stdlib.h>

int mctp_astlpc_kcs_setup(struct mctp_astlpc_kcs *ctx, struct shmb *tx, struct shmb *rx)
{
	shmb_reset(tx);
	ctx->tx = tx;

	shmb_reset(rx);
	ctx->rx = rx;

	ctx->str = 0;

	/* Announce we're setup */
	ctx->str |= MCTP_ASTLPC_BMC_READY;

	ctx->odr = MCTP_ASTLPC_CMD_DUMMY;
	ctx->str |= KCS_STR_OBF;

	return 0;
}

void mctp_astlpc_kcs_reset(struct mctp_astlpc_kcs *ctx)
{
	shmb_restart(ctx->rx);
	shmb_restart(ctx->tx);
	ctx->str &= ~MCTP_ASTLPC_CHANNEL_READY;

	ctx->odr = MCTP_ASTLPC_CMD_DUMMY;
	ctx->str |= KCS_STR_OBF;
}

int mctp_astlpc_kcs_init(struct mctp_astlpc_kcs *ctx)
{
	ctx->str |= MCTP_ASTLPC_CHANNEL_READY;

	ctx->odr = MCTP_ASTLPC_CMD_DUMMY;
	ctx->str |= KCS_STR_OBF;

	return 0;
}

int mctp_astlpc_kcs_tx_begin(struct mctp_astlpc_kcs *ctx)
{
	if (ctx->str & KCS_STR_OBF)
		return -EBUSY;

	ctx->odr = MCTP_ASTLPC_CMD_TX_BEGIN;
	ctx->str |= KCS_STR_OBF;

	return 0;
}

int mctp_astlpc_kcs_rx_complete(struct mctp_astlpc_kcs *ctx)
{
	if (ctx->str & KCS_STR_OBF)
		return -EBUSY;

	ctx->odr = MCTP_ASTLPC_CMD_RX_COMPLETE;
	ctx->str |= KCS_STR_OBF;

	return 0;
}

int mctp_astlpc_kcs_poll(struct mctp_astlpc_kcs *ctx)
{
	struct mctp_pktbuf *pkt;
	uint8_t cmd;
	ssize_t rc;

	/* Handle any outstanding ownership transfers */
	rc = shmb_barrier(ctx->rx);
	if (rc)
		return rc;

	rc = shmb_barrier(ctx->tx);
	if (rc)
		return rc;

	if (!(ctx->str & KCS_STR_IBF))
		return 0;

	cmd = ctx->idr;
	ctx->str &= ~KCS_STR_IBF;

	switch (cmd) {
	case MCTP_ASTLPC_CMD_INIT:
		rc = mctp_astlpc_kcs_init(ctx);
		if (rc)
			goto reset;
		break;
	case MCTP_ASTLPC_CMD_TX_BEGIN:
		rc = shmb_acquire(ctx->rx);
		if (rc)
			goto reset;

		pkt = mctp_pktbuf_alloc(NULL, MCTP_PKTBUF_MAX);
		if (!pkt) {
			rc = -ENOMEM;
			goto reset;
		}

		rc = shmb_prepare(ctx->rx, pkt);
		if (rc)
			goto reset;

		return shmb_release(ctx->rx);
	case MCTP_ASTLPC_CMD_RX_COMPLETE:
		rc = shmb_acquire(ctx->tx);
		if (rc)
			goto reset;
		break;
	case MCTP_ASTLPC_CMD_DUMMY:
		break;
	default:
		abort();
	};

	return 0;
reset:
	mctp_astlpc_kcs_reset(ctx);
	return rc;
}
