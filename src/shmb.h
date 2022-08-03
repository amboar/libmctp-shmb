#ifndef SHMB_SHMB_H
#define SHMB_SHMB_H

#include "mctp.h"

#include <sys/types.h>

enum shmb_state {
	shmb_idle,
	shmb_acquired,
	shmb_prepared,
	shmb_released
};

struct shmb;
struct shmb_ops {
	int (*acquire)(struct shmb *shmb);
	int (*prepare)(struct shmb *shmb, struct mctp_pktbuf *pkt);
	int (*release)(struct shmb *shmb);
};

struct shmb {
	const struct shmb_ops *ops;
	enum shmb_state start;
	enum shmb_state state;
	void *buf;
	size_t len;
};

void shmb_init(struct shmb *ctx, const struct shmb_ops *ops, enum shmb_state start);
void shmb_reset(struct shmb *ctx);
void shmb_restart(struct shmb *ctx);
int shmb_acquire(struct shmb *ctx);
int shmb_prepare(struct shmb *ctx, struct mctp_pktbuf *pkt);
int shmb_release(struct shmb *ctx);
int shmb_barrier(struct shmb *ctx);

#endif
