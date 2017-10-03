% run within other scripts with test video info already set
addpath(genpath('pyr'));

if params.rectify
    params.homography = saveHomographyData(gridfile, input_type, 0);
end

params.corner_data = saveCornerData(cornerfile, ncorners, input_type, 0);
params = setObsXYLocs(params);

if debug
    load(params.corner_data, 'calimg', 'corner');
    c = corner(params.corner_idx,:);
    c(1) = (c(1) - params.xrange(1))/2^params.downlevs + 1;
    c(2) = (c(2) - params.yrange(1))/2^params.downlevs + 1;
    calimg = preprocessFrame(double(calimg), params);
    figure; imagesc(calimg(:,:,1)); hold on;
    plot(c(1), c(2), 'y*');
    plot(params.obs_xlocs, params.obs_ylocs, 'r.');
    colormap gray;
    axis off;
end

for i = 1:length(names)
    name = names{i};
    fprintf('Processing %s\n', name);
    srcfile = fullfile(expfolder, strcat(name, vidtype));
    params.mean_datafile = saveMeanImage(srcfile, input_type, 0);
    params.lambda = estimateFrameNoise(params);
    
    params = getInputProperties(srcfile, input_type, params);
    params.startframe = ceil(params.frame_rate * start_time);
    if debug % faster processing, use predefined parameters
        params.endframe = ceil(params.frame_rate * end_time);
        params.step = step;
    else
        params.endframe = floor(params.maxframe - params.startframe);
        params.step = 1;
    end

    paramstr = sprintf('%s_rect%d_%s_corner%d_lambda%.2f_n%d',...
        name, params.rectify, params.sampling, params.corner_r,...
        params.lambda, params.nsamples);
    outfile = fullfile(resfolder, sprintf('out_%s.mat', paramstr));
    imfile = fullfile(resfolder, sprintf('im_%s.png', paramstr));
    if exist(outfile, 'file')
        fprintf('%s already exists\n', outfile);
    end

    [outframes, params] = videoRecon(srcfile, params);

    scaled = (outframes + 0.05)/0.1;
    if debug
        figure; imagesc(scaled);
    end

    imwrite(scaled, imfile);
    save(outfile);
end
