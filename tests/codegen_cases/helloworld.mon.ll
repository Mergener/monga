source_filename = "tests/codegen_cases/helloworld.mon"

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
	%t.0 = call %string* @RtInternal_StrFromSZ(i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.0, i32 0, i32 0))
	call void @RtInternal_EchoString(%string* %t.0)
	ret void
}


@str.0 = private unnamed_addr constant [14 x i8] c"Hello, world!\00", align 1

