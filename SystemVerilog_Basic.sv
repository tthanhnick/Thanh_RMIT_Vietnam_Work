/* 
Course Digital System Design
Student name: Nguyen Tien Thanh
Student ID S3818111
Assignment 4
*/

//Problem 1
module assignment4(input logic A,B,C,D, output logic Y,Z);
assign Y=(A&B)|(C&D);
assign Z=(A&D)|(B&C);
endmodule


//Problem2
module assignment4(input logic [3:0] IN,
output logic EVEN,ODD
);
always_comb begin
case(IN)
4'd0, 4'd2, 4'd4, 4'd6, 4'd8,4'd10,4'd12,4'd14: EVEN = 1'b1;
default: EVEN =1'b0;
endcase
end

always_comb begin

case(IN)
4'd1, 4'd3, 4'd5, 4'd7, 4'd9,4'd11,4'd13,4'd15: ODD = 1'b1;
default: ODD =1'b0;
endcase
end
endmodule


//Problem 3
module assignment4(input logic A,B,clk,rst,
output logic[2:0] OUT,output logic[1:0]state_var);
logic[1:0] state,next_state;

always_ff@(posedge clk)
begin
if (rst)
state<=2'b00;
else
state<=next_state;
end

always_comb
begin
case(state)
2'b00:
begin
if(A==1'b1&&B==1'b1)
next_state=2'b01;
else if(A==1'b1&&B==1'b0)
next_state=2'b10;
else
next_state=2'b00;
end
2'b01:
begin
if(B==1'b0)
next_state=2'b10;
else
next_state=2'b01;
end
2'b10:
begin
if(B==1'b1)
next_state=2'b00;
else
next_state=2'b10;
end
default: state=next_state;
endcase
end

always_comb
case(state)
2'd00: OUT=3'b001;
2'd01: OUT=3'b010;
2'd10: OUT=3'b100;
default: OUT=3'b000;
endcase
assign state_var=state;
endmodule 


//Problem 4
module assignment4(
input logic rst, clk,press,output logic out)
logic [1:0] counts,next_counts
always_ff@(clk)
begin
if(rst)
counts<=2'b00;
else if(press)
counts<=next_counts
end

endmodule

