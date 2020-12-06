source_filename = "tests/codegen_cases/while_all_return.mon"

%string = type { i64, i64 }
%.array = type { i64 }

declare %string* @RtInternal_StrFromSZ(i8*)
declare i8* @RtInternal_GcAlloc(i64)
declare %.array* @RtInternal_GcAllocArray(i64, i64)
declare i64 @RtInternal_GetGcArrayElemCount(%.array*)
declare void @RtInternal_EchoArray(i8*)
declare void @RtInternal_EchoObject(i8*)
declare void @RtInternal_EchoInteger(i64)
declare void @RtInternal_EchoReal(double)
declare void @RtInternal_EchoChar(i8)
declare void @RtInternal_EchoString(%string*)
declare void @RtInternal_Hash(i8*, i32)
declare void @RtInternal_Init()

define i32 @foo(i32) {
	%t.0 = alloca i32
	store i32 %0, i32* %t.0

	%t.1 = load i32, i32* %t.0
	%t.2 = icmp sgt i32 %t.1, 6
	br i1 %t.2, label %l.1, label %l.0
l.1:
	ret i32 5
l.2:
	%t.3 = load i32, i32* %t.0
	%t.4 = icmp sgt i32 %t.3, 6
	br i1 %t.4, label %l.1, label %l.0
l.0:
	ret i32 3
}

define void @main() {
	call void @RtInternal_Init()

	%t.0 = call i32 @foo(i32 7)
	%t.1 = sext i32 %t.0 to i64
	call void @RtInternal_EchoInteger(i64 %t.1)

	ret void
}



