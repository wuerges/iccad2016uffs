
module top (a, b, y);
input a, b;
output y;

wire c, d;

and(c, a, b);
and(d, a, b);
and(y, c, d);

endmodule
