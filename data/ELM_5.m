clc;
clear;
traindata=load('C:\Users\Administrator\Desktop\serialPort\data\traindata1.csv');

feature=traindata(1:end,5:13); % ����  7.
% gyrsum = sqrt(traindata(1:end,5).^2 + traindata(1:end,6).^2 + traindata(1:end,7).^2);
% feature = [gyrsum feature];

label=traindata(:,14);%��ǩ
X=feature;
[N,n]=size(X);
L=500;
m=5;                          % ����1������2������3������¥�ݡ�����ǰ���������4�����5��  5����
W=rand(n,L)*2-1;%Ȩ��-1��1
b_1=rand(1,L);
b=ones(N,1)*b_1;
H=1./(1+exp(-X*W+b));
temp_T=zeros(N,m);
for i=1:N
    if(label(i)==1)
        temp_T(i,1)=1;
        temp_T(i,2)=0;
        temp_T(i,3)=0;
        temp_T(i,4)=0;
        temp_T(i,5)=0;
    end
    if(label(i)==2)
        temp_T(i,1)=0;
        temp_T(i,2)=1;
        temp_T(i,3)=0;
        temp_T(i,4)=0;
        temp_T(i,5)=0;
    end
    if(label(i)==3)
        temp_T(i,1)=0;
        temp_T(i,2)=0;
        temp_T(i,3)=1;
        temp_T(i,4)=0;
        temp_T(i,5)=0;
    end
    if(label(i)==4)
        temp_T(i,1)=0;
        temp_T(i,2)=0;
        temp_T(i,3)=0;
        temp_T(i,4)=1;
        temp_T(i,5)=0;
    end
    if(label(i)==5)
        temp_T(i,1)=0;
        temp_T(i,2)=0;
        temp_T(i,3)=0;
        temp_T(i,4)=0;
        temp_T(i,5)=1;
    end
end
T=temp_T*2-1;
beta=pinv(H)*T;

x_1=X(:,1);
x_2=X(:,2);

% hold on

% for i=1:N
%     if(label(i)==1)
%         plot(x_1(i),x_2(i),'.g');
%     else
%         plot(x_1(i),x_2(i),'.r');
%    end
% end
% testdata = load('C:\Users\Elena\Desktop\testdata1.csv');
testfeature = feature;
[N,n] = size(testfeature);
testT = zeros(N,1);
Tt = zeros(N,m);
for i = 1:N
    h=1./(1+exp(-testfeature(i,:)*W+b_1));
    t = h * beta;
    Tt(i,:) = t;
    if t(1) > 0 && t(2) < 0 && t(3) < 0 && t(4) < 0 && t(5) < 0
        testT(i) = 1;
    end
    if t(2) > 0 && t(1) < 0 && t(3) < 0 && t(4) < 0 && t(5) < 0
        testT(i) = 2;
    end
    if t(3) > 0 && t(1) < 0 && t(2) < 0 && t(4) < 0 && t(5) < 0
        testT(i) = 3;
    end
    if t(4) > 0 && t(1) < 0 && t(3) < 0 && t(2) < 0 && t(5) < 0
        testT(i) = 4;
    end
    if t(5) > 0 && t(1) < 0 && t(3) < 0 && t(4) < 0 && t(2) < 0
        testT(i) = 5;
    end
end

for i = 1:14
    test = testT(40000+(i-1)*350:40000+i*350,:);
    fall = 0;  stand = 0; 

    for j=1:350
        if test(j) == 4 || test(j) == 5
            fall = fall+1;
        else
            stand = stand+1;
        end     
    end
    if fall > stand
        disp('����')
    else
        disp('����')
    end
end
% test = testT(end-350:end,:);

csvwrite('C:\Users\Administrator\Desktop\serialPort\data\weight.csv',W);
csvwrite('C:\Users\Administrator\Desktop\serialPort\data\beta.csv',beta);
csvwrite('C:\Users\Administrator\Desktop\serialPort\data\bias.csv',b_1);

