#include "compiler.h"
#include "kcs.h"
#include "rxbuf.h"
#include "txbuf.h"

#include <err.h>
#include <stdlib.h>

int main(void)
{
	struct mctp_astlpc_kcs _kcs, *kcs = &_kcs;
	struct mctp_astlpc_txbuf _tx, *tx = &_tx;
	struct mctp_astlpc_rxbuf _rx, *rx = &_rx;
	struct mctp_pktbuf *pkt;
	void *tx_region;
	void *rx_region;
	int rc;

	rc = mctp_astlpc_kcs_setup(kcs, mctp_astlpc_txbuf_get(tx), mctp_astlpc_rxbuf_get(rx));
	if (rc)
		errx(EXIT_FAILURE, "Failed to initialise KCS\n");

	tx_region = malloc(MCTP_PKTBUF_MAX);
	if (!tx_region)
		errx(EXIT_FAILURE, "Failed to allocate Tx region\n");
	mctp_astlpc_txbuf_init(tx, kcs, tx_region, MCTP_PKTBUF_MAX);

	rx_region = malloc(MCTP_PKTBUF_MAX);
	if (!rx_region)
		errx(EXIT_FAILURE, "Failed to allocation Rx region\n");
	mctp_astlpc_rxbuf_init(rx, kcs, rx_region, MCTP_PKTBUF_MAX);

	mctp_astlpc_kcs_poll(kcs);

	pkt = mctp_pktbuf_alloc(NULL, MCTP_PKTBUF_MAX);
	if (!pkt)
		errx(EXIT_FAILURE, "Failed to allocate packet for transmission\n");

	mctp_astlpc_txbuf_xmit(tx, pkt);
	mctp_pktbuf_free(pkt);

	return 0;
}
