struct SSRCDither *SSRCDither_init(int32_t fs, int32_t min, int32_t max, int shaperID, int pdfID, double noisePeak, uint64_t seed);
void SSRCDither_dispose(struct SSRCDither *thiz);
void SSRCDither_quantizeDouble(struct SSRCDither *thiz, int32_t *out, double *in, int32_t nSamples, double gain);
void SSRCDither_getPeaks(struct SSRCDither *thiz, double *peaks);
double SSRCDither_getMaxNoiseStrength(struct SSRCDither *thiz);

const int *SSRCDither_getAllSupportedFS();
int SSRCDither_getNumAvailableShapers(int32_t fs);
int SSRCDither_getAvailableShaperIDs(int *ids, int maxnids, int32_t fs);
const char *SSRCDither_getNameForShaperID(int32_t fs, int id);
