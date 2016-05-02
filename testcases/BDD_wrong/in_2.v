module top (a, b, c, x);
input a, b, c;
output x;

wire n1, n4, n6, n7;

or ( n1, a, b);
nand( n4, n1, c);
nand( n6, n4, n1);
nand( n5, c, n4 );
nand( n7, n5, n6);
buf( x, n7);

endmodule
