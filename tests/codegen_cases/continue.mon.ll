source_filename = "tests/codegen_cases/continue.mon"

%string = type { i32, i32 }

declare %string* @RtInternal_StrFromSZ(i8*)
declare i8* @RtInternal_GcAlloc(i64)
declare void @RtInternal_EchoArray(i8*)
declare void @RtInternal_EchoObject(i8*)
declare void @RtInternal_EchoInteger(i64)
declare void @RtInternal_EchoReal(double)
declare void @RtInternal_EchoChar(i8)
declare void @RtInternal_EchoString(%string*)
declare i8* @RtInternal_GetAllocSize(i64)
declare void @RtInternal_Init()

define i32 @foo(i32) {
	%t.0 = alloca i32
	store i32 %0, i32* %t.0

	%t.1 = load i32, i32* %t.0
	%t.2 = icmp sgt i32 %t.1, 3
	br i1 %t.2, label %l.1, label %l.0
l.1:
	%t.3 = load i32, i32* %t.0
	%t.4 = icmp sgt i32 %t.3, 10
	br i1 %t.4, label %l.3, label %l.4
l.3:
	%t.5 = load i32, i32* %t.0
	%t.6 = sub i32 %t.5, 1
	store i32 %t.6, i32* %t.0

	br label %l.2
l.4:
	ret i32 5
l.2:
	%t.7 = load i32, i32* %t.0
	%t.8 = icmp sgt i32 %t.7, 3
	br i1 %t.8, label %l.1, label %l.0
l.0:
	ret i32 2
}

define void @main() {
	call void @RtInternal_Init()
	%t.0 = call i32 @foo(i32 13)
	call void @printInteger(i32 %t.0)

	ret void
}

declare void @printInteger(i32)


