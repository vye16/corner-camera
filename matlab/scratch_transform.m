clear; close all;

im = imread('cameraman.tif');
pt = [100; 100];
center = size(im)'/2;
v = [0 0 1;
    0 0 10;
    0 0 0];
vim = v + rotz(0);
tform = projective2d(vim');

vpt = v + rotz(1);
imnew = imwarp(im, tform);
ptnew = vpt * [pt - center; 1] + [center; 1];

figure; subplot(121); imagesc(im); hold on; plot(pt(1), pt(2), 'ro');
subplot(122); imagesc(imnew); hold on; plot(ptnew(1), ptnew(2), 'ro');
