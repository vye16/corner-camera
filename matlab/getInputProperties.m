function params = getInputProperties(srcfile, input_type, params)

switch input_type
case 'video'
    v = VideoReader(srcfile);
    params.frame_rate = v.FrameRate;
    params.maxframe = v.Duration * v.FrameRate;
    
case 'images'
    error('does not support directory of images yet');
end
end
