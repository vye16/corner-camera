% blurDnClr.m
% 3-color version of blurDn.  Jan. 2, 2002 wtf
% made this work with single channel images  sept. 20, 2004 billf

function out = blurDnClr(im, nlevs, filt)

%------------------------------------------------------------
%% OPTIONAL ARGS:

if (exist('nlevs') ~= 1) 
  nlevs = 1;
end

if (exist('filt') ~= 1) 
  filt = 'binom5';
end

%------------------------------------------------------------

tmp = blurDn(im(:,:,1), nlevs, filt);
out = zeros(size(tmp,1), size(tmp,2), size(im,3));
out(:,:,1) = tmp;
for clr = 2:size(im,3)
  out(:,:,clr) = blurDn(im(:,:,clr), nlevs, filt);
end
