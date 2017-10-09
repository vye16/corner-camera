function [samples, nanrows] = getObsVec(frame, params)
% interpolate pixel values at previously set observation xy locs
samples = zeros([numel(params.obs_xlocs), size(frame,3)]);

[yy, xx] = ndgrid(1:size(frame,1), 1:size(frame,2));
for i = 1:size(frame,3)
    samples(:,i) = interp2(xx, yy, frame(:,:,i),...
        params.obs_xlocs, params.obs_ylocs);
end

nanrows = any(isnan(samples), 2);
samples(nanrows,:) = []; % delete nanrows
end
