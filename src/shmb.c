#include "shmb.h"

#include <errno.h>
#include <stdlib.h>

void shmb_init(struct shmb *ctx, const struct shmb_ops *ops, enum shmb_state start)
{
	ctx->ops = ops;
	ctx->start = start;
	shmb_restart(ctx);
}

/*
 * We can call shmb_reset() on an otherwise uninitialised struct shmb to force it into a safe state.
 * This helps break the constructor cycle between mctp_astlpc_kcs and shmb.
 */
void shmb_reset(struct shmb *ctx)
{
	ctx->state = shmb_idle;
}

void shmb_restart(struct shmb *ctx)
{
	switch (ctx->state) {
	case shmb_idle:
	case shmb_acquired:
	case shmb_prepared:
	case shmb_released:
		ctx->state = ctx->start;
		return;
	default:
		abort();
	}
}


int shmb_acquire(struct shmb *ctx)
{
	switch (ctx->state) {
	case shmb_idle:
	case shmb_acquired:
	case shmb_prepared:
		return -EPROTO;
	case shmb_released:
		/*
		 * The other side has given us the buffer, so record their expectation before
		 * running callbacks
		 */
		ctx->state = shmb_acquired;

		if (!ctx->ops->acquire)
			return 0;

		return ctx->ops->acquire(ctx);
	default:
		abort();
	}
}

int shmb_prepare(struct shmb *ctx, struct mctp_pktbuf *pkt)
{
	ssize_t rc;

	switch (ctx->state) {
	case shmb_idle:
		return -EPROTO;
	case shmb_acquired:
		rc = ctx->ops->prepare(ctx, pkt);
		if (rc)
			return rc;
		ctx->state = shmb_prepared;
		return 0;
	case shmb_prepared:
	case shmb_released:
		return -EPROTO;
	default:
		abort();
	}
}

int shmb_release(struct shmb *ctx)
{
	int rc;

	switch (ctx->state) {
	case shmb_idle:
	case shmb_acquired:
		return -EPROTO;
	case shmb_prepared:
		rc = ctx->ops->release(ctx);
		if (rc)
			return rc;
		ctx->state = shmb_released;
		return 0;
	case shmb_released:
		return -EPROTO;
	default:
		abort();
	}
}

int shmb_barrier(struct shmb *ctx)
{
	int rc;

	switch (ctx->state) {
	case shmb_idle:
		return -EPROTO;
	case shmb_acquired:
		return 0;
	case shmb_prepared:
		rc = ctx->ops->release(ctx);
		if (rc)
			return rc;
		ctx->state = shmb_released;
		return 0;
	case shmb_released:
		return 0;
	default:
		abort();
	}
}
