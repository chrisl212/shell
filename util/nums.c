#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	char fname[256];
	sprintf(fname, "samples/%d.b", atoi(argv[1]));
	int len = atoi(argv[1]);
	long *nums = malloc(len*sizeof(*nums));
	for (int i = 0; i < len; i++) {
		nums[i] = rand();
	}

	FILE *f = fopen(fname, "w");
	fwrite(nums, sizeof(nums[0]), len, f);
	fclose(f);
	return 0;
}
