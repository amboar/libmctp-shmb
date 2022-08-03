#ifndef SHMB_KCS_H
#define SHMB_KCS_H

#include "shmb.h"

#define MCTP_ASTLPC_CMD_INIT		0
#define MCTP_ASTLPC_CMD_TX_BEGIN	1
#define MCTP_ASTLPC_CMD_RX_COMPLETE	2
#define MCTP_ASTLPC_CMD_DUMMY		255
struct mctp_astlpc_kcs {
	struct shmb *tx;
	struct shmb *rx;

	uint8_t idr;
	uint8_t odr;
	uint8_t str;
#define MCTP_ASTLPC_BMC_READY		(1U << 7)
#define MCTP_ASTLPC_CHANNEL_READY	(1U << 6)
#define KCS_STR_IBF			(1U << 1)
#define KCS_STR_OBF			(1U << 0)
};

int mctp_astlpc_kcs_setup(struct mctp_astlpc_kcs *ctx, struct shmb *tx, struct shmb *rx);
void mctp_astlpc_kcs_reset(struct mctp_astlpc_kcs *ctx);
int mctp_astlpc_kcs_init(struct mctp_astlpc_kcs *ctx);
int mctp_astlpc_kcs_tx_begin(struct mctp_astlpc_kcs *ctx);
int mctp_astlpc_kcs_rx_complete(struct mctp_astlpc_kcs *ctx);
int mctp_astlpc_kcs_poll(struct mctp_astlpc_kcs *ctx);

#endif
