function mednoise = estimateFrameNoise(params)
load(params.mean_datafile, 'variance');
input_fmt = preprocessFrame(variance, params);
mednoise = median(median(mean(input_fmt, 3)));
end