target triple = "x86_64-pc-linux-gnu"
@pfmt0 = constant [6 x i8] c"spl> \00"
@pfmt = constant [5 x i8] c"%ld\0A\00"
declare i32 @printf(i8*,...)
declare i32 @scanf(i8*,...)
@f1 = constant [33 x i8] c"Please enter a positive number.
\00"
@f2 = constant [25 x i8] c"Thank you and good job.
\00"
@f3 = constant [43 x i8] c"You will now be punished with 50 punches.
\00"
@f4 = constant [7 x i8] c"punch
\00"
define i32 @main() {
    call i32(i8*,...) @printf(i8* getelementptr([33 x i8], [33 x i8]* @f1, i32 0, i32 0), i64 0)
    %v1 = alloca i64
    call i32(i8*,...) @printf(i8* getelementptr([6 x i8], [6 x i8]* @pfmt0, i32 0, i32 0), i64* %v1)
    call i32(i8*,...) @scanf(i8* getelementptr([5 x i8], [5 x i8]* @pfmt, i32 0, i32 0), i64* %v1)
    %v2 = load i64, i64* %v1
    %v3 = alloca i64
    store i64 %v2, i64* %v3
    %v4 = load i64, i64* %v3
    %v5 = icmp sgt  i64 %v4, 0
    %v6 = zext i1 %v5 to i64
    %v7 = trunc i64 %v6 to i1
    br i1 %v7, label %b1, label %b2
b1:
    call i32(i8*,...) @printf(i8* getelementptr([25 x i8], [25 x i8]* @f2, i32 0, i32 0), i64 0)
    br label %b3
b2:
    call i32(i8*,...) @printf(i8* getelementptr([43 x i8], [43 x i8]* @f3, i32 0, i32 0), i64 0)
    %v8 = alloca i64
    store i64 0, i64* %v8
    br label %b5
b5:
    %v10 = load i64, i64* %v8
    %v11 = icmp slt  i64 %v10, 50
    %v12 = zext i1 %v11 to i64
    %v9 = trunc i64 %v12 to i1
    br i1 %v9, label %b4, label %b6
b4:
    call i32(i8*,...) @printf(i8* getelementptr([7 x i8], [7 x i8]* @f4, i32 0, i32 0), i64 0)
    %v13 = load i64, i64* %v8
    %v14 = add i64 %v13, 1
    store i64 %v14, i64* %v8
    br label %b5
b6:
    br label %b3
b3:
    ret i32 0
}
