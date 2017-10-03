function [outframes, params] = videoRecon(srcfile, params)

vsrc = VideoReader(srcfile);
if params.online % start the running mean
    % take the frame before the start
    vsrc.CurrentTime = (params.startframe - params.step)/vsrc.FrameRate;
    back_img = double(readFrame(vsrc));
else
    load(params.mean_datafile, 'back_img');
end
back_img = preprocessFrame(back_img, params);
mean_pixel = mean(mean(back_img, 1), 2);
nchans = size(back_img, 3);

frameidx = params.startframe: params.step: params.endframe;
times = frameidx / vsrc.FrameRate;

amat = getAmat(params);
[~, nanrows] = getObsVec(back_img, params);
amat(nanrows,:) = [];

% spatial prior
bmat = eye(size(amat,2)) - diag(ones([size(amat,2)-1,1]), 1);
bmat = bmat(1:end-1,:); % take out last row
bmat(1,:) = 0; % don't use the constant light to smooth

cmat = eye(size(bmat, 2)); % regularize the parameters
cmat(1,:) = 0;

reg = params.beta * (bmat'*bmat + cmat); % beta = 1/sigma^2

% do inference
gain = inv(amat'*amat/params.lambda + reg)*(amat'/params.lambda);

outframes = zeros([length(times), size(amat,2), nchans]);
tic;
for i = 1:length(times)
    fprintf('Frame %i\n', frameidx(i));
    vsrc.CurrentTime = times(i);
    framen = preprocessFrame(double(readFrame(vsrc)), params);
    if params.sub_mean
        y = bsxfun(@plus, getObsVec(framen - back_img, params), mean_pixel);
    else
        y = getObsVec(framen - back_img, params);
    end
    
    for c = 1:nchans
        outframes(i,:,c) = gain * y(:,c);
    end
    
    if params.online
        % update running mean
        back_img = back_img * i/(i+1) + framen/(i+1);
        mean_pixel = mean(mean(back_img, 1), 2);
    end   
    toc;
end
outframes = outframes(:,2:end,:);
end

