function outfile = saveMeanVideoFrame(srcfile, overwrite)

if nargin < 2
    overwrite = 0;
end

[folder, name] = fileparts(srcfile);
outfile = fullfile(folder, sprintf('%s_mean_vidframe.mat', name));
vsrc = VideoReader(srcfile);
times = 2: round(vsrc.FrameRate/30): vsrc.Duration-2;

if overwrite || ~exist(outfile, 'file')
    fprintf('Saving mean video frame in %s\n', outfile);

    back_img = zeros(size(readFrame(vsrc)));
    for i = 1:length(times)
        vsrc.CurrentTime = times(i);
        back_img = back_img + double(readFrame(vsrc));
    end
    back_img = back_img / length(times);

    save(outfile, 'back_img');
end

varinfo = who('-file', outfile);
if ~ismember('variance', varinfo)
    load(outfile, 'back_img');

    % compute the variance
    fprintf('Computing the variance and putting in %s\n', outfile);
    count = 0;
    variance = zeros(size(back_img));
    for i = 1:length(times)
        vsrc.CurrentTime = times(i);
        variance = variance + (double(readFrame(vsrc)) - back_img).^2;
        count = count + 1;
    end
    variance = variance / (count - 1);
    save(outfile, 'variance', '-append');
end
end
