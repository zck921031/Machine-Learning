clear all;
clc;
trainset = csvread('..\..\..\dataset\wine\wine_test.data');
train_data = trainset(:,2:14);
train_lable = trainset(:,1);
[n, dim] = size(trainset);
S = int8( zeros(n) );
D = int8( zeros(n) );

train_data = [
    2,20;
    3,30;
    4,40;
    3,40;
    5,50;
    6,60;
    7,70;
    6,50;
    ];
train_lable = [1,1,1,1,2,2,2,2];
n = 8;
S = int8( zeros(n) );
D = int8( zeros(n) );

for i=1:n
    for j=1:n
        if ( train_lable(i) == train_lable(j) ) 
            S(i,j) = 1;
        else
            D(i,j) = 1;
        end
    end
end
%A = Newton(train_data, S, D, 1);
%a = diag(A);
%disp(a);
A = opt(train_data, S, D, 500);
disp(A);
%while ~feof(ftrain)
    %for i=1:14
    %end
%end