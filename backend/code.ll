define i32 @func_0() {
0:
	%1 = alloca i32
	store i32 2, i32 ptr %1
	br label %2
2:
	%3 = load i32, ptr %1
	%4 = icmp slt i32 %3, 10
	br i1 %4, label %5, label %12
5:
	%6 = load i32, ptr %1
	%7 = add i32 %6, 1
	store i32 %7, i32 ptr %1
	%8 = load i32, ptr %1
	%9 = icmp eq i32 %8, 5
	br i1 %9, label %10, label %11
10:
	store i32 200, i32 ptr %1
	br label %12
	br label %11
11:
	br label %2
12:
	%13 = load i32, ptr %1
	output i32 %13
	ret i32 0
}

