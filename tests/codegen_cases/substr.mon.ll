source_filename = "tests/codegen_cases/substr.mon"

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

define void @main() {
	call void @RtInternal_Init()
	%t.0 = alloca %string*

	store %string* null, %string** %t.0
	%t.1 = call %string* @RtInternal_StrFromSZ(i8* getelementptr inbounds ([28 x i8], [28 x i8]* @str.0, i32 0, i32 0))
	store %string* %t.1, %string** %t.0

	%t.2 = load %string*, %string** %t.0
	%t.3 = call %string* @Substr(%string* %t.2, i32 7, i32 13)
	call void @printString(%string* %t.3)

	ret void
}

declare void @printString(%string*)
declare %string* @Substr(%string*, i32, i32)

@str.0 = private unnamed_addr constant [28 x i8] c"Hello, Hello, world! world!\00", align 1

