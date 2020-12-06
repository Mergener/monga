source_filename = "tests/codegen_cases/helloworld.mon"

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

	call void @RtInternal_EchoString(%string* bitcast ({ i64, i64, [14 x i8] }* @str.0 to %string*))

	ret void
}


@str.0 = private constant { i64, i64, [14 x i8] } { i64 13, i64 11225331567545969040, [14 x i8] c"Hello, world!\00" }

