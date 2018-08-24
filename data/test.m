clc;
clear;
data = load('C:\Users\Administrator\Desktop\build-serialPort-Desktop_Qt_5_10_1_MinGW_32bit-Debug\debug\2018-08-10_23-30-03ahrs.csv');

gyr = data(2:end,5:7);
acc = data(2:end,8:10);

[m,n] = size(gyr);
mean_gx = sum(gyr(:,1))/m;
mean_gy = sum(gyr(:,2))/m;
mean_gz = sum(gyr(:,3))/m;

mean_ax = sum(acc(:,1))/m;
mean_ay = sum(acc(:,2))/m;
mean_az = sum(acc(:,3))/m;

disp('m = ');
disp(m)
