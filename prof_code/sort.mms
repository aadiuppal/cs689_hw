/* sorting algorithm by Knuth, commented by Jimenez */

          LOC   Data_Segment
x0        GREG  @			/* x0 is a pointing to the data segment */
X0        IS    @			/* X0 is the beginning of the data segment */
N         IS    100			/* N is equal to 100 */

j         IS    $0			/* j is register 0 */
m         IS    $1			/* m is register 1 */
kk        IS    $2			/* kk is register 2 */
xk        IS    $3			/* xk is register 3 */
t         IS    $255			/* t is register 255 */
          LOC   #100			/* location of this code is hex 100 */

/* The algorithm looks like this: 		*/
/* Algorithm M: Find the Maximum 		*/
/* Step 1: i := a parameter, m := 0 		*/
/* Step 2: While i >= 0 do Steps 3 and 4	*/
/* Step 3:    if X[i] >= m then m := X[i]	*/
/* Step 4:    i := i - 1			*/
/* Step 5: Return m as the maximum		*/

/* Subroutine implementation in MMIX */
/* find the maximum of octas from x0[$0] downto x0[0], return the index of it in $0 (j)  and the value in $1 (m) */
/* m ($1) will be the maximum element of the array */

Maximum   SL    kk,$0,3			/* kk := $0 * 8  ( i := 0 ) */
          LDO   m,x0,kk			/* m := M8[x0+kk] ( m := X[i] ) */
          JMP   ChangeJ			/* goto ChangeJ */
Loop      LDO   xk,x0,kk		/* xk := M8[x0+kk] ( xk := X[i] ) */
          CMP   t,xk,m			/* compare xk to m, result to t (t = cmp(xk,m)) */
          PBNP  t,DecreaseK		/* if (xk < m) then goto DecreaseK */
ChangeM   SET   m,xk			/* m := xk */ /* here, m is the new maximum */
ChangeJ   SR    j,kk,3			/* j := kk / 8 */
DecreaseK SUB   kk,kk,8			/* kk := kk - 8 ( i := i - 1 ) */
          PBP   kk,Loop			/* if (kk >= 0) goto Loop */
          POP   2,0			/* return, returning the index of the maximum in j ($0) and the maximum in m ($1) */

/* The program looks like this: */
/* Program Sort
/* Step 1: Read 100 numbers into X[0..99] from standard input */
/* Step 2: Let i := 100 				*/
/* Step 3: Let (j,m) := Maximum (X, i) 			*/
/* Step 4: Swap; Let tmp := X[j]  			*/
/* Step 5: X[i] := m 					*/
/* Step 6: X[j] := tmp 					*/
/* Step 7: i := i - 1 					*/
/* Step 8: if i != 0 go to Step 3 			*/
/* Step 9: print X[0..99] to standard output 		*/

Main      GETA  t,9F			/* get address 9(Following) into t */
          TRAP  0,Fread,StdIn		/* read from standard input using 9H as arg */
          SET   $0,N<<3			/* $0 := $100 * 8 */
1H        SR    $2,$0,3			/* $2 = $0 / 8 */
          PUSHJ 1,Maximum		/* call Maximum ($0) */
          LDO   $3,x0,$0		/* $3 := M8[$0 + x0] */
          SL    $2,$2,3			/* $2 = $2 << 3 */
          STO   $1,x0,$0		/* M8[x0+$0] := $1 */
          STO   $3,x0,$2		/* M8[x0+$2] := $3 */
          SUB   $0,$0,1<<3		/* $0 = $0 - 8 */
          PBNZ  $0,1B			/* if $0 != 0 goto 1(before) */
          GETA  t,9F			/* t = address of 9(following) */
          TRAP  0,Fwrite,StdOut		/* write to standard output */
          TRAP  0,Halt,0		/* done */
9H        OCTA  X0+1<<3,N<<3		/* address of data segment + 8, 800 */
