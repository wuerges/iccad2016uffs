module top (a, b, c, y);
input a, b, c;
output y;

wire na, nb, n1, n4, n6, n7;

not(na, a);
not(nb, b);

or(n1, na, nb);
or(n2, n1, c);

nand(n3, n1, c);

nand(n8, n2, n3);
buf(y, n8);

endmodule
