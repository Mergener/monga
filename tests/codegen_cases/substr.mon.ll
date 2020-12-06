source_filename = "tests/codegen_cases/substr.mon"

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

	%t.0 = alloca %string*
	store %string* null, %string** %t.0

	store %string* bitcast ({ i64, i64, [28 x i8] }* @str.0 to %string*), %string** %t.0

	%t.1 = load %string*, %string** %t.0
	%t.2 = call %string* @Substr(%string* %t.1, i32 7, i32 13)
	call void @RtInternal_EchoString(%string* %t.2)

	ret void
}

declare %string* @Substr(%string*, i32, i32)

@str.0 = private constant { i64, i64, [28 x i8] } { i64 27, i64 6730088148463312889, [28 x i8] c"Hello, Hello, world! world!\00" }

