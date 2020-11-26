source_filename = "tests/codegen_cases/condexp.mon"

%string = type { i32, i32 }

declare %string* @RtInternal_StrFromSZ(i8*)
declare i8* @RtInternal_GcAlloc(i64)
declare void @RtInternal_EchoArray(i8*)
declare void @RtInternal_EchoObject(i8*)
declare void @RtInternal_EchoInteger(i64)
declare void @RtInternal_EchoReal(double)
declare void @RtInternal_EchoChar(i8)
declare void @RtInternal_EchoString(%string*)
declare void @RtInternal_Init()

define i32 @max(i32, i32) {
	%t.0 = alloca i32
	store i32 %0, i32* %t.0
	%t.1 = alloca i32
	store i32 %1, i32* %t.1

	%t.2 = load i32, i32* %t.0
	%t.3 = load i32, i32* %t.1
	%t.4 = icmp sgt i32 %t.2, %t.3
	br i1 %t.4, label %l.0, label %l.1
l.0:
	%t.5 = load i32, i32* %t.0
	br label %l.3
l.3:
	br label %l.2
l.1:
	%t.6 = load i32, i32* %t.1
	br label %l.4
l.4:
	br label %l.2
l.2:
	%t.7 = phi i32 [%t.5, %l.3], [%t.6, %l.4]
	ret i32 %t.7
}

define void @main() {
	call void @RtInternal_Init()
	%t.0 = call i32 @max(i32 6, i32 3)
	%t.1 = sext i32 %t.0 to i64
	call void @RtInternal_EchoInteger(i64 %t.1)
	%t.2 = call i32 @max(i32 3, i32 6)
	%t.3 = sext i32 %t.2 to i64
	call void @RtInternal_EchoInteger(i64 %t.3)
	%t.4 = call i32 @max(i32 5, i32 0)
	%t.5 = sext i32 %t.4 to i64
	call void @RtInternal_EchoInteger(i64 %t.5)
	ret void
}



