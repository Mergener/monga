source_filename = "tests/codegen_cases/char.mon"

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

	%t.0 = alloca i8
	store i8 0, i8* %t.0

	store i8 10, i8* %t.0

	%t.1 = load i8, i8* %t.0
	call void @RtInternal_EchoChar(i8 %t.1)

	ret void
}



