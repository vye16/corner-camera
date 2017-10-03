function outframe = preprocessFrame(frame, params)
% crop then blur and downsample the frame
cropped = frame(params.yrange(1):params.yrange(2),...
    params.xrange(1):params.xrange(2),:);
outframe = blurDnClr(cropped, params.downlevs,...
    binomialFilter(params.filter_width));
end