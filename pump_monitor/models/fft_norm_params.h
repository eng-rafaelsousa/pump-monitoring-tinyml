// Médias e desvios dos 10 bins da FFT (para normalização)
#ifndef FFT_NORM_PARAMS_H
#define FFT_NORM_PARAMS_H

#define INPUT_SIZE 10

const float fft_means[INPUT_SIZE] = {0.996050f, 0.875550f, 0.966367f, 0.984717f, 1.087767f, 0.822967f, 0.592733f, 0.531417f, 0.512550f, 0.489733f};
const float fft_scales[INPUT_SIZE] = {1.159310f, 1.081866f, 1.256140f, 1.201076f, 1.405887f, 1.035435f, 0.563364f, 0.525604f, 0.519370f, 0.485579f};

#endif // FFT_NORM_PARAMS_H
