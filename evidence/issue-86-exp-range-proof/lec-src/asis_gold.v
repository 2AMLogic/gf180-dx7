module gold(input [55:0] exp_xq, input [45:0] exp_p2, input [70:0] exp_t3, input [70:0] exp_t4, input [65:0] exp_t5, output [56:0] o);

wire [56:0]  exp_hsum = (57'd1 << 56)
        + {1'b0, exp_xq}
        + {13'b0, exp_p2[44:1]}         // x^2/2, Q0.56
        + {13'b0, {15'b0, exp_t3[70:30]}}        // x^3/6, Q0.56
        + {13'b0, {15'b0, exp_t4[70:30]}}        // x^4/24, Q0.56
        + {13'b0, {16'b0, exp_t5[65:26]}};
assign o = exp_hsum;
endmodule
