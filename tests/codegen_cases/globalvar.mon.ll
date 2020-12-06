source_filename = "tests/codegen_cases/globalvar.mon"

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

@i = internal global i32 0

define void @foo() {
	%t.0 = load i32, i32* @i
	%t.1 = sext i32 %t.0 to i64
	call void @RtInternal_EchoInteger(i64 %t.1)

	ret void
}

define void @main() {
	call void @RtInternal_Init()

	%t.0 = load i32, i32* @i
	%t.1 = sext i32 %t.0 to i64
	call void @RtInternal_EchoInteger(i64 %t.1)

	store i32 5, i32* @i

	call void @foo()

	store i32 3, i32* @i

	call void @foo()

	ret void
}



