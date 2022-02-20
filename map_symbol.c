#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#include "nfp.h"
#include "nfp_cpp.h"
#include "nfp_nffw.h"
#include "nfp-common/nfp_resid.h"

#define NFP_DEVICE 0

int main(int argc, char** argv) {
	if(argc != 4) {
		fprintf(stderr, "Usage: %s <symbol name> <size in bytes> <offset>.\n", argv[0]);
		exit(-1);
	}

	struct nfp_device *h_nfp = nfp_device_open(NFP_DEVICE);
	if(!h_nfp) {
		fprintf(stderr, "Error: failed to open nfp handle (%s)\n", strerror(errno));
		exit(-1);
	}

	const struct nfp_rtsym *rtsym = nfp_rtsym_lookup(h_nfp, argv[1]);
	if(!rtsym) {
		fprintf(stderr, "Error: could not find rtsym %s (%s)\n", argv[1], strerror(errno));
		exit(-1);
	}

	struct nfp_cpp *h_cpp = nfp_device_cpp(h_nfp);
	if(!h_cpp) {
		fprintf(stderr, "Error: failed to get cpp handler (%s)\n", strerror(errno));
		exit(-1);
	}

	ssize_t size = atoi(argv[2]);
	unsigned long long offset = atoi(argv[3]);

	uint32_t id = NFP_CPP_ISLAND_ID(rtsym->target, NFP_CPP_ACTION_RW, 0, rtsym->domain);
	struct nfp_cpp_area *area = nfp_cpp_area_alloc(h_cpp, id, (rtsym->addr + offset), size);

	if(!area) {
		fprintf(stderr, "Error: failed to get cpp area (%s)\n", strerror(errno));
		exit(-1);
	}

	if(nfp_cpp_area_acquire(area)) {
		fprintf(stderr, "Error: failed to acquire cpp area (%s)\n", strerror(errno));
		exit(-1);
	}

	uint8_t *buffer = nfp_cpp_area_mapped(area);
	if(!buffer) {
		fprintf(stderr, "Error: failed to mapped cpp area (%s)\n", strerror(errno));
		exit(-1);
	}

	/* do something on buffer */

	nfp_cpp_area_release_free(area);
	nfp_device_close(h_nfp);

	return 0;
}
