source_filename = "tests/codegen_cases/null.mon"

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
	%t.0 = alloca i32*

	store i32* null, i32** %t.0
	store i32* null, i32** %t.0

	%t.1 = load i32*, i32** %t.0
	%t.2 = icmp eq i32* %t.1, null
	br i1 %t.2, label %l.0, label %l.1
l.0:
	call void @printInteger(i32 1)

	br label %l.2
l.1:
	call void @printInteger(i32 0)

	br label %l.2
l.2:
	ret void
}

declare void @printInteger(i32)


