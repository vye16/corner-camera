function outfile = saveMeanImage(srcfile, input_type, overwrite)

if nargin < 3
    overwrite = 0;
end

switch(input_type)
case 'video'
    outfile = saveMeanVideoFrame(srcfile, overwrite);

case 'images'
    error('does not support directory of images yet');
end

end
