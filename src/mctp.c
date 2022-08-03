#include "mctp.h"

#include <assert.h>
#include <stdlib.h>

struct mctp_pktbuf *mctp_pktbuf_alloc(__unused void *ctx, size_t payload)
{
	struct mctp_pktbuf *pkt;

	pkt = malloc(sizeof(*pkt) + payload);
	if (!pkt)
		return NULL;

	pkt->start = 0;
	pkt->end = payload;

	return pkt;
}

void mctp_pktbuf_free(struct mctp_pktbuf *pkt)
{
	free(pkt);
}

size_t mctp_pktbuf_size(struct mctp_pktbuf *pkt)
{
	assert(pkt->end > pkt->start);
	return pkt->end - pkt->start;
}

void *mctp_pktbuf_data(struct mctp_pktbuf *pkt)
{
	return &pkt->buf[pkt->start];
}

void mctp_binding_set_tx_enabled(__unused void *binding, __unused bool enabled)
{
}
