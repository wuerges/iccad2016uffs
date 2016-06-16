module top (a, b, c,d,e x);
   input a, b, c,d,e;
   output x;
   
   wire n1, n4,n5, n6, n7, n8, n9, n10, n11, n12, n13;
  
   and ( n1, a, b);
   nand( n4, n1, c);
   nand( n6, n4, n1);
   nand( n5, c, n4 );
   nand( n7, n5, n6);
   xor(n8 ,n7,d);
   or(n9,d,e,n7,1'b1);
   or(n10,n8,c);
   nand(n11,c,n9);
   xnor(n12,n10,n11);
   nor(n13,n12,1'b0);
   buf( x, n13);
   
endmodule
