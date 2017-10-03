clear; close all;


datafolder = '/Users/vickieye/Dropbox (MIT)/shadowImaging/edgeImaging/data/';
% datafolder = '/data/vision/billf/shadowImaging/edgeImaging/data/';

test_vid = 3;
switch test_vid
    case 0
        expname = 'testvideos_Jan22';
        name = 'redblue_dark';
        vidtype = '.MOV';
    case 1
        expname = 'testvideos_Mar04';
        name = 'blue_randomwalking1';
        vidtype = '.MP4';
    case 2
        expname = 'testvideos_Mar03';
        name = 'blue_randomwalking2_loc2';
        vidtype = '.MP4';
    case 3
        expname = 'stereoDoor_Feb20';
        name = 'red12_walking';
        vidtype = '.MP4';
end

input_type = 'video';

expfolder = fullfile(datafolder, expname, 'experiments');
srcfile = fullfile(expfolder, strcat(name, vidtype));
cal_datafile = fullfile(expfolder, 'no_rect_cal_data.mat');

load(cal_datafile, 'corner', 'wall_angle', 'wall_point');
corner = round(corner);
fprintf('corner [%d, %d], wall_point [%d, %d], wall_angle %d\n',...
    corner(1), corner(2),...
    round(wall_point(1) + corner(1)),...
    round(wall_point(2) + corner(2)),...
    round(wall_angle * 180/pi));

vsrc = VideoReader(srcfile);
frame1 = rgb2gray(readFrame(vsrc));
r = 50;
nfeatures = 4;
feature_type = 'brisk';

figure; colormap('gray');
subplot(221); imagesc(frame1); hold on;
plot(corner(1), corner(2), 'r*');
switch feature_type
    case 'brisk'
        keypts = detectBRISKFeatures(frame1,...
            'mincontrast', 0.05,...
            'minquality', 0.1,...
            'numoctaves', 4,...
            'roi', [corner(1)-r, corner(2)-r, 2*r, 2*r]);
    case 'fast'
        keypts = detectFASTFeatures(frame1,...
            'mincontrast', 0.05,...
            'roi', [corner(1)-r, corner(2)-r, 2*r, 2*r]);
    case 'harris'
        keypts = detectHarrisFeatures(frame1,...
            'roi', [corner(1)-r, corner(2)-r, 2*r, 2*r]);
    case 'surf'
        keypts = detectSURFFeatures(frame1,...
            'roi', [corner(1)-r, corner(2)-r, 2*r, 2*r]);
end
if keypts.Count > 0
    plot(keypts.selectStrongest(nfeatures));
end

transvec = [4, 8];
for i = 1:1
    frame = imtranslate(imrotate(frame1, 3, 'nearest', 'crop'), transvec);
    switch feature_type
        case 'brisk'
            keypts = detectBRISKFeatures(frame,...
                'mincontrast', 0.05,...
                'minquality', 0.1,...
                'numoctaves', 4,...
                'roi', [corner(1)-r, corner(2)-r, 2*r, 2*r]);
        case 'fast'
            keypts = detectFASTFeatures(frame,...
                'mincontrast', 0.05,...
                'roi', [corner(1)-r, corner(2)-r, 2*r, 2*r]);
        case 'harris'
            keypts = detectHarrisFeatures(frame,...
                'roi', [corner(1)-r, corner(2)-r, 2*r, 2*r]);
        case 'surf'
            keypts = detectSURFFeatures(frame,...
                'roi', [corner(1)-r, corner(2)-r, 2*r, 2*r]);
    end
    subplot(222); imagesc(frame); hold on;
    if keypts.Count > 0
        plot(keypts.selectStrongest(nfeatures));
    end

    im = frame(corner(2)-r:corner(2)+r, corner(1)-r:corner(1)+r);
    % bw = edge(frame, 'canny', [], ceil(max(size(frame))/100));
    bw = edge(im, 'canny', [], r/50);
    subplot(223); imagesc(bw);
    
    t0 = wall_angle*180/pi - 90;
    [h, theta, rho] = hough(bw, 'Theta', t0-10:1:t0+10);
    peaks = houghpeaks(h);
    lines = houghlines(bw, theta, rho, peaks);
    subplot(224); imagesc(im); hold on;
    for k = 1:length(lines)
       xy = [lines(k).point1; lines(k).point2];
       plot(xy(:,1),xy(:,2),'LineWidth',2,'Color','green');

       % Plot beginnings and ends of lines
       plot(xy(1,1),xy(1,2),'x','LineWidth',2,'Color','yellow');
       plot(xy(2,1),xy(2,2),'x','LineWidth',2,'Color','red');
    end
    
end