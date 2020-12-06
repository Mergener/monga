source_filename = "tests/codegen_cases/while.mon"

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

define void @main() {
	call void @RtInternal_Init()

	%t.0 = alloca i32
	store i32 0, i32* %t.0

	store i32 10, i32* %t.0

	%t.1 = load i32, i32* %t.0
	%t.2 = icmp sgt i32 %t.1, 0
	br i1 %t.2, label %l.1, label %l.0
l.1:
	%t.3 = load i32, i32* %t.0
	%t.4 = sext i32 %t.3 to i64
	call void @RtInternal_EchoInteger(i64 %t.4)

	%t.5 = load i32, i32* %t.0
	%t.6 = sub i32 %t.5, 1
	store i32 %t.6, i32* %t.0

	br label %l.2
l.2:
	%t.7 = load i32, i32* %t.0
	%t.8 = icmp sgt i32 %t.7, 0
	br i1 %t.8, label %l.1, label %l.0
l.0:
	ret void
}



