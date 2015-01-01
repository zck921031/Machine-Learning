%3*x^2 + 4*y^2 + 5*x*y - 6*x + 7*y
function newton()
    clc;
    clear all;
    x=0; y=0;
    error = 1.0;
    while error > 1e-15
        disp( [x, y, f0(x,y)] );
%         [x;y]
%         inv( f2(x,y) )
        [x1, y1] = f1( x, y );
        %[x1;y1] 
        Hessian = f2(x,y);
        tmp = [x;y] - inv( Hessian ) * [x1;y1];
        disp( inv( Hessian ) * [x1;y1] );
        %tmp = [x;y] - Hessian \ [x1;y1];
        tmpx = tmp(1); tmpy = tmp(2);
%         if (tmpx<0) 
%             tmpx=0;
%         end 
%         if (tmpy<0) 
%             tmpy=0;
%         end
        error = abs( f0( tmpx, tmpy ) - f0( x,y ) );
        x=tmpx; y=tmpy;
    end
end

function ret = f0(x,y)
    ret = 3*x^2 + 4*y^2 + 5*x*y - 6*x + 7*y;
end

function [x1,y1] = f1(x,y)
    x1 = 6*x + 5*y - 6;
    y1 = 8*y + 5*x + 7;
end

function Hessian = f2(x,y)
    Hessian = [6, 5; 5, 8 ];
end

