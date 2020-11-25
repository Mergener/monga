source_filename = "tests/codegen_cases/helloworld.mon"

%string = type { i32, i32 }

declare %string* @RtInternal_StrFromSZ(i8*)
declare i8* @RtInternal_GcAlloc(i32)
declare void @RtInternal_Init()

define void @main() {
	call void @RtInternal_Init()
	%t.0 = call %string* @RtInternal_StrFromSZ(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @str.0, i32 0, i32 0))
	call void @printString(%string* %t.0)

	ret void
}

declare void @printString(%string*)

@str.0 = private unnamed_addr constant [15 x i8] c"Hello, world!\0A\00", align 1

