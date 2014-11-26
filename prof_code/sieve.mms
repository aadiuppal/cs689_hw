/* Sieve of Eratosthenes: Print a list of primes from 2 through N */
/* code by Daniel A. Jimenez */

	LOC	#100	/* start at hexadecimal address 100 */

/* Sieve of Eratosthenes */

/* Step 1: V is an array [2..N] of bytes, all initially 1 	*/
/* Step 2: Let k := 2 						*/
/* Step 3: While k < N/2 do steps 4 through N			*/
/* Step 4:    Let i := 2k 					*/
/* Step 5:    While i < N do Steps 6 through N 			*/
/* Step 6:       V[i] := 0 					*/
/* Step 7:       i := i + k 					*/
/* Step 8:    End While i					*/
/* Step 9:    k := k + 1					*/
/* Step 10: End while k						*/
/* Step 11: Print the indices of all the remaining 1s		*/

N	IS	1000
n	IS	$2
v	IS	$3
i	IS	$4
k	IS	$5
ONE	IS	$7
n2	IS	$8
ZERO	IS	$9

/* Step 1: Initialize V to all 1s */

Main	GETA	v,V		/* put the address of V into v */
	SET	i,N		/* i := N */
	SET	ONE,1
	SET	ZERO,0
	SET	n,N
Loop1	STB	ONE,v,i		/* v[i] := 1 */
	SUB	i,i,1		/* i := i - 1 */
	BP	i,Loop1		/* if i >= 0 go back to Loop1 */

/* Step 2 */

	SET	k,2
	SET	n2,N/2

/* begin Step 3 */

Loop2	ADD	i,k,k		/* Step 4: i := k + k */

/* begin Step 5 (as a do/while instead of while loop) */

Loop3	STB	ZERO,v,i	/* Step 6: v[i] := 0 */
	ADD	i,i,k		/* Step 7: i := i + k */
	CMP	$10,i,n		/* if i < N goto Loop3 */
	BNP	$10,Loop3
	ADD	k,k,1		/* k := k + 1 */
	CMP	$10,k,n2	/* if k < N/2 goto Step 4*/
	BNP	$10,Loop2

/* now we go through the array printing the indices of all the remaining 1s */

	SET	i,2		/* i := 2 */
Loop4	LDB	$11,v,i		/* $11 := v[i] */
	BZ	$11,Nope	/* if it is zero, skip over the next part */
	SET	$11,i		/* $11 := i */
	PUSHJ	10,PrnDec	/* push 10 registers (our stack frame) and call the print decimal routine; $11 becomes $0 */
	GETA	$255,CR		/* print a carriage return */
	TRAP	0,Fputs,StdOut
Nope	ADD	i,i,1		/* i := i + 1 */
	CMP	$10,i,n		/* if i <= N goto Loop 4 */
	BNP	$10,Loop4
	TRAP  0,Halt,0		/* done! */

CR	BYTE  #a,0,0,0
V	IS	@		/* V starts "here" */
	LOC	V+N+1		/* move "here" forward N+1 bytes and start assembling again */

/* Print Decimal: $0 has the value to print in decimal, no return value */

PrnDec	BP	$0,Not0		/* if $0 is 0, handle this as a special case */
	GETA	$255,Zero	/* print out the string "0" */
	TRAP	0,Fputs,StdOut
	POP	0,0		/* return */
Not0	GETA	$4,Output	/* it's not 0; $4 is the byte array for output */
	SET	$5,14		/* $5 is the index into the array (counting backwards) */
Not01	DIV	$2,$0,#a	/* $2 := $0 / 10 */
	GET	$3,rR		/* $3 is the remainder */
	ADD	$3,$3,'0'	/* convert to ASCII */
	STB	$3,$4,$5	/* store to Output[$5] */
	SET	$0,$2		/* $0 := $0 / 10 */
	SUB	$5,$5,1		/* $5 := $5 - 1 */
	BNZ	$0,Not01	/* if $0 has more digits, continue the loop */
	GETA	$255,Output	/* get Output array address into $255 */
	ADD	$255,$255,$5	/* add $5 plus 1 so we print starting at the right index */
	ADD	$255,$255,1
	TRAP	0,Fputs,StdOut	/* print the decimal string */
	POP	0,0		/* done */

/* the string 0 with three more 0s for alignment */

Zero	BYTE	"0",0,0,0		

/* a buffer to keep the output string */

Output	BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0	
